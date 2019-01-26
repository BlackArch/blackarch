#!/usr/bin/env python3

import sys

__version__ = '0.1'

__home__ = 'blackarch.org'

to_release = ''


# get current version
def get_current_version(name):
    with open('../packages/{name}/PKGBUILD'.format(name=name), 'r') as file:
        for line in file:
            if 'pkgver' in line:
                return str(line[7:].strip())  # pkgver=...


# check arch community
def arch_community_check(name):
    temp = requests.get('https://www.archlinux.org/packages/community/x86_64/' + name + '/')
    if temp.ok:
        print('We can remove: ' + name)


# update pkgbuild
def update_pkgbuild(name, url, current_version, available_version):
    global to_release

    url = url.replace('$pkgname', name).replace('$pkgver', available_version)  # to sha512 check
    req = requests.get(url)  # got file
    sha512 = hashlib.sha512(req.content).hexdigest()  # calculate sha512

    url = url.replace(name, '$pkgname').replace(available_version, '$pkgver')  # style fix

    with open('../packages/{name}/PKGBUILD'.format(name=name), 'r') as file:
        temp = file.read()

    temp = re.sub(current_version, available_version, temp)  # version bump

    temp = re.sub('pkgrel=.*', 'pkgrel=1', temp)  # pkgrel=1

    temp = re.sub('sha512sums=.*', ("sha512sums=('" + sha512 + "')"), temp)  # sha512 update

    temp = re.sub('source=.*', ('source=("' + url + '")'), temp)  # source update

    with open('../packages/{name}/PKGBUILD'.format(name=name), 'w') as file:
        file.write(temp)

    to_release += (name + '\n')
    print('Successfully updated {name} from {current_version} to {available_version}'.format(name=name,
                                                                                             current_version=current_version,
                                                                                             available_version=available_version))


# update python packages version
def python_packages_version_check(name):
    pypi_name = str(name[7:])  # python-...

    if 'python2' in name:
        pypi_name = name[8:]  # python2-...

    req = requests.get('https://pypi.python.org/pypi/{name}/json'.format(name=pypi_name))  # pypi json api ^-^

    if req.ok:
        current_version = get_current_version(name)

        data = json.loads(req.text.encode())

        try:
            available_version = str(max([distutils.version.LooseVersion(release) for release in data['releases'] if
                                         not packaging.version.parse(release).is_prerelease]))  # got new version
            if available_version != current_version:
                url = str(data.get('releases')[available_version][-1]['url'])

                if url[-3:] != 'whl' and url[-3:] != 'egg':
                    update_pkgbuild(name, url, current_version, available_version)
                else:
                    print(
                        'Please, do manual update {name} to {version}, because i cant find any *.tar.gz or *.zip archives'.format(
                            name=name, version=available_version))

        except Exception:
            print('Please, do manual update {name}, because some error occured'.format(name=name))


# update ruby packages version
def ruby_packages_version_check(name):
    ruby_name = name[5:]

    req = requests.get('https://rubygems.org/api/v1/gems/{name}.json'.format(name=ruby_name))  # ruby json api ^-^

    if req.ok:
        current_version = get_current_version(name)

        available_version = str(json.loads(req.text.encode()).get('version'))

        if available_version != current_version:
            url = str(json.loads(req.text.encode()).get('gem_uri'))
            update_pkgbuild(name, url, current_version, available_version)


# update tools
def hacking_tools_update(name):
    current_version = get_current_version(name)
    try:
        i = 0  # no cheat
        with open('../packages/{name}/PKGBUILD'.format(name=name), 'r') as file:

            for line in file:

                if 'source=(' in line and '$pkgver' in line and 'git+' not in line and 'python' not in line and 'ruby' not in line:  # 'git+' for include tarball from github.com
                    url = str(line[9:-3].strip())  # got url without 'source=("' and '")'

                    if 'python' not in name and 'ruby' not in name and current_version.replace('.', '').isdigit():
                        req = requests.get(url.replace('$pkgver', current_version).replace('$pkgname', name))
                        while req.ok and req.headers['Content-Type'] != 'text/html' and req.headers[
                            'Content-Type'] != 'text/html;charset=utf-8':
                            i += 1
                            if current_version.count('.') == 0:
                                available_version = ''.join(current_version.split[:-1]) + str(
                                    int(current_version[-1]) + i)
                            else:
                                available_version = '.'.join(current_version.split('.')[:-1]) + '.' + str(
                                    int(current_version.split('.')[-1]) + i)  # so far only last number++
                            req = requests.get(url.replace('$pkgver', available_version).replace('$pkgname', name))
                        i -= 1
                        if current_version.count('.') == 0:
                            available_version = ''.join(current_version.split[:-1]) + str(int(current_version[-1]) + i)
                        else:
                            available_version = '.'.join(current_version.split('.')[:-1]) + '.' + str(
                                int(current_version.split('.')[-1]) + i)  # so far only last number++
                        if i > 0:
                            print('Time to update: ' + name + ' to: ' + available_version)
                            # update_pkgbuild(name, url, current_version, available_version)
    except Exception:
        pass


def main(function, needed):
    to_check = []

    exclusion = ['python-pyexiftool', 'python2-pyexiftool', 'python2-cement', 'python2-nmap', 'python2-pubsub',
                 'python2-pynfc', 'python2-slugify', 'ruby-unf']

    for root, dirs, files in os.walk('../packages/'):
        for dir in dirs:
            if needed in dir and dir not in exclusion:
                to_check.append(dir)

    with ThreadPoolExecutor(16) as executor:  # i think 16 is enough
        for _ in executor.map(function, to_check):
            pass


if __name__ == '__main__':
    try:
        import distutils.version
        import json
        import hashlib
        import os
        import packaging.version
        import re
        import requests
        from concurrent.futures import ThreadPoolExecutor
    except ModuleNotFoundError as e:
        print('Failure importing module: ' + str(e))
        sys.exit(1)

    main(arch_community_check, '')  # start arch community check

    main(python_packages_version_check, 'python')  # start version updating python packages

    main(ruby_packages_version_check, 'ruby')  # start version updating ruby packages

    main(hacking_tools_update, '')

    with open('../lists/to-release', 'a') as file:
        file.write(to_release)

    print('Done!')
