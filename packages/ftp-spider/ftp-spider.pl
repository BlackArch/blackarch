#!/usr/bin/perl

# code by nithen naidoo  ;)

# -----------------------------------------
# FTP SPIDER
# -----------------------------------------
#  FTP investigation tool
#  Scans ftp server for the following:
#  * reveal entire directory tree structure
#  * detect anonymous access
#  * detect directories with write permissions
#  * find user specified data within repository
# -----------------------------------------

#--------------
# notes
#--------------
# hasn't been tested on windoze ftp
#

# USES
use strict;     	# 4 errors
use warnings;   	# 4 errors
use Data::Dumper;	# 4 output
use Net::FTP;   	# 4 FTP file traversal
use File::Find;   	# 4 system file traversal (coming to a pc near you)
use Pod::Usage;   	# 4 documentation
use Getopt::Std;    	# 4 commandline stuffz

# VARIABLES
use vars qw($opt_s $opt_k $opt_u $opt_p $opt_h $opt_v
      $opt_d $opt_P $opt_i $opt_r $opt_w $opt_F $opt_t @filter);

# GET OPTIONS
getopts('i:s:u:p:r:F:whkvdtP');

# HELP OPTIONS
if ($opt_h)
{
    pod2usage({-exitval => 2,
         -verbose => 2});
}

# Defaults are set here
$opt_s ||= 'localhost';
$opt_u ||= 'anonymous';
$opt_p ||= 'someuser@';
$opt_r ||= '/';

$opt_i = qr/$opt_i/ if $opt_i;    # Whateva Trevor!!!

$|++;       # Autoflush STDIN

# HASHES
my %rem = (); # Store tree of remote system


# -------------------------------------------------------------------------------------
        # Phase 1: Build a representation of what's
        # in the remote site
# ------------------------------------------------------------------------------------

# INTRO
print "\n\n";
print "   //\\   ^^   //\\       FTP\n";
print "  //  \\ (OO) //  \\     SPIDER\n";
print " //    { VV }     \\  VERSION 1.0\n"; 
# CONNECT TO FTP
print "\n\nConnect to remote FTP server...\n";

my $ftp = new Net::FTP ($opt_s, 
      Debug   => $opt_d, 
      Passive   => $opt_P,
      );

die "Failed to connect to server '$opt_s': $!\n" unless $ftp;     #fuxed
die "Failed to login as $opt_u\n" unless $ftp->login($opt_u, $opt_p); #fuxed
die "Cannot change directory to $opt_r\n" unless $ftp->cwd($opt_r); #fuxed
warn "Failed to set binary mode\n" unless $ftp->binary();       #fuxed,... well allmost :?

print "connected\n" if $opt_v;              # shaweeet :)

# SCAN THE FILE SYSTEM
print "Scanning remote FTP server...\n\n";

# PRINT OUTPUT OPTIONS
print "Verbose Output:\n"if $opt_v;
print "Directory tree:\n"if $opt_t;
print "Filter Matched:\n"if $opt_F;
print "Writable Directories:\n"if $opt_w;

# OPEN FILTER FILE
if ($opt_F){
	# open file for reading
	open(DATA,"<$opt_F") || die "filter file error";

	# enter into array
	@filter	= <DATA>;
	chomp @filter;
	
	# close the file
	close(DATA);
}

# recursive sub
sub scan_ftp
{
    my $ftp = shift;
    my $path  = shift;
    my $rrem  = shift;

    # check for content 
    my $rdir = length($path) ? $ftp->dir($path) : $ftp->dir();
    return unless $rdir and @$rdir;

    # get present working directory
    my $pwd = length($path) ? $path : $ftp->pwd();

    # print present working directory - verbose
    print "\n\n$pwd\n"if $opt_v;

    # print present working directory - tree
    print "$pwd\n"if $opt_t;
    
    # check directory name against filter
    if ($opt_F){
	foreach my $term (@filter){
		if (index(lc($pwd), $term)!= -1){print "$pwd\n";}
	}
    }

    # got directory listing - now what???
    for my $f (@$rdir)
    {
  	# clean data
	next if $f =~ m/^d.+\s\.\.?$/;
	
	# print if verbose
	print "$f\n"if $opt_v;
	
	# check
	# get file names for traversal
	my $n = (split(/\s+/, $f, 9))[8];
  	
	# go if all is well
	next unless defined $n;

  	# check name
	my $name = '';
  	$name = $path . '/' if $path;
  	$name .= $n;

	# check directory name against filter
    	if ($opt_F){
        	foreach my $term (@filter){
                	if (index(lc($name), $term)!= -1){print "$name\n";}
        	}
    	}

  	# check ignore
	if ($opt_i and $name =~ m/$opt_i/)
  	{
      		print "ftp: IGNORING $name\n" if $opt_d;
      		next;
  	}

  	# go if all is well
	next if exists $rrem->{$name};

  	# get type for traversal
	my $type = substr($f, 0, 1);
	$type =~ s/-/f/;

        # get file permissions
        if ($opt_w){
		my $write = substr($f, 8, 1);
		my $directory = substr($f, 0, 1);
		if ($directory eq "d"){
			if ($write eq "w") {print "$name\n";}
        	}
	}

  	# warning
	warn "ftp: adding $name ($type)\n" if $opt_d;

	# hash type
	$rrem->{$name} =
  	{
      		type => $type,
  	};

	# recursion :)
  	scan_ftp($ftp, $name, $rrem) if $type eq 'd';
    }
}


# KICK IT!!!
scan_ftp($ftp, '', \%rem);

# done
print "\n\nDone!!!\n";

# i'm outtahere 8^D
exit;

# HELP DOCUMENTATION

__END__

=pod

=head1 NAME

ftp-spider - get hierarchy of files and permissions
on a FTP repository, with added filters and brute forcer.

=head1 SYNOPSIS

ftp-spider [-h] [-v] [-d] [-k] [-P] [-w] [-s server] [-u username] [-p password] [-r
remote] [-i ignore] [-f filterfile]

=head1 ARGUMENTS

The recognized flags are described below:

=over 2

=item B<-h>

Produce this documentation.

=item B<-v>

Produce verbose output.(Produces entire tree with full file listings)

=item B<-d>

Put the C<Net::FTP> object in debug mode and also emit some debugging
information about what's being done.

=item B<-k>

Just kidding. For testing

=item B<-P>

Set passive mode.

=item B<-i ignore>

Specifies a regexp. Files matching this regexp will be left alone.

=item B<-s server>

Specify the FTP server to use. Defaults to C<localhost>.

=item B<-u username>

Specify the username. Defaults to 'anonymous'.

=item B<-p password>

Password used for connection. Defaults to an anonymous pseudo-email
address.

=item B<-r remoteDIR>

Specify remote directory to traverse.

=item B<-w writable>

List all writable directories only.

=item B<-t showTree>

List all directory names only.(To include file names use verbose only)

=item B<-F filterfile>

Filter all directory names, file names and extensions.

=back

=head1 DESCRIPTION

This script is for simple FTP server analysis.
It obtains a hierarchy of files /
directories / permissions of an FTP server.

Always use the C<-k> option before using it in production.

=head1 BUGS

Not quite complete writing a GUI version. This script is not tested
on Windoze FTP yet, and the output is a bit untidy :(

=head1 AUTHORS

Nithen Naidoo (nithen.naidoo@mailbox.co.za)

=head1 SEE ALSO

Perl(1).

=cut