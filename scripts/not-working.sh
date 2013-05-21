#!/bin/bash

if [[ $# = 0 ]] ; then
	packages=(../packages/*)
else
	packages=("${@:2}")
fi

for package in "${packages[@]}" ; do
	info="$package/info"

	if grep -q '^working=false' $info ; then
		echo "'$package' not working"
	fi

	if grep -q '^upstream_working=false' $info ; then
		echo "'$package' upstream not working"
	fi
done
