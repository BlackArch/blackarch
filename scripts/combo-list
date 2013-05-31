#!/bin/bash
# Combines lists.
# list.sh must be in the same directory as this script.

usage() {
	echo $'Sorry, no help is available at this time. Read the source instead.\n\n'
	cat "$0"
}

case "$1" in
	a|and) action=and ;;
	o|or) action=or ;;
	x|xor) action=xor ;;
	*)
		usage
		exit 1
		;;
esac

is_first=true
for arg in "${@:2}" ; do
	if ! $is_first ; then
		case $action in
			and)
				list=$(comm -12 <(echo "$list") <(./list.sh $arg | sort) | column -t | sort)
				;;
			or)
				list=$(sort -u <(./list.sh $arg) <(echo "$list"))
				;;
			xor)
				list=$(comm -3 <(echo "$list") <(./list.sh $arg | sort) | column -t | sort)
				;;
		esac
	else
		list=$(./list.sh $arg | sort)
		is_first=false
	fi
done

echo "$list"
