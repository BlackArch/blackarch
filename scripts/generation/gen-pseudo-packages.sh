#!/bin/bash

cd "$(dirname "$0")/../.."

if [[ $# = 0 ]] ; then
	packages=(pseudo-packages/archtrack*)
else
	packages=("$@")
fi

for pseudo in "${packages[@]}" ; do
	depends=$(grep -l "^groups.*\<$(basename "$pseudo")\>" packages/*/info |
	sed -e 's#/info$##' -e 's#^.*/##g')
	sed \
	-e "/%DEPENDS%/r "<(echo "$depends") \
	-e "/%DEPENDS%/d" \
	-e "s|%PKGNAME%|$(basename "$pseudo")|" \
	-e "s|%PKGDESC%|$(< "$pseudo/description")|" \
	pseudo-packages/PKGBUILD.in > $pseudo/PKGBUILD
done
