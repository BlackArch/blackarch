#!/usr/bin/perl
#
# Tool: MorxCrack v1.2
# Author: Simo Ben youssef
# Contact: simo_at_morxploit_dot_com
# Release date: 15 November 2013
# MorXploit Research
# http://www.morxploit.com
#
# Download:
# http://www.morxploit.com/morxcrack
#
# v1.2 Changes:
# Removed PHPass module.
# Added update option.
# Added uppercase hash support.
#
# Description:
# Tool written in perl to perform a dictionary-based attack on various hashing algorithm and CMS salted-passwords.
#
# For more info visit MorXCrack page:
# http://www.morxploit.com/morxcrack
#
# Author discolaimer:
# This code and all information contained in this entire document is for educational, demonstration and testing purposes only.
# I cannot be held responsible for any malicious use. Use at your own risk.
# You can redistribute it and/or modify it under the same terms as Perl itself.
#
# For questions, bug reports and suggestions please feel free to email me at simo_at_morxploit_dot_com
# Thanks and happy MorXCracking!

use strict;
use Digest::SHA;
use Digest::MD5;
use IO::Socket;

my $version = "1.2";
my $updateserver = "www.morxploit.com";
my $latestversion = "morxtools/morxcrack";
my $updatefile = "latest.txt";

if (defined ($ARGV[0]) && $ARGV[0] eq "update") {
&update();
exit;
}

if(!defined ($ARGV[0]&& $ARGV[1]&& $ARGV[2])) {
usage();
}

sub usage{
banner();

print "[-] An algorithm, a hash and a wordlist are required\n\n";
print "Usage: perl $0 <algorithm> <hash> <wordlist> <salt>\n\n";
print "Example:\n";
print "perl $0 sha 'hash' wordlist\n";
print "perl $0 shatwice 'hash' wordlist\n";
print "perl $0 sha256 'hash' wordlist\n";
print "perl $0 sha512 'hash' wordlist\n";
print "perl $0 md5 'hash' wordlist\n";
print "perl $0 md5twice 'hash' wordlist\n";
print "perl $0 mysql 'hash' wordlist\n";
print "perl $0 crypt 'hash' wordlist\n\n";

print "For CMS salt-hashed passwords:\n";
print "perl $0 joomla hash wordlist salt\n";
print "perl $0 vb hash wordlist salt\n";
print "perl $0 wordpress 'hash' wordlist\n";
print "perl $0 ipb 'hash' wordlist salt\n";
print "perl $0 md5salt 'hash' wordlist salt\n";
print "perl $0 saltmd5 'hash' wordlist salt\n";
print "perl $0 saltsha 'hash' wordlist salt\n";
print "perl $0 shasalt 'hash' wordlist salt\n\n";

print "To update:\n";
print "perl $0 update\n\n";

exit; }

sub saltusage{
banner();
print "[-] You need to specifiy a salt\n\n";
print "Usage: perl $0 <algorithm> <hash> <wordlist> <salt>\n";
print "perl $0 joomla hash wordlist salt\n";
print "perl $0 vb hash wordlist salt\n";
print "perl $0 ipb hash wordlist salt\n";
print "perl $0 saltmd5 'hash' wordlist salt\n";
print "perl $0 md5salt 'hash' wordlist salt\n";
print "perl $0 saltsha 'hash' wordlist salt\n";
print "perl $0 shasalt 'hash' wordlist salt\n\n";
exit; }

my $algo = $ARGV[0];
my $hash = $ARGV[1];
my $wordlist = $ARGV[2];
my $salt = $ARGV[3];
my $algoname = $algo;
my $subalgo = \&$algoname;
my $password;
my $digest;
my $timestart;
$SIG{INT} = \&sigquit;
$SIG{QUIT} = \&sigstats;
my ($itoa64) = join('', my @itoa64 = ('.','/','0'..'9','A'..'Z','a'..'z'));
my ($wpsalt, $wphash)=$hash=~m/^(.{3,12})(.+)/;

if ($hash eq "\U$hash") {
$hash = lc $hash;
}

if (grep { $algo eq $_ && $salt eq ""} qw{joomla vb ibp md5salt saltmd5 shasalt saltsha}) 
{
saltusage();
}

elsif (!grep { $algo eq $_} qw{md5 mysql crypt sha sha256 sha512 joomla md5twice wordpress vb ibp md5salt saltmd5 shatwice shasalt saltsha})
{
usage();
}
else
{
&crack();
}

