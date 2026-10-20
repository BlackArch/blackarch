#!/usr/bin/env python3
"""
detect.py - upstream tracking, PKGBUILD linting and work planning for the
BlackArch auto-update pipeline.  Standard library only (Python >= 3.11).

  detect.py scan  --meta FILE --out DIR [--exclude FILE ...] [--overrides FILE]
      Parses the output of meta-one.sh for every package, lints it, decides
      how each package can be tracked upstream and writes:
        DIR/meta.json       parsed metadata, one entry per package
        DIR/tracking.json   pkg -> {kind, cur, old, via}  (tracked packages)
        DIR/untracked.json  pkg -> reason                 (not tracked)
        DIR/issues.json     PKGBUILD structure problems
        DIR/nvchecker.toml  generated nvchecker configuration
        DIR/oldver.json     nvchecker v2 oldver built from the PKGBUILDs

  detect.py plan  --state DIR [--seen FILE] [--force "a b"] [--max N]
                  [--per-shard N] [--max-shards N] [--retry-failed]
      Compares DIR/newver.json against the PKGBUILDs, folds nvchecker errors
      into DIR/issues.json and writes DIR/updates.json, DIR/shards/<i>.json
      and the GitHub matrix to $GITHUB_OUTPUT.

No state is committed to git: "old" versions always come from the PKGBUILDs
themselves, so a failed update is simply seen again on the next run.
"""

from __future__ import annotations

import argparse
import datetime as dt
import json
import math
import os
import re
import sys
import tomllib
import urllib.parse
from pathlib import Path

LIST_KEYS = {"pkgname", "arch", "groups", "license", "makedepends", "source",
             "has_split_package_func", "depends", "checkdepends", "optdepends",
             "provides", "soverride", "sdep_depends", "sdep_optdepends",
             "sdep_provides"}
HARD_DEPS = ("depends", "makedepends", "checkdepends")
VALID_PKGVER = re.compile(r"[A-Za-z0-9._+]+")
VALID_PKGREL = re.compile(r"[0-9]+(\.[0-9]+)?")
VER_RE = r"[0-9][0-9A-Za-z._+]*"
PRERELEASE = r"(?i).*(alpha|beta|[^a-z]rc[0-9]*|dev|pre|preview|nightly|snapshot|test).*"
# trailing commit hash in a VCS pkgver: 21.59db436, 1.2.r5.g59db436, r123.59db436
HASH_IN_PKGVER = re.compile(r"(?:^|[._+])g?([0-9a-f]{7,40})$")
FAILED_RESULTS = {"build_failed", "source_broken", "pkgbuild_invalid"}
SAFE_URL = re.compile(r"(https?|git|ssh)://[A-Za-z0-9._~:/@%+=-]+")
SAFE_REF = re.compile(r"[A-Za-z0-9._/+-]+")


# --------------------------------------------------------------------------
# vercmp - port of pacman/libalpm rpmvercmp(), so no pacman is needed here
# --------------------------------------------------------------------------
def _isalnum(c: str) -> bool:
    return c.isascii() and c.isalnum()


def _isdigit(c: str) -> bool:
    return "0" <= c <= "9"


def _isalpha(c: str) -> bool:
    return c.isascii() and c.isalpha()


def rpmvercmp(a: str, b: str) -> int:
    if a == b:
        return 0
    i = j = 0
    la, lb = len(a), len(b)
    while i < la and j < lb:
        si, sj = i, j
        while i < la and not _isalnum(a[i]):
            i += 1
        while j < lb and not _isalnum(b[j]):
            j += 1
        if i >= la or j >= lb:
            break
        if (i - si) != (j - sj):
            return -1 if (i - si) < (j - sj) else 1
        p1, p2 = i, j
        if _isdigit(a[p1]):
            while i < la and _isdigit(a[i]):
                i += 1
            while j < lb and _isdigit(b[j]):
                j += 1
            isnum = True
        else:
            while i < la and _isalpha(a[i]):
                i += 1
            while j < lb and _isalpha(b[j]):
                j += 1
            isnum = False
        s1, s2 = a[p1:i], b[p2:j]
        if not s2:
            return 1 if isnum else -1
        if isnum:
            s1, s2 = s1.lstrip("0"), s2.lstrip("0")
            if len(s1) != len(s2):
                return 1 if len(s1) > len(s2) else -1
        if s1 != s2:
            return 1 if s1 > s2 else -1
    if i >= la and j >= lb:
        return 0
    if (i >= la and not _isalpha(b[j])) or (i < la and _isalpha(a[i])):
        return -1
    return 1


def suspicious_jump(cur: str, new: str) -> bool:
    """1.2 -> 2019 (a date or unrelated tag), or 20190101 -> 3.0"""
    a, b = re.match(r"[0-9]*", cur).group(0), re.match(r"[0-9]*", new).group(0)
    if not a or not b:
        return False
    return (len(a) <= 3 and len(b) >= 4) or (len(a) >= 4 and len(b) <= 3)


