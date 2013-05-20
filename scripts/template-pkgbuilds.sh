#!/bin/bash

#for package in aur-packages/hamster ; do
for package in ../packages/* ; do
	pkgbuild=$package/PKGBUILD

	# categories               the package's archtrack categories
	if grep -q '^groups' $pkgbuild ; then
		sed -i '/^groups/s/(.*)/(%CATEGORIES%)/' $pkgbuild
	else
		sed -i '5a\
groups=%GROUPS%' $pkgbuild
	fi

	# description              a description of the package
	sed -i '/^pkgdesc/s/=.*$/=%PKGDESC%/' $pkgbuild

	# package-name         if applicable, the name of the package on the aur
	sed -i '/^pkgname/s/=.*$/=%PKGNAME%/' $pkgbuild

	# maintainer
	if ! grep -q 'Evan Teitelman' $pkgbuild ; then
		sed -ri 's/^#\s?[Mm]aintainer/# Contributor/' $pkgbuild
		sed -i '1i\
# Maintainer: Evan Teitelman <teitelmanevan at gmail dot com>' $pkgbuild
	fi
done
