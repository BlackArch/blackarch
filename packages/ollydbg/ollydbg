#!/bin/bash

set -e

if [ ! -d "$HOME/.ollydbg" ] ; then
	mkdir -p "$HOME/.ollydbg/wine"
	ln -s /usr/share/ollydbg/ollydbg.exe "$HOME/.ollydbg/ollydbg.exe"
	ln -s /usr/share/ollydbg/dbghelp.dll "$HOME/.ollydbg/dbghelp.dll"
fi

wine "$HOME/.ollydbg/ollydbg.exe" "$@"
