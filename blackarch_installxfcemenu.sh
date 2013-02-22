#!/bin/bash
workingpath=$PWD  ## STORE THE CURRENT PATH IN CASE WE NEED IT LATER

## MODIFY THE MAIN xfce-applications.menu FILE (THIS IS SLOPPY, WE'RE GONNA NEED A CLEANER WAY TO DO THIS)
sed -i 's,'"<Name>Accessories</Name>"',\t<Name>BlackArch</Name>\n\t\t<Directory>xfce-blackarch.directory</Directory>\n\t\t<MergeFile>xfce-blackarch.menu</MergeFile>\n\t</Menu>\n\t<Menu>\n\t\t<Name>Accessories</Name>\n\n,' /etc/xdg/menus/xfce-applications.menu

## COPY THE MERGE FILE TO INCLUDE THE MENU ITEMS
wget -O /etc/xdg/menus/xfce-blackarch.menu http://www.blackarch.org/xfce/menu-items/menu_structure/menus/xfce-blackarch.menu

## COPY ALL THE MENU ITEM DIRECTORY FILES
wget -O /usr/share/desktop-directories/xfce-blackarch-root.directory http://www.blackarch.org/xfce/menu-items/menu_structure/desktop-directories/xfce-blackarch-root.directory
wget -O /usr/share/desktop-directories/xfce-blackarch-blackanalysis.directory http://www.blackarch.org/xfce/menu-items/menu_structure/desktop-directories/xfce-blackarch-blackanalysis.directory
wget -O /usr/share/desktop-directories/xfce-blackarch-blackforensics.directory http://www.blackarch.org/xfce/menu-items/menu_structure/desktop-directories/xfce-blackarch-blackforensics.directory
wget -O /usr/share/desktop-directories/xfce-blackarch-blackmisc.directory http://www.blackarch.org/xfce/menu-items/menu_structure/desktop-directories/xfce-blackarch-blackmisc.directory
wget -O /usr/share/desktop-directories/xfce-blackarch-blackpost.directory http://www.blackarch.org/xfce/menu-items/menu_structure/desktop-directories/xfce-blackarch-blackpost.directory
wget -O /usr/share/desktop-directories/xfce-blackarch-blackexploitation.directory http://www.blackarch.org/xfce/menu-items/menu_structure/desktop-directories/xfce-blackarch-blackexploitation.directory
wget -O /usr/share/desktop-directories/xfce-blackarch-blackintel.directory http://www.blackarch.org/xfce/menu-items/menu_structure/desktop-directories/xfce-blackarch-blackintel.directory
wget -O /usr/share/desktop-directories/xfce-blackarch-blackmodeling.directory http://www.blackarch.org/xfce/menu-items/menu_structure/desktop-directories/xfce-blackarch-blackmodeling.directory


## COPY THE ICONS FOR THE MENU ITEMS
wget -O /usr/share/pixmaps/blackarch.png http://www.blackarch.org/xfce/menu-items/icons/blackarch.png
wget -O /usr/share/pixmaps/blackarch_001.png http://www.blackarch.org/xfce/menu-items/icons/blackarch_001.png
wget -O /usr/share/pixmaps/blackarch_002.png http://www.blackarch.org/xfce/menu-items/icons/blackarch_002.png
wget -O /usr/share/pixmaps/blackarch_003.png http://www.blackarch.org/xfce/menu-items/icons/blackarch_003.png
wget -O /usr/share/pixmaps/blackarch_004.png http://www.blackarch.org/xfce/menu-items/icons/blackarch_004.png
wget -O /usr/share/pixmaps/blackarch_005.png http://www.blackarch.org/xfce/menu-items/icons/blackarch_005.png
wget -O /usr/share/pixmaps/blackarch_006.png http://www.blackarch.org/xfce/menu-items/icons/blackarch_006.png
wget -O /usr/share/pixmaps/blackarch_007.png http://www.blackarch.org/xfce/menu-items/icons/blackarch_007.png
wget -O /usr/share/pixmaps/blackarch_008.png http://www.blackarch.org/xfce/menu-items/icons/blackarch_008.png
wget -O /usr/share/pixmaps/blackarch_009.png http://www.blackarch.org/xfce/menu-items/icons/blackarch_009.png
wget -O /usr/share/pixmaps/blackarch_010.png http://www.blackarch.org/xfce/menu-items/icons/blackarch_010.png
wget -O /usr/share/pixmaps/blackarch_011.png http://www.blackarch.org/xfce/menu-items/icons/blackarch_011.png