def vercmp(a: str, b: str) -> int:
    """pacman's vercmp on full versions: [epoch:]pkgver[-pkgrel]"""
    def split(v):
        e, _, rest = v.partition(":") if ":" in v else ("0", "", v)
        ver, _, rel = rest.rpartition("-") if "-" in rest else (rest, "", "")
        return int(e) if e.isdigit() else 0, ver, rel
    (ea, va, ra), (eb, vb, rb) = split(a), split(b)
    if ea != eb:
        return 1 if ea > eb else -1
    c = rpmvercmp(va, vb)
    if c or not ra or not rb:
        return c
    return rpmvercmp(ra, rb)


def full_version(m: dict) -> str:
    e = m.get("epoch") or ""
    return (f"{e}:" if e else "") + f"{m.get('pkgver', '')}-{m.get('pkgrel', '')}"


def load_repo_db(path) -> dict[str, str] | None:
    """'name version' per line (what the workflow extracted from the repo db);
    None when the file does not exist = the db could not be fetched this run"""
    p = Path(path) if path else None
    if not p or not p.exists():
        return None
    repo: dict[str, str] = {}
    for line in p.read_text().splitlines():
        parts = line.split()
        if len(parts) == 2:
            repo[parts[0]] = parts[1]
    return repo


def buildable(m: dict) -> bool:
    """the CI only builds x86_64 (any packages included)"""
    return not m.get("arch") or bool({"any", "x86_64"} & set(m["arch"]))


# --------------------------------------------------------------------------
# metadata parsing
# --------------------------------------------------------------------------
def parse_meta(path: Path) -> dict[str, dict]:
    pkgs: dict[str, dict] = {}
    cur: dict = {}
    for line in path.read_text(errors="replace").splitlines():
        if line == "@@end":
            if "dir" in cur:
                pkgs[cur["dir"]] = cur
            cur = {}
            continue
        key, _, val = line.partition("\t")
        if key in LIST_KEYS:
            cur.setdefault(key, []).append(val)
        else:
            cur[key] = val
    for m in pkgs.values():
        for k in LIST_KEYS:
            m.setdefault(k, [])
    return pkgs


def read_list(path: Path) -> set[str]:
    out = set()
    if path and path.exists():
        for line in path.read_text().splitlines():
            line = line.split("#", 1)[0].strip()
            out.update(line.split())
    return out


def split_source(src: str) -> tuple[str, str]:
    """'name::git+https://x#branch=y' -> ('name', 'git+https://x#branch=y')"""
    if "::" in src:
        name, _, url = src.partition("::")
        return name, url
    return "", src


# --------------------------------------------------------------------------
# lint
# --------------------------------------------------------------------------
def lint(pkg: str, m: dict) -> list[dict]:
    issues = []

    def add(sev, check, msg):
        issues.append({"pkg": pkg, "category": "pkgbuild_invalid",
                       "severity": sev, "check": check, "message": msg})

    if "fatal" in m:
        add("error", "evaluate", m["fatal"])
    if "syntax_error" in m:
        add("error", "syntax", m["syntax_error"])
        return issues
    if "makepkg_lint_error" in m:
        add("error", "makepkg-lint", m["makepkg_lint_error"])
    if "fatal" in m:
        return issues

    if m.get("source_stderr"):
        add("warning", "stderr-on-source",
            "sourcing the PKGBUILD prints errors: " + m["source_stderr"][:200])

    base = m.get("pkgbase") or (m["pkgname"][0] if m["pkgname"] else "")
    if base != pkg:
        add("warning", "name-mismatch",
            f"pkgbase/pkgname '{base}' does not match directory '{pkg}'")

    ver = m.get("pkgver", "")
    if not ver:
        add("error", "pkgver", "pkgver is empty")
    elif not VALID_PKGVER.fullmatch(ver):
        add("error", "pkgver", f"invalid pkgver '{ver}' (allowed: alnum . _ +)")
    if m.get("pkgver_lines") not in ("1",):
        add("warning", "pkgver-line",
            f"{m.get('pkgver_lines')} top-level 'pkgver=' lines; automated bumps need exactly one")
    if not VALID_PKGREL.fullmatch(m.get("pkgrel", "")):
        add("error", "pkgrel", f"invalid pkgrel '{m.get('pkgrel', '')}'")
    if m.get("pkgrel_lines") not in ("1",):
        add("warning", "pkgrel-line",
            f"{m.get('pkgrel_lines')} top-level 'pkgrel=' lines")
    if not m["arch"]:
        add("error", "arch", "arch=() is missing")
    if not m["license"]:
        add("warning", "license", "license=() is missing")

    n_src = int(m.get("n_source", "0") or 0)
    sums = {k[5:]: int(v) for k, v in m.items() if k.startswith("sums_")}
    if n_src and not sums:
        add("error", "checksums", "sources without any checksum array")
    for algo, n in sums.items():
        if n != n_src:
            add("error", "checksums",
                f"{algo}sums has {n} entries but there are {n_src} sources")

    srcs = [split_source(s)[1] for s in m["source"]]
    if any(re.search(r"pythonhosted\.org/packages/[0-9a-f]{2}/[0-9a-f]{2}/[0-9a-f]{20,}/", x) for x in srcs):
        add("warning", "pypi-url", "content-hashed PyPI URL can't be bumped automatically; use "
            "https://files.pythonhosted.org/packages/source/<l>/<name>/<name>-$pkgver.tar.gz")
    has_git = any(s.startswith(("git+", "git://")) for s in srcs)
    if has_git and "git" not in m["makedepends"]:
        add("warning", "makedepends-git", "git source but 'git' not in makedepends")
    if "has_pkgver_func" in m and not any(
            re.match(r"^(git|svn|hg|bzr|fossil)\+", s) or s.startswith("git://")
            for s in srcs):
        add("warning", "pkgver-func", "pkgver() defined but no VCS source")
    if len(m["pkgname"]) > 1:
        missing = set(m["pkgname"]) - set(m["has_split_package_func"])
        if missing and "has_package_func" not in m:
            add("error", "split-package",
                "missing package_<name>() for: " + ", ".join(sorted(missing)))
    elif "has_package_func" not in m and not m["has_split_package_func"]:
        add("error", "package-func", "no package() function")
    return issues


