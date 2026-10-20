#!/usr/bin/env bash
# remove-prepare.sh - manual "remove packages" workflow, stage 1.
#
# Deletes the source directories of the given packages on a branch and opens
# ONE pull request. Nothing is removed from the package repository until that
# PR is merged (release-merged.sh handles the remove/* branch then).
#
#   remove-prepare.sh <pkgtree-dir> "<name> <name> ..."
#
# Env: BASE_BRANCH GH_TOKEN GH_REPO RUN_ID RUN_URL META (meta.json of the
#      current tree, for the dependency check) [PKGMAP] (nested layout)
set -euo pipefail
tree=$1; names=$2
BASE_BRANCH=${BASE_BRANCH:-main}

declare -A PKGDIR
if [[ -n ${PKGMAP:-} ]]; then
  while read -r n d; do [[ -n $n ]] && PKGDIR[$n]=$d; done < "$PKGMAP"
else
  for d in "$tree"/*/; do d=${d%/}; PKGDIR[${d##*/}]=$d; done
fi

todo=() missing=()
for n in $names; do
  if [[ -n ${PKGDIR[$n]:-} && -d ${PKGDIR[$n]} ]]; then todo+=("$n"); else missing+=("$n"); fi
done
(( ${#missing[@]} )) && echo "::warning::not in the tree, ignored: ${missing[*]}"
(( ${#todo[@]} )) || { echo "::error::none of the given packages exist"; exit 1; }

# every package name each PKGBUILD produces (split packages) and who depends
# on them, from the metadata the detect stage already produced
info=$(python3 - "$META" "${todo[@]}" <<'EOF'
import json, re, sys
meta = json.load(open(sys.argv[1])); dirs = sys.argv[2:]
def dep_name(d): return re.split(r"[<>=:]", d, 1)[0].strip()
produced = {d: (meta.get(d, {}).get("pkgname") or [d]) for d in dirs}
removed = {n for v in produced.values() for n in v}
# names these packages provide (top level and per split package) count too,
# unless another package that stays in the tree produces or provides them
def provided(d):
    m = meta.get(d, {})
    out = {dep_name(x) for x in m.get("provides", [])}
    out |= {dep_name(x.split(" ", 1)[1]) for x in m.get("sdep_provides", []) if " " in x}
    return out
others = set()
for y, m in meta.items():
    if y not in dirs:
        others |= set(m.get("pkgname") or [y]) | provided(y)
for d in dirs:
    removed |= provided(d) - others
users = {}
for y, m in meta.items():
    if y in dirs: continue
    for t in ("depends", "makedepends", "checkdepends", "optdepends"):
        for dep in m.get(t, []) + [x.split(" ", 1)[1] for x in m.get("sdep_" + t, []) if " " in x]:
            n = dep_name(dep)
            if n in removed: users.setdefault(n, set()).add(f"{y} ({t})")
print(json.dumps({"produced": produced, "users": {k: sorted(v) for k, v in users.items()}}))
EOF
)

git config --global --add safe.directory "$PWD"
git config user.name  "${GIT_USER_NAME:-ci[bot]}"
git config user.email "${GIT_USER_EMAIL:-ci@example.invalid}"
git fetch -q origin "$BASE_BRANCH"
branch="remove/$(date -u +%Y-%m-%d)-$RUN_ID"
git checkout -q -B "$branch" "origin/$BASE_BRANCH"
for n in "${todo[@]}"; do git rm -rq -- "${PKGDIR[$n]}"; done
# BlackArch: drop the names from the plain package lists too
if [[ -d lists ]]; then
  # drop lines whose first word is a removed package: literal comparison, no
  # regex (names may contain '+' or '.')
  for l in lists/to-release lists/to-release-arm lists/broken lists/to-remove lists/python2-tools; do
    [[ -f $l ]] || continue
    awk -v names="${todo[*]}" 'BEGIN { n = split(names, a, " "); for (i = 1; i <= n; i++) rm[a[i]] = 1 }
                               !($1 in rm)' "$l" > "$l.new" && mv "$l.new" "$l"
  done
  git add lists
fi
git commit -q -m "remove ${todo[*]}" -m "Requested via the remove-packages workflow: $RUN_URL"
git push -q origin "HEAD:refs/heads/$branch"

body=$(mktemp)
{
  # <dir>=<name>,<name>: release-merged.sh removes the names only if this PR
  # really deleted <dir>/PKGBUILD and the directory is gone after the merge
  echo "<!-- remove-packages: $(python3 -c 'import json,sys; d=json.loads(sys.argv[1])["produced"]; print(" ".join(k + "=" + ",".join(sorted(v)) for k, v in sorted(d.items())))' "$info") -->"
  echo "Removes **${#todo[@]}** package(s) from the tree ([run]($RUN_URL))."
  echo
  echo "**Merging this PR also removes the packages below from the package repository** (database entries and files)."
  echo
  echo "| Package directory | Packages removed from the repo |"; echo "|---|---|"
  python3 -c 'import json,sys
d=json.loads(sys.argv[1])
for k,v in d["produced"].items():
    names = ", ".join("`" + n + "`" for n in v)
    print("| `" + k + "` | " + names + " |")' "$info"
  u=$(python3 -c 'import json,sys; d=json.loads(sys.argv[1])["users"]
for n,w in d.items(): print("- `" + n + "` is still needed by: " + ", ".join(w))' "$info")
  if [[ -n $u ]]; then
    echo; echo "> ⚠️ **Other packages still depend on these.** Fix or remove them first, or they will stop installing:"; echo; echo "$u"
  else
    echo; echo "No other package in the tree depends on them."
  fi
} > "$body"
gh label create remove-package --color b60205 --description "Package removal" 2>/dev/null || true
url=$(gh pr create --base "$BASE_BRANCH" --head "$branch" --label remove-package \
  --title "remove: ${todo[*]}" --body-file "$body")
echo "opened $url"
echo "::notice::$url"
