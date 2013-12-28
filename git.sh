#!/bin/sh

files="archpwn packages docs README.md broken AUTHORS COPYING"

if [ "$1" = "get" ]
then
    git pull https://github.com/BlackArch/blackarch master
    for f in own/blackarchirso own/blackarchwiki own/sploitctl
    do
      git pull
    done
    git push origin master
else
    for f in ${files}
    do
      git add ${f}
      git commit ${f}
    done

    git push
fi