# --------------------------------------------------------------------------
# upstream tracking
# --------------------------------------------------------------------------
def tag_entry(kind: str, project: str, tag: str, ver: str) -> dict | None:
    if ver not in tag:
        return None
    prefix, _, suffix = tag.partition(ver)
    e = {"source": kind, kind: project, "use_max_tag": True,
         "include_regex": re.escape(prefix) + VER_RE + re.escape(suffix)}
    if suffix:
        e["from_pattern"] = "^" + re.escape(prefix) + "(.+)" + re.escape(suffix) + "$"
        e["to_pattern"] = r"\1"
    elif prefix:
        e["prefix"] = prefix
    if not re.fullmatch(PRERELEASE, tag) and not re.fullmatch(PRERELEASE, ver):
        e["exclude_regex"] = PRERELEASE
    return e


ARCHIVE_EXT = re.compile(r"\.(tar\.(gz|bz2|xz|zst)|tgz|zip|tar)$")


def release_entry(url: str, ver: str) -> tuple[dict, str] | None:
    u = urllib.parse.urlsplit(url)
    host = u.netloc.lower()
    parts = [p for p in u.path.split("/") if p]
    fname = parts[-1] if parts else ""

    if host in ("github.com", "www.github.com") and len(parts) >= 4:
        proj = parts[0] + "/" + re.sub(r"\.git$", "", parts[1])
        tag = None
        if parts[2] == "archive":
            rest = parts[3:]
            if rest[:2] == ["refs", "tags"]:
                rest = rest[2:]
            if len(rest) >= 2 and ver in rest[0]:
                tag = rest[0]                      # archive/<tag>/<name>.tar.gz
            elif rest:
                tag = ARCHIVE_EXT.sub("", "/".join(rest))
        elif parts[2] == "releases" and len(parts) >= 6 and parts[3] == "download":
            tag = parts[4]
        if tag:
            e = tag_entry("github", proj, tag, ver)
            if e:
                return e, "github"

    if host == "gitlab.com" and "-" in parts and "archive" in parts:
        i = parts.index("-")
        if i >= 2 and len(parts) > i + 2 and parts[i + 1] == "archive":
            e = tag_entry("gitlab", "/".join(parts[:i]), parts[i + 2], ver)
            if e:
                return e, "gitlab"

    if host in ("files.pythonhosted.org", "pypi.python.org", "pypi.io",
                "pypi.org") and ver in fname:
        if len(parts) >= 4 and parts[1] == "source":
            name = parts[3]
        elif len(parts) >= 5 and parts[0] == "packages" and len(parts[3]) >= 20:
            # content-hashed path (packages/ab/cd/<hash>/file): changes every
            # release, so a bump can't produce the new URL
            return None
        else:
            name = fname.split("-" + ver, 1)[0]
        if name and name != fname:
            return {"source": "pypi", "pypi": name}, "pypi"

    if host == "rubygems.org" and fname.endswith(f"-{ver}.gem"):
        return {"source": "gems", "gems": fname[: -len(f"-{ver}.gem")]}, "gems"

    if (host.endswith("cpan.org") or "cpan" in host) and "authors" in parts \
            and f"-{ver}." in fname:
        return {"source": "cpan", "cpan": fname.split(f"-{ver}.", 1)[0]}, "cpan"

    # SourceForge has no nvchecker source; scrape the project's file RSS feed
    if host.endswith("sourceforge.net") and ver in fname and parts:
        proj = None
        if host in ("downloads.sourceforge.net", "download.sourceforge.net"):
            if parts[0] in ("project", "sourceforge") and len(parts) >= 3:
                proj = parts[1]
            elif len(parts) >= 2:
                proj = parts[0]
        elif host == "sourceforge.net" and parts[0] == "projects" and len(parts) >= 4:
            proj = parts[1]
            if fname == "download":
                fname = parts[-2]
        if proj and ver in fname:
            pre, _, suf = fname.partition(ver)
            return {"source": "regex",
                    "url": f"https://sourceforge.net/projects/{proj}/rss?path=/&limit=500",
                    "regex": re.escape(pre) + "(" + VER_RE + ")" + re.escape(suf)}, "sourceforge"
    return None


