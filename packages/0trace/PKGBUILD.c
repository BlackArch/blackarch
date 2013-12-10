pkgname=0trace
pkgver=1.5
pkgrel=1
pkgdesc="A hop enumeration tool"
url="http://jon.oberheide.org/0trace/"
arch=('x86_64' 'i686')
license=('GPLv3')
makedepends=('usleep')
source=("http://lcamtuf.coredump.cx/soft/0trace.tgz"
        "usleep.c"
        "0trace.patch")
md5sums=('f0e1131ed28e5809c081eead73a4b0da')

build(){
  cd "$srcdir"
  #Compiling usleep 
  gcc -o usleep usleep.c
  #Building sendprobe
  make sendprobe >/dev/null
}

package() {
  cd "$srcdir"
  
  #Base directories
  install -dm755 "$pkgdir/usr/bin"
  
  #Binaries
  install -m755 usleep "$pkgdir/usr/bin/usleep"
  install -m755 0trace.sh "$pkgdir/usr/bin/0trace"
  install -m755 sendprobe "$pkgdir/usr/bin/sendprobe"
}
