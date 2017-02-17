# Maintainer : Christian Hofmann <chof@pfho.net>
pkgname=python-malboxes-git
_pkgname=malboxes
pkgver=161.f0a6a97
pkgrel=1
pkgdesc="Builds malware analysis Windows VMs so that you don't have to."
arch=('any')
source='https://github.com/GoSecure/malboxes'
license=('GPL')
depends=('python' 'packer-io' 'virtualbox' 'vagrant' 'python-jsmin')
makedepends=('python-pip' 'git')
options=(!emptydirs)
source=(git+https://github.com/GoSecure/malboxes.git)
md5sums=('SKIP')

pkgver() {
  cd "$srcdir/$_pkgname"

  echo $(git rev-list --count HEAD).$(git rev-parse --short HEAD)
}

package() {
  cd "$srcdir/$_pkgname"
  PIP_CONFIG_FILE=/dev/null pip3 install --isolated --root="$pkgdir" --ignore-installed --no-deps ./
}

# vim:set ts=2 sw=2 et:
