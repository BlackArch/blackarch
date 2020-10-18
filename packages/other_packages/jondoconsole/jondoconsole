#!/bin/sh

CONF="$HOME/.jondoconsole/jondoconsole.conf"
LOGFILE="$HOME/.jondoconsole/jondoconsole.log"
CACHE="$HOME/.jondoconsole/jondoconsole.sqlite"

# Create config directory
if [ ! -d $HOME/.jondoconsole ]; then
	mkdir $HOME/.jondoconsole
fi

if [ -e $HOME/.jondoconsole.log ]; then
	if [ -e $HOME/.jondoconsole.log.1 ]; then
		if [ -e $HOME/.jondoconsole.log.2 ]; then
			rm $HOME/.jondoconsole.log.2
		fi
		mv $HOME/.jondoconsole.log.1 $HOME/.jondoconsole.log.2
	fi
	mv $HOME/.jondoconsole.log $HOME/.jondoconsole.log.1
fi


java -Djava.net.preferIPv4Stack=true -jar /usr/share/java/jondoconsole/JonDoConsole.jar -c $CONF -l $LOGFILE -s $CACHE
