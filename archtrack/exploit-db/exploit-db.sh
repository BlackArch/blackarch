#!/bin/bash
# Download, update and query local exploit database from exploit-db.com
target_dir="/usr/share/exploit-db"

case "$1" in
	update)
		if [ $UID != 0 ]; then
			echo You must run exploit-db update as root!;
			exit;
		fi;
		echo "Downloading exploit archive from exploit-db.com..."
		mkdir -p "$target_dir"
		cd "$target_dir";
		wget "http://www.exploit-db.com/archive.tar.bz2"
		echo "Extracting archive..."
		tar jxpf archive.tar.bz2
		rm archive.tar.bz2
		chmod -R a+r *
		;;
	search)
		cat "$target_dir/files.csv" | grep -i "$2" | while read line; do
			echo $target_dir$(echo $line | sed -r 's/^[0-9]+,([^,]+),"([^"]*)",([0-9-]+),.*/\/opt\/exploit-db\/\1 \2 (\3)/')
		done;
		;;
	usearch)
		$0 update
		$0 search "$2"
		;;
	*)
		echo 'Local exploit-db.com exploit database tool by Deeno9 2010'
		echo 'based on:'
		echo '  Local Milw0rm.com exploit database tool by Harvie 2009 ( http://blog.harvie.cz/ )'
		echo
		echo "Target directory: $target_dir"
		echo "usage: $0 {update|search regexp|usearch regexp}"
esac
