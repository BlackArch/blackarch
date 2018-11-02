#!/usr/bin/env python3

__version__='0.1-alpha'

import json, os, requests
from threading import Thread
from packaging.version import parse

def python_packages_version_check(name):

    # return right version of package
    def get_version(releases):
        version=parse('0')
        for rel in releases:
            ver = parse(rel)
            if not ver.is_prerelease:
                version = max(version, ver)
        return version

    with open('{name}/PKGBUILD'.format(name=name), 'r') as file:
        for line in file:
            if 'pkgver' in line:
                current_version=line[7:].strip() # pkgver=...
                break

    pypi_name=name[7:] # python-...

    if 'python2' in name:
        pypi_name=name[8:] # python2-...

    req=requests.get('https://pypi.python.org/pypi/{name}/json'.format(name=pypi_name)) # pypi json api ^-^

    if req.ok:
        available_version = get_version(json.loads(req.text.encode()).get('releases')) # got new version

        if str(available_version) != str(current_version) and str(available_version) != '0':
            print('We can update {name} to {version}'.format(name=name, version=available_version))

def main(function, needed):

    class MyThread(Thread):
        def __init__(self, name):
            Thread.__init__(self)
            self.name = name

        def run(self):
            function(self.name)

    to_check = []

    for root, dirs, files in os.walk('.'):
        for dir in dirs:
            if needed in dir:
                to_check.append(dir)

    # 16 threads i think is enough
    while len(to_check) > 15:
        c0 = MyThread(to_check.pop(0))
        c1 = MyThread(to_check.pop(0))
        c2 = MyThread(to_check.pop(0))
        c3 = MyThread(to_check.pop(0))
        c4 = MyThread(to_check.pop(0))
        c5 = MyThread(to_check.pop(0))
        c6 = MyThread(to_check.pop(0))
        c7 = MyThread(to_check.pop(0))
        c8 = MyThread(to_check.pop(0))
        c9 = MyThread(to_check.pop(0))
        c10 = MyThread(to_check.pop(0))
        c11 = MyThread(to_check.pop(0))
        c12 = MyThread(to_check.pop(0))
        c13 = MyThread(to_check.pop(0))
        c14 = MyThread(to_check.pop(0))
        c15 = MyThread(to_check.pop(0))

        c0.start()
        c1.start()
        c2.start()
        c3.start()
        c4.start()
        c5.start()
        c6.start()
        c7.start()
        c8.start()
        c9.start()
        c10.start()
        c11.start()
        c12.start()
        c13.start()
        c14.start()
        c15.start()

        c0.join()
        c1.join()
        c2.join()
        c3.join()
        c4.join()
        c5.join()
        c6.join()
        c7.join()
        c8.join()
        c9.join()
        c10.join()
        c11.join()
        c12.join()
        c13.join()
        c14.join()
        c15.join()

    while len(to_check) > 0:
        c0 = MyThread(to_check.pop(0))

        c0.start()

        c0.join()

    print('Done!')

if __name__ == '__main__':
    main(python_packages_version_check, 'python')