sub crack{
banner();
open (LIST, $wordlist) || die "\n[-] Can't find/open $wordlist\n";
print "[*] Hashed password set to $hash\n";
print "[*] Algorithm/CMS set to $algo\n"; 
print "[*] Wordlist set to $wordlist\n";
print "[*] Control^\\ to check stats\n";
print "[*] Control^c to exit the program\n\n";
print "[+] Cracking ...\n\n";
$timestart = time();

while ($password = <LIST>) {
chomp ($password);

&$subalgo();
if ($digest eq $hash)
{
my $timeend = time();
my $runtime = $timeend - $timestart;
print "\n############################################################\n";
print "[+] Your password is $password\n";
print "[+] Found at line $. of $wordlist\n";
print "[+] Cracked in $runtime seconds\n";
print "############################################################\n\n";
close(LIST);
exit;
}
}
my $timeend = time();
my $runtime = $timeend - $timestart;
print "\n############################################################\n";
print "[-] Failed: Couldn't crack the password!\n";
print "[*] Processed $. passwords in $runtime seconds\n";
print "############################################################\n\n";
exit;
}

# Simple SHA hashing
sub sha{
my $sha = Digest::SHA->new;
$sha->add($password);
$digest = $sha->hexdigest;
}

# SHA hashed twice
sub shatwice{
my $sha = Digest::SHA->new;
$sha->add($password);
my $digest1 = $sha->hexdigest;
$sha->add($digest1);
$digest = $sha->hexdigest;
}

# Salt and password (SaltPassword) combined together and hashed to SHA
sub saltsha {
my $key = $salt.$password;
my $sha = Digest::SHA->new;
$sha->add($key);
$digest = $sha->hexdigest;
}

# Password and salt (PasswordSalt) combined together and hashed to SHA
sub shasalt {
my $key = $password.$salt;
my $sha = Digest::SHA->new;
$sha->add($key);
$digest = $sha->hexdigest;
}

# SHA2 256 bits
sub sha256{
my $sha = Digest::SHA->new(256);
$sha->add($password);
$digest = $sha->hexdigest;
}

# SHA2 512 bits
sub sha512{
my $sha = Digest::SHA->new(512);
$sha->add($password);
$digest = $sha->hexdigest;
}

# MySQL 4.1+ hashes the password to SHA then re-hashes the returned binary digest to SHA
sub mysql{
my $sha1 = Digest::SHA->new;
$sha1->add($password);
my $digest1 = $sha1->digest;
$sha1->add($digest1);
$digest = $sha1->hexdigest;
}

# UNIX Crypt (Shadow file)
sub crypt{
$digest = crypt($password, $hash);
}

# Simple md5
sub md5{
my $md5 = Digest::MD5->new;
$md5->add($password);
$digest = $md5->hexdigest;
}

# MD5 hashed twice
sub md5twice {
my $md5 = Digest::MD5->new;
$md5->add($password);
my $digest1 = $md5->hexdigest;
$md5->add($digest1);
$digest = $md5->hexdigest;
}

# Simple md5, returns binary (for WordPress hashing)
sub md5bin{
my $password = $_[0];
my $md5 = Digest::MD5->new;
$md5->add($password);
my $md5bin = $md5->digest;
}

# Joomla CMS. Password and salt combined together and hashed to MD5
sub joomla{
my $key = $password.$salt;
my $md5 = Digest::MD5->new;
$md5->add($key);
$digest = $md5->hexdigest;
}

# Salt and password (SaltPassword) combined together and hashed to MD5
sub saltmd5 {
my $key = $salt.$password;
my $md5 = Digest::MD5->new;
$md5->add($key);
$digest = $md5->hexdigest;
}

# Password and salt (PasswordSalt) combined together and hashed to MD5
sub md5salt {
my $key = $password.$salt;
my $md5 = Digest::MD5->new;
$md5->add($key);
$digest = $md5->hexdigest;
}

# vBulletin: password is hashed to MD5 then a salt is added at the end of the hash and hashed to MD5
sub vb {
my $md5 = Digest::MD5->new;
$md5->add($password);
my $digest1 = $md5->hexdigest;
my $key = $digest1.$salt;
$md5->add($key);
$digest = $md5->hexdigest;
}

# InvisionPowerBoard: both password and salt are hashed then combined (HashedSaltHashedPassword) and hashed to MD5
sub ibp {
my $md5 = Digest::MD5->new;
$md5->add($password);
my $digest1 = $md5->hexdigest;
$md5->add($salt);
my $digest2 = $md5->hexdigest;
my $key = $digest2.$digest1;
$md5->add($key);
$digest = $md5->hexdigest;
}

