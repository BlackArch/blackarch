== The low down ==

Through my experiences I've broken down tools I've used into four phases:

Phase 1 => Discovery [GREEN],
Phase 2 => Enumeration [BLUE],
Phase 3 => Exploitation [RED],
Phase 4 => Post-Exploitation [YELLOW]

Each tool falls into a different "color" which is evident in the XFCE menu design.  But even without the XFCE menu and without the color codes the root "blackarch" directory is the toolset.  This toolset continues to expand daily, please see any of the latest updates at http://www.blackarch.org.

== Packages ==

Some of the packages are not built from a PKGBUILD file, you'll see "ln2tool" file in the path it's because I install the tool from the ArchLinux official repositories instead of building a PKGBUILD file for it, and I softlink to it into the BlackArch toolset.  All tools get placed into the BlackArch path layout as you see in the github repository.  

Some of the packages are not built from a PKGBUILD file, they're installed from the official repositories and softlinked into the BlackArch layout.  This is a little sloppy and we'll fix this in a later release but the _desktops directory contains the .desktop files for these packages to include them in the BlackArch xfce menu.

To install individually follow official AUR instructions and run "makepkg" command on all PKGBUILD files for tools you want installed.

To install the entire BlackArch toolkit:
First, run "blackarch_buildpkg.sh" with a NON-ROOT account.  This script finds all the PKGBUILD files, downloads the source, and builds the packages for you, it handles the dependencies.

Second, run the "blackarch_installpkg.sh" as ROOT.  This script finds all the packages that were built with makepkg and installs them for you.

** NOTE: When installing using the blackarch install script, please make sure the following repositories are enabled: "core" "extra" "community" "multilib" **