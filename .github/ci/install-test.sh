#!/usr/bin/env bash
# install-test.sh - runs in a FRESH builder container (nothing but the base
# system): installs the verified packages from /pkgs, so runtime depends are
# resolved from the real repos and a dependency that only sat in makedepends
# during the build is caught.
set -uo pipefail
shopt -s nullglob
pkgs=(/pkgs/*.pkg.tar.zst)
(( ${#pkgs[@]} )) || { echo "no packages to install"; exit 2; }
sudo pacman -U --noconfirm --noprogressbar "${pkgs[@]}"
