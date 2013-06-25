#!/bin/bash
workingpath=$PWD  ## STORE THE CURRENT PWD WE NEED IT LATER
## IF path.log FILE EXISTS, GET RID OF IT
[[ -f "path.log" ]] && rm path.log

## PRETTY COLORS
function error_exit {
	echo -e '\E[31m'"\033[1mERROR: ${1}\033[0m" 1>&2	
	exit 1
}

## IF WE'RE NOT RUNNING AS ROOT, DIE
[[ "$(/usr/bin/id -u)" -eq 0 ]] || error_exit "script must be run as root!"

## NOT REALLY DEPENDENCIES BUT PACKAGES THAT WE'LL USE FROM OFFICIAL REPOS INSTEAD OF CREATING OUR OWN PKGBUILD
pacman -Syu --noconfirm \
aircrack-ng \
arp-scan \
arpwatch \
base-devel \
ddrescue \
dnsutils \
ettercap-gtk \
gdb \
hping \
john \
jre7-openjdk \
macchanger \
mysql \
mysql-clients \
net-tools \
netcat \
ncrack \
ngrep \
nmap \
openssl \
pv \
pygtk \
rdesktop \
smbclient \
stunnel \
tcpdump \
tcpreplay \
testdisk \
vidalia \
wget \
wine \
wine_gecko \
winetricks \
wireshark-cli \
wireshark-gtk \
proxytunnel

## FIX FOR BUG #1 THIS IS A DEPENDENCY
cpan -i Net::Netmask
## DEPENDENCY FOR NIKTO
cpan -i Crypt::SSLeay

## ADD OFFICIAL PACKAGES AS SOFTLINKS INTO THE BLACKARCH LAYOUT, INCLUDE THE DESKTOP FILES FOR THE MENU
mkdir -p /blackarch/exploitation/aircrack-ng
ln -s /usr/bin/aircrack-ng /blackarch/exploitation/aircrack-ng/aircrack-ng
ln -s /usr/bin/airdecap-ng /blackarch/exploitation/aircrack-ng/airdecap-ng
ln -s /usr/bin/airdecloak-ng /blackarch/exploitation/aircrack-ng/airdecloak-ng
ln -s /usr/bin/airolib-ng /blackarch/exploitation/aircrack-ng/airolib-ng
ln -s /usr/sbin/airbase-ng /blackarch/exploitation/aircrack-ng/airbase-ng
ln -s /usr/sbin/airdrop-ng /blackarch/exploitation/aircrack-ng/airdrop-ng
ln -s /usr/sbin/aireplay-ng /blackarch/exploitation/aircrack-ng/aireplay-ng
ln -s /usr/sbin/airgraph-ng /blackarch/exploitation/aircrack-ng/airgraph-ng
ln -s /usr/sbin/airmon-ng /blackarch/exploitation/aircrack-ng/airmon-ng
ln -s /usr/sbin/airodump-ng /blackarch/exploitation/aircrack-ng/airodump-ng
ln -s /usr/sbin/airserv-ng /blackarch/exploitation/aircrack-ng/airserv-ng
ln -s /usr/sbin/airtun-ng /blackarch/exploitation/aircrack-ng/airtun-ng
wget -O /usr/share/applications/blackarch-aircrack-ng.desktop http://www.blackarch.org/pub/desktops/blackarch-aircrack-ng.desktop

