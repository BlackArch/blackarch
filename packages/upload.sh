#!/bin/bash

uploads=()
for package ; do
	(
	cd "$package"
	rm *.src.tar.gz
	makepkg -f --source
	)
	uploads+=($package/*.src.tar.gz)
done

burp "${uploads[@]}"
