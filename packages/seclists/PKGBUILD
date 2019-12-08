# Maintainer heichblatt <git@hanneseichblatt.de>
pkgname=seclists-git
pkgbase=seclists
pkgver=r283.8ef8694
pkgrel=1
pkgdesc="A collection of multiple types of lists used during security assessments."
arch=('any')
url="https://github.com/danielmiessler/SecLists/"
license=('unknown')
groups=()
depends=()
makedepends=('git')
provides=("${pkgname}")
conflicts=()
replaces=()
backup=()
options=()
install=
source=('seclists-git::git+https://github.com/danielmiessler/SecLists.git')
noextract=()
md5sums=('SKIP')

pkgver() {
	cd "$srcdir/${pkgname}"
	printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}


package() {
	mkdir -p "${pkgdir}/usr/share/wordlists/${pkgname}"
	find "${srcdir}/${pkgname}" -maxdepth 1 | grep -v \\.git | xargs -I{} cp -r {} "${pkgdir}/usr/share/wordlists/${pkgname}"
}
