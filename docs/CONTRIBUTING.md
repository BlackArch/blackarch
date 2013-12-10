# Instructions for Contribution

## Notes

This is a quick-start guide. It shows you how to contribute to the
[blackarchlinux](http://blackarchlinux.org/) project. We accept pull requests of all
sizes, from tiny typo fixes to new packages. For help, suggestions, or
questions feel free to contact us on:

* [irc://irc.freenode.net:6667/blackarch](irc://irc.freenode.net:6667/blackarch)

Everyone is welcome to contribute. All contributions are appreciated.

## Tutorials

Please read the following tutorials before contributing:

- [Arch Packaging Standards](https://wiki.archlinux.org/index.php/Arch_Packaging_Standards)
- [Creating Packages](https://wiki.archlinux.org/index.php/Creating_Packages)
- [PKGBUILD](https://wiki.archlinux.org/index.php/PKGBUILD)
- [Makepkg](https://wiki.archlinux.org/index.php/Makepkg)


## Steps

In order to submit your changes to the BlackArchLinux project, follow these (general) steps:

1. Fork the repository from [https://github.com/BlackArchLinux/blackarchlinux](https://github.com/BlackArchLinux/blackarchlinux)

2. Hack the necessary files, (e.g. PKGBUILD, .patch files, etc).

3. Commit your changes.

4. Push your changes.

5. Ask us to merge in your changes, preferably through a pull request.

### Example

This example demonstrates submitting a new package to the blackarch project. We
use [yaourt](https://wiki.archlinux.org/index.php/yaourt) to fetch a
pre-existing PKGBUILD file for *nfsshell* from the
[AUR](https://aur.archlinux.org/) and adjust it according to our needs.


**1. Fetch PKGBUILD file**
```
user@blackarchlinux $ yaourt -G nfsshell
==> Download nfsshell sources
x LICENSE
x PKGBUILD
x gcc.patch
user@blackarchlinux $ cd nfsshell/
```

**2. Clean up PKGBUILD file and save some time**
```
user@blackarchlinux nfsshell $ ./blarckarch/scripts/prep-packages PKGBUILD
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

**3. Adjust PKGBUILD file**
```
user@blackarchlinux nfsshell $ vi PKGBUILD
```

**4. Build the package**
```
user@blackarchlinux nfsshell $ makepkg -sf
==> Making package: nfsshell 19980519-1 (Mon Dec  2 17:23:51 CET 2013)
==> Checking runtime dependencies...
==> Checking buildtime dependencies...
==> Retrieving sources...
-> Downloading nfsshell.tar.gz...
% Total    % Received % Xferd  Average Speed   Time    Time     Time  CurrentDload  Upload   Total   Spent    Left  Speed100 29213  100 29213    0     0  48150      0 --:--:-- --:--:-- --:--:-- 48206
-> Found gcc.patch
-> Found LICENSE
...
<lots of build process and compiler output here>
...
==> Leaving fakeroot environment.
==> Finished making: nfsshell 19980519-1 (Mon Dec  2 17:23:53 CET 2013)
```

**5. Install and test the package**
```
user@blackarchlinux nfsshell $ pacman -U nfsshell-19980519-1-x86_64.pkg.tar.xz
```

**6. Add, commit and push package**
```
user@blackarchlinux nfsshell $ cd /blackarchlinux/packages
user@blackarchlinux ~/blackarchlinux/packages $ mv ~/nfsshell .
user@blackarchlinux ~/blackarchlinux/packages $ git add nfsshell && git commit nfsshell && git push
```

**7. Create a pull request on github**
```
firefox https://github.com/<contributor>/blackarchlinux
```

### Requests

1. Don't add **Maintainer** or **Contributor** comments to PKGBUILD files. Add
   maintainer and contributor names to the AUTHORS file.
2. For the sake of consistency, please follow the general style of the other
   PKGBUILD files in the repo and use two-space indentation.

## General Tips

- [namcap](http://wiki.archlinux.org/index.php/Namcap) can check packages for
  errors.
