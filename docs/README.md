What is BlackArchLinux?
------------------------

**BlackArchLinux** is a lightweight expansion to Arch Linux for penetration
testers.

The toolset is distributed as an Arch Linux [unofficial user
repository](https://wiki.archlinux.org/index.php/Unofficial_User_Repositories)
so you can install BlackArchLinux on top of an existing Arch Linux
installation. Packages may be installed individually or by category.

<!-- Count tools: pacman -Sgg | grep blackarchlinux | cut -d' ' -f2 | sort -u | wc -l -->
We currently have over 650 tools in our toolset and the repository is
constantly expanding. All tools are thoroughly tested before being added to the
codebase to maintain the quality of the repository.

Get Involved
------------

You can get in touch with the blackarch team. Just check out the following:

**Please, send us pull requests!**

**Web:** http://www.blackarch.org/

**Mail:** team@blackarch.org

**IRC:** [irc://irc.freenode.net/blackarch](irc://irc.freenode.net/blackarch)

Setup
-----------

Add this to
[`/etc/pacman.conf`](https://www.archlinux.org/pacman/pacman.conf.5.html):

```
[blackarch]
Server = http://www.blackarch.org/blackarch/$repo/os/$arch
```
To use the mirrorlist:
```
pacman -S blackarch-mirrorlist
```
And uncomment mirror that is closest to you.

Then add this to
[`/etc/pacman.conf`](https://www.archlinux.org/pacman/pacman.conf.5.html):
```
[blackarch]
Include = /etc/pacman.d/mirrorlist-blackarch
```



For package signing, pull in and sign the package signing keys:

```
 # pacman-key -r 4345771566D76038C7FEB43863EC0ADBEA87E4E3
 # pacman-key --lsign-key 4345771566D76038C7FEB43863EC0ADBEA87E4E3
```

Now run

 ```
 # pacman -Syyu
 ```

Installing packages
-------------------

You may now install tools from the blackarch repository. To list all of the available tools, run

 ```
 # pacman -Sl blackarch
 ```

To install all of the tools, run

 ```
 # pacman -S blackarch
 ```

To install a category of tools, run

 ```
 # pacman -S blackarch-<category>
 ```

To see the blackarch categories, run

 ```
 # pacman -Sg | grep blackarch
 ```
