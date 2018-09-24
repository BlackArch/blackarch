# This file is part of BlackArch Linux ( http://blackarch.org ).
# See COPYING for license details.

pkgname=yate-bts
pkgver=6.0.0
pkgrel=1
pkgdesc="YateBTS is an open source GSM Base Station software."
groups=('blackarch' 'blackarch-radio')
arch=('x86_64')
url="https://yatebts.com/"
license=('GPL')
depends=('yate')
source=("http://yate.null.ro/tarballs/yatebts6/yate-bts-${pkgver}-1.tar.gz"
	"gcc.patch")
sha256sums=('02c9a38f4051141868fb5dabf890f589fb59bb6739edd575892c450be726252f'
						'5b1714e8e789cd4a0f5283e0bfcc07cfaf975cb729d95f2a0165661cda08871d')
backup=('etc/yate/snmp_data.conf'
				'etc/yate/ybts.conf'
				'etc/yate/tmsidata.conf'
				'etc/yate/subscribers.conf')
optdepends=('nginx: nipc_web support'
						'apache: nipc_web support'
						'php-fpm: nipc_web support')

prepare() {
  cd $pkgname
  patch -Np1 -i "${srcdir}/gcc.patch"
}

build() {
  cd $srcdir/${pkgname}
  ./configure --prefix=/usr \
              --sysconfdir=/etc
  make
}

package(){
  cd $srcdir/${pkgname}
  make DESTDIR=$pkgdir install
	touch $pkgdir/etc/yate/snmp_data.conf $pkgdir/etc/yate/tmsidata.conf
}
