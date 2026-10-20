#!/usr/bin/env python3
"""
report.py - final report of an auto-update run.

  report.py --state DIR --results DIR [--publish FILE] [--seen-in FILE]
            --seen-out FILE --out DIR [--run-url URL]

Writes into --out:
  report.json   everything, machine readable
  report.md     full report (all rows, build-log tails)
  issue.md      same sections, fewer rows per section so it fits a GitHub issue
and --seen-out: the status database for the next run
  packages{}    last update attempt per package (skip same failing version)
  builds{}      last build result of each package's *current* PKGBUILD
                (rolling audit + successful updates) -> whole-repo status

For every package that fails to build or has broken sources, the report shows
which BlackArch packages need it (depends / makedepends / checkdepends /
optdepends, per split package) and whether it can be removed safely.
"""
import argparse
import datetime as dt
import hashlib
import json
from collections import Counter
from pathlib import Path

CATS = ("build_failed", "source_broken", "pkgbuild_invalid")
FAILING = ("build_failed", "source_broken")
HARD = ("depends", "makedepends", "checkdepends")
TITLES = {
    "build_failed": "Packages that fail to build",
    "source_broken": "Packages with invalid / broken sources",
    "pkgbuild_invalid": "Packages with invalid or unexpected PKGBUILD structure",
}
SHORT = {"depends": "dep", "makedepends": "make", "checkdepends": "check", "optdepends": "opt"}


def esc(s) -> str:
    return str(s).replace("|", "\\|").replace("\n", " ").strip()


@__import__("functools").lru_cache(None)
def _detect():
    """rpmvercmp from detect.py (same directory)"""
    import importlib.util
    spec = importlib.util.spec_from_file_location("detect", Path(__file__).with_name("detect.py"))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def vercmp(a: str, b: str) -> int:
    """pacman's vercmp on full versions: [epoch:]pkgver[-pkgrel]"""
    rpm = _detect().rpmvercmp

    def split(v):
        e, _, rest = v.partition(":") if ":" in v else ("0", "", v)
        ver, _, rel = rest.rpartition("-") if "-" in rest else (rest, "", "")
        return int(e) if e.isdigit() else 0, ver, rel
    (ea, va, ra), (eb, vb, rb) = split(a), split(b)
    if ea != eb:
        return 1 if ea > eb else -1
    c = rpm(va, vb)
    if c or not ra or not rb:
        return c
    return rpm(ra, rb)


def update_text(kind: str, cur: str, new: str, repo: str = "", ship: bool = False) -> str:
    """VCS updates are detected by upstream commit, not by version; ship = the
    tree is ahead of the repo (cur is the pkgver, new the full version)"""
    if kind == "vcs":
        return f"{cur} → upstream commit {new[:7]}"
    if kind == "ship" or ship:
        return f"{repo or 'not in repo'} (repo) → {new} (tree)"
    return f"{cur} → {new}"


