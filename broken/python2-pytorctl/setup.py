#!/usr/bin/env python

from distutils.core import setup

setup(
    name='TorCtl',
    description='Library for controlling Tor through the control port',
    author='Mike Perry',
    author_email='mikeperry-git@fscked.org>',
    url='https://gitweb.torproject.org/pytorctl.git',
    packages=['TorCtl'],
    package_dir={'TorCtl': 'pytorctl'},
    package_data={'TorCtl': ['GeoLiteCity.dat']}
)
