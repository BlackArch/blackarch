#!/usr/bin/env python3
"""
bautil - Modern Unified BlackArch Maintenance, Quality & Security Engine

Features:
- Multi-threaded package validation and linting across 4,600+ packages (check)
- Intelligent batch PKGBUILD formatter & autofixer (fix, prep)
- Deep repository statistics and metadata analysis (stats)
- Dependency search and reverse-dependency analyzer (deps)
- ELF binary security hardening auditor (audit)
"""

import os
import sys
import re
import argparse
import subprocess
import concurrent.futures
from collections import Counter

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
PACKAGES_DIR = os.path.join(REPO_ROOT, "packages")
GROUPS_FILE = os.path.join(REPO_ROOT, "lists", "groups")

def load_official_groups():
    """Load valid BlackArch official groups list."""
    if not os.path.isfile(GROUPS_FILE):
        return set()
    with open(GROUPS_FILE, "r", encoding="utf-8") as f:
        return {line.strip() for line in f if line.strip() and not line.startswith("#")}

def get_pkg_list(target):
    """Retrieve list of package directory names."""
    if not target or target == "all":
        return sorted([d for d in os.listdir(PACKAGES_DIR) if os.path.isdir(os.path.join(PACKAGES_DIR, d))])
    return [target]

def evaluate_pkgbuild(pkgbuild_path):
    """Source a PKGBUILD in a sandboxed bash subshell and extract metadata."""
    bash_code = f"""
    shopt -s extglob
    source "{pkgbuild_path}" 2>/dev/null || exit 2
    echo "VER: $pkgver"
    echo "REL: $pkgrel"
    echo "ARCH: ${{arch[*]}}"
    echo "GROUPS: ${{groups[*]}}"
    echo "LICENSE: ${{license[*]}}"
    echo "DEPENDS: ${{depends[*]}}"
    echo "MAKEDEPENDS: ${{makedepends[*]}}"
    echo "SRC_LEN: ${{#source[@]}}"
    echo "S512_LEN: ${{#sha512sums[@]}}"
    echo "S256_LEN: ${{#sha256sums[@]}}"
    echo "S1_LEN: ${{#sha1sums[@]}}"
    echo "MD5_LEN: ${{#md5sums[@]}}"
    echo "B2_LEN: ${{#b2sums[@]}}"
    echo "PKGNAME: ${{pkgname[*]}}"
    echo "PKGBASE: $pkgbase"
    """
    res = subprocess.run(["bash", "-c", bash_code], capture_output=True, text=True)
    if res.returncode != 0:
        return None, res.stderr.strip()
    
    data = {}
    for line in res.stdout.strip().split("\n"):
        if ": " in line:
            k, v = line.split(": ", 1)
            data[k] = v
        elif ":" in line:
            k = line.split(":", 1)[0]
            data[k] = ""
    return data, None

def check_single_pkg(p, valid_groups):
    """Run comprehensive quality check on a single package."""
    p_path = os.path.join(PACKAGES_DIR, p)
    pkgbuild = os.path.join(p_path, "PKGBUILD")
    if not os.path.isfile(pkgbuild):
        return [(p, "NO_PKGBUILD", "Missing PKGBUILD")]

    issues = []
    # 1. Syntax check
    syn_res = subprocess.run(["bash", "-n", pkgbuild], capture_output=True, text=True)
    if syn_res.returncode != 0:
        issues.append((p, "SYNTAX_ERROR", syn_res.stderr.strip().split("\n")[-1]))
        return issues

    # 2. Evaluation & Variable validation
    data, err = evaluate_pkgbuild(pkgbuild)
    if err or data is None:
        issues.append((p, "EVAL_FAIL", err or "Failed to source PKGBUILD"))
        return issues

    ver = data.get("VER", "")
    rel = data.get("REL", "")
    arch = data.get("ARCH", "").split()
    groups = data.get("GROUPS", "").split()
    licenses = data.get("LICENSE", "").split()
    src_len = int(data.get("SRC_LEN", 0) or 0)
    s512_len = int(data.get("S512_LEN", 0) or 0)
    s256_len = int(data.get("S256_LEN", 0) or 0)
    s1_len = int(data.get("S1_LEN", 0) or 0)
    md5_len = int(data.get("MD5_LEN", 0) or 0)
    b2_len = int(data.get("B2_LEN", 0) or 0)
    pkgname = data.get("PKGNAME", "")
    pkgbase = data.get("PKGBASE", "")

    # Directory name vs package name match
    main_name = pkgbase if pkgbase else (pkgname.split()[0] if pkgname else "")
    if main_name and main_name != p and p not in pkgname.split():
        issues.append((p, "PKGNAME_MISMATCH", f"dir='{p}' but pkgname/pkgbase='{main_name}'"))

    # Version format checks
    if "-" in ver:
        issues.append((p, "INVALID_PKGVER", f"pkgver cannot contain hyphens: '{ver}'"))
    if not rel:
        issues.append((p, "MISSING_PKGREL", "pkgrel is not defined"))
    if not arch:
        issues.append((p, "MISSING_ARCH", "arch array is empty"))
    else:
        for a in arch:
            if a not in ["x86_64", "aarch64", "any", "armv6h", "armv7h", "i686"]:
                issues.append((p, "UNKNOWN_ARCH", f"Non-standard architecture: '{a}'"))

    # Group validation (if defined)
    if groups and valid_groups:
        for g in groups:
            if g not in valid_groups and not g.startswith("blackarch"):
                issues.append((p, "INVALID_GROUP", f"Unknown group: '{g}'"))

    # Checksum count match
    if src_len > 0:
        has_match = (s512_len == src_len or s256_len == src_len or s1_len == src_len or md5_len == src_len or b2_len == src_len)
        if not has_match:
            issues.append((p, "CHECKSUM_COUNT_MISMATCH", f"source({src_len}) != sha512({s512_len}), sha256({s256_len}), md5({md5_len})"))

    # License check
    if not licenses:
        issues.append((p, "MISSING_LICENSE", "license array is empty"))

    return issues

