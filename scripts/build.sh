#!/bin/bash
# usage: build.sh [package_directory] [build_directory]
# TODO: Report a summary of failures

pkgdir=$(realpath "${1:-.}")
builddir=$(realpath "${2:-build}")
mkdir -p "$builddir"
cd "$builddir"

# Initialize gpg-agent
[[ -z "$GPG_AGENT_INFO" ]] && eval $(gpg-agent --daemon)

# Build all packages under the current directory
find "$pkgdir" -type f -name "PKGBUILD" |
while read pkgbuild
do
	makepkg -cs --noconfirm --sign -p "$pkgbuild"
	mv *.pkg.tar.xz* "$builddir"
done

# Put together a database
repo-add blackarch.db.tar.gz *.pkg.tar.xz