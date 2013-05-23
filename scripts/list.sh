#!/bin/bash
# Provides some informative lists.

usage() {
	echo >&2 "Help will come soon. For now, look at the source."
}

cd "$(dirname "$0")/../packages"
case "$1" in
	p|packages)
		ls -1
		;;
	w|working)
		for package in * ; do
			grep -q '^working=true' "$package/info" && echo "$package"
		done
		;;
	b|broken)
		for package in * ; do
			grep -q '^working=false' "$package/info" && echo "$package"
		done
		;;
	u|upstream-broken)
		for package in * ; do
			grep -q '^upstream_working=false' "$package/info" && echo "$package"
		done
		;;
	t|to-find)
		ls -1 ../research/kali-menu/items-tofind
		;;
	a|all-groups)
		for package in * ; do
			source "$package/info"
			for group in ${groups[@]} ; do
				echo $group
			done
		done | sort -u
		;;
	s|straight-associations)
		for package in * ; do
			( source "$package/info"
			for group in ${groups[@]} ; do
				[[ $group = archtrack ]] || echo "$group $package"
			done )
		done | sort
		;;
	g|groupings)
		for package in * ; do
			( source "$package/info"
			  echo "$package ${groups[@]}" )
		done | sort
		;;
	*)
		usage
		exit 1
		;;
esac