def git_entry(src: str) -> tuple[dict | None, str]:
    url = src[4:] if src.startswith("git+") else src
    url, _, frag = url.partition("#")
    url = url.split("?", 1)[0]
    # nvchecker runs `git ls-remote <url> <branch>` through a shell
    if not SAFE_URL.fullmatch(url):
        return None, "git URL has unusual characters"
    e = {"source": "git", "git": url, "use_commit": True}
    if frag:
        k, _, v = frag.partition("=")
        if k in ("tag", "commit"):
            return None, f"VCS source pinned to {k}"
        if k == "branch" and v:
            if not SAFE_REF.fullmatch(v):
                return None, "git branch name has unusual characters"
            e["branch"] = v
    return e, "git"


def track(pkg: str, m: dict) -> tuple[dict | None, str]:
    """-> (tracking record incl. nvchecker entry, '') or (None, reason)"""
    ver = m.get("pkgver", "")
    if not ver or not VALID_PKGVER.fullmatch(ver):
        return None, "invalid pkgver"
    srcs = [split_source(s)[1] for s in m["source"]]

    if "has_pkgver_func" in m:
        git = [s for s in srcs if s.startswith(("git+", "git://"))]
        if not git:
            other = [s for s in srcs if re.match(r"^(svn|hg|bzr|fossil)\+", s)]
            return None, ("non-git VCS source" if other else "pkgver() without VCS source")
        h = HASH_IN_PKGVER.search(ver)
        if not h or re.fullmatch(r"(19|20)[0-9]{6}", h.group(1)):
            return None, "VCS pkgver contains no commit hash"
        entry, via = git_entry(git[0])
        if not entry:
            return None, via
        return {"kind": "vcs", "cur": ver, "old": h.group(1), "via": via,
                "entry": entry}, ""

    if any(s.startswith(("git+", "git://")) for s in srcs) and not any(
            "://" in s and not s.startswith(("git+", "git://")) for s in srcs):
        return None, "git source without pkgver() (static snapshot)"
    for s in srcs:
        if "://" not in s:
            continue
        r = release_entry(s, ver)
        if r:
            entry, via = r
            return {"kind": "release", "cur": ver, "old": ver, "via": via,
                    "entry": entry}, ""
    hosts = sorted({urllib.parse.urlsplit(s).netloc for s in srcs if "://" in s})
    return None, "no supported upstream" + (f" ({hosts[0]})" if hosts else " (local sources only)")


# --------------------------------------------------------------------------
# reverse dependencies inside the BlackArch tree
# --------------------------------------------------------------------------
def dep_name(d: str) -> str:
    """'python2-foo>=1.2' -> 'python2-foo', 'foo: for bar' -> 'foo'"""
    return re.split(r"[<>=:]", d, maxsplit=1)[0].strip()


def build_depgraph(meta: dict[str, dict]) -> dict[str, dict]:
    """
    -> {dir: {"names": {name: [rdep, ...]}, "rdeps": [rdep, ...]}}
    rdep = {"pkg": <dir that needs it>, "type": depends|makedepends|
            checkdepends|optdepends, "name": <name asked for>, "via": <split
            package of that dir, or "">}
    Split packages are handled per output: package_python2-foo() can have its
    own depends=() that replaces the top-level one.
    """
    owner: dict[str, str] = {}
    graph: dict[str, dict] = {}
    for d, m in meta.items():
        names = list(m["pkgname"])
        prov = [dep_name(x) for x in m["provides"]]
        prov += [dep_name(x.split(" ", 1)[1]) for x in m["sdep_provides"] if " " in x]
        graph[d] = {"names": {n: [] for n in names}, "rdeps": []}
        for n in names + prov:
            owner.setdefault(n, d)
        for n in names:                    # real package names win over provides
            owner[n] = d

    for y, m in meta.items():
        wanted: dict[tuple[str, str], set[str]] = {}
        for t in ("makedepends", "checkdepends"):
            for dep in m[t]:
                wanted.setdefault((t, dep_name(dep)), set()).add("")
        overrides = {tuple(x.split(" ", 1)) for x in m["soverride"] if " " in x}
        split = {}
        for t in ("depends", "optdepends"):
            for x in m["sdep_" + t]:
                if " " in x:
                    p, dep = x.split(" ", 1)
                    split.setdefault((p, t), []).append(dep)
        outputs = m["pkgname"] or [y]
        for t in ("depends", "optdepends"):
            for p in outputs:
                deps = split.get((p, t), []) if (p, t) in overrides else m[t]
                via = p if len(outputs) > 1 else ""
                for dep in deps:
                    wanted.setdefault((t, dep_name(dep)), set()).add(via)
        for (t, name), vias in wanted.items():
            x = owner.get(name)
            if not x or x == y:
                continue
            for via in sorted(vias):
                r = {"pkg": y, "type": t, "name": name, "via": via}
                graph[x]["rdeps"].append(r)
                if name in graph[x]["names"]:
                    graph[x]["names"][name].append(r)
    return graph


PKGCHECK_LINE = re.compile(r"^(?:.*/)?packages/([^/]+)/PKGBUILD:(\d+):\d+: ([EW]\d{3}) (.*)$")


