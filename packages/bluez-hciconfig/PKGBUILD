# $Id$
# Maintainer: Sergej Pupykin <arch+pub@sergej.pp.ru>

pkgname=bluez-hciconfig
pkgver=5.50
pkgrel=1
pkgdesc="deprecated hciconfig tool from bluez"
url="http://www.bluez.org/"
arch=('x86_64')
license=('GPL2')
depends=("bluez")
source=(hciconfig.1 hciconfig.c textfile.c textfile.h csr.h csr.c util.h)
sha256sums=('c7d50bbdec9d17b6c864834a8a9b6b3579b5b24554cb5440e96498858deb7dee'
            '149d814df44c9fe851230f4d037f494a49f3dc7e00eecaa6aba73c9ae1d1a128'
            '88f2a8db6687de0369266eb96a7ab007496e13fdb969de214f208c3866794e85'
            '53317986e699d03f1502ccd03cf487950343769b476f215e67b57b361ae9cf5f'
            '56c3227d3022c317677e9ebfc7fd4043177fc24b61871304ce988b2b29af9c53'
            '44911c2262e1f279c53f910200e72a710fcc1a1d2d411a9c3c1a51fd5dc06611'
            'f1aa4d8199959d94dc1d08160f06a08c3bbb5208c4a0eda881dada31e2eca46a')

build() {
  gcc hciconfig.c csr.c -lbluetooth -o hciconfig -DVERSION=\"$pkgver\"
}

package() {
  install -Dm0755 hciconfig "$pkgdir"/usr/bin/hciconfig
  install -Dm0755 hciconfig.1 "$pkgdir"/usr/share/man/man1/hciconfig.1
}
