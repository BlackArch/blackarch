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
  [[ -n ${PR_BASE_SHA:-} ]] || { echo "PR_BASE_SHA is not set"; exit 1; }
  git fetch -q --depth=1 origin "$PR_BASE_SHA" 2>/dev/null || true
  git cat-file -e "$PR_BASE_SHA^{commit}" 2>/dev/null || { echo "cannot fetch the PR base commit $PR_BASE_SHA"; exit 1; }
  marker=$(printf '%s\n' "${PR_BODY:-}" | sed -nE 's/.*<!-- remove-packages: ([^>]*) -->.*/\1/p' | head -n1)
  marker=$(printf '%s' "$marker" | tr -cs 'A-Za-z0-9._+@=,-' ' ')
  [[ -n $marker ]] || { echo "no remove-packages marker in the PR body"; exit 1; }
  # The marker was written when the PR was opened and the body is editable:
  # only remove what this PR really deleted (PKGBUILD removed in the merged
  # diff) and what is really gone from the base branch after the merge.
  mapfile -t deleted < <(
    gh api --paginate "repos/$GH_REPO/pulls/$PR_NUMBER/files" -q '.[] | select(.status == "removed") | .filename' |
      sed -nE 's|^packages/([^/]+)/PKGBUILD$|\1|p' | sort -u)
  names="" kept=""
  # A PKGBUILD "declares" a name if a top-level pkgname= line or a package_<name>()
  # function mentions it. Pure text matching: nothing from the tree is evaluated
  # in this job (it holds the signing key).
  _declares() { # <name>, PKGBUILD text on stdin
    local q; q=$(printf '%s' "$1" | sed 's/[][\\.^$*+?(){}|]/\\&/g')
    grep -qE "^pkgname=\\(?[[:space:]]*([\"']?[^\"' )]+[\"']?[[:space:]]+)*[\"']?${q}[\"']?([[:space:]]|\\)|$)|^package_${q}[[:space:]]*\\(\\)"
  }
  mapfile -t tree_pkgbuilds < <(ls packages/*/PKGBUILD)
  _still_declared() { # <name>: some PKGBUILD still on the base branch declares it
    local f; for f in "${tree_pkgbuilds[@]}"; do _declares "$1" < "$f" && return 0; done; return 1
  }
  for item in $marker; do
    dir=${item%%=*}; list=${item#*=}
    [[ $item == *=* ]] || { dir=$item; list=$item; }      # old marker format: names only
    if printf '%s\n' "${deleted[@]}" | grep -qxF -- "$dir" && [[ ! -d packages/$dir ]]; then
      # the names must come from the deleted PKGBUILD (as it was before the
      # merge) and must not be produced by anything left in the tree
      old_pkgbuild=$(git show "$PR_BASE_SHA":"packages/$dir"/PKGBUILD 2>/dev/null || true)
      for n in ${list//,/ }; do
        if [[ -n $old_pkgbuild ]] && _declares "$n" <<<"$old_pkgbuild" && ! _still_declared "$n"; then
          names+=" $n"
        else
          kept+=" $n"
        fi
      done
    else
      kept+=" ${list//,/ }"
    fi
  done
  names=${names# }; kept=${kept# }
  [[ -z $kept ]] || echo "::warning::not removed from the repo (restored during review, still produced by the tree, or not declared by the deleted PKGBUILD): $kept"
  if [[ -z $names ]]; then
    echo "ℹ️ Nothing removed from the package repository: the packages are still in the tree ($kept)." > "$comment"
    gh pr comment "$PR_NUMBER" --body-file "$comment" || true
    exit 0
  fi
  rc=0
  # shellcheck disable=SC2086
  if release_setup && remove_packages $names; then
    echo "✅ Removed from the package repository: $names" > "$comment"
    [[ -z $kept ]] || echo "Not removed (still in the tree after the merge): $kept" >> "$comment"
  else
    rc=1; echo "⚠️ Could not remove from the package repository: ${release_error:-unknown error}. Packages: $names. Remove them manually with \`scripts/barelease --remove\`." > "$comment"
  fi
  gh pr comment "$PR_NUMBER" --body-file "$comment" || true
  exit "$rc"
fi

# BUILD_SHA: the commit the originating run built from (from the validated
# run, not from the editable PR body). Required: without it the directory
# comparison below could not tell whether patches/.install changed.
[[ -n ${BUILD_SHA:-} ]] || { echo "BUILD_SHA is not set"; exit 1; }
git fetch -q --depth=1 origin "$BUILD_SHA" 2>/dev/null || true
git cat-file -e "$BUILD_SHA^{commit}" 2>/dev/null || { echo "cannot fetch the build commit $BUILD_SHA"; exit 1; }
mapfile -t changed < <(
  gh api --paginate "repos/$GH_REPO/pulls/$PR_NUMBER/files" -q '.[].filename' |
    sed -nE 's|^packages/([^/]+)/PKGBUILD$|\1|p' | sort -u)

ship=() manual=()
for pkg in "${changed[@]}"; do
  # PKGBUILD must be the built one, and nothing else in the directory (patches,
  # .install files) may have changed since the PR's base commit: a reviewer's
  # fix in the PR, or another commit on the base branch, was never built
  if [[ -f packages/$pkg/PKGBUILD && -f $RESULTS/$pkg/PKGBUILD ]] &&
     git diff --no-index --quiet "packages/$pkg/PKGBUILD" "$RESULTS/$pkg/PKGBUILD" &&
     git diff --quiet "$BUILD_SHA" HEAD -- "packages/$pkg" ":(exclude)packages/$pkg/PKGBUILD" &&
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
