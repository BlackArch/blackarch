# Instructions for Contribution

## Notes

This document is a quick-start guide and shows you how to contribute to the [blackarch](http://blackarch.org/) project.
We accept pull requests of all sizes, from tiny typo fixes to new packages.
For any help, suggestions or questions feel free to visit and contact us on:

* [irc://irc.genscripts.net:6667/blackarch](irc://irc.genscripts.net:6667/blackarch)

Everyone is welcome and we highly appreciate it for any contribution to this project brough by you and others.

## Tutorials

We recommend you to read the following tutorials to get an overall overview and understand the basics before starting to contributing:

- [Arch Packaging Standards](https://wiki.archlinux.org/index.php/Arch_Packaging_Standards)
- [Creating Packages](https://wiki.archlinux.org/index.php/Creating_Packages)
- [PKGBUILD](https://wiki.archlinux.org/index.php/PKGBUILD)
- [Makepkg](https://wiki.archlinux.org/index.php/Makepkg)


## Steps

In order to submit your changes to the blackarch project, the following (general) steps should be performed:

*1. Fork the repository from [https://github.com/BlackArch/blackarch](https://github.com/BlackArch/blackarch)*

*2. Hack the necessary files, e.g. PKGBUILD, .patch files, etc.*

*3. Commit your changes.*

*4. Push your changes.*

*5. Ask us to merge in your changes, preferably through a pull request.*

### Example

The following example illustrates a general approach to build a package and submit to the blackarch repository. However, in this example we use [yaourt](https://wiki.archlinux.org/index.php/yaourt) to fetch a pre-defined PKGBUILD file of the tool, *nfsshell*, from the [AUR](https://aur.archlinux.org/) repository and adjust it according to our needs.


*1. Fetch PKGBUILD file*
```
user@blackarch $ yaourt -G nfsshell
==> Download nfsshell sources
x LICENSE
x PKGBUILD
x gcc.patch
user@blackarch $ cd nfsshell/
```

*2. Clean up PKGBUILD file and save some time*
```
user@blackarch nfsshell $ ./blarckarch/scripts/prep-packages PKGBUILD
cleaning 'PKGBUILD'...
expanding tabs...
removing vim modeline...
removing id comment...
removing contributor and maintainer comments...
squeezing extra blank lines...
removing '|| return'...
removing leading blank line...
removing $pkgname...
removing trailing whitespace...
```

*3. Adjust PKGBUILD file*
```
user@blackarch nfsshell $ vi PKGBUILD
```

*4. Build the package*
```
user@blackarch nfsshell $ makepkg -sf
==> Making package: nfsshell 19980519-1 (Mon Dec  2 17:23:51 CET 2013)
==> Checking runtime dependencies...
==> Checking buildtime dependencies...
==> Retrieving sources...
-> Downloading nfsshell.tar.gz...
% Total    % Received % Xferd  Average Speed   Time    Time     Time  CurrentDload  Upload   Total   Spent    Left  Speed100 29213  100 29213    0     0  48150      0 --:--:-- --:--:-- --:--:-- 48206
-> Found gcc.patch
-> Found LICENSE
...
<lot's of build process and compiler output here>
...
==> Leaving fakeroot environment.
==> Finished making: nfsshell 19980519-1 (Mon Dec  2 17:23:53 CET 2013)
```

*5. Install and test the package*
```
user@blackarch nfsshell $ pacman -U nfsshell-19980519-1-x86_64.pkg.tar.xz 
```

*6. Add, commit and push package*
```
user@blackarch nfsshell $ cd /blackarch/packages
user@blackarch ~/blackarch/packages $ mv ~/nfsshell .
user@blackarch ~/blackarch/packages $ git add nfsshell && git commit nfsshell && git push
```

*7. Create a pull request on github*
```
firefox https://github.com/<contributor>/blackarch
```

### Requests

1. Don't add **Maintainer** or **Contributor** comments to PKGBUILD files.
2. For the sake of consistency, use two-space indentation.


## General Tips

- We have our own fork of namcap. It checks PKGBUILD files. Use it. https://github.com/BlackArch/namcap