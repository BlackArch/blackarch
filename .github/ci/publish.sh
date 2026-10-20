#!/usr/bin/env bash
# publish.sh - hand successfully built updates over, in one of two modes:
#
#   PUBLISH_MODE=push  check the release secrets, commit to master, push,
#                      then release right away with scripts/barelease
#   PUBLISH_MODE=pr    commit to a branch and open ONE pull request for the
#                      whole run; release-merged.sh uploads the packages when
#                      that PR is merged (workflow auto-update-release.yml)
#
# "ship" results (the tree already has a version the repo lacks: a manual bump
# or a new package, built exactly as it is on the base branch) change no
# PKGBUILD, so they are released right away in both modes: the PKGBUILD was
# reviewed when it was merged.
#
# Runs in the publish job (blackarch container, repo checked out, as root).
# Env:
#   BASE_SHA            commit the run started from (detect job)
#   RESULTS PKGS        downloaded results/ and pkgs/ artifact dirs
#   PUBLISH_OUT         where to write publish.json for the report
#   RUN_ID RUN_URL      this workflow run
#   GH_TOKEN GH_REPO    for `gh pr create` (pr mode)
#   + release env, see lib-release.sh (push mode)
set -euo pipefail
here=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=lib-release.sh
. "$here/lib-release.sh"

MODE=${PUBLISH_MODE:-push}
[[ $MODE == push || $MODE == pr ]] || { echo "unknown PUBLISH_MODE '$MODE'"; exit 1; }
mkdir -p "$RESULTS" "$PKGS" "$(dirname "$PUBLISH_OUT")"
RESULTS=$(realpath "$RESULTS") PKGS=$(realpath "$PKGS")
PUBLISH_OUT=$(realpath -m "$PUBLISH_OUT")
export PKGS
unpack_pkgs || { echo "cannot unpack the built packages"; exit 1; }

