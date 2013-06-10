pkgname=newpasswd
pkgver=0.4
pkgrel=1
pkgdesc="Generate a brand new password"
url="https://github.com/simukis/newpasswd"
license=('ICS')
arch=('any')
depends=('python')
short_hash='f55dc57'
source=('https://github.com/simukis/newpasswd/zipball/'$short_hash)
md5sums=('29ecce6796b6abb5f5a6cb3264274873')

package() {
    cd $srcdir/simukis-newpasswd-$short_hash/
    mv usr/ $pkgdir/
    chmod +x $pkgdir/usr/bin/newpasswd
}
