What is BlackArch Linux?
------------------------

**BlackArch Linux** is a lightweight expansion to Arch Linux for penetration
testers.

The toolset is distributed as an Arch Linux unofficial user repository so you
can install BlackArch Linux on top of an existing Arch Linux installation.
Packages may be installed individually or by category.

We currently have over 60 tools in our toolset. We are expanding the repository
to include over 200 tools. All tools are thoroughly tested before being added
to the codebase to maintain the quality of the repository.

Get Involved
------------

You can get in touch with the BlackArch team. Just check out the following:

**Web:** http://blackarch.org/

**Mail:** blackarch@gmail.com

**IRC:** irc://irc.genscripts.net/blackarch

Quick Start
-----------

Add the following to `/etc/pacman.conf`.

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
:: There are 62 members in group blackarch:
:: Repository blackarch
...
Enter a selection (default=all):
```

Select the packages that you wish to install. By default, all of the packages
in the toolset will be installed.

Package Groups
--------------

Tools may be installed by category. The category groups are listed below.

#### Exploitation

```
$ pacman -S blackarch-exploitation
```
- Included packages:
  - beef
  - fern-wifi-cracker
  - fimap
  - metasploit
  - reaver-wps
  - set
  - sqlmap
  - sqlninja
  - websploit
  - xsser

#### Forensics

```
$ pacman -S blackarch-forensics
```

- Included packages:
  - pdf-parser
  - pdfid
  - peepdf
  - volatility

#### Intelligence Gathering

```
$ pacman -S blackarch-intel
```

- Included packages:
  - blindelephant
  - burpsuite
  - cms-explorer
  - dnsbf
  - dnsenum
  - dnsmap
  - hashid
  - netglub
  - nikto
  - snmpcheck
  - ssldump
  - theharvester
  - zaproxy

#### Miscellaneous

```
$ pacman -S blackarch-misc
```

- Included packages:
  - artillery
  - geoipgen

#### Post-Exploitation

```
$ pacman -S blackarch-post
```

- Included packages:
  - chownat
  - netcommander
  - pwnat
  - sslstrip

#### Threat Modeling

```
$ pacman -S blackarch-threat-model
```

- Included packages:
  - magictree

#### Vulnerability Analysis

```
$ pacman -S blackarch-analysis
```

- Included packages:
  - edb
  - fang
  - pentbox
  - scapy
  - sipvicious
  - slowhttptest
  - sslscan
  - tcpjunk

To learn more about each package visit http://www.blackarch.org/packages.html
