# Maintainer: Jaroslav Lichtblau <dragonlord@aur.archlinux.org>
# Contributor: ality@pbrane.org

pkgname=ranpwd
pkgver=1.2
pkgrel=2
pkgdesc="A tool to generate random passwords."
arch=('i686' 'x86_64')
url="http://www.kernel.org/pub/software/utils/admin/ranpwd"
license=('GPL')
source=(http://ftp.cvut.cz/pub/software/utils/admin/$pkgname/$pkgname-$pkgver.tar.bz2)
md5sums=('ce5bb201130aad92f0f9a90fc8be624f')

build() {
  cd ${srcdir}/$pkgname-$pkgver

  ./configure --prefix=/usr --mandir=/usr/share/man
  make
}

package () {
  cd ${srcdir}/$pkgname-$pkgver

  make INSTALLROOT=${pkgdir} install
}
