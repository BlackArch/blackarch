#!/usr/bin/env python3
"""
bautil - Unified BlackArch Repository Maintenance & Security Tool

Consolidates package checking, auditing, formatting (prep), and outdated version tracking into
a high-performance, multi-threaded CLI utility.
"""

import sys
import os
import re
import argparse
import subprocess
import concurrent.futures
import shutil

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
PACKAGES_DIR = os.path.join(REPO_ROOT, "packages")

def cmd_check(args):
    """Check PKGBUILDs for syntax errors, pkgname/pkgbase mismatches, and checksum array counts."""
    target = args.target
    if target == "all" or not target:
        pkgs = sorted(os.listdir(PACKAGES_DIR))
    else:
        pkgs = [target]

    def check_pkg(p):
        p_path = os.path.join(PACKAGES_DIR, p)
        pkgbuild = os.path.join(p_path, "PKGBUILD")
        if not os.path.isfile(pkgbuild):
            return None
        
        issues = []
        # Bash syntax check
        res = subprocess.run(["bash", "-n", pkgbuild], capture_output=True, text=True)
        if res.returncode != 0:
            issues.append((p, "SYNTAX_ERROR", res.stderr.strip().split("\n")[-1]))
            return issues

        # Evaluate PKGBUILD variables
        bash_code = f"""
        shopt -s extglob
        source "{pkgbuild}" 2>/dev/null || exit 2
        echo "VER: $pkgver"
        echo "REL: $pkgrel"
        echo "ARCH: ${{arch[*]}}"
        echo "GROUPS: ${{groups[*]}}"
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
            issues.append((p, "EVAL_FAIL", res.stderr.strip()))
            return issues

        data = {}
        for line in res.stdout.strip().split("\n"):
            if ": " in line:
                k, v = line.split(": ", 1)
                data[k] = v
            elif ":" in line:
                k = line.split(":", 1)[0]
                data[k] = ""

        ver = data.get("VER", "")
        rel = data.get("REL", "")
        arch = data.get("ARCH", "").split()
        groups = data.get("GROUPS", "").split()
        src_len = int(data.get("SRC_LEN", 0) or 0)
        s512_len = int(data.get("S512_LEN", 0) or 0)
        s256_len = int(data.get("S256_LEN", 0) or 0)
        s1_len = int(data.get("S1_LEN", 0) or 0)
        md5_len = int(data.get("MD5_LEN", 0) or 0)
        b2_len = int(data.get("B2_LEN", 0) or 0)
        pkgname = data.get("PKGNAME", "")
        pkgbase = data.get("PKGBASE", "")

        main_name = pkgbase if pkgbase else (pkgname.split()[0] if pkgname else "")
        if main_name and main_name != p and p not in pkgname.split():
            issues.append((p, "PKGNAME_MISMATCH", f"dir={p}, pkgname={pkgname}, pkgbase={pkgbase}"))

        if "-" in ver:
            issues.append((p, "INVALID_PKGVER", f"pkgver contains hyphen: {ver}"))
        if not rel:
            issues.append((p, "MISSING_PKGREL", ""))
        if not arch:
            issues.append((p, "MISSING_ARCH", ""))
        else:
            for a in arch:
                if a not in ["x86_64", "aarch64", "any", "armv6h", "armv7h", "i686"]:
                    issues.append((p, "UNKNOWN_ARCH", a))

        for g in groups:
            if not g.startswith("blackarch"):
                issues.append((p, "INVALID_GROUP", g))

        if src_len > 0:
            if not (s512_len == src_len or s256_len == src_len or s1_len == src_len or md5_len == src_len or b2_len == src_len):
                issues.append((p, "CHECKSUM_COUNT_MISMATCH", f"src={src_len}, s512={s512_len}, s256={s256_len}, s1={s1_len}, md5={md5_len}, b2={b2_len}"))

        return issues

    with concurrent.futures.ThreadPoolExecutor(max_workers=min(32, os.cpu_count() or 4)) as executor:
        results = list(executor.map(check_pkg, pkgs))

    all_issues = []
    for r in results:
        if r:
            all_issues.extend(r)

    if all_issues:
        print(f"FAILED: Found {len(all_issues)} issue(s) in package check:")
        for pkg, code, msg in all_issues:
            print(f"  [{pkg}] {code}: {msg}")
        sys.exit(1)
    else:
        print(f"SUCCESS: Checked {len(pkgs)} package(s). No issues found!")

def cmd_prep(args):
    """Clean tabs, trailing whitespace, vim modelines, and leading blank lines in PKGBUILD."""
    target_file = args.target_file
    if not os.path.isfile(target_file):
        print(f"Error: File '{target_file}' not found.")
        sys.exit(1)

    with open(target_file, "r", encoding="utf-8", errors="replace") as f:
        lines = f.readlines()

    modified = False
    new_lines = []
    for line in lines:
        # Expand tabs to 2 spaces
        if "\t" in line:
            line = line.replace("\t", "  ")
            modified = True
        # Remove vim modeline
        if re.search(r"#\s*vim:", line):
            modified = True
            continue
        # Remove Id comments
        if re.search(r"#\s*\$\s*Id:", line):
            modified = True
            continue
        # Strip trailing whitespace
        stripped = line.rstrip("\r\n")
        if re.search(r"\s+$", stripped):
            stripped = stripped.rstrip()
            modified = True
        new_lines.append(stripped + "\n")

    # Remove leading blank lines
    while new_lines and new_lines[0].strip() == "":
        new_lines.pop(0)
        modified = True

    if modified:
        with open(target_file, "w", encoding="utf-8") as f:
            f.writelines(new_lines)
        print(f"Formatted and sanitized '{target_file}'")
    else:
        print(f"No changes required for '{target_file}'")

def cmd_audit(args):
    """Audit ELF binaries inside a built package tarball for security mitigations."""
    pkg_file = args.pkg_file
    if not os.path.isfile(pkg_file):
        print(f"Error: File '{pkg_file}' not found.")
        sys.exit(1)

    baaudit_script = os.path.join(REPO_ROOT, "scripts", "baaudit")
    if os.path.isfile(baaudit_script) and os.access(baaudit_script, os.X_OK):
        subprocess.run([baaudit_script, pkg_file])
    else:
        print("Error: baaudit script not executable or missing.")

def main():
    parser = argparse.ArgumentParser(prog="bautil", description="Unified BlackArch Maintenance & Security Utility")
    subparsers = parser.add_subparsers(dest="subcommand", help="Available subcommands")

    # check
    p_check = subparsers.add_parser("check", help="Check PKGBUILD syntax and conventions")
    p_check.add_argument("target", nargs="?", default="all", help="Package name or 'all'")

    # prep
    p_prep = subparsers.add_parser("prep", help="Format and sanitize a PKGBUILD")
    p_prep.add_argument("target_file", help="Path to PKGBUILD file")

    # audit
    p_audit = subparsers.add_parser("audit", help="Security audit an ELF package tarball")
    p_audit.add_argument("pkg_file", help="Package archive file (.pkg.tar.zst)")

    args = parser.parse_args()
    if args.subcommand == "check":
        cmd_check(args)
    elif args.subcommand == "prep":
        cmd_prep(args)
    elif args.subcommand == "audit":
        cmd_audit(args)
    else:
        parser.print_help()

if __name__ == "__main__":
    main()
