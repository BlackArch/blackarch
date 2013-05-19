#!/bin/bash

usage() {
	cat <<EOF
$(basename "$0") a[ur]|r[epo]
EOF
	exit
}

if [[ $# = 0 ]] ; then
	usage
fi

for package in ../packages/* ; do
	info="$package/info"

	case "$1" in
		a|aur)
			outdir=aur
			pkgname=$(grep '^aur_name=' "$info" | cut -d'=' -f2)
			groups="()"
			;;
		r|repo)
			outdir=repo
			pkgname=$(basename "$package")
			groups=$(grep '^categories=' "$info" | cut -d'=' -f2)
			;;
		*)
			usage
			;;
	esac
	pkgdesc=$(grep '^description=' "$info" | cut -d'=' -f2)

	rm -rf "$package/$outdir"
	mkdir "$package/$outdir"

	sed -e "s|%PKGNAME%|$pkgname|" \
	    -e "s|%PKGDESC%|$pkgdesc|" \
	    -e "s|%GROUPS%|$groups|" \
	    "$package/PKGBUILD.in" > "$package/$outdir/PKGBUILD"

	# Sorry for parsing ls output like this. extglob wasn't working for some
	#  reason. I made sure no source files contained strange characters in their
	#  names.
	ls "$package" | grep -Ev '^(aur|repo|PKGBUILD.in|info)$' |
	while read src ; do
		ln -fs "$src" "$package/$outdir/$(basename "$src")"
	done
done
