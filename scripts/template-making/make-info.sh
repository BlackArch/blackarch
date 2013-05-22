#!/bin/bash

if (( $# == 0 )) ; then
	packages=("$(dirname "$0")"/../../packages/*)
else
	packages=("$@")
fi

for package in "${packages[@]}" ; do
	pkgbuild=$package/PKGBUILD

	# Skip if PKGBUILD doesn't exist.
	if ! [[ -f $pkgbuild ]] ; then
		continue
	else
		echo "Generating info for '$package'..."
	fi

	# upstream repo
	upstream_repo=$(grep "^$(basename $package)\s" ../package-repos | cut -d' ' -f2)

	# groups               the package's archtrack groups
	groups=($(grep -E "^#?$(basename $package)" ../pseudo-dependency-lists/* | \
	cut -d':' -f1 | cut -d'/' -f3))

	if (( ${#groups[@]} != 0 )) ; then
		groups=(archtrack ${groups[@]})
	else
		echo "Warning: package does not list any groups."
	fi

	# description              a description of the package
	description=$(grep '^pkgdesc' $pkgbuild | cut -d'=' -f2)

	# aur-package-name         if applicable, the name of the package on the aur
	upstream_package_name=$(basename $package)

	# name                     the official/pretty name of the package or primary tool associated with the package
	# we need to make these manually :/
	name=$upstream_package_name

	# aur-maintainer           if applicable, the aur maintainer
	upstream_maintainer='"'$(grep -i '#.*maintainer' $pkgbuild | head -n1 | sed -r 's/^.*:\s?//')'"'
	if [[ -z $upstream_maintainer ]] ; then
		upstream_maintainer='"'$(grep -i '#.*contributor' $pkgbuild | head -n1 | sed -r 's/^.*:\s?//')'"'
	fi

	# working                   describes the status of the package (working, not working)
	if grep -q "#$(basename $package)" ../pseudo-dependency-lists/* ; then
		working=false
	else
		working=true
	fi

	# aur-working               describes the status of the package on the AUR (working, not working)
	upstream_working=$working

	#cat <<EOF
	cat > $package/info <<EOF
pretty_name="${name}"
upstream_name="${upstream_package_name}"
upstream_maintainer=${upstream_maintainer}
groups=(${groups[@]})
description=${description}
working=${working}
upstream_working=${upstream_working}
upstream_repo=${upstream_repo}
EOF
done
