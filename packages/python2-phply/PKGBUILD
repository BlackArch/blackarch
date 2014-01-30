pkgname=python2-phply
pkgver=0.3.1
pkgrel=4
pkgdesc="PHP parser written in Python using PLY"
arch=('any')
url="https://github.com/ramen/phply"
license=('BSD')
depends=('python2')
source=("git+http://github.com/ramen/phply")
md5sums=('SKIP')

package() {
  cd "$srcdir/phply"
  python2 setup.py install "--root=$pkgdir" --optimize=1

  # Fix a conflict with community/python2-poster.
  # This probably shouldn't be here anyways.
  rm -rf "$pkgdir/usr/lib/python2.7/site-packages/tests"
}
