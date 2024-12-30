#!/bin/bash 
#
# Short script to automate upgrading blackarch
#

sudo pacman -Rdd jre-openjdk jdk-openjdk jre11-openjdk jre11-openjdk-headless calamares python-uvicorn
sudo pacman -S archlinux-keyring
sudo pacman -S blackarch-keyring
sudo pacman -S blackarch-mirrorlist
sudo pacman -Syy
sudo pacman -Syu --no-confirm --disable-download-timeout

