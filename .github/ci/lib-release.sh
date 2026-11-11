#!/usr/bin/env bash
# lib-release.sh - sourced by publish.sh and release-merged.sh.
#
# release_setup              import the signing key, check it can sign, load
#                            the SSH key, check the server accepts it. Run it
#                            BEFORE changing anything (push mode: before the
#                            push to master), so broken secrets never leave
#                            committed-but-unreleased packages behind.
# release_upload <pkg>...    wait for the repo lock, run scripts/barelease,
#                            then download the repo database and verify its
#                            signature and the new entries. Any failure queues
#                            the packages in lists/to-release for a human.
# release_packages <pkg>...  setup + upload (used by release-merged.sh)
# queue_for_humans <msg> <pkg>...  append to lists/to-release on master.
#
# All functions set $released / $release_error and return non-zero on
# failure. They never rely on `set -e` (callers use them in `||` lists).
#
# Env: PKGS GPG_KEY_ID GPG_PRIVATE_KEY [GPG_PASSPHRASE] SSH_PRIVATE_KEY
#      SSH_KNOWN_HOSTS SSH_USER [REPO_SITEDIR] [RELEASE=true|false]
#      [LOCK_WAIT_MIN=60]
# shellcheck disable=SC2034  # released/release_error are read by the callers

REPO_HOST=blackarch.org            # scripts/balock hard-codes this host
REPO_SITEDIR=${REPO_SITEDIR:-/var/www/blackarch}

_fail() { release_error=$1; echo "release: $1" >&2; return 1; }

