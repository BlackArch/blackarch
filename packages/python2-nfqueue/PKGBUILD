pkgname=python2-nfqueue
pkgver=0.4
pkgrel=2
pkgdesc="Python bindings for libnetfilter_queue by Pierre Chifflier"
url="https://www.wzdftpd.net/redmine/projects/nfqueue-bindings/wiki/"
arch=('any')
license=('GPL')
depends=('python2' 'libnetfilter_queue')
makedepends=('python2-distribute' 'cmake' 'swig')
source=("http://ftp.de.debian.org/debian/pool/main/n/nfqueue-bindings/nfqueue-bindings_$pkgver.orig.tar.gz"
        python-support.patch)
md5sums=('2ac7cca1c2f41dfee4a600374ef86292'
         'ea00b499e84a4e3b7b34595625da451a')

prepare() {
  cd "$srcdir/nfqueue-bindings-$pkgver"

  patch -p1 -i ../python-support.patch

  # I don't feel like editing the patch.
  sed -i 'i\#include <stdint.h>' nfq.h
  sed -i 's/dist-packages/site-packages/' python/CMakeLists.txt
}

package() {
  cd "$srcdir/nfqueue-bindings-$pkgver"

  # Examples.
  install -dm755 -p "$pkgdir/usr/share/doc/nfqueue-bindings/examples"
  install -m644 examples/*.py "$pkgdir/usr/share/doc/nfqueue-bindings/examples"

  # Library.
  make PREFIX="$pkgdir/usr" install
}
