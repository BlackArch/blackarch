# $Id: PKGBUILD 266875 2017-11-15 14:29:11Z foutrelis $
# Maintainer: Sergej Pupykin <pupykin.s+arch@gmail.com>
# Maintainer: rubenvb vanboxem <dottie> ruben <attie> gmail <dottie> com

_targets="i686-w64-mingw32 x86_64-w64-mingw32"

pkgname=mingw-w64-gcc
pkgver=7.2.1+20171224
_islver=0.18
pkgrel=1
pkgdesc="Cross GCC for the MinGW-w64 cross-compiler"
arch=('x86_64')
url="http://gcc.gnu.org"
license=('GPL' 'LGPL' 'FDL' 'custom')
groups=('mingw-w64-toolchain' 'mingw-w64')
depends=('zlib' 'libmpc'
	 'mingw-w64-crt' 'mingw-w64-binutils' 'mingw-w64-winpthreads'
	 'mingw-w64-headers')
makedepends=("gcc-ada=${pkgver}")
#checkdepends=('dejagnu') # Windows executables could run on Arch through bin_mft and Wine
optdepends=()
provides=('mingw-w64-gcc-base')
replaces=()
backup=()
options=('!strip' 'staticlibs' '!emptydirs' '!buildflags')
source=(https://sources.archlinux.org/other/gcc/gcc-${pkgver/+/-}.tar.xz{,.sig}
	"http://isl.gforge.inria.fr/isl-${_islver}.tar.bz2")
validpgpkeys=(F3691687D867B81B51CE07D9BBE43771487328A9) # bpiotrowski@archlinux.org
sha256sums=('394c416a35dc608e5c9ea5ca902c5b08b51fcbc6b3b39ece05b8eea67033b4a8'
            'SKIP'
            '6b8b0fd7f81d0a957beb3679c81bbb34ccc7568d5682844d8924424a0dadcb1b')

prepare() {
  cd "$srcdir"/gcc
  # link isl for in-tree builds
  ln -sf ../isl-${_islver} isl
}

build() {
  for _target in ${_targets}; do
    mkdir -p "$srcdir"/gcc-build-${_target} && cd "$srcdir"/gcc-build-${_target}

    "$srcdir"/gcc/configure --prefix=/usr --libexecdir=/usr/lib \
        --target=${_target} \
        --enable-languages=c,lto,c++,objc,obj-c++,fortran,ada \
        --enable-shared --enable-static \
        --enable-threads=posix --enable-fully-dynamic-string --enable-libstdcxx-time=yes \
        --with-system-zlib --enable-cloog-backend=isl \
        --enable-lto --disable-dw2-exceptions --enable-libgomp \
        --disable-multilib --enable-checking=release
    make all
  done
}

package() {
  for _target in ${_targets}; do
    cd "$srcdir"/gcc-build-${_target}
    make DESTDIR="$pkgdir" install
    ${_target}-strip "$pkgdir"/usr/${_target}/lib/*.dll
    strip "$pkgdir"/usr/bin/${_target}-*
    strip "$pkgdir"/usr/lib/gcc/${_target}/${pkgver:0:5}/{cc1*,collect2,gnat1,f951,lto*}
    ln -s ${_target}-gcc "$pkgdir"/usr/bin/${_target}-cc
    # mv dlls
    mkdir -p "$pkgdir"/usr/${_target}/bin/
    mv "$pkgdir"/usr/${_target}/lib/*.dll "$pkgdir"/usr/${_target}/bin/
  done
  strip "$pkgdir"/usr/bin/*
  # remove unnecessary files
  rm -r "$pkgdir"/usr/share
  rm -f "$pkgdir"/usr/lib/libcc1.so*
  rm -f "$pkgdir"/usr/lib/libcc1.a
}
