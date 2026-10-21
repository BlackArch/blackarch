#!/usr/bin/env bash
# build-shard.sh <shard.json> <packages-dir> <results-dir> <pkgs-dir> <meta.json>
#
# Runs on the GitHub runner host. For every package of one shard:
#   1. build container (ba-builder): bump + build; upstream code runs here
#   2. verify-output.py: reject anything the build did not legitimately make
#   3. install container: install the verified packages on a clean system
# with a per-package timeout and a shard-wide time budget below the job limit.
#
#   results-dir/<pkg>/{status.json,build.log,PKGBUILD}   small, for the report
#   pkgs-dir/<pkg>/*.pkg.tar.zst                         big, for publishing
set -uo pipefail

shard=$1 pkgsrc=$2 results=$3 pkgout=$4 meta=$5
here=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
PER_PKG_TIMEOUT_MIN=${PER_PKG_TIMEOUT_MIN:-60}
# keep room for setup, one last full build, the install test and the uploads
JOB_TIMEOUT_MIN=${JOB_TIMEOUT_MIN:-340}
BUDGET_SECS=${BUDGET_SECS:-$(( (JOB_TIMEOUT_MIN - 25 - PER_PKG_TIMEOUT_MIN - 20) * 60 ))}
MIN_FREE_GB=${MIN_FREE_GB:-8}
CACHE=${PACMAN_CACHE:-$PWD/.pacman-cache}
mkdir -p "$results" "$pkgout" "$CACHE"
chmod 777 "$CACHE"
t0=${JOB_START:-$(date +%s)}

write_status() { # <pkg> <kind> <cur> <new> <result> <reason>
  jq -n --arg pkg "$1" --arg kind "$2" --arg cur "$3" --arg target "$4" \
        --arg result "$5" --arg reason "$6" \
        '{pkg:$pkg, kind:$kind, cur:$cur, target:$target, result:$result, reason:$reason}' \
        > "$results/$1/status.json"
}

