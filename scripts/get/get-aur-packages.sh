#!/bin/bash

grep 'aur$' package-repos |
cut -d' ' -f1 |
while read package ; do
	packer -G $package 
done
