#!/bin/bash

grep -v '^#' "$1" |
while read package ; do
	yaourt --noconfirm -S $package
	sudo pacman -D $package --asdep
done 
