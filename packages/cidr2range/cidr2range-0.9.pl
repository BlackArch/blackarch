#!/usr/bin/perl
# Script for listing the IP addresses contained in a CIDR netblock.
# Originally written to help someone out at work.
# Copyright Rayner Lucas 2005-2010.

use Getopt::Long;
use Net::CIDR qw(:all);
use strict;
use warnings;

my $VERSION = 0.9;

# Options
my $number_opt = '';
my $range_opt = '';
my $list_opt = '';
my $help_opt = '';
my $version_opt = '';

GetOptions( 'number' => \$number_opt,
            'range' => \$range_opt,
            'list' => \$list_opt,
            'help' => \$help_opt,
            'version' => \$version_opt );

# If no options given, assume --range.
if (!$number_opt && !$range_opt && !$list_opt && !$help_opt && !$version_opt) {
    $range_opt = 'true';
}

my $help_text = <<HELPTEXT;
Usage: cidr2range [OPTIONS] [ADDRESS/MASK]
List IP addresses in a CIDR netblock.

Options:
  -l, --list     List individual IP addresses, one address per line
  -n, --number   Print number of IP addresses in the given block
  -r, --range    Print the range of IP addresses in the given block
  -h, --help     Print this help message and exit
  -v, --version  Print version information and exit

If no options are specified, --range is assumed.

Example:
  ./cidr2range -r 192.168.1.2/23

The script contains full documentation, which you may be able to read with
the command "perldoc cidr2range.pl".
HELPTEXT

if ($help_opt) {
    print $help_text;
    exit(0);
}

if ($version_opt) {
    print "cidr2range $VERSION\n";
    print "Copyright R Lucas 2005-2010.\n";
    print "This program is free software. You may copy or redistribute it ";
    print "under \nthe same terms as Perl itself.\n";
    exit(0);
}

# Argument checking

# fail_validation: function to be called when argument validation fails.
#                  Prints a message and exits with a failure status code.
# 
# Arguments:
# message: failure message to print to STDERR.
sub fail_validation {
    my $message = shift;
    print STDERR "$message\n";
    exit(1);
}

my ($addr, $mask);
my ($w, $x, $y, $z);

if (!$ARGV[0]) {
    fail_validation("Invalid address or netmask. Use cidr2range --help for help.");
} else {
    ($addr, $mask) = split(/\//, $ARGV[0]);
    ($w, $x, $y, $z) = split(/\./, $addr);
    foreach my $octet ($w, $x, $y, $z) {
        if ((!$octet) || ($octet !~ /^\d+$/) || ($octet < 0) || ($octet > 255)) {
            fail_validation("Invalid address or netmask. Use cidr2range --help for help.");
        }
    }
}

if (!defined($mask) || $mask !~ /^\d+$/) {
    fail_validation("Invalid address or netmask. Use cidr2range --help for help.");
}

# Check that the address is valid.
if (!cidrvalidate("$w.$x.$y.$z")) {
    fail_validation("Address specified was not valid. Use cidr2range --help for help.");
}

# Check netmask is within the limits of mathematical possibility.
if ($mask < 0 || $mask > 32) {
    fail_validation("Netmask must be a number from 0 to 32. Use cidr2range --help for help.");
}

# Right, lets get to work.
if ($number_opt) {
    my $num = 2**(32 - $mask);
    print "$num address" . ($num != 1 ? "es" : '') . ".\n";
}

my @cidr_list = ("$w.$x.$y.$z/$mask");
my @range_list = cidr2range(@cidr_list);
if ($range_opt) {
    print "Address range: " . $range_list[0] . "\n";
}

if ($list_opt) {
    # Find the start address in the netblock and store it in $w, $x, $y, $z.
    my @addresses = split(/\-/, $range_list[0]);
    my @octets = split(/\./, $addresses[0]);
    $w = $octets[0];
    $x = $octets[1];
    $y = $octets[2];
    $z = $octets[3];

    # Count up through the addresses and print each one.
    for (my $count = 2**(32 - $mask); $count > 0; $count--) {
        # If the other error checks work properly, this error should
        # never happen.
        if ($w > 255) {
            fail_validation("Invalid address/mask specified: leftmost octet would be greater than 255. Use cidr2range --help for help.");
        }
        print "$w.$x.$y.$z\n";
        $z++;
        if ($z > 255) {
            $y++;
            $z = 0;
        }
        if ($y > 255) {
            $x++;
            $y = 0;
        }
        if ($x > 255) {
            $w++;
            $x = 0;
        }
    }
}

exit(0);



=head1 NAME

cidr2range - convert CIDR notation to a range or list of IP addresses.

=head1 SYNOPSIS

B<cidr2range> I<[OPTIONS]> I<address>B</>I<mask>

=head1 DESCRIPTION

This command converts an address block in CIDR notation (address/netmask)
into a range, list, or number of IP addresses. Options are:

=over 4

=item B<-l, --list>

list all IP addresses in the given CIDR block, one per line.

=item B<-n, --number>

print the number of IP addresses in the given CIDR block.

=item B<-r, --range>

print the range of IP addresses for the given CIDR block.

=item B<-h, --help>

display help text and exit.

=item B<-v, --version>

display version information and exit.

=back

The B<--list>, B<--number>, and B<--range> options can be combined, so 
specifying all three will cause the command to print the number of 
addresses, the range of addresses, and all the individual IP addresses 
for the specified block. If no options are given, B<--range> is assumed.

=head1 EXAMPLES

=over 4

=item cidr2range -l 192.168.1.2/30

Prints the list of IP addresses from 192.168.1.0 to 192.168.1.3, one 
address per line.

=back

=head1 README

Converts CIDR notation (e.g. 192.168.1.1/23) to a list or range of IP 
addresses.

=head1 PREREQUISITES

This script requires the C<Net::CIDR> and C<Getopt::Long> modules. It also 
uses the C<strict> and C<warnings> pragmas.

=head1 COPYRIGHT

Copyright Rayner Lucas, 2005-2010.

This program is free software. You may copy or redistribute it under the
same terms as Perl itself.

=pod OSNAMES

any

=pod SCRIPT CATEGORIES

Networking

=cut

