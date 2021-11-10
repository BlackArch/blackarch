# Maintainer: Kyle Keen <keenerd@gmail.com>

pkgname='python2-jaraco'
pkgver=2019.10.22
pkgrel=4
pkgdesc='A gaggle of idiosyncratic and questionable wrappers for the stdlib.'
arch=('any')
url='https://github.com/jaraco?page=1&tab=repositories&utf8=%E2%9C%93&q=jaraco'
license=('MIT')
depends=('python2-six' 'python2-pytz' 'python2-more-itertools'
         'python2-backports.functools_lru_cache'
         'python2-importlib_resources')
makedepends=('python2-setuptools-scm')

# These should not exist and do not deserve 16 packages.
# Furthermore, try to make a replacement for python-irc so we can nuke it all.
# And now cherrypy depends on this too :-(
# And it chains all the way up to pip, so it can never leave.
_text=3.1
_functools=2.0
_collections=2.1
_classes=2.0
_stream=2.0
_logging=2.0
_itertools=4.4.2
_pyhost="https://files.pythonhosted.org/packages/source"

source=("$_pyhost/j/jaraco.text/jaraco.text-$_text.tar.gz"
        "$_pyhost/j/jaraco.functools/jaraco.functools-$_functools.tar.gz"
        "$_pyhost/j/jaraco.collections/jaraco.collections-$_collections.tar.gz"
        "$_pyhost/j/jaraco.classes/jaraco.classes-$_classes.tar.gz"
        "$_pyhost/j/jaraco.stream/jaraco.stream-$_stream.tar.gz"
        "$_pyhost/j/jaraco.logging/jaraco.logging-$_logging.tar.gz"
        "$_pyhost/j/jaraco.itertools/jaraco.itertools-$_itertools.tar.gz")
sha512sums=('9edd50e3b67d40a1a5dfe8796dd55f80aac52cef9ba2f1ad565cf597c1ff8824d2c61900cedd104210b60dcc6dced8c01321af67e2e1a755be89e1c15945da1e'
            'dce30c08f0e9f3c7b682c78fc3a46a26d9972706f4c8449a12ba11aa5ed130c6d9ca405213fd93b6b4110ef9cea549e5fa925c96bfaf18681cd6957d8ba0889f'
            '64fd3f4155dfc64152dcfb7970c26c7e3e098b0a8282556d17be2aeb80dcac6092b6dca11bf42cda268e0ddda67f2117704c81218c4b6511236e57b1f602da47'
            '83c9a570f951ef6c3708243dce6397cfbfe963db5920550ac807878a69860b34831af5f277f956fd0b327d85462676fd6386a22d6d97c7616e01ca81d7ace694'
            'd379d07772d739abb275cfc8ed7809583c728af695294b9d12ae1276add360b9559de64104cd20d05d82a3250237ae1f6eb23420970a95e9fdf6346ef9312331'
            '9262575c3071f78ded2284ebef3a1a80c8ffd60addecf9b215db04471672bd892f6c6bd06a84f9a82e872adf31d45d4eb3faa2884815fcce608840821f49954e'
            '74db7a3c7595c7deaf2a0c9ba68e9b02d02ccbc6c65142de34a5339aac679c2d71f05b27e27632d265de4f78997e11d7dc43178bd0d64f3010e1568a5085812c')

prepare() {
  # remove inflect dep
  cd "$srcdir/jaraco.itertools-$_itertools"
  sed -i "s/'inflect',//" setup.py
  sed -i "s/inflect//"    jaraco.itertools.egg-info/requires.txt
  sed -i "s/inflect//"    setup.cfg
  sed -i 's/import inflect//' jaraco/itertools.py
  sed -i 's/ord = .*$/ord = str(self.sliceArgs[2])+"th"/' jaraco/itertools.py
}

package() {
  cd "$srcdir/jaraco.text-$_text"
  python2 setup.py install --root="$pkgdir/" --optimize=0
  cd "$srcdir/jaraco.functools-$_functools"
  python2 setup.py install --root="$pkgdir/" --optimize=0
  cd "$srcdir/jaraco.collections-$_collections"
  python2 setup.py install --root="$pkgdir/" --optimize=0
  cd "$srcdir/jaraco.classes-$_classes"
  python2 setup.py install --root="$pkgdir/" --optimize=0
  cd "$srcdir/jaraco.stream-$_stream"
  python2 setup.py install --root="$pkgdir/" --optimize=0
  cd "$srcdir/jaraco.logging-$_logging"
  python2 setup.py install --root="$pkgdir/" --optimize=0
  cd "$srcdir/jaraco.itertools-$_itertools"
  python2 setup.py install --root="$pkgdir/" --optimize=0

  #rm "$pkgdir/usr/bin/calc-prorate"
  #rmdir "$pkgdir/usr/bin"
  #install -Dm644 LICENSE "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
}

# vim:set ts=2 sw=2 et:
