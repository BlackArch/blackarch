# $Id: PKGBUILD 266002 2017-11-09 13:16:03Z spupykin $
# Maintainer: Sergej Pupykin <pupykin.s+arch@gmail.com>
# Maintainer: rubenvb vanboxem <dottie> ruben <attie> gmail <dottie> com

_targets="i686-w64-mingw32 x86_64-w64-mingw32"

pkgname='mingw-w64-headers'
pkgver=5.0.3
_pkgver=${pkgver/rc/-rc}
pkgrel=1
pkgdesc="MinGW-w64 headers for Windows"
arch=('any')
url="http://mingw-w64.sourceforge.net"
license=('custom')
groups=('mingw-w64-toolchain' 'mingw-w64')
options=('!strip' '!libtool' '!emptydirs')
validpgpkeys=('CAF5641F74F7DFBA88AE205693BDB53CD4EBC740')
source=(https://sourceforge.net/projects/mingw-w64/files/mingw-w64/mingw-w64-release/mingw-w64-v${_pkgver}.tar.bz2{,.sig})
sha256sums=('2a601db99ef579b9be69c775218ad956a24a09d7dabc9ff6c5bd60da9ccc9cb4'
            'SKIP')

build() {
  for _target in ${_targets}; do
    msg "Configuring ${_target} headers"
    mkdir -p "$srcdir"/headers-${_target} && cd "$srcdir"/headers-${_target}
    "$srcdir"/mingw-w64-v${_pkgver}/mingw-w64-headers/configure --prefix=/usr/${_target} --enable-sdk=all --enable-secure-api --host=${_target}
  done
}

package() {
  for _target in ${_targets}; do
    msg "Installing ${_target} headers"
    cd "$srcdir"/headers-${_target}
    make DESTDIR="$pkgdir" install
    rm "$pkgdir"/usr/${_target}/include/pthread_signal.h
    rm "$pkgdir"/usr/${_target}/include/pthread_time.h
    rm "$pkgdir"/usr/${_target}/include/pthread_unistd.h
  done

  msg "Installing MinGW-w64 licenses"
  install -Dm644 "$srcdir"/mingw-w64-v${_pkgver}/COPYING.MinGW-w64/COPYING.MinGW-w64.txt "$pkgdir"/usr/share/licenses/${pkgname}/COPYING.MinGW-w64.txt
  install -Dm644 "$srcdir"/mingw-w64-v${_pkgver}/COPYING.MinGW-w64-runtime/COPYING.MinGW-w64-runtime.txt "$pkgdir"/usr/share/licenses/${pkgname}/COPYING.MinGW-w64-runtime.txt
  install -Dm644 "$srcdir"/mingw-w64-v${_pkgver}/mingw-w64-headers/ddk/readme.txt "$pkgdir"/usr/share/licenses/${pkgname}/ddk-readme.txt
  install -Dm644 "$srcdir"/mingw-w64-v${_pkgver}/mingw-w64-headers/direct-x/COPYING.LIB "$pkgdir"/usr/share/licenses/${pkgname}/direct-x-COPYING.LIB
  install -Dm644 "$srcdir"/mingw-w64-v${_pkgver}/mingw-w64-headers/direct-x/readme.txt "$pkgdir"/usr/share/licenses/${pkgname}/direct-x-readme.txt
}
