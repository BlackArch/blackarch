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

Configure Repository
--------------------

BlackArch is compatible with normal Arch installations. It acts as an unofficial user repository.

0. If you have installed BlackArch before and there is an existing [blackarch] entry in /etc/pacman.conf, remove or comment out the entry and run pacman -Syy. 

1. Run the following as root. This is for package signing.

```
wget -q http://blackarch.org/keyring/blackarch-keyring.pkg.tar.xz{,.sig}                                                  
gpg --keyserver hkp://pgp.mit.edu --recv 4345771566D76038C7FEB43863EC0ADBEA87E4E3                                         
gpg --keyserver-o no-auto-key-retrieve --with-f blackarch-keyring.pkg.tar.xz.sig                                          
pacman-key --in                                                                                                           
rm blackarch-keyring.pkg.tar.xz.sig                                                                                       
pacman --noc -U blackarch-keyring.pkg.tar.xz                                                                              
```

2. If possible, please verify the signing key's fingerprint against as many sources as possible. 

3. Append the following lines to your /etc/pacman.conf file:

```
[blackarch]
Server = <mirror_site>/$repo/os/$arch
```
Replace <mirror_site> with a mirror site of your choosing. Please use one of our official mirrors. See the list of [mirror sites](http://blackarch.org/download.html#mirrors) on our website.

4. Now run:

```
$ sudo pacman -Syyu
```

Installing Packages From Repository
-----------------------------------

You may now install tools from the blackarch repository. To list all of the available tools, run

 ```
 # pacman -Sgg | grep blackarch | cut -d' ' -f2 | sort -u
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

BlackArch Install From ArchLinux / BlackArch Live / BlackArch NetInstall
------------------------------------------------------------------------

Install blackarch-install-scripts package

 ```
 # pacman -S blackarch-install-scripts 
 ```

Follow this guide

    1. # blackarch-install
    2. Select Install Mode:
        1. Install from LiveCD.
            - It will dump the current Live image into your hard drive.
        2. Install from BlackArch Official Repository.
            - It will install base system from Arch Linux and
            add Black Arch Linux packages from the official repository
            into your hard drive. You can choose the packets to install.
        3. Install from Blackman.
            - It will install base system from Arch Linux and
            will download and compile all the BlackArch Linux packages
            from their source according to PKGBULD files.
    3. Partitioning:
        3.1. Set the device you want to install BlackArch Linux in.
            -> i.e. /dev/sda (default one)
        3.2. Choose Partition Mode:
            * At the moment, only /boot and / root partition are supported.
            * We will add optional /home and Swap in future releases.
            1. Automatic - only empty HD! new partitions will be created in
               /dev/sda1 and /dev/sda2.
                - This mode its thinking for Virtual Machines environment,
                where hard disk isnt contain any partitions already.
                It will create by default sda1 for /boot partition
                and sda2 for / root partition. Also, it will format
                /boot partition with ext2 type and / root partition with
                ext4 type.
                - The only configurable thing here its the size of both
                partitions. Set it by default to 100MB for /boot and
                10GB for / root partition.
                - WARNING: this mode isnt tested properly in non Virtual
                Machine box. So do not used it at least you know what
                you are doing. You had been warned bitch. ;)
            2. Manual - set manual partitions with cfdisk and format them.
                - This mode will run cfdisk utility for partition creation.
                Once you have set up your proper /boot and / root partition,
                dont forget to Write and you are good to go.
                It will ask you when Quit for the /boot and / root partition
                number chosed, also the partition type is required for
                format purposes.
    4. Install
        - BlackArch Linux will be now installed into your hard drive.
        This will take a while.
    5. Setup up root password.
    6. Setup normal user account (optional).
    7. Grub install (automatic)
    8. Reboot and enjoy your BlackArch distro.
