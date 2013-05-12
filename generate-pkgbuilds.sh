#!/bin/bash
# Generates PKGBUILD files from the category lists.

rm -rf pseudo-packages
mkdir pseudo-packages

for cat in categories/* ; do
(
cat <<EOF
pkgname=archtrack-$(basename $cat)
pkgver=0.1
pkgrel=0
pkgdesc="security pseudo-package"
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
