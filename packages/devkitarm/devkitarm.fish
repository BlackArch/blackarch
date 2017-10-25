# set needed variables
set -gx DEVKITPRO /opt/devkitpro
set -gx DEVKITARM $DEVKITPRO/devkitARM

# add to path
set -gx PATH $PATH $DEVKITARM/bin
