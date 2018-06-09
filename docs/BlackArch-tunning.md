This guide is oriented for tune BlackArch after you installed, don't forget create the secondary account during the installation with administrative rigths. 
#
1. The key is: Install from scratch 
#
2. The perfect cocktail is play with the repos: blackarch + antergos + arch with pr1v8 DNS's from Parrot Security OS, instead of use your ISP and google, with Round Robin feature.
#
Just lookup for the below needed files at
#
https://github.com/4k4xs4pH1r3/blackarch/upload/master/docs
#
» Make a backup of the existents files and replace as below:
#
# 
pacman.conf         (replace at directory: /etc/)
# 
resolv.conf         (replace at directory: /etc/)
# 
antergos-mirrolist  (replace at directory: /etc/pacman.d/)
# 
blackarch-mirrolist (replace at directory: /etc/pacman.d/)
# 
mirrolist           (replace at directory: /etc/pacman.d/)
# 
gpg.conf            (replace at directory: /etc/pacman.d/gnupg/)
# 
pubring.gpg         (replace at directory: /etc/pacman.d/gnupg/)
# 
trustdb.gpg         (replace at directory: /etc/pacman.d/gnupg/)


#
So let's start, in my case I have some issues related with network specifically getting up the interfaces; dhcp; wicd; d-bus / NetworkManager. Once I finished the installation of this impressive distro. 

#
For fix that issues execute this commads from your terminal as root

#
rm -r /var/run/wicd/wicd.pid
#
systemctl enable wicd.service
#
systemctl start wicd.service
#
systemctl status wicd.service

# 
Now let's identify if your network is ready and the interface names with the below commands

#
lspci | grep -i net
#
iwconfig
#
iplink

#Strange or not, idk, not auto created a profile network during the installation (is possible that my machine have a fail related with hw due to the model is from 2011), inclusive having the nic connected before to start the installation, in this case we need to do that manually following this steps:

#
cd /etc/netctl 
#
cp examples/ethernet-static my-network

#
nano my-network

And change the Interface from eth0 to enp19s0 (or whatever your adapter is in ip link).
Finally, I enabled it to use that profile on startup with
#
netctl enable my-network

#
In my case my system reported the below for ethernet: enp19s0, and for wireless: wlp18s0b1

#
ip link set enp19s0 up
#
ip link set wlp18s0b1 up

#
Just in case that from blackarch wm you can't conenct to internet with the both options embedded in this distro, go to your terminal and execute:
#
wifi-menu
#
systemctl enable networkmanager.service
#
systemctl start networkmanager.service
#
systemctl status networkmanager.service

#
Ok, is time to up both interfaces, for that execute this sequence
#
dhcpcd enp19s0
#
dhcpcd wlp18s0b1

#
Now 'll upgrade your system and add deepin & gnome as your wm's
#
pacman -Syuu
#
reboot

#Verify that all the packages of BlackArch are really installed and Upgraded
#
pacman -Syy blackarch --needed --overwrite --noconfirm
#
reboot

3. Use the combo pamac + octopi + yaourt + pacaur + pacman with the respective notifiers and use as default deepin as your wm.. This 'll made You happy!
#
pacman -S pacaur
#
pacman -S yaourt
#
pacman -S octopi
#
pacman -S pamac
#
pacman -S xorg xorg-server
#
pacman -S deepin
#
pacman -S deepin-extra
#
systemctl start lightdm.service
#
systemctl enable lightdm.service
#
systemctl status lightdm.service
#
pacman -S gnome gnome-extra
#
systemctl enable gdm.service
#
systemctl start gdm.service
#
systemctl status gdm.service
#
reboot

#Choose deepin as your window manager in the left down corner from the list and login with the 2nd account that you created (no with root)

#Now install telegram
#
pacman -S telegram-desktop
#
#
#
From this point in advance your mind need be inspired...
