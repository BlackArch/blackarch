# Maintainer: PointerK <ws00298046@163.com>
pkgname=liquid-dsp
pkgver=1.3.1
pkgrel=1
pkgdesc="A Software-Defined Radio Digital Signal Processing Library"
arch=('x86_64'
        'armv6h'
        'armv7h'
        'aarch64')
url="http://liquidsdr.org/"
license=('MIT')
optdepends=('fftw: liquid will take advantage if it is available')
conflicts=('liquid-dsp-git')
source=("http://liquidsdr.org/downloads/liquid-dsp-1.3.1.tar.gz")
sha256sums=("e3f66ce72a3b5d74eea5ccffb049c62c422c91b0ab92d6dbbef21af3c3bfec73")

build() {
	cd "${srcdir}/$pkgname-$pkgver"
    ./bootstrap.sh
	./configure --prefix=/usr
	make
}

check() {
	cd "$pkgname-$pkgver"
	make -k check
}

package() {
	cd "$pkgname-$pkgver"
	make DESTDIR="$pkgdir/" install
}

