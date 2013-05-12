#!/bin/bash
# Generates PKGBUILD files from the category lists.

rm -rf pseudo-packages
mkdir pseudo-packages

for cat in categories/* ; do
(
cat <<EOF
# Maintainer: Evan Teitelman <teitelmanevan at gmail dot com>
pkgname=archtrack-$(basename $cat)
pkgver=0.1
pkgrel=1
pkgdesc="This pseudo-package is part of Archtrack, a project inspired by Backtrack and Kali. Archtrack is able to turn an Arch Linux system into a fully functional penetration testing system."
arch=('any')
url="http://github.com/EvanTeitelman/archtrack"
license=('BSD')
EOF

echo -e "depends=(\n$(< $cat)\n)"

cat <<EOF
EOF

cat <<\EOF

#build() {
#	:
#}

package() {
	:
}
EOF
) > pseudo-packages/PKGBUILD-$(basename $cat)
done
