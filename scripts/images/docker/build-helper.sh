#!/usr/bin/env bash

rm -r /usr/share/man/*
haveged -w 1024
pacman-key --init
pkill haveged
pacman -Rs --noconfirm haveged
pacman-key --populate ARCH_KEYRING
pkill gpg-agent
ln -s /usr/share/zoneinfo/UTC /etc/localtime
echo 'en_US.UTF-8 UTF-8' > /etc/locale.gen
locale-gen
update-ca-trust
pacman-db-upgrade