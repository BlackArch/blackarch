#!/bin/bash
# Gets package source archives from the AUR or ABS.

root="$(dirname "$0")/.."
get_dir="$root/new-packages"
[[ -d "$get_dir" ]] || mkdir -p "$get_dir"
cd "$get_dir" || exit 1

echo "Saving packages to '$get_dir'."

for package ; do
	packer -G $package &>/dev/null &&
	echo "Got '$package' from aur." ||
	{ cp -nr /var/abs/*/$package . 2>/dev/null ||
     echo "Could not find '$package'." &&
     echo "Got '$package' from abs."; }
done
