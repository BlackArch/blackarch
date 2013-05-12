#!/bin/bash
# Generates PKGBUILD files from the category lists.

rm -rf pseudo-packages
mkdir pseudo-packages

for cat in categories/* ; do
(
cat <<EOF
pkgname=archtrack-$(basename $cat)
pkgver=0.1
pkgrel=1
pkgdesc="This pseudo-package is part of Archtrack, a project inspired by Backtrack and Kali. Archtrack is able to turn an Arch Linux system into a fully functional penetration testing system."
arch=('any')
url="http://github.com/EvanTeitelman/archtrack"
license=('BSD')
groups=()
EOF

echo -e "depends=(\n$(< $cat)\n)"

cat <<EOF
makedepends=()
optdepends=()
provides=()
conflicts=()
replaces=()
backup=()
options=()
install=
changelog=
source=()
noextract=()
md5sums=()
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
