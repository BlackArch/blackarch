# This file is part of BlackArch Linux ( http://blackarch.org ).
# See COPYING for license details.

pkgname='blackman'
pkgver='0.5'
pkgrel=1
pkgdesc="Emerge for Blackarch - Download and compile packages as Emerge does"
arch=('any')
url="https://github.com/BlackArch/blackman"
license=('GPL')
source=("https://github.com/BlackArch/blackman/archive/v${pkgver}.tar.gz")
sha1sums=('ac0da4f9ada8986ca30cdf308d2095e87bc83a91')

package() {
  cd "$srcdir/blackman-${pkgver}"

  install -dm755 "$pkgdir/usr/bin/"
  install -dm755 "$pkgdir/usr/share/blackman"
  #install -dm755 "$pkgdir/usr/share/doc/blackman"

  cp -Lr --no-preserve=ownership blackman "$pkgdir/usr/share/blackman"

  ln -s "/usr/share/blackman/blackman" "$pkgdir/usr/bin/blackman"
}
