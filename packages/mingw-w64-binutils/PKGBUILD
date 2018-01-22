# $Id: PKGBUILD 266875 2017-11-15 14:29:11Z foutrelis $
# Maintainer: Sergej Pupykin <pupykin.s+arch@gmail.com>
# Maintainer: rubenvb vanboxem <dottie> ruben <attie> gmail <dottie> com

_targets="i686-w64-mingw32 x86_64-w64-mingw32"

pkgname=mingw-w64-binutils
pkgver=2.29
pkgrel=1
pkgdesc="Cross binutils for the MinGW-w64 cross-compiler"
arch=('x86_64')
url="http://www.gnu.org/software/binutils"
license=('GPL')
groups=('mingw-w64-toolchain' 'mingw-w64')
depends=('zlib')
options=('!libtool' '!emptydirs')
validpgpkeys=('EAF1C276A747E9ED86210CBAC3126D3B4AE55E93')
source=("https://ftp.gnu.org/gnu/binutils/binutils-${pkgver}.tar.gz"{,.sig})
sha256sums=('172e8c89472cf52712fd23a9f14e9bca6182727fb45b0f8f482652a83d5a11b4'
            'SKIP')

prepare() {
  cd "$srcdir"/binutils-${pkgver}
  #do not install libiberty
  sed -i 's/install_to_$(INSTALL_DEST) //' libiberty/Makefile.in
  # hack! - libiberty configure tests for header files using "$CPP $CPPFLAGS"
  sed -i "/ac_cpp=/s/\$CPPFLAGS/\$CPPFLAGS -O2/" libiberty/configure
}

build() {
  for _target in $_targets; do
    msg "Building ${_target} cross binutils"
    mkdir -p "$srcdir"/binutils-${_target} && cd "${srcdir}/binutils-${_target}"
    "$srcdir"/binutils-${pkgver}/configure --prefix=/usr \
        --target=${_target} \
        --infodir=/usr/share/info/${_target} \
        --enable-lto --enable-plugins \
        --disable-multilib --disable-nls \
        --disable-werror
     make
  done
}

package() {
  for _target in ${_targets}; do
    msg "Installing ${_target} cross binutils"
    cd "$srcdir"/binutils-${_target}
    make DESTDIR="$pkgdir" install
  done
}
