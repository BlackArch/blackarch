# This is an example PKGBUILD file. Use this as a start to creating your own,
# and remove these comments. For more information, see 'man PKGBUILD'.
# NOTE: Please fill out the license field for your package! If it is unknown,
# then please put 'unknown'.

# Maintainer: James Buckley <xanium4332@gmail.com>
pkgname=nfcutils
pkgver=0.3.0
pkgrel=1
pkgdesc="Provides a simple 'lsnfc' command that list tags which are in your NFC device field"
arch=('i686' 'x86_64')
url="http://code.google.com/p/nfc-tools"
license=('LGPL3')
depends=('libnfc>=1.4.0')
source=(http://nfc-tools.googlecode.com/files/$pkgname-$pkgver.tar.gz)
md5sums=('161d640eb7f8d17762f57f7b27437f33')

build() {
  cd "$srcdir/$pkgname-$pkgver"

  ./configure --prefix=/usr
  make
}

package() {
  cd "$srcdir/$pkgname-$pkgver"

  make DESTDIR="$pkgdir/" install
}

# vim:set ts=2 sw=2 et:
