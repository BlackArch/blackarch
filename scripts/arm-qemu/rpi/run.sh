#!/bin/bash

root_file=root.img
home_file=home.img

qemu-system-arm \
		-kernel kernel-qemu \
        -cpu arm1176 \
        -m 256 \
        -M versatilepb \
        -no-reboot \
        -append "root=/dev/sda5 rw panic=0 console=ttyAMA0" \
		-nographic \
		-drive "file=$root_file" \
		-drive "file=$home_file"
