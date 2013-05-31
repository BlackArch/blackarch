#!/bin/bash
# Installs all package dependencies from the AUR and official repos.

for pack in "$(dirname "$0")"/packages/* ; do
	(
	source "$pack/PKGBUILD"
	yaourt --noconfirm -S ${makedepends[@]} ${depends[@]}
	)
done
