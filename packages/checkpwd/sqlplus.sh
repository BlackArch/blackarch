#!/bin/sh
cd /usr/share/checkpwd
LD_LIBRARY_PATH="." ./sqlplus $*
