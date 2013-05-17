#!/bin/bash

#for package in aur-packages/* ; do
for package in aur-packages/dnsmap ; do
	pkgbuild=$package/PKGBUILD
	# categories               the package's archtrack categories
	categories=($(egrep "^#?$(basename $package)" pseudo-dependency-lists/* |\
	cut -d':' -f1 | cut -d'/' -f2))

	# description              a description of the package
	description=$(grep '^pkgdesc' $pkgbuild | cut -d'=' -f2)

	# aur-package-name         if applicable, the name of the package on the aur
	aur_package_name=$(basename $package)

	# name                     the official/pretty name of the package or primary tool associated with the package
	# we need to make these manually :/
	name=$aur_package_name

	# aur-maintainer           if applicable, the aur maintainer
	aur_maintainer=$(grep -i '#.*maintainer' $pkgbuild | head -n1 | sed 's/^.*://')
	if [[ -z $aur_maintainer ]] ; then
		aur_maintainer=$(grep -i '#.*contributor' $pkgbuild | head -n1 | sed -r 's/^.*:\s?//')
	fi

	# working                   describes the status of the package (working, not working)
	if grep -q "#$package" pseudo-dependency-lists/* ; then
		working=false
	else
		working=true
	fi

	# aur-working               describes the status of the package on the AUR (working, not working)
	aur_working=$working

	#cat > $package/info <<EOF
	cat <<EOF
pretty_name="${name}"
aur_name="${aur_package_name}"
aur_maintainer="${aur_maintainer}"
categories=(${categories[@]})
description=${description}
working=${working}
aur_working=${aur_working}
EOF
done
