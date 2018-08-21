# Maintainer: Florian Pritz <bluewind@xinu.at>
# Contributor: Jelle van der Waa <jelle@vdwaa.nl>
# Contributor: Hugo Osvaldo Barrera <hugo@barrera.io>
# Contributor: Matthias Maennich <arch@maennich.net>

pkgbase=shiboken
pkgname=(python{2,}-shiboken shiboken)
pkgver=1.2.4
pkgrel=4
arch=('x86_64')
license=('LGPL')
url="http://www.pyside.org"
makedepends=('cmake' 'python2' 'python' 'qt4' 'libxslt')
source=("$pkgbase-$pkgver.tar.gz::https://github.com/PySide/Shiboken/archive/$pkgver.tar.gz"
        support-new-python.diff)
md5sums=('c284197d06ad25d78009ff55f18dd512'
         '19d5032866f4e232f442fe19f12b94de')

prepare(){
    cd "$srcdir/Shiboken-$pkgver"
    patch -p1 -i ../support-new-python.diff
}

build(){
    cd "$srcdir/Shiboken-$pkgver"
    # build python2
    mkdir -p build-py2 && cd build-py2
    cmake ../ -DCMAKE_INSTALL_PREFIX=/usr  \
              -DCMAKE_BUILD_TYPE=Release   \
              -DBUILD_TESTS=OFF            \
              -DPYTHON_EXECUTABLE=/usr/bin/python2 \
              -DPYTHON_LIBRARY=/usr/lib/libpython2.7.so \
              -DPYTHON_INCLUDE_DIR=/usr/include/python2.7 \
              -DQT_QMAKE_EXECUTABLE=qmake-qt4
    make

    # build python3
    cd "$srcdir/Shiboken-$pkgver"
    mkdir -p build-py3 && cd build-py3
    cmake ../ -DCMAKE_INSTALL_PREFIX=/usr  \
              -DCMAKE_BUILD_TYPE=Release   \
              -DBUILD_TESTS=OFF            \
              -DUSE_PYTHON3=yes            \
              -DQT_QMAKE_EXECUTABLE=qmake-qt4
    make
}

package_shiboken() {
  pkgdesc="CPython bindings generator for C++ libraries"
  depends=(python qt4 libxslt)
  optdepends=("python2-shiboken: for compilation against python2"
              "python-shiboken: for compilation against python")

  # Header files/ /usr/bin/shiboke, pkgconfig, man page
  cd "$srcdir/Shiboken-$pkgver/build-py3"
  make DESTDIR="$pkgdir" install

  cd "$srcdir/Shiboken-$pkgver/build-py2"
  cd data
  install -Dm 644 ShibokenConfig-python2.7.cmake "$pkgdir/usr/lib/cmake/Shiboken-$pkgver/"
  install -Dm 644 shiboken.pc "$pkgdir/usr/lib/pkgconfig/shiboken-py2.pc"

  rm -rf "$pkgdir/usr/lib/python"*
  rm -rf "$pkgdir/usr/lib/libshiboken"*
  rm -rf "$pkgdir/usr/lib/pkgconfig/"
  rm "$pkgdir"/usr/lib/cmake/Shiboken-$pkgver/ShibokenConfig*python*.cmake
}
package_python2-shiboken() {
  pkgdesc="Support library for Python2 bindings"
  depends=("qt4>=4.8" "libxslt" "python2" "shiboken")

  cd "$srcdir/Shiboken-$pkgver/build-py2"
  make DESTDIR="$pkgdir" install

  cd "$srcdir/Shiboken-$pkgver/build-py2"
  cd data
  install -Dm 644 ShibokenConfig-python2.7.cmake "$pkgdir/usr/lib/cmake/Shiboken-$pkgver/"
  mv "$pkgdir"/usr/lib/pkgconfig/shiboken{,-py2}.pc

  rm -rf "$pkgdir"/usr/{include,bin,share}
  rm "$pkgdir/usr/lib/cmake/Shiboken-$pkgver/ShibokenConfigVersion.cmake"
  rm "$pkgdir/usr/lib/cmake/Shiboken-$pkgver/ShibokenConfig.cmake"
}

package_python-shiboken() {
  pkgdesc="Support library for Python bindings"
  depends=("qt4>=4.8" "libxslt" "python" "shiboken")

  cd "$srcdir/Shiboken-$pkgver/build-py3"
  make DESTDIR="$pkgdir" install

  rm -rf "$pkgdir"/usr/{include,bin,share}
  rm "$pkgdir/usr/lib/cmake/Shiboken-$pkgver/ShibokenConfigVersion.cmake"
  rm "$pkgdir/usr/lib/cmake/Shiboken-$pkgver/ShibokenConfig.cmake"
}
