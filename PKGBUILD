# Maintainer: s7x <cyber.stx@protonmail.com>
pkgname=qrljacker
pkgver=1
pkgrel=1
pkgdesc='QRLJacker is a highly customizable exploitation framework to demonstrate "QRLJacking Attack Vector".'
arch=('any')
url='https://github.com/OWASP/QRLJacking/tree/master/QRLJacker'
license=('GPL')
groups=('blackarch' 'blackarch-social')
depends=('python' 'python-terminaltables' 'python-selenium' 'python-pillow' 'python-jinja' 'python-user_agent')
makedepends=('git')
provides=('qrljacker')
source=('git+https://github.com/OWASP/QRLJacking.git')
sha512sums=('SKIP')

pkgver() {
  cd QRLJacking

  echo $(git rev-list --count HEAD).$(git rev-parse --short HEAD)
}

package() {
  cd QRLJacking/$pkgname

  install -dm 755 "$pkgdir/usr/bin"

  install -Dm 755 -t "$pkgdir/usr/share/$pkgname" QrlJacker.py
  install -Dm 644 -t "$pkgdir/usr/share/doc/$pkgname" README.md
  install -Dm 644 -t "$pkgdir/usr/share/licenses/$pkgname" ../LICENSE

  cp -a --no-preserve=ownership core/ docs/ sessions/ "$pkgdir/usr/share/$pkgname"

  cat > "$pkgdir/usr/bin/$pkgname" << EOF
#!/bin/sh
cd /usr/share/$pkgname
exec python3 QrlJacker.py "\$@"
EOF

  chmod +x "$pkgdir/usr/bin/$pkgname"
}

