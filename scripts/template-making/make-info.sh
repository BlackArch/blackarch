#!/bin/bash

if (( $# == 0 )) ; then
	packages=("$(dirname "$0")"/../../new-packages/*)
else
	packages=("$@")
fi

for package in "${packages[@]}" ; do
echo "$package"
	pkgbuild=$package/PKGBUILD

	# Skip if PKGBUILD doesn't exist.
	if ! [[ -f $pkgbuild ]] ; then
		continue
	else
		echo "Generating info for '$package'..."
	fi

	upstream_package_name=$(basename "$package")

	upstream_repo=$(yaourt -Si $upstream_package_name | sed '/^Repository/s/^.*: //')

	groups=($(grep -E "^#?$(basename $package)" ../pseudo-dependency-lists/* | \
	          cut -d':' -f1 | cut -d'/' -f3))

	if (( ${#groups[@]} != 0 )) ; then
		groups=(archtrack ${groups[@]})
	else
		echo "Warning: package does not list any groups."
	fi

	description=$(grep '^pkgdesc' $pkgbuild | cut -d'=' -f2)

	name=$upstream_package_name

	upstream_maintainer='"'$(grep -m1 -i '#.*maintainer' $pkgbuild | sed -r 's/^.*:\s?//')'"'
	if [[ -z $upstream_maintainer ]] ; then
		upstream_maintainer='"'$(grep -m1 -i '#.*contributor' $pkgbuild | sed -r 's/^.*:\s?//')'"'
	fi

	working=true
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
