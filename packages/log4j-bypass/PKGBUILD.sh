pkgname='log4j-bypass'
pkgver='0.1.1'
pkgrel='0.1.1'
pkgdesc='Log4j web app tester that includes WAF bypasses'
arch=('any')
url='https://github.com/cyberqueen-meg/log4j-bypass'
license=('MIT')
groups=('blackarch' 'blackarch-webapp' 'blackarch-fuzzer' 'blackarch-scanner')
depends=('python3')
makedepends=('git')
source=("git+https://github.com/cyberqueen-meg/$pkgname.git")

build () {
    git clone $source
    cd $pkgname
}

package() {
    cd $pkgname

    install -dm 755 "$pkgdir/usr/share/$pkgname"

    install -Dm 755 "bin/$pkgname" "$pkgdir/usr/bin/$pkgname"
    install -Dm 644 LICENSE.txt "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
    cp -a doc/* "$pkgdir/usr/share/doc/$pkgname/"
    
}
