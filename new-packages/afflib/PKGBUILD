# Maintainer: kfgz <kfgz at interia dot pl>
# Contributor: Abakus <java5 at arcor dot de>

pkgname=afflib
pkgver=3.7.0
pkgrel=1
pkgdesc="is an extensible open format for the storage of disk images and related forensic information."
arch=('i686' 'x86_64')
url="http://www.afflib.org"
license=('custom')
depends=('zlib' 'openssl' 'fuse')
source=("https://github.com/downloads/simsong/AFFLIBv3/${pkgname}-${pkgver}.tar.gz")
md5sums=('36a6b5e0cebd70e26bae4bc218dbbace')

build() {
  cd "${srcdir}"/${pkgname}-${pkgver}
  ./configure --prefix=/usr --sysconfdir=/etc
  make
}

package() {
  cd "${srcdir}"/${pkgname}-${pkgver}
  make DESTDIR="${pkgdir}" install
}
