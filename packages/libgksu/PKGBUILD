# $Id: PKGBUILD 310007 2017-11-15 14:11:34Z foutrelis $
# Maintainer: Jan de Groot <jgc@archlinux.org>

pkgname=libgksu
pkgver=2.0.12
pkgrel=8
pkgdesc="gksu authorization library"
arch=(x86_64)
url="http://www.nongnu.org/gksu/index.html"
license=(GPL)
depends=('gconf' 'gtk2' 'libgnome-keyring' 'libgtop' 'startup-notification')
makedepends=('intltool' 'gtk-doc')
options=('!emptydirs')
source=(http://people.debian.org/~kov/gksu/${pkgname}-${pkgver}.tar.gz
        libgksu-2.0.0-fbsd.patch
        libgksu-2.0.12-automake-1.11.2.patch
        libgksu-2.0.12-fix-make-3.82.patch
	libgksu-2.0.12-notests.patch
        libgksu-2.0.12-revert-forkpty.patch
        libgksu-2.0.7-libs.patch
        libgksu-2.0.7-polinguas.patch)
md5sums=('c7154c8806f791c10e7626ff123049d3'
         '063a2b45d8e7cbba898d1db413242da0'
         '2eeb34ad9b5bf29e8e2ebf8c8a5a28b6'
         'afeaf5caab03d793258d62d60a9bf0f0'
         '4179d0487d6032e56b8a925010694c0a'
         'aebbe57e5286c654e27cf714cf3b704a'
         '58d3a4a9d2ac741951720043ea3f7b5f'
         '0b5c3d5d9b32cb3e65d9f0bfbcb11a76')

prepare() {
  cd "${srcdir}/${pkgname}-${pkgver}"
  patch -Np1 -i "${srcdir}/libgksu-2.0.0-fbsd.patch"
  patch -Np1 -i "${srcdir}/libgksu-2.0.7-libs.patch"
  patch -Np1 -i "${srcdir}/libgksu-2.0.7-polinguas.patch"
  patch -Np1 -i "${srcdir}/libgksu-2.0.12-revert-forkpty.patch"
  patch -Np0 -i "${srcdir}/libgksu-2.0.12-fix-make-3.82.patch"
  patch -Np1 -i "${srcdir}/libgksu-2.0.12-notests.patch"
  patch -Np1 -i "${srcdir}/libgksu-2.0.12-automake-1.11.2.patch"

  touch NEWS README

  intltoolize --force --copy --automake
  autoreconf -fi
}

build() {
  cd "${srcdir}/${pkgname}-${pkgver}"
  ./configure --prefix=/usr --sysconfdir=/etc \
      --localstatedir=/var --disable-static --disable-schemas-install --disable-gtk-doc
  sed -i -e 's/ -shared / -Wl,-O1,--as-needed\0/g' libtool
  make
}

package() {
  cd "${srcdir}/${pkgname}-${pkgver}"
  make GCONF_DISABLE_MAKEFILE_SCHEMA_INSTALL=1 DESTDIR="${pkgdir}" install
  rm -f ${pkgdir}/usr/lib/*.a
  install -m755 -d "${pkgdir}/usr/share/gconf/schemas"
  gconf-merge-schema "${pkgdir}/usr/share/gconf/schemas/${pkgname}.schemas" --domain libgksu ${pkgdir}/etc/gconf/schemas/*.schemas
  rm -f ${pkgdir}/etc/gconf/schemas/*.schemas
}