def pkgcheck_issues(path: Path) -> list[dict]:
    per: dict[str, list[str]] = {}
    sev: dict[str, str] = {}
    for line in path.read_text(errors="replace").splitlines():
        m = PKGCHECK_LINE.match(line)
        if not m:
            continue
        pkg, ln, code, msg = m.groups()
        per.setdefault(pkg, []).append(f"{code} L{ln} {msg}")
        if code.startswith("E"):
            sev[pkg] = "error"
    return [{"pkg": p, "category": "pkgbuild_invalid", "severity": sev.get(p, "warning"),
             "check": "pkgcheck", "message": "; ".join(v[:6]) + (f" (+{len(v) - 6} more)" if len(v) > 6 else "")}
            for p, v in sorted(per.items())]


def toml_value(v) -> str:
    if isinstance(v, bool):
        return "true" if v else "false"
    if isinstance(v, int):
        return str(v)
    return json.dumps(v)


def cmd_scan(a) -> None:
    out = Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    meta = parse_meta(Path(a.meta))
    exclude: set[str] = set()
    for f in a.exclude or []:
        exclude |= read_list(Path(f))
    overrides_raw, overrides = "", {}
    if a.overrides and Path(a.overrides).exists():
        overrides_raw = Path(a.overrides).read_text()
        overrides = tomllib.loads(overrides_raw)

    if a.packages:
        import hashlib
        for pkg, m in meta.items():
            f = Path(a.packages) / pkg / "PKGBUILD"
            if f.exists():
                m["sha"] = hashlib.sha1(f.read_bytes()).hexdigest()

    issues, tracking, untracked = [], {}, {}
    if a.pkgcheck and Path(a.pkgcheck).exists():
        issues += pkgcheck_issues(Path(a.pkgcheck))
    for pkg in sorted(meta):
        m = meta[pkg]
        issues += lint(pkg, m)
        if pkg in exclude:
            untracked[pkg] = "excluded"
            continue
        if a.exclude_regex and re.search(a.exclude_regex, pkg):
            untracked[pkg] = "excluded (name pattern)"
            continue
        if pkg in overrides:
            ver = m.get("pkgver", "")
            kind = "vcs" if "has_pkgver_func" in m else "release"
            h = HASH_IN_PKGVER.search(ver) if kind == "vcs" else None
            tracking[pkg] = {"kind": kind, "cur": ver,
                             "old": h.group(1) if h else ver, "via": "override",
                             "entry": None}
            continue
        rec, why = track(pkg, m)
        if rec:
            tracking[pkg] = rec
        else:
            untracked[pkg] = why

    lines = ["# generated by .github/ci/detect.py - do not edit",
             "[__config__]", 'oldver = "oldver.json"', 'newver = "newver.json"',
             f"max_concurrency = {a.concurrency}", ""]
    for pkg, rec in tracking.items():
        if rec["entry"] is None:
            continue
        lines.append(f"[{json.dumps(pkg)}]")
        lines += [f"{k} = {toml_value(v)}" for k, v in rec["entry"].items()]
        lines.append("")
    if overrides_raw:
        lines += ["# ---- .github/ci/nvchecker-overrides.toml ----", overrides_raw]
    (out / "nvchecker.toml").write_text("\n".join(lines) + "\n")

    oldver = {"version": 2, "data": {p: {"version": r["old"]} for p, r in tracking.items()}}
    (out / "oldver.json").write_text(json.dumps(oldver, indent=1, sort_keys=True))
    (out / "meta.json").write_text(json.dumps(meta, sort_keys=True))
    (out / "tracking.json").write_text(json.dumps(tracking, indent=1, sort_keys=True))
    (out / "untracked.json").write_text(json.dumps(untracked, indent=1, sort_keys=True))
    (out / "issues.json").write_text(json.dumps(issues, indent=1))
    (out / "depgraph.json").write_text(json.dumps(build_depgraph(meta), sort_keys=True))

    kinds = {}
    for r in tracking.values():
        kinds[r["via"]] = kinds.get(r["via"], 0) + 1
    print(f"packages: {len(meta)}  tracked: {len(tracking)} {kinds}  "
          f"untracked: {len(untracked)}  lint issues: {len(issues)} "
          f"({sum(i['severity'] == 'error' for i in issues)} errors)")


# --------------------------------------------------------------------------
# plan
# --------------------------------------------------------------------------
DEAD_REPO = re.compile(r"not found|404|410|terminal prompts disabled|could not read "
                       r"Username|does not exist|Repository unavailable|"
                       r"access denied|Could not resolve host", re.I)


def nvchecker_errors(log: Path) -> dict[str, str]:
    errs: dict[str, str] = {}
    if not log.exists():
        return errs
    for line in log.read_text(errors="replace").splitlines():
        try:
            ev = json.loads(line)
        except json.JSONDecodeError:
            continue
        if ev.get("level") not in ("error", "warning") or "name" not in ev:
            continue
        msg = str(ev.get("event", ""))
        for k in ("error", "exc_info", "output", "returncode", "url"):
            if ev.get(k):
                msg += f" | {k}: {str(ev[k])[-300:]}"
        # nvchecker logs the real error first, then a generic "no-result":
        # keep every event, first one first
        prev = errs.get(ev["name"])
        errs[ev["name"]] = (prev + " || " + msg if prev else msg)[:900]
    return errs


