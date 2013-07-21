#
# Parameters to be passed to snort
#

# options taken from Fedora
# http://cvs.fedoraproject.org/viewcvs/devel/snort/sysconfig.snort?rev=1.2&view=markup

# Where is the snort.conf file.
SNORT_CONF="/etc/snort/snort.conf"

# What user account should we run under.
USER="snort"

# What group account should we run under. 
GROUP="snort"

# define the interface we listen on
INTERFACE="eth0"

# If you are using prelude, delete the '-A fast' option
SNORT_OPTIONS="-A fast -b -l /var/log/snort -D -p"
