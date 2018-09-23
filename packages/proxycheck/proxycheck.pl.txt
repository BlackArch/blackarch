#!/usr/bin/perl
#
#  Tool to easily check for the HTTP CONNECT method and get verbose output from a webserver.  Mainly useful to verify false positives of
#  automated web vulnerability assessment tools.  2007 - Tom Van de Wiele (tom@ashrae.be).
#
#  Please use responsibly.
#
use strict;        # an offering to the perl nazi's
use IO::Socket;

my ($targethost, $targetport, $useragent, $rhost, $rport, $found200, $foundmethods);

sub usage() {
        print "HTTP 1.0 \"CONNECT\" method checker -- Tom Van de Wiele (tom\@ashrae.be)\n";
        print "proxycheck.pl <target-to-test> <target-proxyport> [rhost] [rport] [useragent]\n";
}

if ($#ARGV < 1) {
        usage();
        exit(0);
}

$targethost  = $ARGV[0];
$targetport  = $ARGV[1];

print "HTTP 1.0 \"CONNECT\" method checker -- Tom Van de Wiele (tom\@ashrae.be)\n\n";

if ($ARGV[2]) {
        $rhost = $ARGV[2];
}
else {
        print "(-) No rhost specified, going to try www.google.com\n";
        $rhost = "www.google.com";
}


if ($ARGV[3]) {
        $rport = $ARGV[3];
}
else {
        print "(-) No rport specified, gonna use port 80\n";
        $rport = "80";
}


if ($ARGV[4]) {
        $useragent = $ARGV[4];
}
else {
        print "(-) No useragent specified, gonna use: Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.8.1.3) (Debian)\n";
        $useragent = "Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.8.1.3) (Debian)";
}




my $request = "CONNECT $rhost:$rport HTTP/1.0\nUser-agent: $useragent\nHost: $targethost\n\n\n";

# everyone has at least one dirty sock

my $dirty_sock = new IO::Socket::INET(
                   PeerAddr => $targethost,
                   PeerPort => $targetport,
                   Proto    => 'tcp');
$dirty_sock or die "(!) Connection failed: $!";

print $dirty_sock $request;

my @copy;

while (<$dirty_sock>) {

        if (/(HTTP\/1.[0-9] 200.*)/) {
                my $http_response = $1;
                print "(-)\n(+) $targethost accepted the connection\n(-)\n";
                $found200 = 1;
        }

        if (/^Allow:\s+.*CONNECT.*$/) {
                print "(-)\n(+) Found \"ALLOW\" method in HTTP header\n(-)\n";
                $foundmethods = 1;
        }
        push(@copy, $_);

}

if ($found200 == 0) {
        print "(-)\n(+) Server $targethost refused or returned no HTTP 200\n(-)\n";
}
if ($foundmethods == 0) {
        print "(+) Server $targethost did not give out allowed HTTP methods\n(-)\n";
}

print "(+) Response from webserver:\n(-)\n";

foreach my $line (@copy) {
        print "(+) " . $line;
}
print "\n";

exit(0);

#EOF
