#!/bin/bash

set -e

for arg ; do
	echo "Checking '$arg'..."
	grep -v '^#' "$arg" |
	while read package ; do
		if ! yaourt -Qi $package &>/dev/null ; then
			yaourt --noconfirm -S $package
			sudo pacman -D $package --asdep
		else
			echo "'$package' already installed."
		fi
	done 
done
