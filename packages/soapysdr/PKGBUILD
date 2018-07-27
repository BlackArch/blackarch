# Maintainer: Filipe Laíns (FFY00) <filipe.lains@gmail.com>
pkgname=soapysdr
_pkgname=SoapySDR
pkgver=0.6.1
_gitver=soapy-sdr-$pkgver
pkgrel=1
pkgdesc="Vendor and platform neutral SDR support library"
arch=('any')
url="https://github.com/pothosware/SoapySDR"
license=('custom:Boost')
makedepends=('cmake')
optdependes=('swig: bindings'
	    'python: python bindings'
	    'doxygen: documentation')
source=("$pkgname-$pkgver.tar.gz::$url/archive/$_gitver.tar.gz")
sha256sums=('2ecf8e43518dfb24bd172923adf6ce25a6d3e39de3a8aadf26c890f60dd1e9f7')

build() {
  mkdir -p "$srcdir"/$_pkgname-$_gitver/build
  cd "$srcdir"/$_pkgname-$_gitver/build

  cmake .. \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DCMAKE_BUILD_TYPE=Release

  make
}

package() {
  cd "$srcdir"/$_pkgname-$_gitver/build

  make DESTDIR="$pkgdir" install

  install -dm 644 "$pkgdir"/usr/share/licenses/$pkgname
  install -Dm 644 "$srcdir"/$_pkgname-$_gitver/LICENSE_1_0.txt "$pkgdir"/usr/share/licenses/$pkgname/LICENSE.txt
}
