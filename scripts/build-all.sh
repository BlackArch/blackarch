#!/bin/bash

[[ -z "$GPG_AGENT_INFO" ]] && eval $(gpg-agent --daemon)

# Syntax fix
#)

echo "Note: this must be run from the root of the repository."

ls -1 packages > all-packages

tmux att -t archtrack-build 2>/dev/null ||
tmux \
  new -s get -n reverse \; \
  send-keys 'scripts/do-build.sh -r' C-m \; \
  neww -n random1 \; \
  send-keys 'scripts/do-build.sh -R' C-m \; \
  neww -n random2  \; \
  send-keys 'scripts/do-build.sh -R' C-m \; \
  neww -n normal \; \
  send-keys 'scripts/do-build.sh' C-m \; \
  selectw -t random1
