#!/bin/bash
workingpath=$PWD  ## STORE THE CURRENT PATH IN CASE WE NEED IT LATER

## MODIFY THE MAIN xfce-applications.menu FILE (THIS IS SLOPPY, WE'RE GONNA NEED A CLEANER WAY TO DO THIS)
sed -i 's,'"<Name>Accessories</Name>"',\t<Name>BlackArch</Name>\n\t\t<Directory>xfce-blackarch.directory</Directory>\n\t\t<MergeFile>xfce-blackarch.menu</MergeFile>\n\t</Menu>\n\t<Menu>\n\t\t<Name>Accessories</Name>\n\n,' /etc/xdg/menus/xfce-applications.menu

## COPY THE MERGE FILE TO INCLUDE THE MENU ITEMS
cp _menu-items/menu_structure/menus/* /etc/xdg/menus/

## COPY ALL THE MENU ITEM DIRECTORY FILES
cp _menu-items/menu_structure/desktop-directories/* /usr/share/desktop-directories/

## COPY THE ICONS FOR THE MENU ITEMS
cp _menu-items/logos/* /usr/share/pixmaps/