#!/usr/bin/env bash
BASEDIR="$( cd "$( dirname "$( readlink -f ${BASH_SOURCE[0]} )" )" && pwd )"
mono "$BASEDIR/de4dot-x64.exe" "$@"
