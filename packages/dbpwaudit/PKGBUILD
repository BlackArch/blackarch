# Archtrack maintainer: Evan Teitelman <teitelmanevan at gmail dot com>
# Contributor: Francesco Piccinno <stack.box@gmail.com>

pkgname=dbpwaudit
pkgver=0.8
pkgrel=1
pkgdesc="DBPwAudit is a Java tool that allows you to perform online audits of password quality for several database engines."
url="http://www.cqure.net/wp/dbpwaudit/"
updateurl="http://www.cqure.net/wp/dbpwaudit=>dbpwaudit_"
license=('GPL')
depends=('java-runtime')
arch=(i686 x86_64)
source=(http://www.cqure.net/tools/${pkgname}_${pkgver//./_}.zip)
md5sums=('74b2ab5d0a142f0f03d6c67452dba6b2')

groups=(archtrack archtrack-vulnerability-analysis archtrack-password-attacks)

build() {
  cd "$srcdir/DBPwAudit"
  chmod -x *.conf

  mkdir -p $pkgdir/usr/share/$pkgname/
  cp -r * $pkgdir/usr/share/$pkgname/
}
