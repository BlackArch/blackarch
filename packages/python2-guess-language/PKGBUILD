pkgname=python2-guess-language
pkgver=0.2
pkgrel=2
pkgdesc="Attempts to determine the natural language of a selection of Unicode (utf-8) text"
arch=('any')
url='https://pypi.python.org/pypi/guess-language'
license=('LGPL')
depends=('python2')
source=("https://pypi.python.org/packages/source/g/guess-language/guess-language-$pkgver.tar.gz")
md5sums=('3a3574c2e8b44f84e656f90684f0c6aa')

package() {
  cd "$srcdir/guess-language-$pkgver"
  python2 setup.py install "--root=$pkgdir" --optimize=1
}