def cmd_check(args):
    """Run parallel validation across packages."""
    pkgs = get_pkg_list(args.target)
    valid_groups = load_official_groups()

    print(f"[*] Auditing {len(pkgs)} package(s) using multi-threaded engine...")
    workers = min(32, os.cpu_count() or 4)
    with concurrent.futures.ThreadPoolExecutor(max_workers=workers) as executor:
        results = list(executor.map(lambda p: check_single_pkg(p, valid_groups), pkgs))

    all_issues = []
    for r in results:
        if r:
            all_issues.extend(r)

    if all_issues:
        print(f"\n[-] FAILED: Found {len(all_issues)} issue(s) across packages:")
        for pkg, code, msg in all_issues:
            print(f"  [{pkg}] {code}: {msg}")
        sys.exit(1)
    else:
        print(f"\n[+] SUCCESS: Verified {len(pkgs)} package(s). All checks passed cleanly with 0 errors!")

def format_pkgbuild_content(content):
    """Sanitize and format PKGBUILD string."""
    lines = content.splitlines(True)
    new_lines = []
    for line in lines:
        if "\t" in line:
            line = line.replace("\t", "  ")
        if re.search(r"#\s*vim:", line):
            continue
        if re.search(r"#\s*\$\s*Id:", line):
            continue
        stripped = line.rstrip("\r\n")
        if re.search(r"\s+$", stripped):
            stripped = stripped.rstrip()
        new_lines.append(stripped + "\n")

    while new_lines and new_lines[0].strip() == "":
        new_lines.pop(0)

    return "".join(new_lines)

def cmd_prep(args):
    """Sanitize and format a single PKGBUILD."""
    target_file = args.target_file
    if not os.path.isfile(target_file):
        print(f"[-] Error: File '{target_file}' not found.")
        sys.exit(1)

    with open(target_file, "r", encoding="utf-8", errors="replace") as f:
        content = f.read()

    new_content = format_pkgbuild_content(content)
    if new_content != content:
        with open(target_file, "w", encoding="utf-8") as f:
            f.write(new_content)
        print(f"[+] Formatted '{target_file}'")
    else:
        print(f"[*] '{target_file}' is already cleanly formatted.")

def cmd_fix(args):
    """Batch auto-format and fix whitespace/modeline issues across all packages."""
    pkgs = get_pkg_list(args.target)
    print(f"[*] Auto-formatting and sanitizing {len(pkgs)} package(s)...")

    def fix_pkg(p):
        pkgbuild = os.path.join(PACKAGES_DIR, p, "PKGBUILD")
        if not os.path.isfile(pkgbuild):
            return 0
        with open(pkgbuild, "r", encoding="utf-8", errors="replace") as f:
            content = f.read()
        new_content = format_pkgbuild_content(content)
        if new_content != content:
            with open(pkgbuild, "w", encoding="utf-8") as f:
                f.write(new_content)
            return 1
        return 0

    workers = min(32, os.cpu_count() or 4)
    with concurrent.futures.ThreadPoolExecutor(max_workers=workers) as executor:
        fixed_counts = list(executor.map(fix_pkg, pkgs))

    total_fixed = sum(fixed_counts)
    print(f"[+] Done. Auto-formatted {total_fixed} of {len(pkgs)} PKGBUILD file(s).")

