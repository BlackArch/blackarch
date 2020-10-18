#!/usr/bin/perl
#---------------------------------------
#
# SSL Cipher Check
#   Writen by Lee 'MadHat' Heath (madhat@unspecific.com)
# http://www.unspecific.com/ssl/
#
# Patches/fixes provided by:
#   markus.theissinger in .de
#
# This script uses the openssl executable to connect to a server and test
# each of the SSL cipher methods supported by the local openssl client.
# By default it checks 443, for HTTPs, but will work on any SSL enabled
# port.  The default output is just a listing of each cipher and TLS1,
# SSLv2 and SSLv3 and SUCCESS or FAIL message.  It automatically creates
# a log called ssl_dump.log (over written with each run) that has the 
# specific output of each openssl call.
#
# Copyright (c) 2009, MadHat (madhat@unspecific.com)
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#   * Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in
#     the documentation and/or other materials provided with the distribution.
#   * Neither the name of MadHat Productions nor the names of its
#     contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#  Note:  BEAST  and CRIME are coming.
#   BEAST takes advantage of a flaw in block ciphers
#        The vuln is in TLS1.0 only and only there when using block 
#         ciphers with CBC.  Basically if the server used TLS1.0 and 
#         supports any cipher other than RC4 it is potentially vulnerable.
#   CRIME takes advantage in how SSL compression is handled in TLS1.0
#
# As of October 2014 SSLv3 is considered weak due to POODLE
#
#
#---------------------------------------
use Data::Dumper;
use Getopt::Std;
    
my $openssl = '/usr/bin/openssl';
my $gnutls = '/usr/bin/gnutls-cli-debug';
my $cafile = 'ca-bundle.crt';
my $DEBUG = 0;

@weak = ('ADH-AES128-SHA', 'ADH-AES256-SHA','ADH-DES-CBC-SHA', 
    'ADH-DES-CBC3-SHA', 'ADH-RC4-MD5', 'EXP-ADH-DES-CBC-SHA', 
    'EXP-ADH-RC4-MD5', 'EDH-RSA-DES-CBC-SHA', 'EXP-EDH-RSA-DES-CBC-SHA', 
    'EDH-DSS-DES-CBC-SHA', 'EXP-EDH-DSS-DES-CBC-SHA', 'DES-CBC-SHA', 
    'EXP-DES-CBC-SHA', 'EXP-RC2-CBC-MD5', 'EXP-RC4-MD5', 'DES-CBC-MD5', 
    'EXP-RC2-CBC-MD5', 'EXP-RC4-MD5', 'NULL-SHA', 'NULL-MD5', 'AECDH-NULL-SHA');
%names = ('OU' => 'Organizational Unit (OU)',
          'O'  => 'Company (O)',
          'L'  => 'City (L)',
          'ST' => 'State (ST)',
          'CN' => 'Common Name (CN)',
          'C'  => 'Country (C)',
          '1.3.6.1.4.1.311.60.2.1.3'  => 'Country',
          '1.3.6.1.4.1.311.60.2.1.2'  => 'State',
          'serialNumber' => 'Serial Number',
          'emailAddress' => 'Contact',
          '2.5.4.9' => 'Address',
          '2.5.4.17' => 'Zip Code',
          '2.5.4.15' => 'Business Category'
    );
###########################################################################

$VERSION = '1.8';

getopts('dgvwas');

