# Contributor: Francesco Piccinno <stack.box@gmail.com>

pkgname=
pkgver=
pkgrel=1
pkgdesc=""
url="http://"
groups=('archpwn' 'archpwn-')
license=('GPL')
depends=('')
source=(${pkgname}-${pkgver}.tar.gz)
md5sums=()

build() {
  cd "$srcdir"
  make || return 1

  install -d $pkgdir/usr/bin
  install -d $pkgdir/usr/share/doc/$pkgname

  install -Dm755 $pkgname $pkgdir/usr/bin/
  install -Dm644 README $pkgdir/usr/share/doc/$pkgname/
}

# vim:set ts=2 sw=2 et:
