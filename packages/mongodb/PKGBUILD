# Maintainer: JustKidding <jk@vin.ovh>
# Maintainer: James P. Harvey <jamespharvey20 at gmail dot com>
# Maintainer: Christoph Bayer <chrbayer@criby.de>
# Contributor: Felix Yan <felixonmars@archlinux.org>
# Contributor: Sven-Hendrik Haase <sh@lutzhaase.com>
# Contributor: Thomas Dziedzic < gostrc at gmail >
# Contributor: Mathias Stearn <mathias@10gen.com>
# Contributor: Alec Thomas
# Contributor: Fredy García <frealgagu at gmail dot com>

pkgname=mongodb
# #.<odd number>.# releases are unstable development/testing
pkgver=5.0.6
pkgrel=2
pkgdesc="A high-performance, open source, schema-free document-oriented database"
arch=("x86_64")
url="https://www.mongodb.com/"
license=("custom:SSPL")
depends=('curl' 'libstemmer' 'snappy' 'gperftools' 'boost-libs' 'pcre' 'yaml-cpp')
makedepends=('scons' 'python-psutil' 'python-setuptools' 'python-regex' 'python-cheetah3' 'python-yaml' 'python-requests' 'boost')
optdepends=('mongodb-tools: mongoimport, mongodump, mongotop, etc')
checkdepends=("python-pymongo")
backup=("etc/mongodb.conf")
source=(https://fastdl.mongodb.org/src/mongodb-src-r$pkgver.tar.gz
        mongodb.sysusers
        mongodb.tmpfiles
        mongodb-5.0.2-skip-no-exceptions.patch
        mongodb-5.0.2-no-compass.patch
        mongodb-4.4.1-boost.patch)
sha256sums=('9d514eef9093d383120aebe4469c8118a39f390afcd8cd9af2399076b27abb52'
            '3757d548cfb0e697f59b9104f39a344bb3d15f802608085f838cb2495c065795'
            'b7d18726225cd447e353007f896ff7e4cbedb2f641077bce70ab9d292e8f8d39'
            '5b81ebc3ed68b307df76277aca3226feee33a00d8bb396206bdc7a8a1f58f3e4'
            '41b75d19ed7c4671225f08589e317295b7abee934b876859c8777916272f3052'
            'd3bc20d0cb4b8662b5326b8a3f2215281df5aed57550fa13de465e05e2044c25')

_scons_args=(
  --use-system-pcre # wait for pcre 8.44+ https://jira.mongodb.org/browse/SERVER-40836 and https://jira.mongodb.org/browse/SERVER-42990
  --use-system-snappy
  --use-system-yaml # https://jira.mongodb.org/browse/SERVER-43980
  --use-system-zlib
  #--use-system-wiredtiger # https://jira.mongodb.org/browse/SERVER-42813 upstream broke this in 4.2.0, says in meantime not to use it
  --use-system-stemmer
  --use-sasl-client
  --ssl
  --disable-warnings-as-errors
  # --use-system-asio     # https://jira.mongodb.org/browse/SERVER-21839 marked as fixed, but still doesn't compile.  MongoDB uses custom patches.
  # --use-system-icu      # Doesn't compile
  --use-system-tcmalloc   # in gperftools
  --use-system-boost    # Doesn't compile
  --use-system-zstd
  # --use-system-valgrind # Compiles, but namcap says not used
  # --use-system-sqlite   #   "
  # --use-system-mongo-c  # Doesn't compile
)

prepare() {
  cd "${srcdir}/${pkgname}-src-r${pkgver}"

  # Keep historical Arch dbPath
  sed -i 's|dbPath: /var/lib/mongo|dbPath: /var/lib/mongodb|' rpm/mongod.conf

  # Keep historical Arch conf file name
  sed -i 's|-f /etc/mongod.conf|-f /etc/mongodb.conf|' rpm/mongod.service

  # Keep historical Arch user name (no need for separate daemon group name)
  sed -i 's/User=mongod/User=mongodb/' rpm/mongod.service
  sed -i 's/Group=mongod/Group=mongodb/' rpm/mongod.service
  sed -i 's/chown mongod:mongod/chown mongodb:mongodb/' rpm/mongod.service

  # Remove sysconfig file, used by upstream's init.d script not used on Arch
  sed -i '/EnvironmentFile=-\/etc\/sysconfig\/mongod/d' rpm/mongod.service

  # Make systemd wait as long as it takes for MongoDB to start
  # If MongoDB needs a long time to start, prevent systemd from restarting it every 90 seconds
  # See: https://jira.mongodb.org/browse/SERVER-38086
  sed -i 's/\[Service]/[Service]\nTimeoutStartSec=infinity/' rpm/mongod.service

  # Prevent building using debug symbols, since binaries will be stripped anyway
  # Reduces makepkg -- which fully runs check() -- build size from 259GB to 2.3GB, and time by at least 37%
  # See: https://jira.mongodb.org/browse/SERVER-44038
  sed -i '/"-ggdb" if not env.TargetOSIs/d' SConstruct

  # gentoo patches
  patch -Np1 -i ../mongodb-5.0.2-skip-no-exceptions.patch
  patch -Np1 -i ../mongodb-5.0.2-no-compass.patch
  patch -Np1 -i ../mongodb-4.4.1-boost.patch
}

build() {
  cd "${srcdir}/${pkgname}-src-r${pkgver}"

  export SCONSFLAGS="$MAKEFLAGS"
  scons install-core "${_scons_args[@]}"
}

package() {
  cd "${srcdir}/${pkgname}-src-r${pkgver}"

  scons PREFIX=/usr DESTDIR="$pkgdir" install-core "${_scons_args[@]}"

  # Keep historical Arch conf file name
  install -Dm644 "rpm/mongod.conf" "${pkgdir}/etc/${pkgname}.conf"

  # Keep historical Arch service name
  install -Dm644 "rpm/mongod.service" "${pkgdir}/usr/lib/systemd/system/${pkgname}.service"

  # Install manpages
  install -Dm644 "debian/mongo.1" "${pkgdir}/usr/share/man/man1/mongo.1"
  install -Dm644 "debian/mongod.1" "${pkgdir}/usr/share/man/man1/mongod.1"
  install -Dm644 "debian/mongos.1" "${pkgdir}/usr/share/man/man1/mongos.1"

  install -Dm644 "${srcdir}/${pkgname}.sysusers" "${pkgdir}/usr/lib/sysusers.d/${pkgname}.conf"
  install -Dm644 "${srcdir}/${pkgname}.tmpfiles" "${pkgdir}/usr/lib/tmpfiles.d/${pkgname}.conf"

  # move licenses
  mkdir -p $pkgdir/usr/share/licenses/$pkgname
  mv $pkgdir/usr/{LICENSE-Community.txt,MPL-2,README,THIRD-PARTY-NOTICES} $pkgdir/usr/share/licenses/$pkgname
}
# vim:set ts=2 sw=2 et:

