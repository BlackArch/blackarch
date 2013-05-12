#!/bin/bash

uploads=()
for package ; do
	uploads+=($package/*.src.tar.gz)
	(
	cd "$package"
	makepkg -f --source
	)
done

burp "${uploads[@]}"
