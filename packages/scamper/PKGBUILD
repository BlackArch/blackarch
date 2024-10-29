# Maintainer: hendrikcech <hendrik.cech@gmail.com>
# Co-Maintainer: Marek Küthe <m.k@mk16.de>
# Contributor: Spider.007 <archPackage@spider007.net>

pkgname=scamper
pkgver=20240920
pkgrel=1
pkgdesc="a tool that actively probes the Internet in order to analyze topology and performance"
url="https://www.caida.org/catalog/software/scamper/"
source=("https://www.caida.org/catalog/software/scamper/code/$pkgname-cvs-$pkgver.tar.gz")
sha256sums=('02d242c577e31d61d3babec0a088e997f3c96bcc517f6d2ad35eb41cf1c5f616')
depends=('openssl' 'zlib' 'bzip2' 'xz')
license=('GPL-2.0-or-later')
arch=('i686' 'x86_64')

build() {
    cd "$pkgname-cvs-$pkgver"

    ./configure --prefix=/usr
    make
}

package() {
    cd "$pkgname-cvs-$pkgver"

    make DESTDIR="$pkgdir/" install
    chmod u+s $pkgdir/usr/bin/scamper
}
