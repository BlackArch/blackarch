#!/bin/bash
# Gets package source archives from the AUR or ABS.

root="$(dirname "$0")/../.."
get_dir="$root/new-packages"

echo "Saving packages to '$get_dir'..."

for package ; do
	packer -G $(basename "$package") ||
	{ abs $package;
	  cp -nr /var/abs/*/$package "$get_dir" ||
     echo "Could not find '$package'."; }
done
