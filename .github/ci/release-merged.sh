#!/usr/bin/env bash
# release-merged.sh - runs when an auto-update PR is merged
# (workflow auto-update-release.yml). Uploads the packages that the original
# run built, but only for PKGBUILDs that reached master exactly as built.
# Anything changed during review, or whose build artifacts expired, goes to
# lists/to-release for the normal manual flow.
#
# Env: PR_NUMBER RESULTS PKGS GH_TOKEN GH_REPO + release env (lib-release.sh)
set -euo pipefail
here=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
# shellcheck source=lib-release.sh
. "$here/lib-release.sh"

released=false release_error=""
mkdir -p "$RESULTS" "$PKGS"
RESULTS=$(realpath "$RESULTS") PKGS=$(realpath "$PKGS")
export PKGS
unpack_pkgs || { echo "cannot unpack the built packages"; exit 1; }
comment=$(mktemp)
git config --global --add safe.directory "$PWD"
git config user.name  "blackarch-ci[bot]"
git config user.email "team@blackarch.org"
git fetch -q origin master
git checkout -q -B master origin/master

# ---- a merged remove/* PR: remove the listed packages from the repo
if [[ ${HEAD_REF:-} == remove/* ]]; then
  names=$(printf '%s\n' "${PR_BODY:-}" | sed -nE 's/.*<!-- remove-packages: ([^>]*) -->.*/\1/p' | head -n1)
  names=$(printf '%s' "$names" | tr -cs 'A-Za-z0-9._+@-' ' ')
  [[ -n $names ]] || { echo "no remove-packages marker in the PR body"; exit 1; }
  rc=0
  # shellcheck disable=SC2086
  if release_setup && remove_packages $names; then
    echo "✅ Removed from the package repository: $names" > "$comment"
  else
    rc=1; echo "⚠️ Could not remove from the package repository: ${release_error:-unknown error}. Packages: $names. Remove them manually with \`scripts/barelease --remove\`." > "$comment"
  fi
  gh pr comment "$PR_NUMBER" --body-file "$comment" || true
  exit "$rc"
fi

mapfile -t changed < <(
  gh api --paginate "repos/$GH_REPO/pulls/$PR_NUMBER/files" -q '.[].filename' |
    sed -nE 's|^packages/([^/]+)/PKGBUILD$|\1|p' | sort -u)

ship=() manual=()
for pkg in "${changed[@]}"; do
  if [[ -f packages/$pkg/PKGBUILD && -f $RESULTS/$pkg/PKGBUILD ]] &&
     git diff --no-index --quiet "packages/$pkg/PKGBUILD" "$RESULTS/$pkg/PKGBUILD" &&
     compgen -G "$PKGS/$pkg/*.pkg.tar.zst" >/dev/null; then
    ship+=("$pkg")
  elif [[ -f packages/$pkg/PKGBUILD ]]; then
    manual+=("$pkg")
  fi
done
echo "release: ${#ship[@]}  manual: ${#manual[@]}"

rc=0
release_packages "${ship[@]}" || rc=1
if (( ${#manual[@]} )); then
  queue_for_humans "auto-update PR #$PR_NUMBER: changed in review or artifacts expired" \
    "${manual[@]}" || rc=1
fi

{
  if [[ $released == true ]]; then
    echo "✅ Released ${#ship[@]} packages to the repo."
  elif (( ${#ship[@]} )); then
    echo "⚠️ Packages were not released: ${release_error:-unknown error}"
  fi
  if (( ${#manual[@]} )); then
    echo
    echo "Added to \`lists/to-release\` (changed during review or build artifacts expired):"
    printf -- '- `%s`\n' "${manual[@]}"
  fi
} > "$comment"
[[ -s $comment ]] && gh pr comment "$PR_NUMBER" --body-file "$comment" || true
exit "$rc"
