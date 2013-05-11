#!/bin/bash

for cat in categories/* ; do
(
cat <<EOF
pkgname=archtrack-$cat
pkgver=0.0
pkgrel=0
pkgdesc="security pseudo-package"
arch=('any')
url="http://github.com/EvanTeitelman/archtrack"
license=()
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
) >> pkgbuilds/PKGBUILD-$(basename $cat)
done
