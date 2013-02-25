#!/bin/bash
workingpath=$PWD  ## store the current path so we can use it later

## FIND ALL THE PKGBUILD FILES AND STORE THE PATHS IN path.log
find $workingpath -type f -name "PKGBUILD" >> path.log
## SAVE THE PATHS WE FOUND IN AN ARRAY
logfile=( `cat "path.log"`)
## LOOP THROUGH EACH PATH IN THE ARRAY
for pkgbuild in "${logfile[@]}"
do
	## CHANGE TO THE DIRECTORY OF THE PKGBUILD
	cd `dirname $pkgbuild`
	## BUILD THE PACKAGE AND AUTOMATICALLY PULL ANY DEPENDENCIES
	makepkg -s PKGBUILD
done

## DELETE THE path.log
rm $workingpath/path.log
