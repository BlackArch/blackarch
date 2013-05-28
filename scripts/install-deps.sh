#!/bin/bash
# Installs all package dependencies.

for pack in "$(dirname "$0")"/packages/* ; do
	(
	source "$pack/PKGBUILD"
	yaourt --noconfirm -S ${makedepends[@]} ${depends[@]}
	)
done