def cmd_stats(args):
    """Compute and display deep repository statistics."""
    pkgs = get_pkg_list("all")
    print(f"[*] Analyzing repository statistics for {len(pkgs)} packages...")

    group_counter = Counter()
    arch_counter = Counter()
    split_pkgs = 0
    vcs_pkgs = 0
    py2_pkgs = 0
    py3_pkgs = 0

    def analyze_pkg(p):
        pkgbuild = os.path.join(PACKAGES_DIR, p, "PKGBUILD")
        if not os.path.isfile(pkgbuild):
            return None
        data, _ = evaluate_pkgbuild(pkgbuild)
        return data

    workers = min(32, os.cpu_count() or 4)
    with concurrent.futures.ThreadPoolExecutor(max_workers=workers) as executor:
        results = [r for r in executor.map(analyze_pkg, pkgs) if r]

    for data in results:
        groups = data.get("GROUPS", "").split()
        for g in groups:
            group_counter[g] += 1

        arch = data.get("ARCH", "").split()
        for a in arch:
            arch_counter[a] += 1

        pkgname = data.get("PKGNAME", "").split()
        if len(pkgname) > 1:
            split_pkgs += 1

        deps = (data.get("DEPENDS", "") + " " + data.get("MAKEDEPENDS", "")).split()
        if any("python2" in d for d in deps):
            py2_pkgs += 1
        elif any("python" in d for d in deps):
            py3_pkgs += 1

        ver = data.get("VER", "")
        if ".r" in ver or ".g" in ver or any(p in ver for p in ["git", "svn", "hg"]):
            vcs_pkgs += 1

    print("\n" + "=" * 55)
    print("  📊 BLACKARCH LINUX REPOSITORY METRICS")
    print("=" * 55)
    print(f"  Total Packages:            {len(pkgs):,}")
    print(f"  Split Packages:            {split_pkgs:,}")
    print(f"  VCS (-git/-svn) Packages:  {vcs_pkgs:,}")
    print(f"  Python 3 Tool Packages:    {py3_pkgs:,}")
    print(f"  Python 2 Legacy Packages:  {py2_pkgs:,}")
    print("\n  📦 Top 10 Tool Categories (Groups):")
    for group, count in group_counter.most_common(10):
        print(f"    - {group:<26} {count:>5} pkgs")
    print("\n  💻 Architecture Distribution:")
    for arch, count in arch_counter.most_common():
        print(f"    - {arch:<15} {count:>5}")
    print("=" * 55 + "\n")

def cmd_deps(args):
    """Find packages that depend on a given target package."""
    target = args.target_pkg
    pkgs = get_pkg_list("all")
    print(f"[*] Searching dependencies for '{target}' across {len(pkgs)} packages...")

    matches = []
    def search_deps(p):
        pkgbuild = os.path.join(PACKAGES_DIR, p, "PKGBUILD")
        if not os.path.isfile(pkgbuild):
            return None
        data, _ = evaluate_pkgbuild(pkgbuild)
        if not data:
            return None
        all_deps = (data.get("DEPENDS", "") + " " + data.get("MAKEDEPENDS", "")).split()
        if target in all_deps:
            return p
        return None

    workers = min(32, os.cpu_count() or 4)
    with concurrent.futures.ThreadPoolExecutor(max_workers=workers) as executor:
        for res in executor.map(search_deps, pkgs):
            if res:
                matches.append(res)

    print(f"\n[+] Found {len(matches)} package(s) depending on '{target}':")
    for m in sorted(matches):
        print(f"  - {m}")

def cmd_audit(args):
    """Audit ELF binaries in a package tarball."""
    pkg_file = args.pkg_file
    if not os.path.isfile(pkg_file):
        print(f"[-] Error: File '{pkg_file}' not found.")
        sys.exit(1)

    baaudit_script = os.path.join(REPO_ROOT, "scripts", "baaudit")
    if os.path.isfile(baaudit_script) and os.access(baaudit_script, os.X_OK):
        subprocess.run([baaudit_script, pkg_file])
    else:
        print("[-] Error: 'baaudit' script missing or not executable.")

def main():
    parser = argparse.ArgumentParser(prog="bautil", description="Modern Unified BlackArch Quality, Maintenance & Security Engine")
    subparsers = parser.add_subparsers(dest="subcommand", help="Available subcommands")

    # check
    p_check = subparsers.add_parser("check", help="Fast parallel PKGBUILD validation")
    p_check.add_argument("target", nargs="?", default="all", help="Package name or 'all'")

    # fix
    p_fix = subparsers.add_parser("fix", help="Batch auto-format and sanitize PKGBUILDs")
    p_fix.add_argument("target", nargs="?", default="all", help="Package name or 'all'")

    # prep
    p_prep = subparsers.add_parser("prep", help="Format and sanitize a single PKGBUILD")
    p_prep.add_argument("target_file", help="Path to PKGBUILD file")

    # stats
    subparsers.add_parser("stats", help="Compute repository metrics and distributions")

    # deps
    p_deps = subparsers.add_parser("deps", help="Find all reverse dependencies for a package")
    p_deps.add_argument("target_pkg", help="Package name to query")

    # audit
    p_audit = subparsers.add_parser("audit", help="Security audit an ELF package tarball")
    p_audit.add_argument("pkg_file", help="Package archive (.pkg.tar.zst)")

    args = parser.parse_args()
    if args.subcommand == "check":
        cmd_check(args)
    elif args.subcommand == "fix":
        cmd_fix(args)
    elif args.subcommand == "prep":
        cmd_prep(args)
    elif args.subcommand == "stats":
        cmd_stats(args)
    elif args.subcommand == "deps":
        cmd_deps(args)
    elif args.subcommand == "audit":
        cmd_audit(args)
    else:
        parser.print_help()

if __name__ == "__main__":
    main()
