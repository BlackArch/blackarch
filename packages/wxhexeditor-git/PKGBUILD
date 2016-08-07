# Maintainer: Steven Honeyman <stevenhoneyman at gmail com>

pkgname=wxhexeditor-git
_pkgbasename=wxhexeditor
pkgver=r451.8c81439
pkgrel=1
pkgdesc="a free hex editor / disk editor for Linux, Windows and MacOSX"
arch=('i686' 'x86_64')
url="http://wxhexeditor.sourceforge.net/"
license=('GPL')
depends=('wxgtk')
makedepends=('git')
provides=('wxhexeditor')
conflicts=('wxhexeditor')
source=('wxhexeditor::git+https://github.com/EUA/wxHexEditor.git')
md5sums=('SKIP')

pkgver() {
  cd "$srcdir/$_pkgbasename"

  ## stolen from the wiki ##
  printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

build() {
  cd "$srcdir/$_pkgbasename"
  make CXXFLAGS="${CXXFLAGS/-fno-rtti/}"
}

package() {
  cd "$srcdir/$_pkgbasename"
  make DESTDIR="$pkgdir" PREFIX="/usr" install
}
