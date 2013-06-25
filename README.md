BlackArch
=========

*Not all tools have a GUI.  When you click on tools in the menu that are CLI based you will find that sometimes a black terminal will appear and quickly disappear.  Tools are placed in the menu so they're easily visible and you know what is available in the toolset, if you see this happens just run the tool from a terminal.*

*Most tools will appear they don't run properly, when all else fails run as `root` user! The toolset is designed to be used as the `root` user since most tools need to do manipulative computey stuff with packets and devices, these actions are only allowed to be performed by the `root` user.*

Packages
--------

Some of the packages are not built from a PKGBUILD file, you'll see "ln2tool" file in the path it's because the tool is installed from the official ArchLinux repositories instead of building a PKGBUILD file for it, and I softlink to it into the BlackArch toolset.  All tools get placed into the BlackArch path layout as you see in the repository.  

To install any of the tools individually follow official <a href="https://wiki.archlinux.org/index.php/Arch_User_Repository#Installing_packages">AUR instructions</a> and run "makepkg" command on any PKGBUILD files for tools you want installed.

To install the entire BlackArch toolkit
---------------------------------------
*NOTE: When installing using the blackarch install script, please make sure the following repositories are enabled: `core` `extra` `community` `multilib` and lastly make sure to synchronize your pacman database and be fully up-to-date with your packages with `pacman -Syu`*

First, run:

	bash~$ blackarch_buildpkg.sh

with a `NON-ROOT account`.  This script finds all the PKGBUILD files, downloads the source, and builds the packages for you, it handles the dependencies.

Second, run:

	bash~$ blackarch_installpkg.sh

`as ROOT`.  This script finds all the packages that were built with makepkg and installs them for you.

Lastly (and optionally), run:

	bash~$ blackarch_installxfcemenu.sh

`as ROOT`.  This final script will install the menu items for XFCE.
