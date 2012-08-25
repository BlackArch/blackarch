#!/bin/bash
workingpath=$PWD  ## STORE THE CURRENT PWD WE NEED IT LATER

## PRETTY COLORS
function error_exit {
	echo -e '\E[31m'"\033[1mERROR: ${1}\033[0m" 1>&2	
	exit 1
}

## IF WE'RE NOT RUNNING AS ROOT, DIE
[[ "$(/usr/bin/id -u)" -eq 0 ]] || error_exit "script must be run as root!"

## NOT REALLY DEPENDENCIES BUT PACKAGES THAT WE'LL USE FROM OFFICIAL REPOS INSTEAD OF CREATING OUR OWN PKGBUILD
pacman -Syu --noconfirm mysql mysql-clients dnsutils wine winetricks wine_gecko arpwatch aircrack-ng arp-scan vidalia tcpreplay pv ddrescue stunnel hping nmap pygtk gdb wireshark-cli wireshark-gtk ettercap ettercap-gtk tcpdump rdesktop ngrep netcat openssl jre7-openjdk testdisk macchanger smbclient net-tools base-devel

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
cp _desktops/blackarch-aircrack-ng.desktop /usr/share/applications/blackarch-aircrack-ng.desktop

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
cp _desktops/blackarch-sleuthkit.desktop /usr/share/applications/blackarch-sleuthkit.desktop

mkdir -p /blackarch/forensics/testdisk
ln -s /usr/bin/testdisk /blackarch/forensics/testdisk/testdisk
cp _desktops/blackarch-testdisk.desktop /usr/share/applications/blackarch-testdisk.desktop

mkdir -p /blackarch/forensics/ddrescue
ln -s /usr/bin/ddrescue /blackarch/forensics/ddrescue/ddrescue
cp _desktops/blackarch-ddrescue.desktop /usr/share/applications/blackarch-ddrescue.desktop

mkdir -p /blackarch/intelligence_gathering/nmap
ln -s /usr/bin/nmap /blackarch/intelligence_gathering/nmap/nmap
ln -s /usr/bin/zenmap /blackarch/intelligence_gathering/nmap/zenmap
cp _desktops/blackarch-nmap.desktop /usr/share/applications/blackarch-nmap.desktop

mkdir -p /blackarch/intelligence_gathering/arp-scan
ln -s /usr/bin/arp-scan /blackarch/intelligence_gathering/arp-scan/arp-scan
cp _desktops/blackarch-arp-scan.desktop /usr/share/applications/blackarch-arp-scan.desktop

mkdir -p /blackarch/intelligence_gathering/dig
ln -s /usr/bin/dig /blackarch/intelligence_gathering/dig/dig
cp _desktops/blackarch-dig.desktop /usr/share/applications/blackarch-dig.desktop

mkdir -p /blackarch/intelligence_gathering/tcpdump
ln -s /usr/sbin/tcpdump /blackarch/intelligence_gathering/tcpdump/tcpdump
cp _desktops/blackarch-tcpdump.desktop /usr/share/applications/blackarch-tcpdump.desktop

mkdir -p /blackarch/intelligence_gathering/wireshark
ln -s /usr/bin/wireshark /blackarch/intelligence_gathering/wireshark/wireshark
ln -s /usr/bin/tshark /blackarch/intelligence_gathering/wireshark/tshark
ln -s /usr/bin/editcap /blackarch/intelligence_gathering/wireshark/editcap
cp _desktops/blackarch-wireshark.desktop /usr/share/applications/blackarch-wireshark.desktop

mkdir -p /blackarch/post_exploitation/netcat
ln -s /usr/bin/netcat /blackarch/post_exploitation/netcat/netcat
ln -s /usr/bin/ncat /blackarch/post_exploitation/netcat/ncat
cp _desktops/blackarch-netcat.desktop /usr/share/applications/blackarch-netcat.desktop

mkdir -p /blackarch/post_exploitation/stunnel
ln -s /usr/bin/stunnel /blackarch/post_exploitation/stunnel/stunnel
cp _desktops/blackarch-stunnel.desktop /usr/share/applications/blackarch-stunnel.desktop

mkdir -p /blackarch/post_exploitation/ettercap
ln -s /usr/bin/ettercap /blackarch/post_exploitation/ettercap/ettercap
cp _desktops/blackarch-ettercap.desktop /usr/share/applications/blackarch-ettercap.desktop

mkdir -p /blackarch/post_exploitation/socat
ln -s /usr/bin/socat /blackarch/post_exploitation/socat/socat
cp _desktops/blackarch-socat.desktop /usr/share/applications/blackarch-socat.desktop

mkdir -p /blackarch/vulnerability_analysis/macchanger
ln -s /usr/bin/macchanger /blackarch/vulnerability_analysis/macchanger/macchanger
cp _desktops/blackarch-macchanger.desktop /usr/share/applications/blackarch-macchanger.desktop

mkdir -p /blackarch/vulnerability_analysis/hping
ln -s /usr/sbin/hping /blackarch/vulnerability_analysis/hping/hping
cp _desktops/blackarch-hping.desktop /usr/share/applications/blackarch-hping.desktop

mkdir -p /blackarch/vulnerability_analysis/tcpreplay
ln -s /usr/bin/tcpreplay /blackarch/vulnerability_analysis/tcpreplay/tcpreplay
cp _desktops/blackarch-tcpreplay.desktop /usr/share/applications/blackarch-tcpreplay.desktop

mkdir -p /blackarch/miscellaneous/windows_binaries
cp -R  $workingpath/miscellaneous/windows_binaries/* /blackarch/miscellaneous/windows_binaries

mkdir -p /blackarch/miscellaneous/arpwatch
ln -s /usr/sbin/arpwatch /blackarch/miscellaneous/arpwatch/arpwatch
cp _desktops/blackarch-arpwatch.desktop /usr/share/applications/blackarch-arpwatch.desktop

mkdir -p /blackarch/miscellaneous/vidalia
ln -s /usr/bin/vidalia /blackarch/miscellaneous/vidalia/vidalia
cp _desktops/blackarch-vidalia.desktop /usr/share/applications/blackarch-vidalia.desktop

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