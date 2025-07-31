# This file is part of BlackArch Linux ( https://www.blackarch.org/ ).
# See COPYING for license details.

pkgname=blackarch-helper
pkgver=2.1.0
pkgrel=1
pkgdesc="Helper to find, inspect, and build BlackArch packages from source using makepkg."
arch=('any')
url='https://github.com/x0rgus/blackarch-helper'
license=('MIT')
depends=('bash' 'curl' 'fzf')
source=("${pkgname}"
        "LICENSE"
        "${pkgname}.1"
        "${pkgname}.bash-completion"
        "_${pkgname}")
sha256sums=('SKIP'
            'SKIP'
            'SKIP'
            'SKIP'
            'SKIP')

package() {
    install -Dm755 "$srcdir/$pkgname" "$pkgdir/usr/bin/$pkgname"
    install -Dm644 "$srcdir/LICENSE" "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
    install -Dm644 "$srcdir/${pkgname}.1" "${pkgdir}/usr/share/man/man1/${pkgname}.1"
    install -Dm644 "$srcdir/${pkgname}.bash-completion" "${pkgdir}/usr/share/bash-completion/completions/${pkgname}"
    install -Dm644 "$srcdir/_${pkgname}" "${pkgdir}/usr/share/zsh/site-functions/_${pkgname}"
}