if (!$ARGV[0]) {
  print " : SSL Cipher Check: $VERSION\n";
  print " : written by Lee 'MadHat' Heath (at) Unspecific.com\n";
  print " :  - http://www.unspecific.com/ssl/\n";
  print "Usage:\n";
  print "  $0 [ -dgvwas ] <host> [<port>]\n";
  print "default port is 443\n";
  print "-d  Add debug info (show it all, lots of stuff)\n";
  print "-v  Verbose.  Show more info about what is found\n";
  print "-g  GNUTLS.  Use gnutls-cli-debug (if installed) to show more info\n";
  print "-w  Show only weak ciphers enabled.\n";
  print "-a  Show all ciphers, enabled or not\n";
  print "-s  Show only the STRONG ciphers enabled.\n";
  print "\n";
  exit;
} else {
  $host = $ARGV[0];
}
if ($opt_d) {
  $DEBUG = 1;
  $opt_a = 1;
  $opt_w = 0;
  $opt_v = 1;
  $opt_s = 0;
}
my $options = '';
my $disable_v2 = '';
if (-e $cafile) {
  $options = "-CAfile $cafile ";
}
my $start = time;
my $port = $ARGV[1]?$ARGV[1]:443;
open (STDERR, ">ssl_dump.log");
print STDERR localtime() . " START\n";
print localtime() . " START\n" if ($opt_v);
print STDERR "Testing $host:$port\n";
print "Testing $host:$port\n";

if ($opt_v) {
  print " : SSL Cipher Check: $VERSION\n";
  print " :  - http://www.unspecific.com/ssl/\n";
  open (SSL, "$openssl version|") or die "$!\n";
  while (<SSL>) {
    print "Testing with $_";
  }
  close (SSL);
}

open (SSL, "$openssl ciphers 'ALL:eNULL' -v|") or die "$!\n";
while (<SSL>) {
  chomp;
  my @cipher = split(' ');
  my $cipher = $cipher[0];
  if ($cipher[4] eq 'Enc=None') {
    $ciphers{$cipher[0]} = 'None';
  } else {
    $cipher[4] =~ /\((\d+)\)/;
    $ciphers{$cipher[0]} = "$1 bits";
  }
  push @ciphers, $cipher[0];
}
close (SSL);
open (SSL, "$openssl ciphers 'ALL:aNULL' -v|") or die "$!\n";
while (<SSL>) {
  chomp;
  my @cipher = split(' ');
  my $cipher = $cipher[0];
  if ($cipher[4] eq 'Enc=None') {
    $ciphers{$cipher[0]} = 'None';
  } else {
    $cipher[4] =~ /\((\d+)\)/;
    $ciphers{$cipher[0]} = "$1 bits";
  }
  push @ciphers, $cipher[0];
}
close (SSL);
undef %saw;
my @out = grep(!$saw{$_}++, @ciphers);
@ciphers = @out;
my $pci = 0;
my %results;

my $ts = $#ciphers * 3;
print "Running a total of $ts scans\n" if ($opt_v);

print join(", ", @ciphers) . "\n" if ($DEBUG);

if ($opt_v) {
  print "Getting default Cipher/Proto\n" if ($DEBUG);
  print "echo | $openssl s_client $options -connect $host:$port\n" if ($DEBUG);
  open (SSL, "echo | $openssl s_client $options -connect $host:$port 2>&1 |");
  while (<SSL>) {
    chomp;
    print STDERR "`-DEF:$cipher: $_\n";;
    if (/^    Protocol  : (.+)$/) {
      $results{'default_proto'} = $1;
    } elsif (/^    Cipher    :(.+)$/) {
      $results{'default_cipher'} = $1;
    }
  }
  close(SSL);
}

