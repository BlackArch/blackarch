#!/bin/bash

usage() {
	cat >&2 <<EOF
$(basename "$0") n[ormal]|a[rchtrack]|p[seudo] p[ackage]|d[epends]|s[ource]
                       |        |           |        |         |         |
                       |        |           |        |         |         -- package source (.src)
                       |        |           |        |         -- package without dependencies (.pkg)
                       |        |           |        -- package (.pkg)
                       |        |           -- pseudo packages
                       |        -- packages (archtrack)
                       -- packages (normal)

This script makes distributions for packages and pseudo-packages.
EOF
}

root="$(dirname "$0")/../../"

make_source=false
make_package=false
make_depends=false

case "$1" in
	n|normal|packages)
		packages=("$root"/packages/*/normal)
		;;
	a|archtrack)
		packages=("$root"/packages/*/archtrack)
		;;
	p|pseudo)
		packages=("$root"/pseudo-packages/archtrack*)
		;;
	*)
		usage
		exit 1
		;;
esac

for arg in "${@:2}" ; do
	case "$arg" in
		p|package) make_package=true ;;
		d|deps|depends) make_depends=true ;;
		s|source) make_source=true ;;
	esac
done

if ! $make_source &&
	! $make_package &&
	! $make_depends ; then
	usage
	exit 1
fi

for package in "${packages[@]}"; do
	(
	#echo $package
	source $package/PKGBUILD
	cd "$package"

	if $make_source ; then
		echo "Generating source archive for '$pkgname'..."
		makepkg -f --source
	fi

	if $make_package ; then
		echo "Generating package archive for '$pkgname'..."
		makepkg -df
	fi

	if $make_depends ; then
		echo "Generating package archive for '$pkgname' with dependency checks..."
		makepkg -f
	fi
	)
done
