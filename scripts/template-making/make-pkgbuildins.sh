#!/bin/bash
# Generates PKGBUILD.in files from PKGBUILD files
# Use 'make-info' to generate info files for the PKGBUILD files.
# TODO: merge with 'make-info'

if [[ $# = 0 ]] ; then
	cat <EOF
	$(basename "$0") FILE...
EOF
	exit 1
fi

for pkgbuild ; do
	pkgbuildin="$(dirname "$pkgbuild")/$(basename "$pkgbuild").in"
	cp "$pkgbuild" "$pkgbuildin"
	echo "Creating '$pkgbuildin' from '$pkgbuild'..."

	# groups
	if grep -q '^groups' $pkgbuildin ; then
		echo "Substituting package groups..."
		sed -i '/^groups/s/(.*)/(%GROUPS%)/' $pkgbuildin
	else
		echo "Adding package groups..."
		sed -i '5a\groups=%GROUPS%' $pkgbuildin
	fi

	# description
	echo "Substituting package description"
	sed -i '/^pkgdesc/s/=.*$/=%PKGDESC%/' $pkgbuildin

	# package name
	echo "Substituting package name..."
	sed -i '/^pkgname/s/=.*$/=%PKGNAME%/' $pkgbuildin

	# maintainer (we maintain this package now)
	echo "Changing maintainer..."
	if ! grep -q 'Evan Teitelman' $pkgbuildin ; then
		sed -ri 's/^#\s?[Mm]aintainer/# Contributor/' $pkgbuildin
		sed -i '1i\# Maintainer: Evan Teitelman <teitelmanevan at gmail dot com>' $pkgbuildin
	fi
done