committed=() shipped=() released_pkgs=() skipped=() released=false release_error="" pr_url=""
_json() { printf '%s\n' "$@" | jq -R . | jq -sc 'map(select(. != ""))'; }
write_out() {
  jq -n --arg mode "$MODE" --arg pr_url "$pr_url" \
        --argjson committed "$(_json "${committed[@]}")" \
        --argjson shipped "$(_json "${shipped[@]}")" \
        --argjson released_pkgs "$(_json "${released_pkgs[@]}")" \
        --argjson skipped "$(_json "${skipped[@]}")" \
        --argjson released "$released" --arg release_error "$release_error" \
        '{mode:$mode, pr_url:$pr_url, committed:$committed, shipped:$shipped,
          released_pkgs:$released_pkgs, skipped:$skipped,
          released:$released, release_error:$release_error}' > "$PUBLISH_OUT"
}
# release_upload + bookkeeping of what actually went out
do_release() { # <pkg>...
  (( $# )) || return 0
  if release_upload "$@"; then released_pkgs+=("$@"); return 0; fi
  return 1
}
trap write_out EXIT

# ------------------------------------------------------------------ select
# status.json was rewritten on the runner host by verify-output.py; the
# package name is the directory name, never a value from the build container
ok=() ship=() vcs_ship=()
for d in "$RESULTS"/*/; do
  pkg=$(basename "$d")
  [[ -f $d/status.json ]] || continue
  case $(jq -r '.result + "/" + .kind' "$d/status.json") in
    ok/release|ok/vcs|ok/rebuild) ok+=("$pkg") ;;
    ok/ship) ship+=("$pkg") ;;
  esac
  case $(jq -r '.result + "/" + (.ship // false | tostring)' "$d/status.json") in
    ok/true) vcs_ship+=("$pkg") ;;   # may turn out unchanged -> shipped as-is
  esac
done
echo "built OK: ${#ok[@]} to commit, ${#ship[@]} to ship as-is  (mode: $MODE)"
(( ${#ok[@]} + ${#ship[@]} )) || exit 0

git config --global --add safe.directory "$PWD"
git config user.name  "blackarch-ci[bot]"
git config user.email "team@blackarch.org"
git fetch --quiet origin master
git fetch --quiet --depth=1 origin "$BASE_SHA" 2>/dev/null || true

todo=()
for pkg in "${ok[@]}"; do
  if [[ ! -d packages/$pkg ]]; then
    skipped+=("$pkg: no such package on master"); continue
  fi
  if [[ ! -f $RESULTS/$pkg/PKGBUILD ]] || ! compgen -G "$PKGS/$pkg/*.pkg.tar.zst" >/dev/null; then
    skipped+=("$pkg: build artifacts missing"); continue
  fi
  if ! git diff --quiet "$BASE_SHA" origin/master -- "packages/$pkg"; then
    skipped+=("$pkg: changed on master during the run"); continue
  fi
  todo+=("$pkg")
done

# ship: the built PKGBUILD must be byte-identical to the one on the base branch
for pkg in "${ship[@]}"; do
  dir=packages/$pkg
  if [[ -z $dir || ! -d $dir ]]; then
    skipped+=("$pkg: no such package on master"); continue
  fi
  if [[ ! -f $RESULTS/$pkg/PKGBUILD ]] || ! compgen -G "$PKGS/$pkg/*.pkg.tar.zst" >/dev/null; then
    skipped+=("$pkg: build artifacts missing"); continue
  fi
  if ! git show origin/master:"$dir/PKGBUILD" > "$RESULTS/$pkg/PKGBUILD.base" 2>/dev/null ||
     ! git diff --no-index --quiet "$RESULTS/$pkg/PKGBUILD.base" "$RESULTS/$pkg/PKGBUILD"; then
    skipped+=("$pkg: PKGBUILD changed on master during the run"); continue
  fi
  shipped+=("$pkg")
done

# push mode: make sure the upload can work BEFORE anything reaches master
# (and before shipping anything, in either mode)
need_setup=false
(( ${#shipped[@]} + ${#vcs_ship[@]} > 0 )) && need_setup=true
[[ $MODE == push && ${#todo[@]} -gt 0 ]] && need_setup=true
if [[ ${RELEASE:-true} == true && $need_setup == true ]]; then
  if ! release_setup; then
    release_error="not published: $release_error"
    echo "$release_error"
    exit 1
  fi
fi


# ------------------------------------------------------------------ commit
if [[ $MODE == pr ]]; then
  branch="auto-update/$(date -u +%Y-%m-%d)-$RUN_ID"
  # no [skip ci] here: GitHub would then also skip the release workflow
  # that runs when this PR is merged
  trailer=()
else
  branch=master
  trailer=(-m "[skip ci]")
fi
git checkout -q -B "$branch" origin/master
for pkg in "${todo[@]}"; do
  cp "$RESULTS/$pkg/PKGBUILD" "packages/$pkg/PKGBUILD"
  git add "packages/$pkg/PKGBUILD"
  if git diff --cached --quiet; then
    # a VCS package planned as ship whose pkgver() found nothing new: the tree
    # version is what the repo lacks, release it as it is
    if [[ $(jq -r '.ship // false' "$RESULTS/$pkg/status.json") == true ]]; then
      shipped+=("$pkg")
    else
      skipped+=("$pkg: no diff")
    fi
    continue
  fi
  ver=$(jq -r .pkgver "$RESULTS/$pkg/status.json")
  git commit -q -m "$pkg: auto-update to $ver." \
    -m "Built in a clean container and install-tested by $RUN_URL" "${trailer[@]}"
  committed+=("$pkg")
done
# pr mode: ship now, the PR only carries the bumps. push mode: ship together
# with the pushed bumps below (one repo-add).
if [[ $MODE == pr && ${#shipped[@]} -gt 0 ]]; then
  if [[ ${RELEASE:-true} == true ]]; then
    do_release "${shipped[@]}" || echo "shipping failed: $release_error"
  else
    release_packages "${shipped[@]}"     # AUTO_RELEASE=false -> lists/to-release
  fi
fi
if (( ${#committed[@]} == 0 )); then
  if [[ $MODE == push && ${#shipped[@]} -gt 0 ]]; then
    if [[ ${RELEASE:-true} == true ]]; then do_release "${shipped[@]}" || exit 1
    else release_packages "${shipped[@]}"; fi
  fi
  exit 0
fi

# ------------------------------------------------------------------ pr mode
if [[ $MODE == pr ]]; then
  git push -q origin "HEAD:refs/heads/$branch"
  body=$(mktemp)
  {
    echo "<!-- auto-update-run-id: $RUN_ID -->"
    echo "Automated update of **${#committed[@]}** packages. Each one was bumped,"
    echo "built in a clean container and install-tested with \`pacman -U\` on a"
    echo "fresh system ([run]($RUN_URL), build logs in the \`auto-update-report\` artifact)."
    echo
    echo "**Merging this PR uploads the already-built packages to the repo.**"
    echo "To keep a package out, revert its commit before merging. If you edit a"
    echo "PKGBUILD here instead, that package is not uploaded but added to"
    echo "\`lists/to-release\` for a manual build."
    echo
    echo "| Package | From | To |"
    echo "|---|---|---|"
    for pkg in "${committed[@]}"; do
      jq -r --arg p "$pkg" '"| `\($p)` | \(.cur | gsub("[^A-Za-z0-9._+:~-]";"")) | \(.pkgver | gsub("[^A-Za-z0-9._+]";"")) |"' \
        "$RESULTS/$pkg/status.json"
    done
    if (( ${#skipped[@]} )); then
      echo
      echo "<details><summary>Not included (${#skipped[@]})</summary>"
      echo
      printf -- '- %s\n' "${skipped[@]}"
      echo "</details>"
    fi
  } > "$body"
  head -c 60000 "$body" > "$body.cut"
  gh label create auto-update --color 0075ca \
    --description "Automated PKGBUILD updates" 2>/dev/null || true
  pr_url=$(gh pr create --base master --head "$branch" --label auto-update \
    --title "auto-update: ${#committed[@]} packages ($(date -u +%Y-%m-%d))" \
    --body-file "$body.cut")
  echo "opened $pr_url"
  exit 0
fi

# ------------------------------------------------------------------ push mode
pushed=false
for try in 1 2 3; do
  if git push -q origin HEAD:master; then pushed=true; break; fi
  git fetch -q origin master
  git rebase -q origin/master || { git rebase --abort; break; }
done
if ! $pushed; then
  committed=()
  release_error="push to master failed (conflict or permissions); nothing published"
  echo "$release_error"
  exit 1
fi
echo "pushed ${#committed[@]} commits"

if [[ ${RELEASE:-true} != true ]]; then
  release_packages "${committed[@]}" "${shipped[@]}"     # AUTO_RELEASE=false -> lists/to-release
  exit 0
fi
do_release "${committed[@]}" "${shipped[@]}" || exit 1
