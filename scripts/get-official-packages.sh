#!/bin/bash

# Get packages from the official repos
mkdir ../official-packages

grep -v 'aur$' ../package-repos |
cut -d' ' -f1 |
while read package ; do
	cp -nr /var/abs/*/$package ../official-packages
done
