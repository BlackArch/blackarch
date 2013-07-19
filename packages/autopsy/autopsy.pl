# Autopsy configuration settings

# when set to 1, the server will stop after it receives no
# connections for STIMEOUT seconds.
$USE_STIMEOUT = 0;
$STIMEOUT = 3600;

# number of seconds that child waits for input from client
$CTIMEOUT = 15;

# set to 1 to save the cookie value in a file (for scripting)
$SAVE_COOKIE = 1;

$INSTALLDIR = '/usr/lib/autopsy/';


# System Utilities
$GREP_EXE = '/usr/bin/grep';
$FILE_EXE = '/usr/bin/file';
$MD5_EXE = '/usr/bin/md5sum';
$SHA1_EXE = '/usr/bin/sha1sum';


# Directories
$TSKDIR = '/usr/bin/';
$NSRLDB = '';
$LOCKDIR = '/tmp';
