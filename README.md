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

**Please, send us pull requests!**

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

#### Forensics

```
$ pacman -S blackarch-forensics
```

#### Intelligence Gathering

```
$ pacman -S blackarch-intel
```

#### Miscellaneous

```
$ pacman -S blackarch-misc
```

#### Post-Exploitation

```
$ pacman -S blackarch-post
```

#### Threat Modeling

```
$ pacman -S blackarch-threat-model
```

#### Vulnerability Analysis

```
$ pacman -S blackarch-analysis
```

To learn more about what the groups and each included tool visit http://www.blackarch.org/packages.html
