#!/bin/sh

# arg 1:  the new package version
# arg 2:  the old package version
post_upgrade() {
  if [ "$(vercmp $2 1.14-1)" -le 0 ]; then
    cat << EOF
===> sslh systemd service has been splitted in sslh-fork.service and sslh-select.service
EOF
    fi
}

# vim:set ts=2 sw=2 ft=sh et:
