#!/bin/bash

root_file=root.img
home_file=home.img
# in bytes (default set to 9G)
home_size=$(( 9 * 1024 * 1024 * 1024 ))
# human-readable
swap_size=3G

umask 022

check_priv() {
	if (( $EUID != 0 )) ; then
		echo >&2 'you must be root.'
		exit 1
	fi
}

get_root() {
	wget -nc http://archlinuxarm.org/os/ArchLinuxARM-rpi-latest.zip
	unzip -o ArchLinuxARM-rpi-latest.zip
	mv -f ArchLinuxARM-2014.02-rpi.img "$root_file"

	# for debugging
	#cp -f ArchLinuxARM-2014.02-rpi.img "$root_file"
}

get_kernel() {
	wget -nc http://xecdesign.com/downloads/linux-qemu/kernel-qemu
}

mod_root() {
	mkdir -p root
	mount -o loop,offset=$(( 188416 * 512 )) "$root_file" root
	echo '/dev/sda1   /boot           vfat    defaults        0       0' > root/etc/fstab
	echo '/dev/sdb    /home           ext4    defaults        0       0' >> root/etc/fstab
	echo '/home/swap  none            swap    defaults        0       0' >> root/etc/fstab
	echo '[blackarch]' >> root/etc/pacman.conf
	echo 'Server = http://blackarch.org/blackarch/$repo/os/armv6h' >> root/etc/pacman.conf

	sed -i 's|^Include = .*|Server = http://mirror.archlinuxarm.org/$arch/$repo|' root/etc/pacman.conf
	pacstrap -G -M -C root/etc/pacman.conf root base-devel devtools-alarm blackarch-devtools git vim

	sync
	umount root
	rmdir root
}

make_home() {
	rm -f "$home_file"
	fallocate -l "$(( home_size + 2048 * 512 ))" "$home_file"
	mkfs.ext4 -F "$home_file"

	mkdir -p home
	mount -o loop "$home_file" home

	git clone https://github.com/BlackArch/blackarch home/blackarch
	fallocate -l "$swap_size" home/swap
	mkswap home/swap

	sync
	umount home
	rmdir home
}

main() {
	check_priv

	get_root
	get_kernel
	mod_root
	make_home
}

main
