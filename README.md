BlackArch
=========

To make use of the pentest set of tools available in the BlackArch toolset please add the BlackArch repository to `/etc/pacman.conf`


```
[blackarch]
SigLevel = Optional TrustAll
Server = http://www.blackarch.org/pub/blackarch/$arch
```

It's also recommended to add `/blackarch` to your $PATH

Package Groups
--------------

To see all available packages follow http://www.blackarch.org/packages.html and install any tool individually.  For quicker installs leverage the package groups:

- BlackArch Exploitation

```
$ pacman -S blackarch-exploitation
```
Included packages:

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

- BlackArch Forensics

```
$ pacman -S blackarch-forensics
```

Included packages:

```
ddrescue
pdf-parser
pdfid
peepdf
sleuthkit
testdisk
volatility
```

- BlackArch Intelligence Gathering

```
$ pacman -S blackarch-intel
```

Included packages:

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

- BlackArch Miscellaneous

```
$ pacman -S blackarch-misc
```

Included packages:

```
arpwatch
artillery
geoipgen
vidalia
```

- BlackArch Post-Exploitation

```
$ pacman -S blackarch-post
```

Included packages:

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

- BlackArch Threat Modeling

```
$ pacman -S threat-model
```

Included packages:

```
magictree
```

- BlackArch Vulnerability Analysis

```
$ pacman -S blackarch-analysis
```

Included packages:

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
