#!/bin/bash

root_file=root.img
home_file=home.img

qemu-system-arm \
		-kernel zImage \
        -m 128 \
        -M versatilepb \
        -no-reboot \
		-drive "file=$root_file"
        #-append "root=/dev/sda rw panic=0 console=ttyAMA0" \
		#-nographic \
        #-cpu cortex-a8 \