# pkgs-<shard> artifacts are tars (package names may contain ':'); unpack them
# into $PKGS/<pkg>/. They were written by the runner after verification.
unpack_pkgs() {
  local t
  shopt -s nullglob
  for t in "$PKGS"/*.tar; do
    tar -xf "$t" -C "$PKGS" --no-same-owner --no-same-permissions || return 1
    rm -f "$t"
  done
  shopt -u nullglob
}

release_setup() {
  local v
  # accept "0xABCD…", spaces and lower case; pacman-key -l prints upper case
  GPG_KEY_ID=${GPG_KEY_ID:-}; GPG_KEY_ID=${GPG_KEY_ID#0x}; GPG_KEY_ID=${GPG_KEY_ID//[[:space:]]/}; GPG_KEY_ID=${GPG_KEY_ID^^}
  for v in GPG_KEY_ID GPG_PRIVATE_KEY SSH_PRIVATE_KEY SSH_KNOWN_HOSTS SSH_USER; do
    [[ -n ${!v:-} ]] || { _fail "secret/variable $v is not set"; return 1; }
  done

  export GNUPGHOME
  GNUPGHOME=$(mktemp -d) && chmod 700 "$GNUPGHOME" || { _fail "cannot create GNUPGHOME"; return 1; }
  echo allow-preset-passphrase > "$GNUPGHOME/gpg-agent.conf"
  gpg --batch --quiet --import <<<"$GPG_PRIVATE_KEY" || { _fail "cannot import the signing key"; return 1; }
  if [[ -n ${GPG_PASSPHRASE:-} ]]; then
    gpgconf --launch gpg-agent || { _fail "cannot start gpg-agent"; return 1; }
    local grip
    for grip in $(gpg --with-keygrip -K "$GPG_KEY_ID" | awk '/Keygrip/ {print $3}'); do
      /usr/lib/gnupg/gpg-preset-passphrase --preset -P "$GPG_PASSPHRASE" "$grip" ||
        { _fail "cannot preset the key passphrase"; return 1; }
    done
  fi
  export GPG_AGENT_INFO=preset      # barelease: "gpg-agent already started"
  local t
  t=$(mktemp -d)
  echo probe > "$t/f"
  gpg --batch --yes --no-tty --default-key "$GPG_KEY_ID" -b "$t/f" 2>/dev/null &&
    gpg --batch --verify "$t/f.sig" "$t/f" 2>/dev/null ||
    { _fail "the signing key cannot sign (wrong key id or passphrase?)"; return 1; }
  # barelease checks the key is in pacman's keyring. Users only trust its
  # signatures if the key is also in blackarch-keyring!
  gpg --armor --export "$GPG_KEY_ID" > "$t/pub.asc" &&
    pacman-key --add "$t/pub.asc" >/dev/null && pacman-key --lsign-key "$GPG_KEY_ID" >/dev/null ||
    { _fail "cannot add the key to pacman's keyring"; return 1; }
  rm -rf "$t"

  # ssh reads ~ from passwd, not $HOME (which is /github/home in containers)
  local home
  home=$(getent passwd "$(id -u)" | cut -d: -f6)
  mkdir -p /etc/ssh "$home/.ssh" && chmod 700 "$home/.ssh"
  printf '%s\n' "$SSH_KNOWN_HOSTS" | tee -a /etc/ssh/ssh_known_hosts > "$home/.ssh/known_hosts"
  eval "$(ssh-agent -s)" >/dev/null || { _fail "cannot start ssh-agent"; return 1; }
  ssh-add -q - <<<"$SSH_PRIVATE_KEY" || { _fail "cannot load the SSH key"; return 1; }
  ssh -o BatchMode=yes -o ConnectTimeout=20 -l "$SSH_USER" "$REPO_HOST" true ||
    { _fail "SSH login to $SSH_USER@$REPO_HOST failed"; return 1; }
  return 0
}

# Pre-wait (with a stale-lock warning) before letting barelease take the lock
# itself: its mkdir is atomic and, with the patched scripts/barelease, its
# cleanup only removes a lock it really took, so no check-then-acquire race.
_wait_for_lock() {
  local deadline=$(( $(date +%s) + ${LOCK_WAIT_MIN:-60} * 60 )) age
  while age=$(ssh -o BatchMode=yes -l "$SSH_USER" "$REPO_HOST" \
                '[ -e /tmp/blackarch.lck ] && echo $(( $(date +%s) - $(stat -c %Y /tmp/blackarch.lck) ))'); [[ -n $age ]]; do
    (( $(date +%s) < deadline )) || return 1
    if (( age > ${LOCK_STALE_MIN:-180} * 60 )); then
      echo "::warning::/tmp/blackarch.lck on $REPO_HOST is $(( age / 60 )) min old; if no release is running, remove it by hand (scripts/balock -u)"
    fi
    echo "repo is locked by someone else; waiting..."
    sleep 30
  done
}

_verify_remote() { # <pkg>...  -> checks db signatures and that every package is in the db
  local t arch files=() f entry missing=()
  t=$(mktemp -d)
  for pkg in "$@"; do files+=("$PKGS/$pkg"/*.pkg.tar.zst); done
  local arches=(x86_64)
  for f in "${files[@]}"; do [[ $f == *-any.pkg.tar.zst ]] && { arches+=(aarch64); break; }; done
  for arch in "${arches[@]}"; do
    mkdir -p "$t/$arch"
    rsync -q -e "ssh -o BatchMode=yes -l $SSH_USER" \
      "$REPO_HOST:$REPO_SITEDIR/blackarch/blackarch/os/$arch/blackarch.db.tar.gz"{,.sig} "$t/$arch/" ||
      { _fail "cannot download the $arch repo database to verify the upload"; return 1; }
    gpg --batch --verify "$t/$arch/blackarch.db.tar.gz.sig" "$t/$arch/blackarch.db.tar.gz" 2>/dev/null ||
      { _fail "UPLOADED, BUT THE $arch DATABASE SIGNATURE DOES NOT VERIFY - check the repo now"; return 1; }
    tar -tzf "$t/$arch/blackarch.db.tar.gz" 2>/dev/null | sed 's|^\./||' > "$t/$arch/list"
    for f in "${files[@]}"; do
      [[ $arch == aarch64 && $f != *-any.pkg.tar.zst ]] && continue
      entry=$(basename "$f"); entry=${entry%-*.pkg.tar.zst}     # name-ver-rel
      grep -qxF "$entry/" "$t/$arch/list" || missing+=("$arch/$entry")
    done
  done
  rm -rf "$t"
  (( ${#missing[@]} == 0 )) || { _fail "not in the repo database after upload: ${missing[*]}"; return 1; }

  # barelease ignores rsync failures (`|| true`) and never overwrites an
  # existing remote file: compare the remote package files and signatures
  # with the local ones, byte for byte (sha256), in one ssh call per arch
  local bad=() remote_dir names=() sums
  for arch in "${arches[@]}"; do
    remote_dir="$REPO_SITEDIR/blackarch/blackarch/os/$arch"
    names=()
    for f in "${files[@]}"; do
      [[ $arch == aarch64 && $f != *-any.pkg.tar.zst ]] && continue
      names+=("${f##*/}" "${f##*/}.sig")
    done
    (( ${#names[@]} )) || continue
    sums=$(ssh -o BatchMode=yes -l "$SSH_USER" "$REPO_HOST" "cd '$remote_dir' && sha256sum -- $(printf "'%s' " "${names[@]}")" 2>/dev/null) ||
      { _fail "cannot checksum the uploaded files on the server ($arch)"; return 1; }
    for f in "${files[@]}"; do
      [[ $arch == aarch64 && $f != *-any.pkg.tar.zst ]] && continue
      local x
      for x in "$f" "$f.sig"; do
        [[ -f $x ]] || { bad+=("$arch/${x##*/} (no local signature)"); continue; }
        grep -qF "$(sha256sum "$x" | cut -d' ' -f1)  ${x##*/}" <<<"$sums" || bad+=("$arch/${x##*/}")
      done
    done
  done
  (( ${#bad[@]} == 0 )) || { _fail "UPLOADED FILES DIFFER FROM THE BUILT ONES OR ARE MISSING: ${bad[*]} - the db points at them, check the repo now"; return 1; }
}

release_upload() {
  (( $# )) || return 0
  local files=() pkg rc f
  for pkg in "$@"; do files+=("$PKGS/$pkg"/*.pkg.tar.zst); done
  # sign here and pass -b (use existing signatures): the local .sig is then
  # exactly what is uploaded, so _verify_remote can compare it
  for f in "${files[@]}"; do
    gpg --batch --yes --no-tty --default-key "$GPG_KEY_ID" -b "$f" 2>/dev/null ||
      { _fail "cannot sign ${f##*/}"; return 1; }
  done

  if ! _wait_for_lock; then
    _fail "repo lock still held after ${LOCK_WAIT_MIN:-60} min"
  else
    # no -w: if someone grabbed the lock in the meantime barelease waits for it
    timeout 90m ./scripts/barelease -b -k "$GPG_KEY_ID" -u "$SSH_USER" -d "$REPO_SITEDIR" "${files[@]}"
    rc=$?
    if (( rc != 0 )); then
      _fail "barelease failed (rc=$rc)"
    elif _verify_remote "$@"; then
      released=true
      ssh-agent -k >/dev/null 2>&1
      return 0
    fi
  fi
  ssh-agent -k >/dev/null 2>&1
  release_error+="; packages added to lists/to-release"
  queue_for_humans "auto-update packages awaiting release" "$@" ||
    release_error+=" (and pushing lists/to-release failed!)"
  return 1
}

# remove_packages <pkgname>...  barelease --remove each name (repo-remove on
# the server for both arches), then verify the served database. Package files
# are left on the server for scripts/baclean, as a manual --remove does.
remove_packages() {
  (( $# )) || return 0
  local rc=0 n
  if ! _wait_for_lock; then
    _fail "repo lock still held after ${LOCK_WAIT_MIN:-60} min"; return 1
  fi
  for n in "$@"; do
    timeout 30m ./scripts/barelease --remove -k "$GPG_KEY_ID" -u "$SSH_USER" -d "$REPO_SITEDIR" "$n" || rc=1
  done
  (( rc == 0 )) || { ssh-agent -k >/dev/null 2>&1; _fail "barelease --remove failed for at least one package"; return 1; }
  # verify with the key still loaded; the agent is only killed at the end
  local t arch still=()
  t=$(mktemp -d)
  for arch in x86_64 aarch64; do
    rsync -q -e "ssh -o BatchMode=yes -l $SSH_USER" \
      "$REPO_HOST:$REPO_SITEDIR/blackarch/blackarch/os/$arch/blackarch.db.tar.gz" "$t/$arch.db" ||
      { rm -rf "$t"; ssh-agent -k >/dev/null 2>&1; _fail "cannot download the $arch database to verify the removal"; return 1; }
    # package names in the db: "name-ver-rel/" entries minus the last two fields;
    # literal comparison (names may contain '+'), full listing read (no SIGPIPE)
    if ! tar -tzf "$t/$arch.db" > "$t/$arch.list"; then
      rm -rf "$t"; ssh-agent -k >/dev/null 2>&1; _fail "cannot read the $arch database to verify the removal"; return 1
    fi
    sed -nE 's|^(\./)?([^/]+)/$|\2|p' "$t/$arch.list" | sed -E 's/-[^-]+-[^-]+$//' | sort -u > "$t/$arch.names"
    for n in "$@"; do
      grep -qxF -- "$n" "$t/$arch.names" && still+=("$arch/$n")
    done
  done
  rm -rf "$t"
  ssh-agent -k >/dev/null 2>&1
  (( ${#still[@]} == 0 )) || { _fail "still in the repo database after removal: ${still[*]}"; return 1; }
  released=true
}

release_packages() {
  (( $# )) || return 0
  if [[ ${RELEASE:-true} != true ]]; then
    release_error="AUTO_RELEASE=false: packages added to lists/to-release for a manual release"
    queue_for_humans "auto-update packages awaiting manual release" "$@"
    return
  fi
  if ! release_setup; then
    release_error+="; packages added to lists/to-release"
    queue_for_humans "auto-update packages awaiting release" "$@" ||
      release_error+=" (and pushing lists/to-release failed!)"
    return 1
  fi
  release_upload "$@"
}

queue_for_humans() {
  local msg=$1 try; shift
  (( $# )) || return 0
  for try in 1 2 3; do
    git fetch -q origin master && git checkout -q -B master origin/master || return 1
    printf '%s\n' "$@" >> lists/to-release
    sort -u -o lists/to-release lists/to-release
    git add lists/to-release
    git diff --cached --quiet && return 0
    git commit -q -m "lists/to-release: $msg" -m "[skip ci]" || return 1
    git push -q origin HEAD:master && return 0
    sleep $(( try * 5 ))
  done
  return 1
}
