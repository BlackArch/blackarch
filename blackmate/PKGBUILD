# This file is part of BlackArch Linux ( http://blackarch.org ).
# See COPYING for license details.

pkgname='blackmate'
pkgver='0.1'
pkgrel=1
groups=('blackarch')
pkgdesc="BlackArch specific Mate Desktop wm"
arch=('any')
url="https://github.com/Anyon3/blackmate"
license=('GPL')
depends=('xdg-utils' 'gksu', 'mate-terminal', 'mate-menus', 'blackarch-config-gtk')
source=("git+https://github.com/Anyon3/blackmate")
sha1sums=('SKIP')
       
package() {

  cp "$srcdir/blackmate.sh" "${pkgdir}/usr/bin/blackmate"
  chmod +x "${pkgdir}/usr/bin/blackmate"

   mkdir -p "$pkgdir/usr/share/blackmate"
   cp "$srcdir/*" "$pkgdir/usr/share/blackmate" 
}