print STDERR "\n";
for my $cipher (sort @ciphers) {
  next if ($cipher =~ /^\s*$/);
  print "Checking $cipher\n" if ($DEBUG);
  print STDERR '-' x 72 . "\n";
  print STDERR localtime() . " $cipher\n";
  my @cert;
  
  # check TLS1.0
  print "Checking TLS1.0 - $cipher\n" if ($DEBUG);
  print "." if ($opt_v and !$DEBUG);
  print "$openssl s_client -tls1 -no_ssl2 -no_ssl3 $options -cipher $cipher -connect $host:$port\n" if ($DEBUG);
  open (SSL1, "echo | $openssl s_client -tls1 -no_ssl2 -no_ssl3 $options -cipher $cipher -connect $host:$port 2>&1 |");
  while (<SSL1>) {
    chomp;
    print STDERR "`-TLS1:$cipher: $_\n";;
    push(@cert, $_);
  }
  close(SSL1);
  # examine what we received
  &check_cert('TLSv1.0', $cipher, @cert);
  print STDERR "\n";

  # clear cert between runs
  undef @cert;
  
  # check TLS1.1
  print "Checking TLS1.1 - $cipher\n" if ($DEBUG);
  print "." if ($opt_v and !$DEBUG);
  print "$openssl s_client -tls1_1 -no_ssl2 -no_ssl3 $options -cipher $cipher -connect $host:$port" if ($DEBUG);
  open (SSL1, "echo | $openssl s_client -tls1_1 -no_ssl2 -no_ssl3 $options -cipher $cipher -connect $host:$port 2>&1 |");
  while (<SSL1>) {
    chomp;
    print STDERR "`-TLS1_1:$cipher: $_\n";;
    push(@cert, $_);
  }
  close(SSL1);
  # examine what we received
  &check_cert('TLSv1.1', $cipher, @cert);
  print STDERR "\n";

  # clear cert between runs
  undef @cert;

  # check TLS1.2
  print "Checking TLS1.2 - $cipher\n" if ($DEBUG);
  print "." if ($opt_v and !$DEBUG);
  print "$openssl s_client -tls1_2 -no_ssl2 -no_ssl3 $options -cipher $cipher -connect $host:$port" if ($DEBUG);
  open (SSL1, "echo | $openssl s_client -tls1_2 -no_ssl2 -no_ssl3 $options -cipher $cipher -connect $host:$port 2>&1 |");
  while (<SSL1>) {
    chomp;
    print STDERR "`-TLS1_2:$cipher: $_\n";;
    push(@cert, $_);
  }
  close(SSL1);
  # examine what we received
  &check_cert('TLSv1.2', $cipher, @cert);
  print STDERR "\n";

  # clear cert between runs
  undef @cert;

  if (!$disable_v2) {
    # check SSLv2
    print "Checking SSLv2 - $cipher\n" if ($DEBUG);
    print "." if ($opt_v and !$DEBUG);
    print "$openssl s_client -ssl2 -no_tls1 -no_ssl3 $options -cipher $cipher -connect $host:$port\n" if ($DEBUG);
    open (SSL2, "echo | $openssl s_client -ssl2 -no_tls1 -no_ssl3 $options -cipher $cipher -connect $host:$port 2>&1 |");
    while (<SSL2>) {
      chomp;
      print STDERR "`-SSLv2:$cipher: $_\n";; 
      push(@cert, $_);
    }
    close(SSL2);
    # examine what we received
    &check_cert('SSLv2', $cipher, @cert);
    print STDERR "\n";

    # clear cert between runs
    undef @cert;
  }
  
  # check SSLv3
  print "Checking SSLv3 - $cipher\n" if ($DEBUG);
  print "." if ($opt_v and !$DEBUG);
  print "$openssl s_client -ssl3 -no_ssl2 -no_tls1 $options -cipher $cipher -connect $host:$port\n" if ($DEBUG);
  open (SSL3, "echo | $openssl s_client -ssl3 -no_ssl2 -no_tls1 $options -cipher $cipher -connect $host:$port 2>&1 |");
  while (<SSL3>) {
    chomp;
    print STDERR "`-SSLv3:$cipher: $_\n";;
    push(@cert, $_);
  }
  close(SSL3);
  # examine what we received
  &check_cert('SSLv3', $cipher, @cert);
}
print Dumper( %results ) if ($DEBUG);

print "\n\n" if ($opt_v);