while read -r u; do
  pkg=$(jq -r .pkg <<<"$u"); kind=$(jq -r .kind <<<"$u")
  cur=$(jq -r .cur <<<"$u"); new=$(jq -r .new <<<"$u")
  ship=$(jq -r 'if .ship == true then 1 else 0 end' <<<"$u")
  mkdir -p "$results/$pkg"
  case $kind in
    vcs)          echo "::group::$pkg (vcs: $cur, upstream now at commit ${new:0:7})" ;;
    test|rebuild) echo "::group::$pkg ($kind: $cur)" ;;
    ship) echo "::group::$pkg (ship: $new, not in the repo yet)" ;;
    *)            echo "::group::$pkg ($kind: $cur -> $new)" ;;
  esac

  if (( $(date +%s) - t0 > BUDGET_SECS )); then
    write_status "$pkg" "$kind" "$cur" "$new" skipped "shard time budget exhausted; retried next run"
    echo "::endgroup::"; continue
  fi
  free_gb=$(df -BG --output=avail "$PWD" | tail -1 | tr -dc 0-9)
  if (( free_gb < MIN_FREE_GB )); then
    # never prune images: ba-builder must survive
    sudo rm -rf "${CACHE:?}"/*
    docker container prune -f >/dev/null 2>&1 || true
    docker builder prune -af >/dev/null 2>&1 || true
  fi

  cname="build-${pkg//[^a-zA-Z0-9_.-]/_}"
  work=$(mktemp -d) host=$(mktemp -d)
  cp -a "$pkgsrc/$pkg/." "$work/"
  mkdir -p "$work/.out"
  chmod -R a+rwX "$work"

  started=$(date +%s)
  timeout --kill-after=2m "${PER_PKG_TIMEOUT_MIN}m" \
    docker run --rm --name "$cname" \
      -e PKG="$pkg" -e KIND="$kind" -e CUR="$cur" -e NEW="$new" -e SHIP="$ship" -e GIT_TERMINAL_PROMPT=0 \
      -v "$work:/build" -v "$work/.out:/out" \
      -v "$CACHE:/var/cache/pacman/pkg" \
      ba-builder < /dev/null
  rc=$?
  took=$(( $(date +%s) - started ))
  if (( rc == 124 || rc == 137 )); then
    docker rm -f "$cname" >/dev/null 2>&1
    if (( took >= PER_PKG_TIMEOUT_MIN * 60 )); then
      why="timed out after ${PER_PKG_TIMEOUT_MIN} min"
    else
      why="build was killed (rc=$rc, probably out of memory)"
    fi
    write_status "$pkg" "$kind" "$cur" "$new" build_failed "$why"
  elif [[ -L $work/.out || ! -d $work/.out ]]; then
    write_status "$pkg" "$kind" "$cur" "$new" build_failed "output rejected: output directory was replaced"
  elif (( rc != 0 )) && [[ ! -s $work/.out/status.json ]]; then
    write_status "$pkg" "$kind" "$cur" "$new" build_failed "builder container failed to run (rc=$rc)"
  else
    # ---- 2. verify everything the container produced. Results go to $host,
    # which was never mounted into the container.
    sudo chown -R "$(id -u):$(id -g)" "$work/.out"
    # only regular files and directories survive (no symlinks, FIFOs, devices)
    find "$work/.out" ! -type f ! -type d -delete
    python3 "$here/verify-output.py" --pkg "$pkg" --kind "$kind" --cur "$cur" --target "$new" --ship "$ship" \
      --orig "$pkgsrc/$pkg/PKGBUILD" --work "$work/.out" --meta "$meta" \
      --status-out "$host/status.json" --verified-out "$host/PKGBUILD" --valid-list "$host/valid.txt"
    cp "$host/status.json" "$results/$pkg/status.json"

    # ---- 3. install test in a fresh container
    if [[ $(jq -r .result "$results/$pkg/status.json") == ok ]]; then
      inst=$(mktemp -d)
      while read -r f; do [[ -n $f ]] && cp "$f" "$inst/"; done < "$host/valid.txt"
      chmod -R a+rX "$inst"
      { echo; echo "==> install test in a clean container"; } >> "$work/.out/build.log"
      if timeout --kill-after=2m 20m docker run --rm --name "inst-${cname#build-}" --entrypoint /usr/local/bin/install-test.sh \
           -v "$inst:/pkgs:ro" -v "$CACHE:/var/cache/pacman/pkg" ba-builder \
           < /dev/null >> "$work/.out/build.log" 2>&1; then
        jq '.reason = "built and installed"' "$results/$pkg/status.json" > "$work/s" &&
          mv "$work/s" "$results/$pkg/status.json"
        cp "$host/PKGBUILD" "$results/$pkg/PKGBUILD"
        if [[ $kind != test ]]; then
          mkdir -p "$pkgout/$pkg"
          cp "$inst"/*.pkg.tar.zst "$pkgout/$pkg/"
        fi
      else
        # an install hook that ignores SIGTERM leaves the container running after the timeout
        docker rm -f "inst-${cname#build-}" >/dev/null 2>&1 || true
        jq --arg t "$(tail -n 40 "$work/.out/build.log" | cut -c1-300)" \
          '.result = "build_failed" | .reason = "does not install on a clean system (missing runtime depends or file conflicts)" | .log_tail = $t' \
          "$results/$pkg/status.json" > "$work/s" && mv "$work/s" "$results/$pkg/status.json"
      fi
      sudo rm -rf "$inst"
    fi
  fi

  # keep logs small (the build controls this file): first 64 KiB + last 1 MiB
  log=$work/.out/build.log
  if [[ ! -L $work/.out && -f $log && ! -L $log ]]; then
    if (( $(stat -c %s "$log") > 1200000 )); then
      { head -c 65536 "$log"; printf '\n[... truncated ...]\n'; tail -c 1048576 "$log"; } \
        > "$results/$pkg/build.log"
    else
      cp "$log" "$results/$pkg/build.log"
    fi
  fi
  jq -r '"result: \(.result) - \(.reason)"' "$results/$pkg/status.json"
  # failures: show the end of the build log on the console too (the full log
  # is in the results-N artifact, the same tail is in report.md)
  if [[ $(jq -r .result "$results/$pkg/status.json") != ok && -s $results/$pkg/build.log ]]; then
    echo "--- last 40 lines of build.log ---"
    tail -n 40 "$results/$pkg/build.log" | cut -c1-300
    echo "--- end of build.log ---"
  fi
  sudo rm -rf "$work" "$host"
  echo "::endgroup::"
done < <(jq -c '.[]' "$shard")