def load_newver(path: Path) -> dict[str, str]:
    if not path.exists():
        return {}
    d = json.loads(path.read_text())
    if d.get("version") == 2:
        return {k: v["version"] for k, v in d.get("data", {}).items()
                if isinstance(v, dict) and "version" in v}
    return {k: v for k, v in d.items() if isinstance(v, str)}


def cmd_plan(a) -> None:
    st = Path(a.state)
    tracking = json.loads((st / "tracking.json").read_text())
    meta = json.loads((st / "meta.json").read_text())
    issues = json.loads((st / "issues.json").read_text())
    newver = load_newver(st / "newver.json")
    seen, builds = {}, {}
    if a.seen and Path(a.seen).exists():
        db = json.loads(Path(a.seen).read_text())
        seen, builds = db.get("packages", {}), db.get("builds", {})
    pending = {}
    if a.pending and Path(a.pending).exists():
        pending = json.loads(Path(a.pending).read_text())
    now = dt.datetime.now(dt.timezone.utc)

    for pkg, msg in nvchecker_errors(st / "nvchecker.log").items():
        rec = tracking.get(pkg, {})
        dead = rec.get("kind") == "vcs" and DEAD_REPO.search(msg)
        issues.append({"pkg": pkg,
                       "category": "source_broken" if dead else "upstream_check",
                       "severity": "error" if dead else "warning",
                       "check": "nvchecker-" + rec.get("via", "?"), "message": msg})

    updates, carried, deferred, waiting = [], [], [], []
    for pkg, rec in sorted(tracking.items()):
        new = newver.get(pkg)
        if not new:
            continue
        if rec["kind"] == "vcs":
            if new.startswith(rec["old"]) or rec["old"].startswith(new):
                continue
            upd = {"pkg": pkg, "kind": "vcs", "cur": rec["cur"], "new": new}
        else:
            if not VALID_PKGVER.fullmatch(new):
                issues.append({"pkg": pkg, "category": "pkgbuild_invalid",
                               "severity": "warning", "check": "upstream-version",
                               "message": f"upstream version '{new}' is not a valid pkgver; "
                                          "add an override with from_pattern/to_pattern"})
                continue
            c = rpmvercmp(new, rec["cur"])
            if c < 0:
                issues.append({"pkg": pkg, "category": "upstream_check",
                               "severity": "warning", "check": "upstream-older",
                               "message": f"upstream reports {new} < PKGBUILD {rec['cur']}; "
                                          "tracking is probably wrong (override it)"})
            if c <= 0:
                continue
            if rec.get("via") != "override" and suspicious_jump(rec["cur"], new):
                issues.append({"pkg": pkg, "category": "upstream_check",
                               "severity": "warning", "check": "upstream-jump",
                               "message": f"upstream reports {new} for {rec['cur']}: looks like a "
                                          "different version scheme (date or unrelated tag), not "
                                          "updated; add an override if it is real"})
                continue
            upd = {"pkg": pkg, "kind": "release", "cur": rec["cur"], "new": new}

        if not buildable(meta.get(pkg, {})):
            continue
        if pkg in pending:     # don't build it again while its PR is open
            waiting.append({**upd, "pr": pending[pkg]})
            continue
        s = seen.get(pkg)
        if s and s.get("target") == upd["new"] and not a.retry_failed:
            age = (now - dt.datetime.fromisoformat(s["date"])).days
            if s.get("result") == "no_change":
                continue
            if a.skip_built_ok and s.get("result") == "ok":
                continue   # dry runs: already built fine for this version
            if s.get("result") in FAILED_RESULTS and age < a.retry_days:
                carried.append({**upd, **{k: s.get(k) for k in ("result", "reason", "date", "run_url")}})
                continue
        updates.append(upd)

    # ship: the tree already carries a version the repo does not have (manual
    # bump merged to the base branch, new package). Build as-is and publish.
    repo = load_repo_db(a.repo_db)
    if repo is None and a.repo_db:
        print("warning: repo database not available, 'ship' detection skipped", file=sys.stderr)
    shipped = 0
    if repo is not None:
        busy = {u["pkg"] for u in updates}
        for d, m in sorted(meta.items()):
            if d in busy or not buildable(m) or "fatal" in m or "syntax_error" in m \
                    or not m.get("pkgver") or not m.get("pkgrel"):
                continue
            rec = tracking.get(d, {})
            is_vcs = rec.get("kind") == "vcs"
            if "has_pkgver_func" in m and not is_vcs:
                continue   # pkgver() without a trackable git source: version unknown until built
            full = full_version(m)
            names = m.get("pkgname") or [d]
            in_repo = [n for n in names if n in repo]
            if any(vercmp(full, repo[n]) < 0 for n in in_repo):
                issues.append({"pkg": d, "category": "upstream_check", "severity": "warning",
                               "check": "repo-newer",
                               "message": f"the repo has {repo[in_repo[0]]} but the tree only {full}; "
                                          "was the PKGBUILD downgraded?"})
                continue
            if in_repo and all(vercmp(full, repo[n]) == 0 for n in in_repo) and len(in_repo) == len(names):
                continue
            if is_vcs:
                # VCS: pkgver() runs at build time. Plan it as a vcs update that
                # may legitimately find nothing new (build-one.sh: SHIP=1 keeps
                # going instead of 'no_change'); publish.sh ships it as-is when
                # the PKGBUILD came back unchanged, commits it when it moved on.
                upd = {"pkg": d, "kind": "vcs", "cur": m["pkgver"], "new": newver.get(d) or rec["old"],
                       "ship": True, "repo": repo[in_repo[0]] if in_repo else "", "tree": full}
            else:
                # cur = pkgver (verify-output falls back to it); new = the full version
                upd = {"pkg": d, "kind": "ship", "cur": m["pkgver"], "new": full,
                       "repo": repo[in_repo[0]] if in_repo else ""}
            if d in pending:
                waiting.append({**upd, "pr": pending[d]})
                continue
            s = seen.get(d)
            if s and s.get("target") == upd["new"] and not a.retry_failed:
                age = (now - dt.datetime.fromisoformat(s["date"])).days
                if a.skip_built_ok and s.get("result") == "ok":
                    continue
                if s.get("result") in FAILED_RESULTS and age < a.retry_days:
                    carried.append({**upd, **{k: s.get(k) for k in ("result", "reason", "date", "run_url")}})
                    continue
            updates.append(upd)
            shipped += 1

    # names may be split packages (python2-foo) -> their PKGBUILD directory
    by_name = {}
    for d, m in meta.items():
        for n in m.get("pkgname", []):
            by_name.setdefault(n, d)
    forced = []
    for p in (a.force or "").split():
        d = p if p in meta else by_name.get(p)
        if not d:
            print(f"warning: forced package '{p}' does not exist", file=sys.stderr)
        elif d not in forced:
            forced.append(d)
    shipping = {u["pkg"]: u for u in updates if u["kind"] == "ship" or u.get("ship")}
    for p in forced:
        if shipping.get(p):
            updates = [u for u in updates if u["pkg"] != p]
            updates.insert(0, {**shipping[p], "forced": True})
            continue
        updates = [u for u in updates if u["pkg"] != p]
        rec = tracking.get(p)
        new = newver.get(p, "")
        kind = rec["kind"] if rec else "rebuild"
        if kind == "release" and not (new and rpmvercmp(new, meta[p]["pkgver"]) > 0):
            kind = "rebuild"
        if kind == "vcs" and (not new or new.startswith(rec["old"]) or rec["old"].startswith(new)):
            kind = "rebuild"        # nothing new upstream: rebuild with pkgrel+1
        updates.insert(0, {"pkg": p, "kind": kind, "cur": meta[p]["pkgver"],
                           "new": new if kind != "rebuild" else meta[p]["pkgver"],
                           "forced": True})

    # releases first (they matter most to users), then VCS; cap the batch
    order = {"release": 0, "ship": 1, "vcs": 2}
    updates.sort(key=lambda u: (not u.get("forced"), order.get(u["kind"], 3), u["pkg"]))
    # the cap never drops packages named in --force; 0 means "no updates"
    forced_u = [u for u in updates if u.get("forced")]
    normal = [u for u in updates if not u.get("forced")]
    limit = max(0, a.max)
    deferred = normal[limit:]
    updates = forced_u + normal[:limit]

    for u in updates:
        u["sha"] = meta.get(u["pkg"], {}).get("sha", "")

    # rolling audit: build packages that were NOT updated, as they are, so the
    # report knows the build status of the whole repo (full cycle every
    # ~len(meta)/audit days). Never tested / PKGBUILD changed since the last
    # test come first, then the oldest results.
    audit = []
    if a.audit:
        busy = {u["pkg"] for u in updates} | set(pending)
        cands = [p for p, m in meta.items() if p not in busy and buildable(m)
                 and "fatal" not in m and "syntax_error" not in m and m.get("pkgver")]

        def prio(p):
            b = builds.get(p)
            if not b:
                return (0, "")
            if b.get("sha") != meta[p].get("sha"):
                return (1, b.get("date", ""))
            return (2, b.get("date", ""))
        cands.sort(key=lambda p: (prio(p), p))
        audit = [{"pkg": p, "kind": "test", "cur": meta[p]["pkgver"], "new": meta[p]["pkgver"],
                  "sha": meta[p].get("sha", "")} for p in cands[: a.audit]]

    work = updates + audit
    n_shards = min(a.max_shards, max(1, math.ceil(len(work) / a.per_shard))) if work else 0
    shard_dir = st / "shards"
    shard_dir.mkdir(exist_ok=True)
    shards = [[] for _ in range(n_shards)]
    for i, u in enumerate(work):
        shards[i % n_shards].append(u)
    for i, s in enumerate(shards):
        (shard_dir / f"{i}.json").write_text(json.dumps(s, indent=1))

    (st / "issues.json").write_text(json.dumps(issues, indent=1))
    (st / "updates.json").write_text(json.dumps(
        {"updates": updates, "audit": audit, "carried": carried, "deferred": deferred,
         "pending": waiting}, indent=1))

    print(f"updates: {len(updates)} (release {sum(u['kind'] == 'release' for u in updates)}, "
          f"vcs {sum(u['kind'] == 'vcs' for u in updates)}, ship {shipped}, forced {len(forced)})  "
          f"still failing (not retried): {len(carried)}  deferred: {len(deferred)}  "
          f"waiting in open PRs: {len(waiting)}  audit builds: {len(audit)}  "
          f"shards: {n_shards}")
    gh_out = os.environ.get("GITHUB_OUTPUT")
    if gh_out:
        with open(gh_out, "a") as f:
            f.write(f"count={len(work)}\n")
            f.write(f"updates={len(updates)}\n")
            f.write("matrix=" + json.dumps({"shard": list(range(n_shards))}) + "\n")
    summary = str(st / "build-jobs.md")
    if shards:
        kinds = {"release": "update", "vcs": "update (git)", "rebuild": "rebuild", "test": "audit",
                 "ship": "ship: tree newer than repo"}
        with open(summary, "w") as f:
            f.write("\n### Build jobs\n\n| Job | Packages |\n|---|---|\n")
            for i, sh in enumerate(shards):
                cells = [f"`{u['pkg']}`" + ("" if u["kind"] == "test" else
                                              f" ({kinds['ship'] if u.get('ship') else kinds.get(u['kind'], u['kind'])})")
                         for u in sh]
                f.write(f"| build {i} | {', '.join(cells)} |\n")
            f.write("\nPackages without a tag are audit builds (built as they are, never published).\n")


