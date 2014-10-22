# This file is part of BlackArch Linux ( http://blackarch.org ).
# See COPYING for license details.

pkgname='wifiphisher'
pkgver=12.73f24a7
pkgrel=1
groups=('blackarch' 'blackarch-wireless' 'blackarch-social')
pkgdesc='Fast automated phishing attacks against WPA networks.'
arch=('any')
url='https://github.com/sophron/wifiphisher'
license=('MIT')
depends=('python2' 'python2-httplib2' 'scapy' 'aircrack-ng')
makedepends=('git')
source=('git+https://github.com/sophron/wifiphisher.git')
sha1sums=('SKIP')

pkgver() {
  cd "$srcdir/wifiphisher"

  echo $(git rev-list --count HEAD).$(git rev-parse --short HEAD)
}

package() {
  cd "$srcdir/wifiphisher"

  mkdir -p "$pkgdir/usr/bin"
  mkdir -p "$pkgdir/usr/share/wifiphisher"

  install -Dm644 README.md "$pkgdir/usr/share/doc/wifiphisher/README.md"
  install -Dm644 LICENSE "$pkgdir/usr/share/licenses/wifiphisher"

  rm README.md LICENSE

  cp -a * "$pkgdir/usr/share/wifiphisher"

  cat > "$pkgdir/usr/bin/wifiphisher" << EOF
#!/bin/sh
cd /usr/share/wifiphisher
exec python2 wifiphisher.py "\$@"
EOF

  chmod a+x "$pkgdir/usr/bin/wifiphisher"
}
