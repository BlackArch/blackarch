#!/bin/bash
#set -e

mkdir dists
cd dists
for pkgbuild in ../pkgbuilds/PKGBUILD-* ; do
	makepkg -f --source -p "$pkgbuild"
done
