#!/usr/bin/env bash

# build.sh -

# Adapted from
# https://github.com/docker/docker/commit/4137a0ea327ea1775c1b57892fd684da2c738f3e
# Generate a minimal filesystem for BlackArch and load it into the local
# docker as "blackarch".
# requires root

set -e

TERM=${TERM:-xterm}
export TERM

# simple error message wrapper
err()
{
    echo >&2 `tput bold; tput setaf 1`"[-] ERROR: ${*}"`tput sgr0`
    exit 1337
}

# simple warning message wrapper
warn()
{
    echo >&2 `tput bold; tput setaf 1`"[!] WARNING: ${*}"`tput sgr0`
}

# simple echo wrapper
msg()
{
    echo `tput bold; tput setaf 2`"[+] ${*}"`tput sgr0`
}

msg 'checking requirements'
REQUIREMENTS="pacstrap expect"
for req in $REQUIREMENTS ; do
	hash $req &>/dev/null || {
		echo "Could not find $req."
	  exit 1
  }
done

export LANG="C.UTF-8"

ROOTFS=$(mktemp -d /output/rootfs-blackarch-XXXXXXXXXX)
chmod 755 $ROOTFS

# packages to ignore for space savings
PKGIGNORE=(
    cryptsetup
    device-mapper
    dhcpcd
    iproute2
    jfsutils
    linux
    lvm2
    man-db
    man-pages
    mdadm
    nano
    netctl
    openresolv
    pciutils
    pcmciautils
    reiserfsprogs
    s-nail
    systemd-sysvcompat
    usbutils
    vi
    xfsprogs
)
IFS=','
PKGIGNORE="${PKGIGNORE[*]}"
unset IFS

PACMAN_CONF='/usr/local/etc/build-pacman.conf'
PACMAN_MIRRORLIST='Server = https://mirrors.kernel.org/archlinux/$repo/os/$arch'
PACMAN_EXTRA_PKGS=''
EXPECT_TIMEOUT=360
ARCH_KEYRING=archlinux
DOCKER_IMAGE_NAME=blackarch

export PACMAN_MIRRORLIST

msg 'pacstrapping minimal arch installation'

expect <<EOF
	set send_slow {1 .1}
	proc send {ignore arg} {
		sleep .1
		exp_send -s -- \$arg
	}
	set timeout $EXPECT_TIMEOUT

	spawn pacstrap -C $PACMAN_CONF -c -d -G -i $ROOTFS base ca-certificates haveged $PACMAN_EXTRA_PKGS --ignore $PKGIGNORE
	expect {
		-exact "anyway? \[Y/n\] " { send -- "n\r"; exp_continue }
		-exact "(default=all): " { send -- "\r"; exp_continue }
		-exact "installation? \[Y/n\]" { send -- "y\r"; exp_continue }
	}
EOF

msg 'injecting and running config helper script'
cp -v /usr/local/bin/build-helper.sh $ROOTFS/bin/
sed -i 's/ARCH_KEYRING/'$ARCH_KEYRING'/g' $ROOTFS/bin/build-helper.sh
echo $PACMAN_MIRRORLIST > $ROOTFS/etc/pacman.d/mirrorlist
arch-chroot $ROOTFS /bin/build-helper.sh
rm -v $ROOTFS/bin/build-helper.sh

msg 'bootstrapping BlackArch keys and repos'
cp -v /usr/local/bin/strap.sh $ROOTFS/bin/
arch-chroot $ROOTFS /bin/strap.sh
rm $ROOTFS/bin/strap.sh

msg 'creating device nodes'
# udev doesn't work in containers, rebuild /dev
DEV=$ROOTFS/dev
rm -rf $DEV
mkdir -p $DEV
mknod -m 666 $DEV/null c 1 3
mknod -m 666 $DEV/zero c 1 5
mknod -m 666 $DEV/random c 1 8
mknod -m 666 $DEV/urandom c 1 9
mkdir -m 755 $DEV/pts
mkdir -m 1777 $DEV/shm
mknod -m 666 $DEV/tty c 5 0
mknod -m 600 $DEV/console c 5 1
mknod -m 666 $DEV/tty0 c 4 0
mknod -m 666 $DEV/full c 1 7
mknod -m 600 $DEV/initctl p
mknod -m 666 $DEV/ptmx c 5 2
ln -sf /proc/self/fd $DEV/fd

msg 'importing finished image into docker daemon'
pacman --noconfirm -S docker
tar --numeric-owner --xattrs --acls -C $ROOTFS -c . > $DOCKER_IMAGE_NAME.tar
docker import - $DOCKER_IMAGE_NAME < $DOCKER_IMAGE_NAME.tar
docker run --rm -t $DOCKER_IMAGE_NAME echo Success.
rm -rf $ROOTFS