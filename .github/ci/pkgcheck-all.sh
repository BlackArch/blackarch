#!/usr/bin/env bash
# pkgcheck-all.sh <packages-dir> <out-file> [jobs]
#
# Runs pkgcheck (pkgcheck-arch, the same style/syntax check tests.yml runs on
# PRs) over EVERY PKGBUILD. Output lines: "<path>:<line>:1: <CODE> <message>".
#
# pkgcheck runs `pacman -Si base-devel` once per PKGBUILD *line*; a caching
# shim on PATH makes the full-repo run take minutes instead of an hour.
set -uo pipefail
pkgs=$1 out=$2 jobs=${3:-$(nproc)}
shim=$(mktemp -d)
trap 'rm -rf "$shim"' EXIT

real=$(command -v pacman)
"$real" -Si base-devel > "$shim/base-devel.si" 2>/dev/null || true
cat > "$shim/pacman" <<EOF
#!/bin/sh
if [ "\$*" = "-Si base-devel" ] && [ -s "$shim/base-devel.si" ]; then
  exec cat "$shim/base-devel.si"
fi
exec "$real" "\$@"
EOF
chmod +x "$shim/pacman"

find "$pkgs" -mindepth 2 -maxdepth 2 -name PKGBUILD | sort |
  PATH="$shim:$PATH" xargs -P "$jobs" -n 40 pkgcheck > "$out" 2>&1
echo "pkgcheck: $(grep -cE ':[0-9]+:[0-9]+: E[0-9]{3}' "$out") errors, $(grep -cE ':[0-9]+:[0-9]+: W[0-9]{3}' "$out") warnings in $(cut -d: -f1 "$out" | sort -u | wc -l) files"
exit 0
