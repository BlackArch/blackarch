#!/usr/bin/env bash
# meta-one.sh <package-dir>
#
# Dumps the metadata of ONE PKGBUILD as "key<TAB>value" lines on stdout, so
# detect.py can lint and track 4000+ packages without makepkg in the loop.
# Arrays are emitted one line per element. The record ends with "@@end".
#
# The PKGBUILD is sourced in a clean `env -i` bash, exactly like
# scripts/checkpkgs does. When makepkg is available (CI container) we also run
# `makepkg --printsrcinfo`, which applies makepkg's own lint_pkgbuild rules.

dir=${1%/}
name=${dir##*/}

emit() { printf '%s\t%s\n' "$1" "${2//$'\n'/ }"; }

emit dir "$name"

if [[ ! -f "$dir/PKGBUILD" ]]; then
  emit fatal "no PKGBUILD"
  echo '@@end'
  exit 0
fi

if ! syn=$(bash -O extglob -n "$dir/PKGBUILD" 2>&1); then
  emit syntax_error "${syn:0:300}"
fi

emit pkgver_lines "$(grep -cE '^pkgver=' "$dir/PKGBUILD")"
emit pkgrel_lines "$(grep -cE '^pkgrel=' "$dir/PKGBUILD")"

errf=$(mktemp)
env -i PATH=/usr/bin:/bin HOME=/nonexistent CARCH=x86_64 bash -O extglob -c '
  cd "$1" || exit 1
  source ./PKGBUILD >/dev/null || echo "__source_rc=$?" >&2
  e() { printf "%s\t%s\n" "$1" "${2//$'"'"'\n'"'"'/ }"; }
  ea() { local k=$1; shift; local v; for v in "$@"; do e "$k" "$v"; done; }
  e pkgbase "${pkgbase:-}"
  ea pkgname "${pkgname[@]}"
  e pkgver "${pkgver:-}"
  e pkgrel "${pkgrel:-}"
  e epoch "${epoch:-}"
  e url "${url:-}"
  ea arch "${arch[@]}"
  ea groups "${groups[@]}"
  ea license "${license[@]}"
  ea makedepends "${makedepends[@]}" "${makedepends_x86_64[@]}"
  ea depends "${depends[@]}" "${depends_x86_64[@]}"
  ea checkdepends "${checkdepends[@]}" "${checkdepends_x86_64[@]}"
  ea optdepends "${optdepends[@]}" "${optdepends_x86_64[@]}"
  ea provides "${provides[@]}" "${provides_x86_64[@]}"
  ea source "${source[@]}" "${source_x86_64[@]}"
  for a in md5 sha1 sha224 sha256 sha384 sha512 b2 ck; do
    declare -n ref="${a}sums" ref64="${a}sums_x86_64"
    if declare -p "${a}sums" >/dev/null 2>&1 || declare -p "${a}sums_x86_64" >/dev/null 2>&1; then
      e "sums_${a}" "$(( ${#ref[@]} + ${#ref64[@]} ))"
    fi
    unset -n ref ref64
  done
  e n_source "$(( ${#source[@]} + ${#source_x86_64[@]} ))"
  declare -F pkgver >/dev/null && e has_pkgver_func 1
  declare -F build  >/dev/null && e has_build_func 1
  declare -F package >/dev/null && e has_package_func 1
  # split packages may override depends/optdepends/provides inside
  # package_<name>(); `declare -f` prints each array on one line, so the
  # assignment can be evaluated on its own (same idea as makepkg --printsrcinfo)
  for p in "${pkgname[@]}"; do
    declare -F "package_$p" >/dev/null || continue
    e has_split_package_func "$p"
    while IFS= read -r line; do
      [[ $line =~ ^[[:space:]]*(depends|optdepends|provides)(_x86_64)?\+?=\( ]] || continue
      t=${BASH_REMATCH[1]} v=${BASH_REMATCH[1]}${BASH_REMATCH[2]}
      e soverride "$p $t"
      (
        eval "$line" >/dev/null 2>&1
        declare -n arr=$v
        for d in "${arr[@]}"; do e "sdep_$t" "$p $d"; done
      )
    done < <(declare -f "package_$p")
  done
  exit 0
' _ "$dir" 2>"$errf"
rc=$?
(( rc != 0 )) && emit fatal "could not evaluate PKGBUILD (rc=$rc)"
if [[ -s "$errf" ]]; then
  emit source_stderr "$(head -c 400 "$errf")"
fi
rm -f "$errf"

# makepkg's own linting (only where makepkg exists and we are not root)
if command -v makepkg >/dev/null 2>&1 && (( EUID != 0 )); then
  if ! out=$(cd "$dir" && makepkg --printsrcinfo 2>&1 >/dev/null); then
    emit makepkg_lint_error "$(printf '%s' "$out" | grep -E 'ERROR|error' | head -3 | tr '\n' ' ' | cut -c1-400)"
  fi
fi

echo '@@end'
