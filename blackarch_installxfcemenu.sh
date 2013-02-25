#!/bin/bash
workingpath=$PWD  ## STORE THE CURRENT PATH IN CASE WE NEED IT LATER

## MODIFY THE MAIN xfce-applications.menu FILE (THIS IS SLOPPY, WE'RE GONNA NEED A CLEANER WAY TO DO THIS)
sed -i 's,'"<Name>Accessories</Name>"',\t<Name>BlackArch</Name>\n\t\t<Directory>xfce-blackarch.directory</Directory>\n\t\t<MergeFile>xfce-blackarch.menu</MergeFile>\n\t</Menu>\n\t<Menu>\n\t\t<Name>Accessories</Name>\n\n,' /etc/xdg/menus/xfce-applications.menu

## COPY THE MERGE FILE TO INCLUDE THE MENU ITEMS
wget -O /etc/xdg/menus/xfce-blackarch.menu http://www.blackarch.org/pub/xfce/menu-items/menu_structure/menus/xfce-blackarch.menu

## COPY ALL THE MENU ITEM DIRECTORY FILES
wget -O /usr/share/desktop-directories/xfce-blackarch-root.directory http://www.blackarch.org/pub/xfce/menu-items/menu_structure/desktop-directories/xfce-blackarch-root.directory
wget -O /usr/share/desktop-directories/xfce-blackarch-blackanalysis.directory http://www.blackarch.org/pub/xfce/menu-items/menu_structure/desktop-directories/xfce-blackarch-blackanalysis.directory
wget -O /usr/share/desktop-directories/xfce-blackarch-blackforensics.directory http://www.blackarch.org/pub/xfce/menu-items/menu_structure/desktop-directories/xfce-blackarch-blackforensics.directory
wget -O /usr/share/desktop-directories/xfce-blackarch-blackmisc.directory http://www.blackarch.org/pub/xfce/menu-items/menu_structure/desktop-directories/xfce-blackarch-blackmisc.directory
wget -O /usr/share/desktop-directories/xfce-blackarch-blackpost.directory http://www.blackarch.org/pub/xfce/menu-items/menu_structure/desktop-directories/xfce-blackarch-blackpost.directory
wget -O /usr/share/desktop-directories/xfce-blackarch-blackexploitation.directory http://www.blackarch.org/pub/xfce/menu-items/menu_structure/desktop-directories/xfce-blackarch-blackexploitation.directory
wget -O /usr/share/desktop-directories/xfce-blackarch-blackintel.directory http://www.blackarch.org/pub/xfce/menu-items/menu_structure/desktop-directories/xfce-blackarch-blackintel.directory
wget -O /usr/share/desktop-directories/xfce-blackarch-blackmodeling.directory http://www.blackarch.org/pub/xfce/menu-items/menu_structure/desktop-directories/xfce-blackarch-blackmodeling.directory


## COPY THE ICONS FOR THE MENU ITEMS
wget -O /usr/share/pixmaps/blackarch-start.png http://www.blackarch.org/pub/xfce/menu-items/icons/blackarch-start.png
wget -O /usr/share/pixmaps/blackarch-main-black.png http://www.blackarch.org/pub/xfce/menu-items/icons/blackarch-main-black.png
wget -O /usr/share/pixmaps/blackarch-main-blue.png http://www.blackarch.org/pub/xfce/menu-items/icons/blackarch-main-blue.png
wget -O /usr/share/pixmaps/blackarch-main-brown.png http://www.blackarch.org/pub/xfce/menu-items/icons/blackarch-main-brown.png
wget -O /usr/share/pixmaps/blackarch-main-green.png http://www.blackarch.org/pub/xfce/menu-items/icons/blackarch-main-green.png
wget -O /usr/share/pixmaps/blackarch-main-grey.png http://www.blackarch.org/pub/xfce/menu-items/icons/blackarch-main-grey.png
wget -O /usr/share/pixmaps/blackarch-main-purple.png http://www.blackarch.org/pub/xfce/menu-items/icons/blackarch-main-purple.png
wget -O /usr/share/pixmaps/blackarch-main-red.png http://www.blackarch.org/pub/xfce/menu-items/icons/blackarch-main-red.png
wget -O /usr/share/pixmaps/blackarch-main-yellow.png http://www.blackarch.org/pub/xfce/menu-items/icons/blackarch-main-yellow.png