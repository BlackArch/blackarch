#!/bin/bash

#
# Simple script to run electric fence on stuff.  (needs libefence.so).
#
# $Id: ef,v 1.1 1999/03/31 21:31:23 kefka Exp $
#

# Usage check.
if [ "$1" = "" ]; then
	echo "$0: Usage: ef [executable] [arguments]."
	echo "	Runs the executable under the Electric Fence malloc debugger."
	exit
fi

# Set the environment to load eletric fence.
LD_PRELOAD=libefence.so.0.0 exec "$@"