mkdir -p /blackarch/forensics/sleuthkit
ln -s /usr/bin/sorter /blackarch/forensics/sleuthkit/sorter
ln -s /usr/bin/mactime /blackarch/forensics/sleuthkit/mactime
ln -s /usr/bin/img_cat /blackarch/forensics/sleuthkit/img_cat
ln -s /usr/bin/img_stat /blackarch/forensics/sleuthkit/img_stat
ln -s /usr/bin/mmls /blackarch/forensics/sleuthkit/mmls
ln -s /usr/bin/mmstat /blackarch/forensics/sleuthkit/mmstat
ln -s /usr/bin/mmcat /blackarch/forensics/sleuthkit/mmcat
ln -s /usr/bin/blkcalc /blackarch/forensics/sleuthkit/blkcalc
ln -s /usr/bin/blkcat /blackarch/forensics/sleuthkit/blkcat
ln -s /usr/bin/blkls /blackarch/forensics/sleuthkit/blkls
ln -s /usr/bin/blkstat /blackarch/forensics/sleuthkit/blkstat
ln -s /usr/bin/ffind /blackarch/forensics/sleuthkit/ffind
ln -s /usr/bin/fls /blackarch/forensics/sleuthkit/fls
ln -s /usr/bin/fsstat /blackarch/forensics/sleuthkit/fsstat
ln -s /usr/bin/icat /blackarch/forensics/sleuthkit/icat
ln -s /usr/bin/ifind /blackarch/forensics/sleuthkit/ifind
ln -s /usr/bin/ils /blackarch/forensics/sleuthkit/ils
ln -s /usr/bin/istat /blackarch/forensics/sleuthkit/istat
ln -s /usr/bin/jcat /blackarch/forensics/sleuthkit/jcat
ln -s /usr/bin/jls /blackarch/forensics/sleuthkit/jls
ln -s /usr/bin/hfind /blackarch/forensics/sleuthkit/hfind
ln -s /usr/bin/srch_strings /blackarch/forensics/sleuthkit/srch_strings
ln -s /usr/bin/sigfind /blackarch/forensics/sleuthkit/sigfind
ln -s /usr/bin/tsk_recover /blackarch/forensics/sleuthkit/tsk_recover
ln -s /usr/bin/tsk_loaddb /blackarch/forensics/sleuthkit/tsk_loaddb
ln -s /usr/bin/tsk_comparedir /blackarch/forensics/sleuthkit/tsk_comparedir
ln -s /usr/bin/tsk_gettimes /blackarch/forensics/sleuthkit/tsk_gettimes
wget -O /usr/share/applications/blackarch-sleuthkit.desktop http://www.blackarch.org/pub/desktops/blackarch-sleuthkit.desktop

mkdir -p /blackarch/forensics/testdisk
ln -s /usr/bin/testdisk /blackarch/forensics/testdisk/testdisk
wget -O /usr/share/applications/blackarch-testdisk.desktop http://www.blackarch.org/pub/desktops/blackarch-testdisk.desktop

mkdir -p /blackarch/forensics/ddrescue
ln -s /usr/bin/ddrescue /blackarch/forensics/ddrescue/ddrescue
wget -O /usr/share/applications/blackarch-ddrescue.desktop http://www.blackarch.org/pub/desktops/blackarch-ddrescue.desktop

mkdir -p /blackarch/intelligence_gathering/nmap
ln -s /usr/bin/nmap /blackarch/intelligence_gathering/nmap/nmap
ln -s /usr/bin/zenmap /blackarch/intelligence_gathering/nmap/zenmap
wget -O /usr/share/applications/blackarch-nmap.desktop http://www.blackarch.org/pub/desktops/blackarch-nmap.desktop

mkdir -p /blackarch/intelligence_gathering/arp-scan
ln -s /usr/bin/arp-scan /blackarch/intelligence_gathering/arp-scan/arp-scan
wget -O /usr/share/applications/blackarch-arp-scan.desktop http://www.blackarch.org/pub/desktops/blackarch-arp-scan.desktop

mkdir -p /blackarch/intelligence_gathering/dig
ln -s /usr/bin/dig /blackarch/intelligence_gathering/dig/dig
wget -O /usr/share/applications/blackarch-dig.desktop http://www.blackarch.org/pub/desktops/blackarch-dig.desktop

mkdir -p /blackarch/intelligence_gathering/tcpdump
ln -s /usr/sbin/tcpdump /blackarch/intelligence_gathering/tcpdump/tcpdump
wget -O /usr/share/applications/blackarch-tcpdump.desktop http://www.blackarch.org/pub/desktops/blackarch-tcpdump.desktop

mkdir -p /blackarch/intelligence_gathering/wireshark
ln -s /usr/bin/wireshark /blackarch/intelligence_gathering/wireshark/wireshark
ln -s /usr/bin/tshark /blackarch/intelligence_gathering/wireshark/tshark
ln -s /usr/bin/editcap /blackarch/intelligence_gathering/wireshark/editcap
wget -O /usr/share/applications/blackarch-wireshark.desktop http://www.blackarch.org/pub/desktops/blackarch-wireshark.desktop

mkdir -p /blackarch/post_exploitation/proxytunnel
ln -s /usr/bin/proxytunnel /blackarch/post_exploitation/proxytunnel/proxytunnel
wget -O /usr/share/applications/blackarch-proxytunnel.desktop http://www.blackarch.org/pub/desktops/blackarch-proxytunnel.desktop

