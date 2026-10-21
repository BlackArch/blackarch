#!/usr/bin/env bash
# meta-all.sh <packages-dir> <out-file> [jobs]
# Runs meta-one.sh for every package in parallel (one temp file per package so
# parallel output never interleaves) and concatenates the records.
set -euo pipefail
pkgs=$1 out=$2 jobs=${3:-$(nproc)}
here=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

find "$pkgs" -mindepth 1 -maxdepth 1 -type d -printf '%f\n' | sort |
  xargs -P "$jobs" -I{} sh -c '"$1" "$2/$3" > "$4/$3.meta" 2>/dev/null || true' \
    _ "$here/meta-one.sh" "$pkgs" {} "$tmp"

cat "$tmp"/*.meta > "$out"
echo "metadata for $(grep -c '^@@end$' "$out") packages -> $out"
