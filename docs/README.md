What is BlackArchLinux?
------------------------

**BlackArchLinux** is a lightweight expansion to Arch Linux for penetration
testers.

The toolset is distributed as an Arch Linux [unofficial user
repository](https://wiki.archlinux.org/index.php/Unofficial_User_Repositories)
so you can install BlackArchLinux on top of an existing Arch Linux
installation. Packages may be installed individually or by category.

<!-- Count tools: pacman -Sgg | grep blackarchlinux | cut -d' ' -f2 | sort -u | wc -l -->
We currently have over 185 tools in our toolset and the repository is
constantly expanding. All tools are thoroughly tested before being added to the
codebase to maintain the quality of the repository.

Get Involved
------------

You can get in touch with the blackarchlinux team. Just check out the following:

**Please, send us pull requests!**

**Web:** http://blackarchlinux.org/

**Mail:**

**IRC:** [irc://irc.freenode.net/blackarch](irc://irc.freenode.net/blackarch)

Setup
-----------

Add this to
[`/etc/pacman.conf`](https://www.archlinux.org/pacman/pacman.conf.5.html):

```
[blackarchlinux]
Server = http://www.blackarchlinux.org/pub/blackarchlinux/$arch
```

For package signing, pull in and sign the package signing keys:

```
 # pacman-key -r 12135932909A15567BCC2AA91FA5B9C987E7BDF3
 # pacman-key --lsign-key 12135932909A15567BCC2AA91FA5B9C987E7BDF3
 # pacman-key -r 4345771566D76038C7FEB43863EC0ADBEA87E4E3
 # pacman-key --lsign-key 4345771566D76038C7FEB43863EC0ADBEA87E4E3
```

Now run

 ```
 # pacman -Syyu
 ```

Installing packages
-------------------

You may now install tools from the blackarchlinux repository. To list all of the available tools, run

 ```
 # pacman -Sgg | grep blackarchlinux
 ```

To install all of the tools, run

 ```
 # pacman -S blackarchlinux
 ```

To install a category of tools, run

 ```
 # pacman -S blackarchlinux-<category>
 ```

To see the blackarchlinux categories, run

 ```
 # pacman -Sg | grep blackarchlinux
 ```

<!-- Run `scripts/make-category-list` to generate this list. -->
Package Groups
--------------

Tools may be installed by category. The category groups are listed below.  To learn more about each included tool visit http://www.blackarchlinux.org/packages.html

#### Intelligence Gathering
 ```
 # pacman -S blackarchlinux-intel
 ```

Tools in this group:

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
  - sbd
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
  - udptunnel
  - urlcrazy
  - voiphopper
  - waffit
  - weevely
  - wol-e
  - xprobe2
  - zaproxy

#### Forensics
 ```
 # pacman -S blackarchlinux-forensics
 ```

Tools in this group:

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

#### Exploitation
 ```
 # pacman -S blackarchlinux-exploitation
 ```

Tools in this group:

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

#### Defensive
 ```
 # pacman -S blackarchlinux-defensive
 ```

Tools in this group:

  - artillery
  - inundator
  - maltego
  - metagoofil
  - secure-delete
  - sniffjoke

#### Wireless Attacks
 ```
 # pacman -S blackarchlinux-wireless
 ```

Tools in this group:

  - asleap
  - bluelog
  - bss
  - bt_audit
  - btscanner
  - cowpatty
  - fern-wifi-cracker
  - freeradius-wpe
  - giskismet
  - hidattack
  - killerbee
  - mdk3
  - mfcuk
  - mfoc
  - redfang
  - rfidiot
  - rfidtool
  - spooftooph
  - wifi-honey
  - wifitap
  - wifite

#### Threat Analysis
 ```
 # pacman -S blackarchlinux-analysis
 ```

Tools in this group:

  - bed
  - cflow
  - cisco-auditing-tool
  - cisco-global-exploiter
  - dbpwaudit
  - fang
  - lynis
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

#### Web Applications
 ```
 # pacman -S blackarchlinux-web-apps
 ```

Tools in this group:

  - blindelephant
  - burpsuite
  - davtest
  - dirb
  - dirbuster
  - fimap
  - grabber
  - joomscan
  - mfcuk
  - mfoc
  - nikto
  - paros
  - plecost
  - powerfuzzer
  - skipfish
  - sqlmap
  - sqlninja
  - sqlsus
  - w3af
  - wapiti
  - webscarab
  - webslayer
  - wfuzz
  - whatweb
  - xsser
  - zaproxy

#### Password Attacks
 ```
 # pacman -S blackarchlinux-password-attacks
 ```

Tools in this group:

  - burpsuite
  - cisco-auditing-tool
  - cmospwd
  - crunch
  - dbpwaudit
  - findmyhash
  - hash-identifier
  - hashcat
  - hydra
  - johnny
  - keimpx
  - medusa
  - onesixtyone
  - pack
  - patator
  - rcracki_mt
  - samdump2
  - sipcrack
  - sucrack
  - thc-pptp-bruter
  - webscarab
  - zaproxy

#### Hardware Hacking
 ```
 # pacman -S blackarchlinux-hardware-hacking
 ```

Tools in this group:

  - dex2jar
  - smali

#### Reverse Engineering
 ```
 # pacman -S blackarchlinux-reversing
 ```

Tools in this group:

  - dex2jar
  - edb-debugger
  - flasm
  - ollydbg
  - pev
  - radare2
  - recstudio
  - scanmem

#### Stress Testing
 ```
 # pacman -S blackarchlinux-stress-testing
 ```

Tools in this group:

  - iaxflood
  - mdk3
  - t50
  - thc-ipv6
  - thc-ssl-dos

#### Threat Modeling
 ```
 # pacman -S blackarchlinux-threat-model
 ```

Tools in this group:

  - magictree

