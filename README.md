## Description

**BlackArch Linux** is an ArchLinux based penetration testing distribution for
penetration testers and security researchers. The repository contains
[2825 tools](https://www.blackarch.org/tools.html). You can install tools
individually or in groups. BlackArch Linux is compatible with existing Arch
installs. For more information, see the installation instructions.

To report bugs and request new tools, please visit the issue tracker on Github,
stop by Matrix, or email us.

## Download and Installation

BlackArch Linux only takes a moment to setup.

There are three ways to go:

  1. [Install on an existing Arch machine](https://www.blackarch.org/downloads.html#install-repo).
  2. Use the [live ISO](https://blackarch.org/downloads.html).
  3. The Full and Netinstall ISOs come with a text-based installer
     (`blackarch-install`). The Slim ISO comes with a GUI-based installer.
     You can use the installer to install BlackArch Linux onto your hard disk.

## Notes on the current ISO/OVA images (2023) 
If you try `sudo pacman -Syu` you might notice that there is a JDk/JRE 21 package conflicts. To get passed this simply run the command below:

`sudo pacman -Sy jdk-openjdk && pacman -Su` 

If you wanted to read more on this go [here](https://archlinux.org/news/incoming-changes-in-jdk-jre-21-packages-may-require-manual-intervention/)

In additon, there maybe some key errors, feel free to go [here](https://github.com/BlackArch/blackarch/issues/4290#issuecommnet-2424171751)

## Get Involved

You can get in touch with the BlackArch Linux team. Just check out the following:

**Please, send us pull requests!**

To start developing for BlackArch please refer to the [Dev-Guide](./docs/HOWTO-DEV.md)

**Web:** https://www.blackarch.org/

**Mail:** team@blackarch.org

**Matrix:** [#BlackArch:matrix.org](https://matrix.to/#/#BlackArch:matrix.org)
