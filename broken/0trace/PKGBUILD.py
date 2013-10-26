pkgname=0trace
pkgver=1.5
pkgrel=1
pkgdesc="A hop enumeration tool"
url="http://jon.oberheide.org/0trace/"
arch=('x86_64' 'i686')
license=('GPLv3')
depends=('libdnet' 'pytho2-dpkt' 'pypcap')
source=("http://jon.oberheide.org/0trace/downloads/0trace.py")
md5sums=('f0e1131ed28e5809c081eead73a4b0da')


package() {
  cd "$srcdir/${pkgname}-${pkgver}"
}