# Process %results
for my $proto (keys %results) {
  next if ($proto ne 'SSLv3' 
        and $proto ne 'SSLv2'
        and $proto ne 'TLSv1.0'
        and $proto ne 'TLSv1.1'
        and $proto ne 'TLSv1.2');
  for my $cipher (keys %{$results{$proto}}) {
    if ($results{$proto}{$cipher}{'enabled'} eq 'TRUE') {
      if ((grep(/^$cipher$/, @weak) 
           or $proto eq 'SSLv3'
           or $proto eq 'SSLv2')) {
          if (!$opt_s) {
            print "** $proto:$cipher - ENABLED - WEAK $ciphers{$cipher} **\n";
          }
      } elsif (!$opt_w) {
        print "   $proto:$cipher - ENABLED - STRONG $ciphers{$cipher} \n";
      }
    } elsif ($opt_a) {
      if (grep(/^$cipher$/, @weak) or $proto eq 'SSLv2') {
        print "   $proto:$cipher - DISABLED - WEAK $ciphers{$cipher}  *";
        if ($opt_v) {
          print $results{$proto}{$cipher}{'err'};
        }
        print "\n";
      } else {
        print "   $proto:$cipher - DISABLED - STRONG $ciphers{$cipher}  *";
        if ($opt_v) {
          print $results{$proto}{$cipher}{'err'};
        }
        print "\n";      
      }
    }
  }
  if ($opt_v) {
    for my $err (keys %{$results{$proto}{'error'}}) {
      print "  Error $err: " . $results{$proto}{'error'}{$err} . "\n";
    }
  }
  print "\n";
}

if ($opt_v) {
  print "Default (for openssl client):\n";
  print "  $results{'default_proto'} $results{'default_cipher'}\n";
  print "\n";
  print "Certificate Details:\n";
  if ($results{'key_size'} < 2048) {
    print "*WARNING* Weak";
  }
  print "  Key Size: " . $results{'key_size'} . "bits\n";
  print "  Issuer: \n";
  for my $entry (keys %{$results{'issuer'}}) {
    print "\t" . $names{$entry} . " : " . $results{'issuer'}{$entry} . "\n";
  }          
  print "  Subject: \n";
  for my $entry (keys %{$results{'subject'}}) {
    print "\t" . $names{$entry} . " : " . $results{'subject'}{$entry} . "\n";
  }
  print "\n";
  if ($opt_g) {
    if (-e $gnutls) { 
      open (TLS, "$gnutls -p $port $host |") or print "ERROR: $gnutls $!\n";
      while (<TLS>) {
        print STDERR "`-GNUTLS-$_";
        if ( /^Checking/ and 
           ( $_ !~ /for certificate informaiton/ or
             $_ !~ /RSA\-export ciphersuite info/ or
             $_ !~ /ephemeral Diffie Hellman group info/ or
             $_ !~ /for trusted CAs/ )
         ) {
          print $_;
        }
      }
      close(TLS);
    } else {
      print "ERROR: Unable to find $gnutls.\n";
      print "  Please install the gnutls-devel/gnutls-bin (Debian based)\n";
      print "    or gnutls-utils (RedHat based) package\n";
    }
  }
  print "\n";
}
if ($results{'expired'}) {
  print "WARNING: Expired Certificate - " . $results{'expired'} . "\n";
}
if ($results{'self_signed'}) {
  print "WARNING: Self Signed Certificate\n";
}

# show some stats
if ($results{'weak'} > 0) {
  print "*WARNING* " . $results{'weak'} . " WEAK Ciphers Enabled.\n";
}
if ($results{'poodle'} > 0) {
  print "*WARNING* " . $results{'poodle'} . " Ciphers Enabled Vulnerable to POODLE.\n";
}
print "Total Ciphers Enabled: " . $results{'total'} . "\n";
my $time = time - $start;
print "Scan took $time secs to finish\n" if ($opt_v);
print STDERR localtime() . " FINISHED\n";
print localtime() . " FINISHED\n" if ($opt_v);

# get the F out of here
close (STDERR);
exit;

