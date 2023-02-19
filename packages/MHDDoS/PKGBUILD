# This file is part of BlackArch Linux ( https://www.blackarch.org/ ).

pkgname=MHDDoS
pkgver=2.4.1
pkgrel=1
pkgdesc='Best DDoS Attack Script Python3, (Cyber / DDos) Attack With 56 Methods.'
arch=('any')
groups=('blackarch' 'blackarch-dos')
url='https://github.com/MatrixTM/MHDDoS'
license=('MIT')
depends=('python3')
makedepends=('git' 'python-setuptools' 'python-pip')
source=("git+https://github.com/MatrixTM/$pkgname.git")
sha512sums=('SKIP')
install="$pkgname.install"

pkgver() {
  cd $pkgname

  git describe --long --tags | sed 's/\([^-]*-g\)/r\1/;s/-/./g'
}

package() {
  cd $pkgname

  install -dm 755 "$pkgdir/usr/bin"
  install -Dm 644 requirements.txt "$pkgdir/usr/share/$pkgname/requirements.txt"
  install -Dm 644 -t "$pkgdir/usr/share/doc/$pkgname/" *.md

  install -Dm 644 LICENSE "$pkgdir/usr/share/licenses/$pkgname/LICENSE"

  rm -rf LICENSE *.md .gitignore

  cp -a * "$pkgdir/usr/share/$pkgname/"

  cat > "$pkgdir/usr/bin/$pkgname" << EOF
#!/bin/sh
source /usr/share/$pkgname/venv/bin/activate
exec python /usr/share/$pkgname/start.py "\$@"
EOF

  chmod a+x "$pkgdir/usr/bin/$pkgname"
}
