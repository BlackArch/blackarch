What is BlackArch Linux?
------------------------

**BlackArch Linux** is a lightweight expansion to Arch Linux for penetration
testers.

The toolset is distributed as an Arch Linux [unofficial user
repository](https://wiki.archlinux.org/index.php/Unofficial_User_Repositories)
so you can install BlackArch Linux on top of an existing Arch Linux
installation. Packages may be installed individually or by category.

We currently have over 175 tools in our toolset. We are expanding the repository
to include over 200 tools. All tools are thoroughly tested before being added
to the codebase to maintain the quality of the repository.

Get Involved
------------

You can get in touch with the BlackArch team. Just check out the following:

**Please, send us pull requests!**

**Web:** http://blackarch.org/

**Mail:** blackarch@gmail.com

**IRC:** irc://irc.genscripts.net/blackarch

Quick Start
-----------

Add the following to
[`/etc/pacman.conf`](https://www.archlinux.org/pacman/pacman.conf.5.html).

```
[blackarch]
SigLevel = Optional TrustAll
Server = http://www.blackarch.org/pub/blackarch/$arch
```

To install the toolset, run

```
$ pacman -S blackarch
```

You will see a prompt:

```
:: There are 179 members in group blackarch:
:: Repository blackarch
...
Enter a selection (default=all):
```

Select the packages that you wish to install. By default, all of the packages
in the toolset will be installed.

Package Groups
--------------

Tools may be installed by category. The category groups are listed below.  To learn more about each included tool visit http://www.blackarch.org/packages.html

#### Exploitation

```
$ pacman -S blackarch-exploitation
```

- Tools in exploitation group:
 - armitage
 - cisco-auditing-tool
 - cisco-global-exploiter
 - creepy
 - exploit-db
 - metasploit
 - miranda-upnp
 - reaver
 - set
 - thc-ipv6
 - websploit
 - yersinia

#### Forensics

```
$ pacman -S blackarch-forensics
```

- Tools in forensics group:
 - afflib
 - autopsy
 - binwalk
 - bulk-extractor
 - dc3dd
 - foremost
 - galleta
 - guymager
 - magicrescue
 - md5deep
 - p0f
 - pasco
 - pdf-parser
 - pdfid
 - peepdf
 - pev
 - radare2
 - recoverjpeg
 - reglookup
 - rifiuti
 - safecopy
 - scrounge-ntfs
 - tiger
 - vinetto
 - volatility

#### Intelligence Gathering

```
$ pacman -S blackarch-intel
```

- Tools in intelligence gathering group:
 - ace
 - braa
 - chownat
 - cisco-auditing-tool
 - cms-explorer
 - creepy
 - cryptcat
 - cutycapt
 - dmitry
 - dns2tcp
 - dnsbf
 - dnsenum
 - dnsmap
 - dnswalk
 - enumiax
 - evilgrade
 - fierce
 - fiked
 - fragroute
 - hamster
 - hashid
 - hexinject
 - iaxflood
 - ike-scan
 - inguma
 - intrace
 - iodine
 - irpas
 - lbd
 - maltego
 - metagoofil
 - miranda-upnp
 - mitmproxy
 - netcommander
 - netdiscover
 - netglub
 - netmask
 - netsniff-ng
 - onesixtyone
 - p0f
 - proxychains
 - ptunnel
 - pwnat
 - rtpbreak
 - sdb
 - sipp
 - sipsak
 - sipvicious
 - snmpcheck
 - ssldump
 - sslscan
 - sslsniff
 - sslstrip
 - sslyze
 - thc-ipv6
 - theharvester
 - tuxcut
 - updtunnel
 - urlcrazy
 - voiphopper
 - waffit
 - xprobe2
 - zaproxy

#### Threat Modeling

```
$ pacman -S blackarch-threat-model
```

- Tools in threat modeling group:
 - magictree

#### Vulnerability Analysis

```
$ pacman -S blackarch-analysis
```

- Tools in vulnerability analysis group:
 - bed
 - cflow
 - cisco-auditing-tool
 - cisco-global-exploiter
 - dbpwaudit
 - fang
 - lynix
 - maltego
 - mdbtools
 - nikto
 - oscanner
 - pentbox
 - powerfuzzer
 - scapy
 - sfuzz
 - slowhttptest
 - spike
 - sqlmap
 - sqlninja
 - sqlsus
 - tcpjunk
 - yersinia
