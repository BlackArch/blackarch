# Maintainer: onny <onny@project-insanity.org>
# Contributor: onny <onny@project-insanity.org>

pkgname=oclhashcat-plus-bin
pkgver=0.15
pkgrel=2
pkgdesc="Worlds fastest WPA cracker with dictionary mutation engine"
url=('http://hashcat.net/oclhashcat-plus/')
arch=('i686' 'x86_64')
license=('custom')
optdepends=('nvidia-utils>=310.32: For for gpu support via Nvidia CUDA' 
	    'catalyst-utils>=13.4: For gpu support via ATI Stream'
	    'opencl-catalyst: For gpu support via ATI Stream')
source=("http://hashcat.net/files/oclHashcat-plus-${pkgver}.7z")
sha512sums=('0b11aa805b186a432ee8a9f6ff5823235986d2552fc99b5365150c165bd10232a76408b375887483375cf1e29d8e78de2dadf2e046863061c7ce8722c7591da4')
options=('!strip')
package() {
  find "${srcdir}/oclHashcat-plus-${pkgver}" \( -name "*.cmd" -o -name "*.exe" \) -print | xargs -i rm {}
  mkdir -p "${pkgdir}/opt" "${pkgdir}/usr/bin"
  if [ "${CARCH}" = "x86_64" ]; then
    rm ${srcdir}/oclHashcat-plus-${pkgver}/cudaHashcat-plus32.bin
    rm ${srcdir}/oclHashcat-plus-${pkgver}/oclHashcat-plus32.bin
    echo -e "#!/bin/bash\n/opt/oclhashcat-plus/cudaHashcat-plus64.bin \$@" > $pkgdir/usr/bin/cudaHashcat-plus
    chmod a+x $pkgdir/usr/bin/cudaHashcat-plus
    echo -e "#!/bin/bash\n/opt/oclhashcat-plus/oclHashcat-plus64.bin \$@" > $pkgdir/usr/bin/oclHashcat-plus
    chmod a+x $pkgdir/usr/bin/oclHashcat-plus
  else
    rm ${srcdir}/oclHashcat-plus-${pkgver}/cudaHashcat-plus64.bin
    rm ${srcdir}/oclHashcat-plus-${pkgver}/oclHashcat-plus64.bin
    echo -e "#!/bin/bash\n/opt/oclhashcat-plus/cudaHashcat-plus32.bin \$@" > $pkgdir/usr/bin/cudaHashcat-plus
    chmod a+x $pkgdir/usr/bin/cudaHashcat-plus
    echo -e "#!/bin/bash\n/opt/oclhashcat-plus/oclHashcat-plus32.bin \$@" > $pkgdir/usr/bin/oclHashcat-plus
    chmod a+x $pkgdir/usr/bin/oclHashcat-plus
  fi
  cp -rv "${srcdir}/oclHashcat-plus-${pkgver}" "${pkgdir}/opt/oclhashcat-plus"
}
