# Contributor: Leonardo Gallego <leonardo@archlinux-es.org>
pkgname=libvidcap
pkgver=0.2.1
pkgrel=1
pkgdesc="libvidcap is a cross-platform library for capturing video from webcams and other video capture devices."
arch=('i686' 'x86_64')
url="http://libvidcap.sourceforge.net"
license=('GPL')
source=(http://downloads.sourceforge.net/sourceforge/libvidcap/$pkgname-$pkgver.tar.gz)
md5sums=('99521148426bff8319436494bbc5f0cb')

build() {
  cd "$srcdir/$pkgname-$pkgver"

  ./configure --prefix=/usr
  make || return 1
  make DESTDIR="$pkgdir/" install
}

# vim:set ts=2 sw=2 et:
