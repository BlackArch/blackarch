pkgname=python2-pygithub
pkgver=1.20.0
pkgrel=2
pkgdesc="Provides access to the full Github API v3"
arch=('any')
url='https://pypi.python.org/pypi/PyGithub'
license=('LGPL')
depends=('python2')
source=("https://pypi.python.org/packages/source/P/PyGithub/PyGithub-$pkgver.tar.gz")
md5sums=('f917f81eaa5ccc5ef9097c30ace0b078')

package() {
  cd "$srcdir/PyGithub-$pkgver"
  python2 setup.py install "--root=$pkgdir" --optimize=1
}
