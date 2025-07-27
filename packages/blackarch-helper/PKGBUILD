# This file is part of BlackArch Linux ( https://www.blackarch.org/ ).
# See COPYING for license details.

pkgname=blackarch-helper
pkgver=1.1.0
pkgrel=1
pkgdesc='TUI/CLI helper to manage and install tools from the BlackArch repository'
arch=('any')
url='https://github.com/x0rgus/blackarch-helper'
license=('MIT')
groups=('blackarch')
depends=('bash' 'fzf')
source=('blackarch-helper'
        'LICENSE'
        'blackarch-helper.1'
        'blackarch-helper.bash-completion'
        '_blackarch-helper')
sha512sums=('SKIP'
            'SKIP'
            'SKIP'
            'SKIP'
            'SKIP')

package() {
    install -Dm755 "$srcdir/blackarch-helper" "$pkgdir/usr/bin/blackarch-helper"
    install -Dm644 LICENSE "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
    install -Dm644 "$srcdir/blackarch-helper.1" "$pkgdir/usr/share/man/man1/$pkgname.1"
    install -Dm644 "$srcdir/blackarch-helper.bash-completion" "$pkgdir/usr/share/bash-completion/completions/$pkgname"
    install -Dm644 "$srcdir/_blackarch-helper" "$pkgdir/usr/share/zsh/site-functions/_$pkgname"
}