def load(path, default):
    p = Path(path) if path else None
    return json.loads(p.read_text()) if p and p.exists() else default


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--state", required=True)
    ap.add_argument("--results", required=True)
    ap.add_argument("--publish")
    ap.add_argument("--seen-in")
    ap.add_argument("--seen-out", required=True)
    ap.add_argument("--out", required=True)
    ap.add_argument("--run-url", default="")
    a = ap.parse_args()

    st, out, res_dir = Path(a.state), Path(a.out), Path(a.results)
    out.mkdir(parents=True, exist_ok=True)
    now = dt.datetime.now(dt.timezone.utc)
    today = now.isoformat(timespec="seconds")
    issues = load(st / "issues.json", [])
    plan = load(st / "updates.json", {})
    for k in ("updates", "audit", "carried", "deferred", "pending"):
        plan.setdefault(k, [])
    untracked = load(st / "untracked.json", {})
    tracking = load(st / "tracking.json", {})
    meta = load(st / "meta.json", {})
    graph = load(st / "depgraph.json", {})
    publish = load(a.publish, {})
    db = load(a.seen_in, {})
    seen, builds = db.get("packages", {}), db.get("builds", {})

    # ------------------------------------------------------------ results
    planned = {u["pkg"]: u for u in plan["updates"] + plan["audit"]}
    results = {}
    for f in res_dir.glob("*/status.json"):
        r = json.loads(f.read_text())
        results[r["pkg"]] = r
    for p, u in planned.items():   # shard died / was cancelled
        results.setdefault(p, {**u, "target": u["new"], "result": "skipped",
                               "reason": "no result (build job failed or was cancelled)"})

    # ------------------------------------------------------------ status db
    for p, r in results.items():
        if r["result"] == "skipped":
            continue
        if r["result"] == "no_change":   # remembered so the VCS clone isn't repeated daily
            seen[p] = {"target": r["target"], "result": "no_change", "reason": r["reason"],
                       "date": today, "run_url": a.run_url}
            continue
        rec = {"result": r["result"], "reason": r["reason"], "kind": r["kind"],
               "pkgver": r.get("pkgver") or r["cur"], "date": today, "run_url": a.run_url}
        f = res_dir / p / "PKGBUILD"
        if r["result"] == "ok" and r["kind"] != "test" and f.exists():
            # the bumped PKGBUILD is what goes to master
            builds[p] = {**rec, "sha": hashlib.sha1(f.read_bytes()).hexdigest()}
        elif r["kind"] in ("test", "rebuild"):
            builds[p] = {**rec, "sha": planned.get(p, {}).get("sha", "")}
        if r["kind"] != "test":
            seen[p] = {"target": r["target"], "result": r["result"], "reason": r["reason"],
                       "date": today, "run_url": a.run_url}
    builds = {p: b for p, b in builds.items() if p in meta}
    # updated OK this run: the fingerprint is of the bumped PKGBUILD, which only
    # reaches master after publishing, so count it as known too
    updated_now = {p for p, r in results.items() if r["result"] == "ok" and r["kind"] != "test"}
    known = {p: b for p, b in builds.items()
             if b.get("sha") == meta[p].get("sha") or p in updated_now}
    stale = len(builds) - len(known)

    # ------------------------------------------------------------ problem lists
    lists = {c: [] for c in CATS}
    this_run = set()
    for r in results.values():
        if r["result"] in CATS:
            this_run.add(r["pkg"])
            upd = "current PKGBUILD" if r["kind"] in ("test", "rebuild") else \
                update_text(r["kind"], r["cur"], planned.get(r["pkg"], {}).get("tree") or r["target"],
                            planned.get(r["pkg"], {}).get("repo", ""), bool(r.get("ship")))
            lists[r["result"]].append({"pkg": r["pkg"], "found_by": "build " + r["kind"],
                                       "severity": "error", "detail": r["reason"], "update": upd,
                                       "since": today[:10], "log_tail": r.get("log_tail", "")})
    for c in plan["carried"]:
        if c.get("result") in CATS:
            lists[c["result"]].append({"pkg": c["pkg"], "found_by": "update (earlier run)",
                                       "severity": "error", "detail": c.get("reason", ""),
                                       "update": update_text(c["kind"], c["cur"], c.get("tree") or c["new"],
                                                             c.get("repo", ""), bool(c.get("ship"))),
                                       "since": (c.get("date") or "")[:10], "log_tail": "",
                                       "run_url": c.get("run_url")})
    for p, b in known.items():
        if b["result"] in CATS and p not in this_run:
            lists[b["result"]].append({"pkg": p, "found_by": "audit build", "severity": "error",
                                       "detail": b["reason"], "update": "current PKGBUILD",
                                       "since": b["date"][:10], "log_tail": "",
                                       "run_url": b.get("run_url")})
    warnings = []
    for i in issues:
        if i["category"] in CATS:
            lists[i["category"]].append({"pkg": i["pkg"], "found_by": i["check"],
                                         "severity": i["severity"], "detail": i["message"],
                                         "update": "", "since": today[:10], "log_tail": ""})
        else:
            warnings.append(i)

    # ------------------------------------------------------------ reverse deps
    failing = {x["pkg"] for c in FAILING for x in lists[c] if x["severity"] == "error"}

    def rdeps(p):
        return [r for r in graph.get(p, {}).get("rdeps", []) if r["pkg"] != p]

    def verdict(p):
        rd = rdeps(p)
        hard = {r["pkg"] for r in rd if r["type"] in HARD}
        if not rd:
            return "safe", "✅ nothing in BlackArch needs it"
        if not hard:
            return "safe", "✅ only optional (optdepends)"
        blockers = sorted(hard - failing)
        if not blockers:
            return "with-dependents", "⚠️ only needed by failing packages: " + ", ".join(sorted(hard))
        return "needed", f"❌ needed by {len(blockers)} working package(s)"

    def needed_by(p, limit=8):
        names = graph.get(p, {}).get("names", {})
        parts = []
        multi = len(names) > 1
        for n, rs in sorted(names.items()):
            rs = [r for r in rs if r["pkg"] != p]
            if not rs and not multi:
                continue
            items = []
            for r in rs[:limit]:
                mark = "❌" if r["pkg"] in failing else ""
                items.append(f'{mark}{r["pkg"]} ({SHORT.get(r["type"], r["type"])})')
            if len(rs) > limit:
                items.append(f"+{len(rs) - limit} more")
            parts.append((f"`{n}` ← " if multi else "") + (", ".join(items) if items else "nothing"))
        prov = [r for r in rdeps(p) if r["name"] not in names]
        if prov:   # via provides=()
            parts.append("via provides: " + ", ".join(f'{r["pkg"]} ({SHORT.get(r["type"])})'
                                                     for r in prov[:limit]))
        return "; ".join(parts) or "-"

    for c in CATS:
        for x in lists[c]:
            x["needed_by"] = [{k: r[k] for k in ("pkg", "type", "name", "via")} for r in rdeps(x["pkg"])]
            x["removable"], x["verdict"] = verdict(x["pkg"])
        lists[c].sort(key=lambda x: (x["severity"] != "error", x["pkg"]))

    removal = []
    for p in sorted(failing):
        kind, text = verdict(p)
        if kind != "needed":
            probs = sorted({c for c in FAILING for x in lists[c] if x["pkg"] == p})
            removal.append({"pkg": p, "problem": ", ".join(probs), "verdict": text,
                            "kind": kind, "produces": sorted(graph.get(p, {}).get("names", {})),
                            "needed_by": needed_by(p)})

    py2 = []
    for d, g in sorted(graph.items()):
        for n, rs in sorted(g["names"].items()):
            if not n.startswith("python2-"):
                continue
            rs = [r for r in rs if r["pkg"] != d]
            if any(r["type"] in HARD for r in rs):
                continue
            others = sorted(x for x in g["names"] if x != n)
            b = known.get(d)
            py2.append({"name": n, "dir": d, "also_builds": others,
                        "build": (b["result"] if b else "unknown") if d not in failing else "failing",
                        "optdepends_of": sorted({r["pkg"] for r in rs}),
                        "action": (f"drop package_{n}() from {d}" if others
                                   else f"remove package {d}")})

    # ------------------------------------------------------------ duplicates of Arch packages
    arch = {}
    for line in ((st / "arch-official.txt").read_text().splitlines()
                 if (st / "arch-official.txt").exists() else []):
        f = line.split()
        if len(f) >= 3:
            arch.setdefault(f[1], (f[0], f[2]))
    official_list = {}
    for line in ((st / "lists-official.txt").read_text().splitlines()
                 if (st / "lists-official.txt").exists() else []):
        f = line.split("#", 1)[0].split()
        if f:
            official_list[f[0]] = f[1:]
    in_arch = []
    for d, m in sorted(meta.items()):
        outs = m.get("pkgname") or [d]
        dup = [n for n in outs if n in arch]
        listed = [n for n in outs if n in official_list]
        if not dup and not listed:
            continue
        ba_full = ((m.get("epoch") + ":") if m.get("epoch") else "") + \
            f"{m.get('pkgver', '')}-{m.get('pkgrel', '')}"
        groups = [g for g in m.get("groups", []) if g != "blackarch"]
        for n in sorted(set(dup) | set(listed)):
            repo, aver = arch.get(n, ("", ""))
            if aver and "has_pkgver_func" in m:
                newer = "not comparable (BlackArch builds from git)"
            elif aver:
                c = vercmp(aver, ba_full)
                newer = "Arch is newer" if c > 0 else ("same" if c == 0 else "BlackArch is newer")
            else:
                newer = "not in Arch's repos"
            nb = len([r for r in graph.get(d, {}).get("names", {}).get(n, []) if r["pkg"] != d])
            whole = set(outs) <= set(dup)
            if not aver:
                action = (f"remove `{n}` from lists/official: it is not in Arch's repos, "
                          f"and BlackArch builds it itself")
            elif whole:
                action = (f"remove packages/{d}; add `{n} {' '.join(groups)}` to lists/official"
                          if n not in official_list else f"remove packages/{d} (already in lists/official)")
            else:
                action = f"drop package_{n}() from {d}; add `{n} {' '.join(groups)}` to lists/official"
            in_arch.append({"pkg": n, "dir": d, "blackarch": ba_full, "arch_repo": repo,
                            "arch": aver, "compare": newer, "in_lists_official": n in official_list,
                            "blackarch_dependents": nb, "action": action.replace("  ", " ")})

    committed = set(publish.get("committed", []))
    shipped = set(publish.get("shipped", []))
    if "released_pkgs" in publish:
        published = set(publish["released_pkgs"])
    else:   # older publish.json
        published = committed if publish.get("released") else set()
    ok = sorted((r for r in results.values() if r["result"] == "ok" and r["kind"] != "test"),
                key=lambda r: r["pkg"])
    audit_ok = sum(1 for r in results.values() if r["kind"] == "test" and r["result"] == "ok")
    oldest = min((b["date"] for b in known.values()), default="")[:10]

    totals = {
        "packages": len(meta), "tracked": len(tracking), "untracked": len(untracked),
        "updates_planned": len(plan["updates"]), "deferred": len(plan["deferred"]),
        "waiting_in_pr": len(plan["pending"]), "audit_builds": len(plan["audit"]),
        "audit_ok": audit_ok, "build_status_known": len(known), "build_status_stale": stale,
        "built_ok": len(ok), "committed": len(committed), "shipped": len(shipped),
        "released": len(published),
        "no_change": sum(r["result"] == "no_change" for r in results.values()),
        "skipped": sum(r["result"] == "skipped" for r in results.values()),
        **{c: len({x["pkg"] for x in lists[c]}) for c in CATS},
        "removal_candidates": len(removal), "python2_unused": len(py2),
        "also_in_arch": len({x["dir"] for x in in_arch}),
    }
    report = {"generated": today, "run_url": a.run_url, "totals": totals, **lists,
              "removal_candidates": removal, "python2_without_hard_dependents": py2,
              "also_in_arch_official": in_arch,
              "updated": [{"pkg": r["pkg"], "kind": r["kind"],
                           "from": planned.get(r["pkg"], {}).get("repo", "") if r["kind"] == "ship" else r["cur"],
                           "to": r["target"] if r["kind"] == "ship" else (r.get("pkgver") or r["target"]),
                           "committed": r["pkg"] in committed, "shipped": r["pkg"] in shipped,
                           "released": r["pkg"] in published}
                          for r in ok],
              "publish": publish, "warnings": warnings,
              "untracked_by_reason": Counter(untracked.values()).most_common(),
              "untracked": untracked}
    (out / "report.json").write_text(json.dumps(report, indent=1))

    # ------------------------------------------------------------ markdown
    def md(full: bool, lim: int | None = None) -> str:
        """full=True: every row and log tail; otherwise at most `lim` rows per section"""
        if full:
            lim = None
        t = totals
        L = [f"# BlackArch auto-update report - {now:%Y-%m-%d}", ""]
        if a.run_url:
            L += [f"Run: {a.run_url} - full report and logs in the `auto-update-report` artifact", ""]
        L += ["| | |", "|---|---:|",
              f"| Packages / tracked upstream | {t['packages']} / {t['tracked']} |",
              f"| Updates found (deferred to next run) | {t['updates_planned']} ({t['deferred']}) |",
              f"| Built + install-tested OK | {t['built_ok']} |",
              f"| Committed / shipped as-is / released | {t['committed']} / {t['shipped']} / {t['released']} |",
              f"| Waiting in open auto-update PRs | {t['waiting_in_pr']} |",
              f"| Audit builds this run (OK) | {t['audit_builds']} ({t['audit_ok']}) |",
              f"| Build status known for current PKGBUILDs | {t['build_status_known']} / {t['packages']}"
              + (f" (oldest {oldest})" if oldest else "") + " |",
              f"| ❌ Fail to build | {t['build_failed']} |",
              f"| 🔗 Broken sources | {t['source_broken']} |",
              f"| 🧩 Invalid PKGBUILD structure | {t['pkgbuild_invalid']} |",
              f"| 🗑️ Failing and safe(ish) to remove | {t['removal_candidates']} |",
              f"| 🐍 python2 packages nothing hard-depends on | {t['python2_unused']} |",
              f"| 📦 Also in Arch official repositories | {t['also_in_arch']} |", ""]
        if publish.get("pr_url"):
            L += [f"> 📬 **Pull request:** {publish['pr_url']} - packages are released when it is merged.", ""]
        if publish.get("release_error"):
            L += [f"> ⚠️ **Release:** {publish['release_error']}", ""]

        for c, icon in zip(CATS, ("❌", "🔗", "🧩")):
            rows = lists[c]
            L += [f"## {icon} {TITLES[c]} ({len({x['pkg'] for x in rows})})", ""]
            if not rows:
                L += ["None 🎉", ""]
                continue
            if c in FAILING:
                L += ["| Package | Found by | Update | Details | Needed by (in BlackArch) | Removable? |",
                      "|---|---|---|---|---|---|"]
                for x in rows[:lim]:
                    L.append(f"| `{x['pkg']}` | {esc(x['found_by'])} (since {x['since']}) | {esc(x['update'])} "
                             f"| {esc(x['detail'])[:220]} | {esc(needed_by(x['pkg']))} | {esc(x['verdict'])} |")
            else:
                L += ["| Package | Severity | Check | Details |", "|---|---|---|---|"]
                for x in rows[:lim]:
                    L.append(f"| `{x['pkg']}` | {x['severity']} | {esc(x['found_by'])} | {esc(x['detail'])[:260]} |")
            L.append("")
            if lim and len(rows) > lim:
                L += [f"_… {len(rows) - lim} more in report.md / report.json_", ""]
            if full:
                for x in rows:
                    if x.get("log_tail"):
                        L += [f"<details><summary><code>{x['pkg']}</code> build log tail</summary>",
                              "", "```", x["log_tail"], "```", "</details>", ""]

        L += [f"## 🗑️ Removal candidates ({len(removal)})", "",
              "Failing packages (build or sources) that no *working* BlackArch package hard-depends on.", ""]
        if removal:
            L += ["| Package | Problem | Produces | Verdict | Needed by |", "|---|---|---|---|---|"]
            for x in removal[:lim]:
                L.append(f"| `{x['pkg']}` | {x['problem']} | {', '.join(x['produces'])} | {esc(x['verdict'])} "
                         f"| {esc(x['needed_by'])} |")
        L.append("")
        if lim and len(removal) > lim:
            L += [f"_… {len(removal) - lim} more in report.md / report.json_", ""]

        L += [f"## 🐍 Python 2 packages that nothing hard-depends on ({len(py2)})", "",
              "No depends/makedepends/checkdepends on them anywhere in the tree "
              "(split packages are checked one by one).", ""]
        if py2:
            L += ["| Package | From PKGBUILD | Also builds | Latest build | optdepends of | Suggested action |",
                  "|---|---|---|---|---|---|"]
            for x in py2[:lim]:
                L.append(f"| `{x['name']}` | {x['dir']} | {', '.join(x['also_builds']) or '-'} | {x['build']} "
                         f"| {', '.join(x['optdepends_of']) or '-'} | {x['action']} |")
        L.append("")
        if lim and len(py2) > lim:
            L += [f"_… {len(py2) - lim} more in report.md / report.json_", ""]

        L += [f"## 📦 Also in Arch official repositories ({t['also_in_arch']})", "",
              "Packages built here that Arch also ships in core/extra/multilib, or that are "
              "both built here and listed in `lists/official`. Usually the BlackArch copy can be "
              "dropped in favour of `lists/official`; check first whether it is kept on purpose "
              "(newer, patched, or an unrelated program with the same name). Dependents keep "
              "working: the package name stays the same.", ""]
        if in_arch:
            L += ["| Package | PKGBUILD | BlackArch | Arch | | In lists/official | BlackArch dependents | Suggested action |",
                  "|---|---|---|---|---|---|---|---|"]
            for x in in_arch[:lim]:
                arch_col = f"{x['arch']} ({x['arch_repo']})" if x["arch"] else "-"
                L.append(f"| `{x['pkg']}` | {x['dir']} | {esc(x['blackarch'])} | {esc(arch_col)} | {x['compare']} "
                         f"| {'yes' if x['in_lists_official'] else 'no'} | {x['blackarch_dependents']} "
                         f"| {esc(x['action'])} |")
        L.append("")
        if lim and len(in_arch) > lim:
            L += [f"_… {len(in_arch) - lim} more in report.md / report.json_", ""]

        L += [f"## ✅ Updated ({len(ok)})", ""]
        if ok:
            L += ["| Package | From | To | Released |", "|---|---|---|---|"]
            for r in ok[:lim]:
                state = "yes" if r["pkg"] in published else (
                    ("in PR" if publish.get("pr_url") else "committed") if r["pkg"] in committed else "no")
                if r["kind"] == "ship" or (r.get("ship") and r["pkg"] in shipped):
                    frm = (planned.get(r["pkg"], {}).get("repo") or "not in repo") + " (repo)"
                    to = (planned.get(r["pkg"], {}).get("tree") or r["target"]) + " (tree, shipped as-is)"
                else:
                    frm, to = r["cur"], r.get("pkgver") or r["target"]
                L.append(f"| `{r['pkg']}` | {esc(frm)} | {esc(to)} | {state} |")
        L.append("")
        if lim and len(ok) > lim:
            L += [f"_… {len(ok) - lim} more in report.md / report.json_", ""]
        if publish.get("skipped"):
            L += ["**Not published:** " + "; ".join(publish["skipped"]), ""]
        if warnings:
            wc = Counter(w["check"] for w in warnings)
            L += [f"<details><summary>Upstream-check warnings ({len(warnings)})</summary>", ""]
            L += [f"- {k}: {v}" for k, v in wc.most_common()]
            if full:
                L += ["", "| Package | Check | Message |", "|---|---|---|"]
                L += [f"| `{w['pkg']}` | {w['check']} | {esc(w['message'])[:250]} |" for w in warnings]
            L += ["</details>", ""]
        L += [f"<details><summary>Packages not tracked upstream ({len(untracked)}) - "
              "add entries to .github/ci/nvchecker-overrides.toml</summary>", ""]
        if full:   # report.md: the package names, grouped by reason
            by_reason = {}
            for pkg, r in sorted(untracked.items()):
                by_reason.setdefault(r, []).append(pkg)
            for r, n in Counter(untracked.values()).most_common():
                L += [f"- **{r}: {n}**", "  " + ", ".join(f"`{x}`" for x in by_reason[r])]
            L += ["", "The issue shows only the counts; see also `untracked.json` in this artifact.", "</details>", ""]
        else:
            L += [f"- {r}: {n}" for r, n in Counter(untracked.values()).most_common(25)]
            L += ["", "_Package names per reason: `report.md` in the `auto-update-report` artifact._", "</details>", ""]
        return "\n".join(L)

    (out / "report.md").write_text(md(True))
    # GitHub issue bodies are limited to 65536 characters. Keep every section
    # and shrink the number of rows shown per section until the text fits.
    issue_limit = 60000
    for rows in (100, 75, 50, 35, 25, 15, 10, 5, 3, 1):
        issue = md(False, rows)
        if len(issue) <= issue_limit:
            break
    if rows < 100:
        issue = issue.replace("\n## ", f"\n> Showing at most {rows} rows per section to fit "
                              "the issue; the full lists are in report.md / report.json in the "
                              "`auto-update-report` artifact.\n\n## ", 1)
    if len(issue) > issue_limit:      # last resort: even 1 row per section is too long
        issue = issue[:issue_limit - 1000] + "\n\n… truncated, see the report artifact.\n"
    (out / "issue.md").write_text(issue)

    cutoff = now - dt.timedelta(days=60)
    seen = {k: v for k, v in seen.items() if dt.datetime.fromisoformat(v["date"]) > cutoff}
    Path(a.seen_out).write_text(json.dumps({"packages": seen, "builds": builds},
                                           indent=1, sort_keys=True))
    print(json.dumps(totals))


if __name__ == "__main__":
    main()
