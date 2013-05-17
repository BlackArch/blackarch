#!/bin/bash

for package in aur-packages/* ; do
	pkgbuild=$package/PKGBUILD
	if ! grep -q '%PKGDESC%' $pkgbuild ; then
		echo $package "missing desc"
	fi
	if ! grep -q '%PKGNAME%' $pkgbuild ; then
		echo $package "missing name"
	fi
	if ! grep -q '%CATEGORIES%' $pkgbuild ; then
		echo $package "Missing cats"
	fi
done
