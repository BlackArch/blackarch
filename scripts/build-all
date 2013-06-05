#!/bin/bash

[[ -z "$GPG_AGENT_INFO" ]] && eval $(gpg-agent --daemon)

# Syntax fix
#)

ls -1 packages > all-packages

while read pack ; do
		(
		cd packages/$pack &&
		if grep -q '^working=true' info ; then
			makepkg --config ../../scripts/conf/makepkg.x86_64.conf 2>&1 | tee ../../builds.log
		fi
		)
done < <(sort "$@" all-packages)
