#!/usr/bin/env sh

export OPENJFX_MODULES_PATH='/usr/share/java/java-openjfx'
export OPENJFX_LIBRARY_PATH='/usr/lib/java-openjfx'
export OPENJFX_JAVA_OPTIONS="-Djava.library.path=$OPENJFX_LIBRARY_PATH --module-path=$OPENJFX_MODULES_PATH"