# --------------------------------------------------------------------------
# pending: packages already waiting in an open auto-update PR (PUBLISH_MODE=pr)
# --------------------------------------------------------------------------
def _gh_pages(url: str, token: str):
    import urllib.request
    while url:
        req = urllib.request.Request(url, headers={
            "Accept": "application/vnd.github+json",
            **({"Authorization": f"Bearer {token}"} if token else {})})
        with urllib.request.urlopen(req, timeout=30) as r:
            yield from json.loads(r.read())
            nxt = re.search(r'<([^>]+)>;\s*rel="next"', r.headers.get("Link", ""))
            url = nxt.group(1) if nxt else None


def cmd_pending(a) -> None:
    token = os.environ.get("GH_TOKEN", "")
    api = f"https://api.github.com/repos/{a.repo}"
    pending: dict[str, str] = {}
    try:
        for pr in _gh_pages(f"{api}/pulls?state=open&per_page=100", token):
            head = pr.get("head") or {}
            if not head.get("ref", "").startswith("auto-update/") or \
                    (head.get("repo") or {}).get("full_name") != a.repo:
                continue
            for f in _gh_pages(f"{api}/pulls/{pr['number']}/files?per_page=100", token):
                m = re.fullmatch(r"packages/([^/]+)/PKGBUILD", f["filename"])
                if m:
                    pending[m.group(1)] = pr["html_url"]
    except Exception as e:  # noqa: BLE001
        if a.strict:   # pr mode: a wrong answer means duplicate PRs
            sys.exit(f"error: could not list open PRs: {e}")
        print(f"warning: could not list open PRs: {e}", file=sys.stderr)
    Path(a.out).write_text(json.dumps(pending, indent=1, sort_keys=True))
    print(f"packages waiting in open auto-update PRs: {len(pending)}")


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd", required=True)
    s = sub.add_parser("scan")
    s.add_argument("--meta", required=True)
    s.add_argument("--out", required=True)
    s.add_argument("--exclude", action="append")
    s.add_argument("--overrides")
    s.add_argument("--packages", help="packages dir, to fingerprint each PKGBUILD")
    s.add_argument("--pkgcheck", help="output of pkgcheck-all.sh")
    s.add_argument("--exclude-regex", default=r"^python2-|-py2$",
                   help="package names never auto-updated (legacy python2 pins)")
    s.add_argument("--concurrency", type=int, default=24)
    s.set_defaults(func=cmd_scan)
    q = sub.add_parser("plan")
    q.add_argument("--state", required=True)
    q.add_argument("--seen")
    q.add_argument("--force", default="")
    q.add_argument("--max", type=int, default=250)
    q.add_argument("--per-shard", type=int, default=12)
    q.add_argument("--max-shards", type=int, default=200,
                   help="GitHub allows 256 matrix jobs; max-parallel limits how many run at once")
    q.add_argument("--retry-failed", action="store_true")
    q.add_argument("--retry-days", type=int, default=7)
    q.add_argument("--pending", help="JSON {pkg: pr_url} from `detect.py pending`")
    q.add_argument("--skip-built-ok", action="store_true",
                   help="skip updates that already built OK for the same version (dry runs)")
    q.add_argument("--repo-db", help="'name version' lines extracted from the package repo db; "
                   "tree versions newer than it (or missing from it) are built and shipped")
    q.add_argument("--audit", type=int, default=0,
                   help="also build N not-updated packages as-is (rolling full-repo build test)")
    q.set_defaults(func=cmd_plan)
    r = sub.add_parser("pending")
    r.add_argument("--repo", required=True)
    r.add_argument("--out", required=True)
    r.add_argument("--strict", action="store_true", help="fail instead of assuming no open PRs")
    r.set_defaults(func=cmd_pending)
    a = p.parse_args()
    a.func(a)


if __name__ == "__main__":
    main()
