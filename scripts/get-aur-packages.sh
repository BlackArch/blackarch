#!/bin/bash

grep 'aur$' package-repos |
cut -d' ' -f1 |
while read package ; do
	packer -G $package 
done

exit

# Get packages from the official repos (not used)
grep -v 'aur$' package-repos |
cut -d' ' -f1 |
while read package ; do
	cp -nr ~/ecod/arch/packages/$package aur-packages
done
