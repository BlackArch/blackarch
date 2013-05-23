#!/bin/bash

for package in "$(dirname "$0")"/../../new-packages/* ; do
	pkgbuild=$package/PKGBUILD.in

	if ! [[ -f $pkgbuild ]] ; then
		echo >&2 $package missing entirely
		continue
	fi

	if ! grep -q '%PKGDESC%' $pkgbuild ; then
		echo $package is missing desc
	fi
	if ! grep -q '%PKGNAME%' $pkgbuild ; then
		echo $package is missing name
	fi
	if ! grep -q '%GROUPS%' $pkgbuild ; then
		echo $package is missing groups
	fi
done
