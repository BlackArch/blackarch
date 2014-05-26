#!/usr/bin/perl
# 
# ssl_hostname_resolver.pl
# CN (Common Name) grabber on X.509 Certificates over HTTPS
#
# Copyright (c) 2013 by Alejandro Hernandez, IOActive, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# I had this simple idea in my previous pentest where, for 
# some reason, I couldn't resolve some IP addresses to their 
# corresponding hostnames, but their CN fields (SSL certs 
# on HTTPS) had resolvable hostnames. So, I did this simple script !
#
# ----------------------------------------------------------------- 
#
# For some security scanners, such as Nessus and Nikto2,
# it's considered a security vulnerability:
#
# "Standard certificate validation procedures require the subject 
# CN field of a certificate to match the actual name of the entity 
# presenting the certificate. For example, in a certificate presented 
# by https://www.example.com/, the CN should be www.example.com."
# Source:  X.509 Certificate Subject CN Does Not Match the Entity Name
# http://www.rapid7.com/vulndb/lookup/certificate-common-name-mismatch
#
#
# Alejandro Hernandez < alejandro.hernandez [at] ioactive.co.uk >
# http://twitter.com/nitr0usmx
#
# IOActive Labs Research
# http://blog.ioactive.com
# 

use Net::CIDR::Lite;
use NetAddr::IP;
use Net::SSL;

$connection_timeout = 3; # seconds
$SIG{ALRM} = sub { }; # Do nothing when the connection timeout is reached instead of die

print <<HDR;

-=[ ssl_hostname_resolver.pl
-=[ by \@nitr0usmx
-=[
-=[ CN (Common Name) grabber on X.509 Certificates over HTTPS

HDR

sub usage
{
print <<USG;
-=[ Usage: $0 <CIDR block | IP range | single IP> [port to connect (443 default)]
-=[ E.g. CIDR block : $0 192.168.1.0/24
-=[      IP Range   : $0 192.168.1.1-192.168.13.37
-=[      Single IP  : $0 192.168.1.254

USG
}

my $targets = shift or (usage and die "No targets given!\n");
my $port    = shift || 443;

my $cidr_lite = Net::CIDR::Lite->new($targets);
@cidr_list = $cidr_lite->list;

print "-=[ Connecting every host on port: $port\n\n";

print "-------------------------------------------------\n";
print "      IP        =>   CN (Common Name)\n";
print "-------------------------------------------------\n";

for my $cidr( @cidr_list ){
	my $n = NetAddr::IP->new($cidr);

	for my $ip( @{ $n->splitref( $n->bits() ) } ){
		printf("%-15s =>   %s\n", $ip->addr, getCNFromX509($ip->addr));
	}
}

sub getCNFromX509
{
	my $ip = shift;

	eval{
		alarm($connection_timeout); # No. of seconds of connection timeout

		$ssl = Net::SSL->new(
					PeerAddr => $ip,
					PeerPort => $port,
					Timeout => 5 # Never reached, alarm() will be triggered before
					);
	};

	alarm(0);

	if($@){ # some error occurred 
		return "(Couldn't connect to port $port)";
	}

	$ssl->get_peer_certificate->subject_name =~ /CN=(.*)/g;

	return $1;
}
