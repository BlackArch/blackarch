#!/bin/sh

usage() {
cat << _EOF
Usage:
  gconfpkg [OPTION] [PACKAGE] 

  Help Options:
    -?, --help            Show help options

  Application Options:
    --install             Install schemas for a given package
    --uninstall           Uninstall schemas for a given package

_EOF
}

install() {
  GCONF_CONFIG_SOURCE=`/usr/bin/gconftool-2 --get-default-source` \
      /usr/bin/gconftool-2 --makefile-install-rule /usr/share/gconf/schemas/${pkgname}.schemas >/dev/null
}

uninstall() {
  if [ -f /usr/share/gconf/schemas/${pkgname}.schemas ]; then
    schemas=/usr/share/gconf/schemas/${pkgname}.schemas
  elif [ -f /opt/gnome/share/gconf/schemas/${pkgname}.schemas ]; then
    schemas=/opt/gnome/share/gconf/schemas/${pkgname}.schemas
  else
    schemas=`pacman -Ql ${pkgname} | grep 'gconf/schemas/.*schemas$' | awk '{ print $2 }'`
  fi
  GCONF_CONFIG_SOURCE=`/usr/bin/gconftool-2 --get-default-source` \
      /usr/bin/gconftool-2 --makefile-uninstall-rule ${schemas} >/dev/null
}

if [ -z "$2" ]; then
  usage
else
  pkgname="$2"
  case "$1" in
    --install)
      install
    ;;
    --uninstall)
      uninstall
    ;;
    *)
    usage
    ;;
  esac
fi
