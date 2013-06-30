BlackArch
=========

To make use of the pentest set of tools available in the BlackArch toolset please add the BlackArch repository to `/etc/pacman.conf`

# 32-bit

```
[blackarch]
SigLevel = Optional TrustAll
Server = http://www.blackarch.org/pub/blackarch/i686
```

# 64-bit
```
[blackarch]
SigLevel = Optional TrustAll
Server = http://www.blackarch.org/pub/blackarch/x86_64
```

It's also recommended to add `/blackarch` to your $PATH

Package Groups
--------------
# BlackArch Exploitation
```
$ pacman -S blackarch-exploitation
```
```
aircrack-ng
beef
fern-wifi-cracker
fimap
metasploit
reaver-wps
set
sqlmap
sqlninja
websploit
xsser
```
# BlackArch Forensics
```
$ pacman -S blackarch-forensics
```
ddrescue
pdf-parser
pdfid
peepdf
sleuthkit
testdisk
volatility
```
# BlackArch Intelligence Gathering
```
$ pacman -S blackarch-intel
```
```
arp-scan
blindelephant
burpsuite
cms-explorer
dig
dirbuster
dnsbf
dnsenum
dnsmap
hashid
netglub
nikto
nmap
snmpcheck
ssldump
tcpdump
theharvester
wireshark
zaproxy
```
# BlackArch Miscellaneous
```
$ pacman -S blackarch-misc
```
```
arpwatch
artillery
geoipgen
vidalia
```
# BlackArch Post-Exploitation
```
$ pacman -S blackarch-post
```
```
chownat
ettercap
netcat
netcommander
proxytunnel
pwnat
socat
sslstrip
stunnel
```
# BlackArch Threat Modeling
```
$ pacman -S threat-model
```
```
magictree
```
# BlackArch Vulnerability Analysis
```
$ pacman -S blackarch-analysis
```
```
edb
fang
hping
john
macchanger
ncrack
pentbox
scapy
sipvicious
slowhttptest
sslscan
tcpjunk
tcpreplay
```

To learn more about each package visit http://www.blackarch.org/packages.html
