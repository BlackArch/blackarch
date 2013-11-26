# Maintainer: onny <onny@project-insanity.org>
# Contributor: onny <onny@project-insanity.org>

pkgname=oclhashcat-lite-bin
pkgver=0.15
pkgrel=2
pkgdesc="Worlds fastest NTLM, MD5, SHA1, SHA256 and descrypt cracker"
url=('http://hashcat.net/oclhashcat-lite/')
arch=('i686' 'x86_64')
license=('custom')
optdepends=('nvidia-utils>=310.32: For for gpu support via Nvidia CUDA' 
	    'catalyst-utils=13.1: For gpu support via ATI Stream'
	    'opencl-catalyst: For gpu support via ATI Stream')
source=("http://hashcat.net/files/oclHashcat-lite-${pkgver}.7z")
sha512sums=('b70738a40cdbc5062cb72f1de93a2d29cdd824b7f3cb0f5bc42e3a7e83246d2247385040efe230afffa26ddf26823ecefabf16f31b1925c7460e7cd8933342b8')
options=('!strip')
package() {
  find "${srcdir}/oclHashcat-lite-${pkgver}" \( -name "*.cmd" -o -name "*.exe" \) -print | xargs -i rm {}
  mkdir -p "${pkgdir}/opt" "${pkgdir}/usr/bin"
  if [ "${CARCH}" = "x86_64" ]; then
    rm ${srcdir}/oclHashcat-lite-${pkgver}/cudaHashcat-lite32.bin
    rm ${srcdir}/oclHashcat-lite-${pkgver}/oclHashcat-lite32.bin
    echo -e "#!/bin/bash\n/opt/oclhashcat-lite/cudaHashcat-lite64.bin \$@" > $pkgdir/usr/bin/cudaHashcat-lite
    chmod a+x $pkgdir/usr/bin/cudaHashcat-lite
    echo -e "#!/bin/bash\n/opt/oclhashcat-lite/oclHashcat-lite64.bin \$@" > $pkgdir/usr/bin/oclHashcat-lite
    chmod a+x $pkgdir/usr/bin/oclHashcat-lite
    echo -e "#!/bin/bash\n/opt/oclhashcat-lite/vclHashcat-lite64.bin \$@" > $pkgdir/usr/bin/vclHashcat-lite
    chmod a+x $pkgdir/usr/bin/vclHashcat-lite
  else
    rm ${srcdir}/oclHashcat-lite-${pkgver}/cudaHashcat-lite64.bin
    rm ${srcdir}/oclHashcat-lite-${pkgver}/oclHashcat-lite64.bin
    rm ${srcdir}/oclHashcat-lite-${pkgver}/vclHashcat-lite64.bin
    echo -e "#!/bin/bash\n/opt/oclhashcat-lite/cudaHashcat-lite32.bin \$@" > $pkgdir/usr/bin/cudaHashcat-lite
    chmod a+x $pkgdir/usr/bin/cudaHashcat-lite
    echo -e "#!/bin/bash\n/opt/oclhashcat-lite/oclHashcat-lite32.bin \$@" > $pkgdir/usr/bin/oclHashcat-lite
    chmod a+x $pkgdir/usr/bin/oclHashcat-lite
  fi
  cp -rv "${srcdir}/oclHashcat-lite-${pkgver}" "${pkgdir}/opt/oclhashcat-lite"
}
