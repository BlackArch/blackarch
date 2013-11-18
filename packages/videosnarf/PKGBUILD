# Maintainer: M0Rf30

pkgname=videosnarf
pkgver=0.63
pkgrel=3
pkgdesc="A new security assessment tool for pcap analysis"
arch=('i686' 'x86_64')
url="http://ucsniff.sourceforge.net/videosnarf.html"
license=('GPL')
depends=('libpcap' 'libnet')
source=(http://downloads.sourceforge.net/project/ucsniff/$pkgname/$pkgname-$pkgver.tar.gz
	videosnarf-0.63-fix-linking.patch)

build() {
  cd "$srcdir/$pkgname-$pkgver"
  patch -Np1 -i ../videosnarf-0.63-fix-linking.patch
  autoreconf -fi
  ./configure --prefix=/usr
  make
}

package() {
  cd "$srcdir/$pkgname-$pkgver"

  make DESTDIR="$pkgdir/" install
}

md5sums=('19acd3bcb5c3912b1c5795b505f11958'
         '78472de65f6d4e7e2e41f98624b28147')
