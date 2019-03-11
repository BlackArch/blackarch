#!/bin/bash
cd src
git clone https://github.com/ntop/nDPI.git
cd nDPI
sudo pacman -S gcc autogen automake autoconf libtool
./autogen.sh
./configure
make
sudo make install


