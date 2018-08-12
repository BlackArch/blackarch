# Contributor: John D Jones III AKA jnbek <jnbek1972 -_AT_- g m a i l -_Dot_- com>
# Generator  : CPANPLUS::Dist::Arch 1.32

pkgname='perl-test-sharedfork'
pkgver='0.35'
pkgrel='1'
pkgdesc="fork test"
arch=('any')
license=('PerlArtistic' 'GPL')
options=('!emptydirs')
depends=('perl-test-requires' 'perl>=5.008_001')
makedepends=()
url='https://metacpan.org/release/Test-SharedFork'
source=('http://search.cpan.org/CPAN/authors/id/E/EX/EXODIST/Test-SharedFork-0.35.tar.gz')
md5sums=('3101aea2e3ae41d48fd8874414430cef')
sha512sums=('e5948dfb34cfaf35a2fb2783c726f1cea2ae2f4bbdc4cbb912f83c877422ceabb3e0323c8c669b3c3a08e6d2a12106309528f0f674657001e6aeb3cb3c1dfd9f')
_distdir="Test-SharedFork-0.35"

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
