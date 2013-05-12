# Maintainer: Evan Teitelman <teitelmanevan@gmail.com>

pkgname=ollydbg
pkgver=201g
pkgrel=1
pkgdesc='A 32-bit assembler-level analysing debugger'
arch=('any')
url='http://www.ollydbg.de'
license=('custom')
depends=('wine' 'wine_gecko' 'wine-mono')
makedepends=('unzip')
source=(ollydbg "${url}/odbg${pkgver}.zip")
md5sums=('17e173635428db7cf7b12530dccf7076'
         '1cf4d4f9217c0d8bb6a33eb3c89c7606')

build() {
	cd $srcdir
}

package() {
	cd $srcdir
	install -D -m644 ollydbg.exe $pkgdir/usr/share/ollydbg/ollydbg.exe
	install -D -m644 dbghelp.dll $pkgdir/usr/share/ollydbg/dbghelp.dll
	install -D -m755 ollydbg $pkgdir/usr/bin/ollydbg
}