sub check_cert {
  my ($proto, $cipher, @cert) = @_;
  print "Verifying $proto - $cipher results\n" if ($DEBUG);
  for my $line (@cert) {
    print "  $line\n" if ($DEBUG);
    my $verify;
    # Deal with errors
    if ($line =~ /^unknown option \-ssl2$/) {
      $disable_v2 = TRUE;
      print "SSLv2 disabled in Client, disabling SSLv2 check\n" if ($DEBUG);
    } elsif ($line =~ /^\d+:error:/) {
      my ($pid, $err, $unk, $routine, $func, 
        $msg, $file, $line, $err_msg) = split(':', $line);
      if ($func eq 'SSL3_READ_BYTES') {
        if ($msg eq 'sslv3 alert handshake failure') {
          $results{$proto}{$cipher}{'enabled'} = 'FALSE';
          $results{$proto}{$cipher}{'err'} = 'handshake failure';
        }
      } elsif ($func eq 'SSL3_WRITE_BYTES') {
        if ($msg eq 'ssl handshake failure') {
          $results{$proto}{$cipher}{'enabled'} = 'FALSE';
          $results{$proto}{$cipher}{'err'} = 'handshake failure';
        }
      } elsif ($func eq 'CLIENT_HELLO') {
        if ($msg eq 'no ciphers available') {
          $results{$proto}{$cipher}{'enabled'} = 'FALSE';
          $results{$proto}{$cipher}{'err'} = 'no ciphers available';
        }
      } elsif ($func eq 'GET_SERVER_HELLO') {
        if ($msg eq 'no cipher list') {
          $results{$proto}{$cipher}{'enabled'} = 'FALSE';
          $results{$proto}{$cipher}{'err'} = 'no cipher list';
        }
      }
    } elsif ($line =~ /^notAfter=(.+)$/ and !$results{'expired'}) {
      $results{'expired'} = $1;        
    } elsif ($line =~ /^New, .*, Cipher is (.+)$/ and $cipher ne $1) {
      $results{$proto}{$cipher}{'enabled'} = 'FALSE';
      $results{$proto}{$cipher}{'err'} = 'cipher changed';
    } elsif ($line =~ /^(\s*)[Vv]erify return( code)?:/
        and !$results{$proto}{$cipher}{'enabled'}) {
      $results{$proto}{$cipher}{'enabled'} = 'TRUE';
      $results{'total'}++;
      if (grep(/^$cipher$/, @weak) 
          or $proto eq 'SSLv3'
          or $proto eq 'SSLv2') {
        $results{'weak'}++;
        if ($proto eq 'SSLv3') {
          $results{'poodle'}++;
        }
      }
    } elsif ($results{$proto}{$cipher}{'enabled'} == 'TRUE' 
        and $line =~ /^subject=(.+)$/ 
        and !$results{'subject'} ) {
      # Subject of Cert
      my $subject = $1;
      for my $entry (split('/', $subject)) {
        next if (!$entry);
        my ($key, $val) = split('=', $entry);
        print "$key => $val\n" if ($DEBUG);
        $results{'subject'}{$key} = $val;
      }
    } elsif ($results{$proto}{$cipher}{'enabled'} == 'TRUE' 
        and $line =~ /^issuer=(.+)$/ 
        and !$results{'issuer'} ) {
      # Issuer
      my $issuer = $1;
      for my $entry (split('/', $issuer)) {
        next if (!$entry);
        my ($key, $val) = split('=', $entry);
        print "$key => $val\n" if ($DEBUG);
        if ($key and $val and !$results{'issuer'}{$key}) {
          $results{'issuer'}{$key} = $val;
        }
      }
    } elsif ($results{$proto}{$cipher}{'enabled'} == 'TRUE' 
        and $line =~ /^New, (.+), Cipher is (.+)$/ ) {
      $results{$proto}{$cipher}{'real_proto'} = $1;
      $results{$proto}{$cipher}{'real_cipher'} = $2;
    } elsif ($results{$proto}{$cipher}{'enabled'} == 'TRUE' 
        and $line =~ /^Server public key is (\d+) bit$/ ) {
      $results{'key_size'} = $1;
    } elsif ($results{$proto}{$cipher}{'enabled'} == 'TRUE' 
        and $line =~ /^    Verify return code: (\d+ .+)$/) {
      $results{$proto}{$cipher}{'return_code'} = $1;
    } elsif ($results{$proto}{$cipher}{'enabled'} == 'TRUE' 
        and $line =~ /^verify error:num=(\d+):(.+)$/) {
      my $num = $1;
      my $err = $2;
      $results{$proto}{'error'}{$num} = $err;
      if ($num == 18) {
        $results{'self_signed'} = 'TRUE';
      }
    }
  }
}
