#!/bin/bash

root_file=root.img
# in bytes (default set to 9G)
root_size=$(( 10 * 1024 * 1024 * 1024 ))
# human-readable
swap_size=1G

umask 022

check_priv() {
	if (( $EUID != 0 )) ; then
		echo >&2 'you must be root.'
		exit 1
	fi
}

make_root() {
	mkdir -p root

	# image
	rm -f "$root_file"
	fallocate -l "$(( root_size + 2048 * 512 ))" "$root_file"
	mkfs.ext4 -F "$root_file"
	mount -o loop "$root_file" root

	# rootfs
	wget -nc http://archlinuxarm.org/os/ArchLinuxARM-omap-smp-latest.tar.gz
	tar xzf ArchLinuxARM-omap-smp-latest.tar.gz -C root

	# fstab
	echo '/dev/sda1   /boot           vfat    defaults        0       0' > root/etc/fstab
	echo '/dev/sdb    /home           ext4    defaults        0       0' >> root/etc/fstab
	echo '/home/swap  none            swap    defaults        0       0' >> root/etc/fstab

	# TODO: tweak swappiness
	# pacman.conf
	echo '[blackarch]' >> root/etc/pacman.conf
	echo 'Server = http://blackarch.org/blackarch/$repo/os/armv6h' >> root/etc/pacman.conf
	sed -i 's|^Include = .*|Server = http://mirror.archlinuxarm.org/$arch/$repo|' root/etc/pacman.conf

	# Some more packages
	pacstrap -d -G -M -C root/etc/pacman.conf root base-devel devtools-alarm blackarch-devtools git vim

	# Git
	git clone https://github.com/BlackArch/blackarch root/home/blackarch

	# Swap
	fallocate -l "$swap_size" root/home/swap
	mkswap root/home/swap

	sync
	umount root
	rmdir root
}

get_kernel() {
	wget -nc http://xecdesign.com/downloads/linux-qemu/kernel-qemu
}

main() {
	check_priv

	make_root
	get_kernel
}

main
