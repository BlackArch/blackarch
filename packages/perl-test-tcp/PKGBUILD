# Contributor: John D Jones III AKA jnbek <jnbek1972 -_AT_- g m a i l -_Dot_- com>
# Generator  : CPANPLUS::Dist::Arch 1.32

pkgname='perl-test-tcp'
pkgver='2.17'
pkgrel='1'
pkgdesc="testing TCP program"
arch=('any')
license=('PerlArtistic' 'GPL')
options=('!emptydirs')
depends=('perl-test-sharedfork>=0.29' 'perl>=5.8.1')
makedepends=()
url='https://metacpan.org/release/Test-TCP'
source=('http://search.cpan.org/CPAN/authors/id/S/SY/SYOHEX/Test-TCP-2.17.tar.gz')
md5sums=('587ecdd5593be06d57216c6dc2eab12f')
sha512sums=('41795ae4287d43b28f8892ef9e545d155cb61bb7daf35a937a5c9bfd5f480b8714b7c07f393e540ff6bc3711eebb8fc8380833e6163bf66d09e9c1d28b5a30c9')
_distdir="Test-TCP-2.17"

build() {
  ( export PERL_MM_USE_DEFAULT=1 PERL5LIB=""                 \
      PERL_AUTOINSTALL=--skipdeps                            \
      PERL_MM_OPT="INSTALLDIRS=vendor DESTDIR='$pkgdir'"     \
      PERL_MB_OPT="--installdirs vendor --destdir '$pkgdir'" \
      MODULEBUILDRC=/dev/null

    cd "$srcdir/$_distdir"
    /usr/bin/perl Makefile.PL
    make
  )
}

check() {
  cd "$srcdir/$_distdir"
  ( export PERL_MM_USE_DEFAULT=1 PERL5LIB=""
    make test
  )
}

package() {
  cd "$srcdir/$_distdir"
  make install

  find "$pkgdir" -name .packlist -o -name perllocal.pod -delete
}

# Local Variables:
# mode: shell-script
# sh-basic-offset: 2
# End:
# vim:set ts=2 sw=2 et:
