# Maintainer:   Maximilian Weiss <$(echo "bWF4QG1heHdlaXNzLmlv" | base64 -d)>
# Contributor:  Felix Yan <$(echo "ZmVsaXhvbm1hcnNAZ21haWwuY29t" | base64 -d)>


pkgname=libunique3
pkgver=3.0.2
pkgrel=7
pkgdesc='Library for writing single instance applications for GTK3'
arch=('i686' 'x86_64')
url='http://live.gnome.org/LibUnique'
license=('LGPL')
depends=('gtk3')
makedepends=('gtk-doc' 'gobject-introspection')
makepkgopt=('strip' 'docs' '!libtool' '!staticlibs' 'emptydirs' 'zipman' 'purge' '!upx' '!debug')
provides=('libunique3')
conflicts=('libunique3')
source_i686=('https://archive.archlinux.org/repos/2014/07/01/extra/os/i686/libunique3-3.0.2-3-i686.pkg.tar.xz')
source_x86_64=('https://archive.archlinux.org/repos/2014/07/01/extra/os/x86_64/libunique3-3.0.2-3-x86_64.pkg.tar.xz')
sha256sums_i686=('bb3fcdf763d0cf4fe35eb6eb6a49321ebac4de9251f7840c8b2977968f48ddb0')
sha256sums_x86_64=('ee634bcbbdf420edec1ce34b5d55dedfa6c77bc1df43309647517533a240aee8')

package() {
    cd "$srcdir/"
    install -Dm644 "usr/include/unique-3.0/unique/unique.h" "$pkgdir/usr/include/unique-3.0/unique/unique.h"
    install -Dm644 "usr/include/unique-3.0/unique/uniqueapp.h" "$pkgdir/usr/include/unique-3.0/unique/uniqueapp.h"
    install -Dm644 "usr/include/unique-3.0/unique/uniquebackend.h" "$pkgdir/usr/include/unique-3.0/unique/uniquebackend.h"
    install -Dm644 "usr/include/unique-3.0/unique/uniqueenumtypes.h" "$pkgdir/usr/include/unique-3.0/unique/uniqueenumtypes.h"
    install -Dm644 "usr/include/unique-3.0/unique/uniquemessage.h" "$pkgdir/usr/include/unique-3.0/unique/uniquemessage.h"
    install -Dm644 "usr/include/unique-3.0/unique/uniqueversion.h" "$pkgdir/usr/include/unique-3.0/unique/uniqueversion.h"
    install -Dm644 "usr/lib/girepository-1.0/Unique-3.0.typelib" "$pkgdir/usr/lib/girepository-1.0/Unique-3.0.typelib"
    install -Dm644 "usr/lib/pkgconfig/unique-3.0.pc" "$pkgdir/usr/lib/pkgconfig/unique-3.0.pc"
    install -Dm644 "usr/share/gir-1.0/Unique-3.0.gir" "$pkgdir/usr/share/gir-1.0/Unique-3.0.gir"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/UniqueApp.html" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/UniqueApp.html"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/UniqueBackend.html" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/UniqueBackend.html"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/home.png" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/home.png"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/index.html" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/index.html"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/index.sgml" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/index.sgml"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/ix01.html" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/ix01.html"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/left.png" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/left.png"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/license.html" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/license.html"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/right.png" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/right.png"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/style.css" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/style.css"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/unique-3.0.devhelp" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/unique-3.0.devhelp"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/unique-3.0.devhelp2" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/unique-3.0.devhelp2"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/unique-Message-Data.html" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/unique-Message-Data.html"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/unique-Versioning-Information.html" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/unique-Versioning-Information.html"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/unique-glossary.html" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/unique-glossary.html"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/unique-overview.html" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/unique-overview.html"
    install -Dm644 "usr/share/gtk-doc/html/unique-3.0/up.png" "$pkgdir/usr/share/gtk-doc/html/unique-3.0/up.png"
    install -Dm755 "usr/lib/libunique-3.0.so.0.0.2" "$pkgdir/usr/lib/libunique-3.0.so.0.0.2"
    cd "$pkgdir/usr/lib/"
    ln -s "libunique-3.0.so.0.0.2" "libunique-3.0.so.0"
    ln -s "libunique-3.0.so.0.0.2" "libunique-3.0.so"
}
