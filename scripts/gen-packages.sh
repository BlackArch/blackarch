#!/bin/bash

usage() {
	cat <<EOF
$(basename "$0") n[ormal]|a[archtrack]
EOF
}

if [[ $# = 0 ]] ; then
	usage
	exit 1
fi

if (( $# > 1 )) ; then
	packages=("${@:2}")
else
	packages=(../packages/*)
fi

for package in ${packages[@]} ; do
	if [[ ! -d $package ]] ; then
		echo >&2 "'$package' not found."
		continue
	fi

	info="$package/info"

	case "$1" in
		n|normal)
			outdir=normal
			pkgname=$(grep '^upstream_name=' "$info" | cut -d'=' -f2)
			groups="()"
			name_suffix=
			;;
		a|archtrack)
			outdir=archtrack
			pkgname=$(basename "$package")
			groups=$(grep '^groups=' "$info" | cut -d'=' -f2)

			# We add the '-archtrack' suffix to package names if they exist in the
			#  official repositories.
			if ! grep -q 'upstream_repo=aur' $info ; then
				name_suffix='-archtrack'
			else
				name_suffix=
			fi
			;;
		*)
			usage
			exit 1
			;;
	esac
	pkgdesc=$(grep '^description=' "$info" | cut -d'=' -f2)

	rm -rf "$package/$outdir"
	mkdir "$package/$outdir"

	sed -e "s|%PKGNAME%|$pkgname$name_suffix|" \
	    -e "s|%PKGDESC%|$pkgdesc|" \
	    -e "s|%GROUPS%|$groups|" \
	    "$package/PKGBUILD.in" > "$package/$outdir/PKGBUILD"

	# Sorry for parsing ls output like this. extglob wasn't working for some
	#  reason. I made sure no source files contained strange characters in their
	#  names.
	ls "$package" | grep -Ev '^(normal|archtrack|PKGBUILD.in|info)$' |
	while read src ; do
		ln -fs "../$src" "$package/$outdir/$(basename "$src")"
		#cp "$package/$src" "$package/$outdir"
	done
done
