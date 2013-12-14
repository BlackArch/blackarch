pkgname=python-xbee
pkgver=2.1.0
pkgrel=1
pkgdesc='Python tools for working with XBee radios'
url='https://pypi.python.org/pypi/XBee'
arch=('any')
license=('MIT')
depends=('python' 'python-pyserial')
source=("https://pypi.python.org/packages/source/X/XBee/XBee-${pkgver}.tar.gz")
md5sums=('ed3c8c24fd375d980fcef9ce520c53e8')

package() {
  cd XBee-$pkgver
  python setup.py install --root="$pkgdir"
}