# WordPress hashing
# Based on PHPass (https://core.svn.wordpress.org/tags/3.2.1/wp-includes/class-phpass.php)

sub enc64{
my $input = $_[0];
my @input = split(//,$input);
my $count = $_[1];
my $output = '';
my $i; do { my $value = ord($input[$i++]);
$output .= $itoa64[$value & 0x3f];
if ($i < $count){
$value |= ord($input[$i]) << 8; }
$output .= $itoa64[($value >> 6) & 0x3f];
if ($i++ >= $count){
return $output; }
if ($i < $count)
{
$value |= ord($input[$i]) << 16;
}
$output .= $itoa64[($value >> 12) & 0x3f];
return $output if $i++ >= $count;
$output .= $itoa64[($value >> 18) & 0x3f];
} while ($i < $count);
return $output;
}

sub crackwp{
my $pass = $_[0];
my $setting = $_[1];
my @setting = split(//,$setting);
my $count2 = index($itoa64, $setting[3]);
my $count = 1 << $count2;
my $wpsalt = substr($setting, 4, 8);
$wphash = md5bin($wpsalt . $pass);
do { $wphash = md5bin($wphash . $pass); } while (--$count);
$setting .= &enc64($wphash, 16);
return $setting;
}

sub wordpress{
$digest = crackwp($password,$wpsalt);
}

sub update {
banner();
print "\n[*] checking for latest version ...\n";
my $get = "GET /$latestversion/$updatefile HTTP/1.1";
my $agent = "User-Agent: MorXCrack-Update";
my $accept = "Accept: */*";
my $hostname = "Host: $updateserver";
my $connection = "Connection: close";
my $sock = new IO::Socket::INET ( PeerAddr => "$updateserver",PeerPort => "80",Proto => "tcp"); die "[-] Can't creat socket: $!\n" unless $sock;
my $sock2 = new IO::Socket::INET ( PeerAddr => "$updateserver",PeerPort => "80",Proto => "tcp"); die "[-] Can't creat socket: $!\n" unless $sock2;
print $sock "$get\n";
print $sock "$agent\n";
print $sock "$accept\n";
print $sock "$hostname\n";
print $sock "$connection\n\n";

my $lv = 1;
while(my $result = <$sock>){
$result =~ /MorXCrack.v(.*?).pl/;
$lv = $1;
}
$sock->close();

my $morxcrack;
if ($lv > $version)
{
print "[+] New version is available: MorXCrack v$lv\n";
print "[+] Downloading now ...\n";
$morxcrack = "MorXCrack.v$lv.pl";
print $sock2 "GET /$latestversion/$morxcrack\n";
print $sock2 "$agent\n";
print $sock2 "$accept\n";
print $sock2 "$hostname\n";
print $sock2 "$connection\n\n";
my $buf = "";
while (sysread($sock2, $buf, 8*1024, length($buf))) {
}
$sock2->close();

open (FH, ">$morxcrack") || die "[-] Can't save to $morxcrack\n";
binmode (FH);
print FH $buf;
print "[+] $morxcrack has been successfuly downloaded!\n\n";
close (FH);
}

else {
print "[*] No update is available at this time.\n";
print "[*] You have the latest version: MorXCrack v$lv\n\n";
$sock->close();
exit;
}
}

# Cracking stats once Ctrl^\ is hit
sub sigstats {
my $sigtime = time();
my $cctime = $sigtime - $timestart;
my $speed = $. / $cctime;
print "\n############################################################\n";
print "[*] Current pwd: $password\n";
print "[*] Line number: $.\n";
print "[*] Time elapsed: $cctime\n";
print "[*] Speed: $speed pwd per second\n";
print "############################################################\n\n";
print "[*] Cracking ...\n";
}

# Quits the program once Ctrl^c is hit
sub sigquit {
print "\n############################################################\n";
print "[*] Exited at line $.\n";
print "[*] Aurevoir!\n";
print "############################################################\n\n";
close(LIST);
exit;
}

sub banner {
print "\n#################################################################\n";
print "### MorxCrack v$version Multi-Algorithm/CMS password cracking tool ###\n";
print "### By Simo Ben youssef <simo_at_morxploit_com>               ###\n"; 
print "### www.morxploit.com                                         ###\n";
print "#################################################################\n\n";
}
