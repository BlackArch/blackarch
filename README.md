BlackArch
=========

*Not all tools have a GUI.  Since they are CLI based tools when you click on them in the menu you will find that sometimes a black terminal will appear and quickly disappear.  Tools are placed in the menu so they're easily visible and you know what is available in the toolset, if you see this happens just run the tool from a terminal.*

*When all else fails, run `as ROOT` user! The toolset is designed to be used as the ROOT user since most tools need to do manipulative computey stuff with packets and devices, these actions are only allowed to be performed by the ROOT user.*

Packages
--------

Some of the packages are not built from a PKGBUILD file, you'll see "ln2tool" file in the path it's because the tool is installed from the official ArchLinux repositories instead of building a PKGBUILD file for it, and I softlink to it into the BlackArch toolset.  All tools get placed into the BlackArch path layout as you see in the repository.  

To install any of the tools individually follow official <a href="https://wiki.archlinux.org/index.php/Arch_User_Repository#Installing_packages">AUR instructions</a> and run "makepkg" command on any PKGBUILD files for tools you want installed.

To install the entire BlackArch toolkit
---------------------------------------
*NOTE: When installing using the blackarch install script, please make sure the following repositories are enabled: `core` `extra` `community` `multilib`*

First, run:

	bash~$ blackarch_buildpkg.sh

with a `NON-ROOT account`.  This script finds all the PKGBUILD files, downloads the source, and builds the packages for you, it handles the dependencies.

Second, run:

	bash~$ blackarch_installpkg.sh

`as ROOT`.  This script finds all the packages that were built with makepkg and installs them for you.

Lastly, run:

	bash~$ blackarch_menus.sh

`as ROOT`.  This final script will install the menu items for XFCE.