mkdir -p /blackarch/post_exploitation/netcat
ln -s /usr/bin/netcat /blackarch/post_exploitation/netcat/netcat
ln -s /usr/bin/ncat /blackarch/post_exploitation/netcat/ncat
wget -O /usr/share/applications/blackarch-netcat.desktop http://www.blackarch.org/pub/desktops/blackarch-netcat.desktop

mkdir -p /blackarch/post_exploitation/stunnel
ln -s /usr/bin/stunnel /blackarch/post_exploitation/stunnel/stunnel
wget -O /usr/share/applications/blackarch-stunnel.desktop http://www.blackarch.org/pub/desktops/blackarch-stunnel.desktop

mkdir -p /blackarch/post_exploitation/ettercap
ln -s /usr/bin/ettercap /blackarch/post_exploitation/ettercap/ettercap
wget -O /usr/share/applications/blackarch-ettercap.desktop http://www.blackarch.org/pub/desktops/blackarch-ettercap.desktop

mkdir -p /blackarch/post_exploitation/socat
ln -s /usr/bin/socat /blackarch/post_exploitation/socat/socat
wget -O /usr/share/applications/blackarch-socat.desktop http://www.blackarch.org/pub/desktops/blackarch-socat.desktop

mkdir -p /blackarch/vulnerability_analysis/john
ln -s /usr/bin/john /blackarch/vulnerability_analysis/john/john
wget -O /usr/share/applications/blackarch-john.desktop http://www.blackarch.org/pub/desktops/blackarch-john.desktop

mkdir -p /blackarch/vulnerability_analysis/ncrack
ln -s /usr/bin/ncrack /blackarch/vulnerability_analysis/ncrack/ncrack
wget -O /usr/share/applications/blackarch-ncrack.desktop http://www.blackarch.org/pub/desktops/blackarch-ncrack.desktop

mkdir -p /blackarch/vulnerability_analysis/macchanger
ln -s /usr/bin/macchanger /blackarch/vulnerability_analysis/macchanger/macchanger
wget -O /usr/share/applications/blackarch-macchanger.desktop http://www.blackarch.org/pub/desktops/blackarch-macchanger.desktop

mkdir -p /blackarch/vulnerability_analysis/hping
ln -s /usr/sbin/hping /blackarch/vulnerability_analysis/hping/hping
wget -O /usr/share/applications/blackarch-hping.desktop http://www.blackarch.org/pub/desktops/blackarch-hping.desktop

mkdir -p /blackarch/vulnerability_analysis/tcpreplay
ln -s /usr/bin/tcpreplay /blackarch/vulnerability_analysis/tcpreplay/tcpreplay
wget -O /usr/share/applications/blackarch-tcpreplay.desktop http://www.blackarch.org/pub/desktops/blackarch-tcpreplay.desktop

mkdir -p /blackarch/miscellaneous/arpwatch
ln -s /usr/sbin/arpwatch /blackarch/miscellaneous/arpwatch/arpwatch
wget -O /usr/share/applications/blackarch-arpwatch.desktop http://www.blackarch.org/pub/desktops/blackarch-arpwatch.desktop

mkdir -p /blackarch/miscellaneous/vidalia
ln -s /usr/bin/vidalia /blackarch/miscellaneous/vidalia/vidalia
wget -O /usr/share/applications/blackarch-vidalia.desktop http://www.blackarch.org/pub/desktops/blackarch-vidalia.desktop

mkdir -p /blackarch/miscellaneous/windows_binaries
cp -R  $workingpath/miscellaneous/windows_binaries/* /blackarch/miscellaneous/windows_binaries

## FIND ALL THE PACKAGES WE BUILT RECURSIVELY AND STORE THE PATHS IN path.log
find $workingpath -type f -name "*.tar.xz" >> path.log
## POP ALL THE PATHS IN path.log AND STORE THEM IN AN ARRAY
logfile=( `cat "path.log"`)
## LOOP THROUGH EACH PATH WE STORED
for pkgbuild in "${logfile[@]}"
do
	## CHANGE TO THE DIRECTORY IN THE PATH
	cd `dirname $pkgbuild`
	## INSTALL THE PACKAGE
	pacman -U --noconfirm `basename $pkgbuild`
done

## DELETE THE path.log
rm $workingpath/path.log
