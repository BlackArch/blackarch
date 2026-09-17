# Maintainer: Mathias Nesci <maththiasneesci@gmail.com>
pkgname=copia-forense
pkgver=1.0.0
pkgrel=1
pkgdesc="Forensic Logical Acquisition Tool with SHA-256 validation"
arch=('any')
url="https://github.com/N3sci/CopiaForense"
license=('MIT')
depends=('python' 'python-fpdf2')
makedepends=('python-build' 'python-installer' 'python-wheel' 'python-setuptools')
_pkgname=copia_forense
source=("https://files.pythonhosted.org/packages/source/c/${pkgname}/${_pkgname}-${pkgver}.tar.gz")
sha256sums=('SKIP')

build() {
  cd "${_pkgname}-${pkgver}"
  python -m build --wheel --no-isolation
}

package() {
  cd "${_pkgname}-${pkgver}"
  python -m installer --destdir="${pkgdir}" dist/*.whl
}
