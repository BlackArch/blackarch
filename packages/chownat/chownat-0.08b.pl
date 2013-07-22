#!/usr/bin/perl
#
# Copyright (c) 2004-2005 Samy Kamkar
# Portions Copyright (c) 2005 Matthew Balman
#
# chownat, pronounced "chone nat", v0.08a-beta
# developed 08/16/04
#
# chownat allows two peers behind two seperate NATs
# with NO port forwarding and NO DMZ to communicate
# with each other. more importantly, it opens up a
# tunnel between the two machines so one peer can
# access a service, such as a web server, on the other
# machine which is also behind a NAT. there is NO
# middle man, NO proxy, NO 3rd party, and the
# application runs as an unprivileged user on both
# ends.
#
# example usage:
# nat1 w/ssh server$	./chownat.pl -d -s 22 nat2.com
# nat2$			./chownat.pl -d -c 1234 nat1.com
#
# nat2 runs `ssh -p 1234 username@localhost` to ssh as 'username' to
# machine nat1 and gets right through any NATs on either or both sides
#
# 
#######################################################################



# some modules the system might not have but we need
my @MODULES = qw/
	Digest::MD4
	Time::HiRes
/;
use Digest::MD4;
use Time::HiRes;

# check we have the appropriate modules installed and install necessary ones
check_modules(@MODULES);


# check for correct usage
my ($DEBUG, $mode, $localport, $remoteaddr, $remoteport) = &usage();

my $localhost = "127.0.0.1";
my $size = 262140;
$ENV{LANG} = "C";
local $/ = undef; #otherwise, packets sent every \n

use strict;
use CPAN;
use Socket;
use ExtUtils::MakeMaker qw(prompt);
use IO::Select;
#use Time::HiRes qw( usleep ualarm gettimeofday tv_interval );


my $comp = undef;
my $decomp = undef;
my $digest = undef;
my $digest_hex = undef;
my $digest_base64 = undef;
my $digestSize = 0;


#ucomment next 2 lines to use Compression
#use Compress::LZV1;
#$comp = \&compress; $decomp = \&uncompress; 

#uncomment one of the 3 groups below in order to use Digests to assure message integrity

#use Digest::SHA1  qw(sha1 sha1_hex sha1_base64);  #too strong/slow for what we need, also longer hash size
#$digest = \&sha1; $digest_hex = \&sha1_hex; $digest_base64 = \&sha1_base64;
#$digestSize = 20;

#use Digest::Perl::MD4 qw(md4 md4_hex md4_base64); #Slower MD4, but should use if Digest::MD4 is unavailable
#$digest = \&md4; $digest_hex = \&md4_hex; $digest_base64 = \&md4_base64;
#$digestSize = 16;

#use Digest::MD4 qw(md4 md4_hex md4_base64);  #preferred fastest/smallest overhead Hash digest method
$digest = \&Digest::MD4::md4; $digest_hex = \&Digest::MD4::md4_hex; $digest_base64 = \&Digest::MD4::md4_base64;
$digestSize = 16;

&debug("Opening socket on port $remoteport");
socket(CHOWNAT, PF_INET, SOCK_DGRAM, getprotobyname("udp")) or die "socket: $!";
bind(CHOWNAT, sockaddr_in($remoteport, INADDR_ANY)) || die "bind: $!";

$remoteaddr = inet_aton($remoteaddr);
$remoteport = sockaddr_in($remoteport, $remoteaddr);
my $disconnectMe=0;
my $disconnectThem=0;
my @inBuff;
my @outBuff;
my @todo;
my @confirm;
my @missing;
my $expectConfirmed=0;
my $recentlyMissed;
my $compressFlag = 0;
my $digestFlag = 0;
my $totalMissing=0;
my $id = 0;
my $expected=0;
my $lastPacket;
my $sConfirmed;
my $mConfirmed;
my $lastConfirmed=-1;
my $lastMissed=0;
my $closed=0;
my $select;
my $lastWritten=0;
my $packet;
my $command;

if ($mode =~ /x/ && $comp != undef) {
	$compressFlag = 1;
}
if ($mode =~ /h/ && $digest != undef) {
	$digestFlag = 1;
}

# client mode
if ($mode =~ /c/)
{
	# open a port on the machine to allow connections
	&client_bind($localport);

	# we received a connection to the local port
	while (my $ipaddr = accept(SOCK, WAITCLI))
	{
	&debug("Received a connection to the local port");
	$lastPacket = time();
	# establish a "connection" with the remote chownat
	&client_chownat_connect();

	my %connections;
	my $inputlen;
	my @ready;
	my @missing;
	my $preDigest = 0;
	my $retryCounter=0;
	
	$select = IO::Select->new(\*SOCK, \*CHOWNAT);
	
	while (!$closed)
	{
		while (@ready = $select->can_read(5))
		{
			foreach my $fh (@ready) 
			{
			if (fileno($fh) == fileno(SOCK))
			{
				# Read a buffer full of data
				unless (sysread($fh, $_, $size))
				{
					#debug("1Disconnect area");
					if($disconnectMe==0) {
						debug("REMOTE: 1Attempting to disconnect");
						#$disconnectMe=1;
						checkDisconnect(1);
					}
				}
				else
				{
					if($compressFlag) {
						$_ = comp($_);
					}
					if($digestFlag) {
						$_ .= digest($_);
					}
					$outBuff[$id] = $_;
					debug("sent ".$id);
					chowout(chr(9) . chr(($id & 0xFF00) / 0x0100) . chr($id & 0xff) . $_);
					#$lastPacket = time();
					$id++;
				}
			}
			else
			{
				# We got a packet from the remote CHOWNAT
				unless (recv($fh, $packet, $size, 0))
				{
					debug("2Disconnect area");
					if($disconnectMe==0) {
						debug("REMOTE: 2Attempting to disconnect");
						#$disconnectMe = 1;
						checkDisconnect(1);
					}
				}
				#$lastPacket = time();
				my $command = ord($packet);
				my $len = length($packet);
				my $data="";
				my $got = ord(substr($packet,1,1))*0x0100 + ord(substr($packet,2,1));
				#debug("got = $got,command = $command");
				if(length($packet)>3) {
					#debug("command=$command");
					if($digestFlag) {
						$preDigest = substr($packet, $len-$digestSize,$digestSize, "");
					}
					$data = substr($packet, 3, $len-3, "");
					if($digestFlag) {
						my $postDigest = digest($data);
						if($preDigest != $postDigest) {
							debug("Data Corrupt!!!");
						}
					}
					if($compressFlag) {
						$data = decomp $data;
					}
				}
				
				#keepalive packet w/ remote's $expected next packet
				if($command == 0)
				{
					next;
				#	debug("keepalive $got");
				#	if($id!=$got) {
				#		my $i;
				#		for($i=$got;$i<$id;$i++) {
				#			&debug("Remote missed $i, on $id");
				#			send(CHOWNAT, chr(9) . chr(($i & 0xFF00) / 0x0100) . c#hr($i & 0xff) . $outBuff[$i], 0, $remoteport);
				#		}
				#	}
				}
				# remote NAT wants to close the connection
				elsif ($command == 2)
				{
					debug("REMOTE: 3Attempting to disconnect");
					$disconnectThem = 1;
					checkDisconnect();
				}
				# connection opened
				elsif ($command == 3)
				{
					$disconnectMe = 0;
					$disconnectThem = 0;
					next;
				}
				# remote chownat is missing some packets
				elsif ($command == 8)
				{
					&debug("Remote needs $got, on $id");
					chowout( chr(9) . chr(($got & 0xFF00) / 0x0100) . chr($got & 0xff) . $outBuff[$got]);
				}
				# Got data from remote CHOWNAT for our local socket
				elsif ($command == 9)
				{
					#debug("local $data = -->".$data."<--");
					processData($data,$got);
				}
				elsif ($command == 10) #confirmation of delivery
				{
					confirmation($got);
				}
			} # else
			} # foreach fh
			keepalive();
		} # while select
		#if($totalMissing) {
		#	if(time() - $lastMissed > 0) { #ask for missing packets every 1/2 second
		#		requestMissing($lastWritten,65536);
		#	}
		#}
		checkConfirm();
		keepalive();
	} # while not closed
	debug("Connection closed, exiting one loop");
	} # while accept
	debug("exiting");
	exit;
}

# server mode
elsif ($mode =~ /s/)
{
	$select = IO::Select->new(\*CHOWNAT);
	my $command;
	my @ready;
	my $preDigest = 0;
	while (1)
	{
		while(@ready = $select->can_read(5))
		{
		foreach my $fh (@ready)
		{
			if (fileno(SOCK) && fileno $fh == fileno SOCK)
			{
			# send to chownat
			unless (sysread($fh, $_, $size))
			{
				debug("4Disconnect area");
				if($disconnectMe==0) {
					&debug("REMOTE: 4Attempting to disconnect");
					#$disconnectMe = 1;
					checkDisconnect(1);
				}
			}
			else
			{
				if($compressFlag == 1) {
					$_ = comp($_);
				}
				if($digestFlag == 1) {
					$_ .= digest($_);
				}
				$outBuff[$id]=$_;
				debug("sent ".$id);
				send(CHOWNAT, chr(9) . chr(($id & 0xFF00) / 0x0100) . chr($id & 0xFF) . $_, 0, $remoteport);
				#$lastPacket = time();
				$id++;
			}
			}
			# send to client
			else
			{
			unless (recv($fh, $packet, $size, 0))
			{
				debug("5Disconnect area");
				if($disconnectMe==0) {
					&debug("REMOTE: 5Attempting to disconnect");
					#$disconnectMe = 1;
					checkDisconnect(0);
				}
			}
			#$lastPacket = time();
			my $command = ord($packet);
			my $len = length($packet);
			my $len = length($packet);
			my $data="";
			my $got = ord(substr($packet,1,1))*0x0100 + ord(substr($packet,2,1));
			#debug("got = $got,command = $command");
			if($len>3) {
				if($digestFlag==1) {
					$preDigest = substr($packet, $len-$digestSize,$digestSize, "");
				}
				#debug("they = ".substr($command,2,2));
				$data = substr($packet, 3, length($packet)-3, "");
				if($digestFlag==1) {
					my $postDigest = digest($data);
					if($preDigest != $postDigest) {
						debug("Data Corruption!!");
					}
				}
				#debug("data = [$data]");
				if($compressFlag==1) {
					$data = decomp $data;
				}
			}
			# user is trying to connect to me -- new connection
			if($command == 0)
			{
				next;
				#debug("keepalive $got");
				#if($id!=$got) {
				#	my $i;
				#	for($i=$got;$i<$id;$i++) {
				#		&debug("Remote missed $i, on $id");
				#		send(CHOWNAT, chr(9) . chr(($i & 0xFF00) / 0x0100) . chr($i & 0xff) . $outBuff[$i], 0, $remoteport);
				#	}
				#}
			}
			# remote NAT wants to close the connection
			elsif ($command == 1) #eq "01\n")
			{
				$disconnectMe = 0;
				$disconnectThem = 0;
				# send back "you're connected!"
				&debug("REMOTE: 6Attempted to connect to us, initializing connection");
				&server_chownat_connect($remoteport);

				# open a real connection to the local port we are tunneling
				my $paddr = sockaddr_in($localport, inet_aton($localhost));

				# close any SOCK that might already be open
				close(SOCK);
				socket(SOCK, PF_INET, SOCK_STREAM, getprotobyname('tcp')) || die "socket: $!";
				connect(SOCK, $paddr) || die "Can't open socket to $localhost:$localport: $!";
				&debug("Connection to local daemon (port $localport) opened");
				$select->add(\*SOCK);
			}

			# user is disconnecting
			elsif ($command == 2) #eq "02\n")
			{
				&debug("REMOTE: 7Attempting to disconnect");
				$disconnectThem = 1;
				checkDisconnect(1);
			}
			
			# user is handshaking
			elsif ($command == 3) #eq "03\n")
			{
				&debug("REMOTE: handshake", 5);
				send(CHOWNAT, chr(3), 0, $remoteport);
				next;
			}

			# remote chownat is missing some packets
			elsif ($command == 8) #=~ /^08(.)(.)/s)
			{
				&debug("Remote needs $got, on $id");
				send(CHOWNAT, chr(9) . chr(($got & 0xFF00) / 0x0100) . chr($got & 0xff) . $outBuff[$got], 0, $remoteport);
			}
			
			# Got data from remote CHOWNAT for our local socket
			elsif ($command == 9) #=~ /^09(.)(.)/s)
			{
				processData($data,$got);
				#&debug("Got $got, wanted $expected", ($got == $expected ? 4 : 1));
			}
			elsif ($command == 10) #confirmation of delivery
			{
				confirmation($got);
			}
			} #else
		} #foreach fh
		} #while select
		#if($totalMissing) {
		#	#debug("checking missing totalMissing = $totalMissing");
		#	if(time() - $lastMissed > 0) { #ask for missing packets every second
		#		requestMissing($lastWritten,65536);
		#	}
		#}
		checkConfirm();
		#Send keep-alive
		keepalive();
	} #while not closed
}
else
{
	die "Invalid mode.\n";
}

sub chowout
{
	send(CHOWNAT, $_[0], 0, $remoteport);
	$lastPacket = time();
}

sub checkConfirm
{
	if($id>$expectConfirmed) {
		my $mill;
		(undef,$mill) = Time::HiRes::gettimeofday();
		if($mill-$mConfirmed > 449) {
			my $i;
			for($i=$expectConfirmed;$i<$id;$i++) {
				if($confirm[$i]==0) {
					debug("resending(c) $i");
					chowout(chr(9) . chr(($i & 0xFF00) / 0x0100) . chr($i & 0xff) . 	$outBuff[$i]);
				}
			}
			(undef,$mConfirmed) = Time::HiRes::gettimeofday();
		}
	}
}

sub confirmation
{
	#debug("local $data = -->".$data."<--");
	my $got = $_[0];
	if($confirm[$got]==0) {
	$confirm[$got]=1;
	debug("Confirmed $got");
	$lastConfirmed = $got;
	if($got>$expectConfirmed) {
		my $i;
		for($i=$expectConfirmed;$i<$got;$i++) {
			if($confirm[$i]==0) {
				&debug("resending(c1) $i");
				&chowout(chr(9) . chr(($i & 0xFF00) / 0x0100) . chr($i & 0xff) . $outBuff[$i]);
			}
		}
	}
	elsif($got==$expectConfirmed) {
		while($confirm[$expectConfirmed]==1) {
			if(++$expectConfirmed==65536) {
				$expectConfirmed = 0;
			}
		}
		(undef,$mConfirmed) = Time::HiRes::gettimeofday();
		
	}
	}
}

sub keepalive
{
	if(time() - $lastPacket > 4) {	#send every second
		&chowout(chr(0) . chr(($expected & 0xFF00) / 0x0100) . chr($expected & 0xff));
		&debug("Sent keepalive");
	}
}

sub checkDisconnect
{
	if($_[0]==1) {
		$select->remove(\*SOCK);
		close SOCK;
		$lastWritten = 0;
	}
	
	if(!$closed) {
	if($totalMissing==0 && $disconnectMe==0) {
		&chownat_disconnect($remoteport);
		$disconnectMe = 1;
		$lastWritten = 0;
	}
				
	if($disconnectThem == 1 && $closed == 0) {
		if($totalMissing==0) {
			$id = 0;
			$expected = 0;
			@outBuff = ();
			@inBuff = ();
			@todo = ();
			@confirm = ();
			
			$select->remove(\*SOCK);
			close SOCK;
			$lastWritten = 0;
			$closed = 1;
			&debug("Disconnected");
		}
		else {
			debug("Can't disconnect, data missing");
		}
	}
	}
}

sub requestMissing #if some of these packets don't make it ask for them again
{
	#&debug("requestMissing");
	my $i;
	my $found=0;
	for($i=$_[0];$found<$totalMissing && $i<$_[1];$i++) {
		if($missing[$i]==1) {
			&chowout(chr(8) . chr(($i & 0xFF00) / 0x0100) . chr($i & 0xFF));
			&debug("asking resend2 $i");
			$found++;
		}
	}
	$lastMissed = time();
}

sub removeMissing
{
	$todo[$_[0]]=1;
	if($missing[$_[0]]==1) {
		$missing[$_[0]]=0;
		$totalMissing--;
		&debug("\tGot missing $_[0]");
		$lastMissed = time();
		$recentlyMissed = $_[0];
		return 1;
	}
	return 0;
}

sub checkMissing
{
	#&debug("checkMissing $_[0] and $_[1]");
	my $i;
	for($i=$_[0];$i<$_[1];$i++) {
		if($todo[$i]==0 && $missing[$i]==0) {
			#send(CHOWNAT, chr(8) . chr(($i & 0xFF00) / 0x0100) . chr($i & 0xFF), 0, $remoteport);
			&debug("asking resend $i");
			$missing[$i]=1;
			$totalMissing++;
			if($totalMissing == 1) {
				$lastMissed = time();
			}
			
		}
	}
}

sub processData
{
	my $data = $_[0];
	my $got = $_[1];
	my $mesg = "Got $got";
	my $pkt = chr(10) . chr(($got & 0xFF00) / 0x0100) . chr($got & 0xFF);
	$inBuff[$got]=$data;
	
	# make sure this is the expected packet
	
	&chowout($pkt);
	if ($got != $expected)
	{
		if ($todo[$got]==1)
		{
			&debug("Got duplicate $got");
		}
		elsif(!&removeMissing($got)) {
			#$todo[$got]=1;
			#&debug("set todo on $got");
			&debug($mesg . ", expected $expected");
			$mesg = "";
			if($got < $expected) {
				&checkMissing($expected,65536);
				&checkMissing(0,$got);
			}
			else {
				&checkMissing($expected,$got);
			}
			
			if($got<65535) {
				$expected = $got+1;
			}
			else {
				$expected = 0;
			}
		}
	}
	else {
		if($expected<65535) {
			$expected++;
		}
		else	{
			$expected = 0;
		}
	}
	$todo[$got]=1;
	#&debug("set todo on $got");
	if($mesg ne "") {
		&debug($mesg);
	}
	
	while($todo[$lastWritten]==1) {
		# send data from remote chownat to our client
		send(SOCK, $inBuff[$lastWritten], 0);
		#$todo[$lastWritten]=0;
		#debug("local out = -->".$buffer2[$lastWritten]."<--");
		$lastWritten = 0 if ++$lastWritten == 65536;
	}
	
	if(($disconnectMe==1 || $disconnectThem==1) && $totalMissing==0) {
		&debug("Got missing packets, Disconnecting");
		&checkDisconnect(1);
	}
	#send confirm packet again, this reduces the odds of this info not getting through
	#increases throughput by ~25% or more in my testing. Overhead is small, so should be ok.
	#we're trained so if we get the same confirmed packet in a row we ignore it
	&chowout($pkt);
}

sub debug
{
	my $msg = shift;
	$msg =~ s/\r?\n//g;

	print "DEBUG: $msg\n" if (shift > 1 ? $DEBUG > 1 : $DEBUG >= 1);
}

sub usage
{
	my $debug = 0;
	if ($ARGV[0] eq "-d")
	{
		$debug++;
		shift(@ARGV);
	}

	if ($ARGV[0] eq "-d")
	{
		$debug++;
		shift(@ARGV);
	}

	if ($ARGV[0] eq "-dd")
	{
		$debug = 2;
		shift(@ARGV);
	}

	$ARGV[3] ||= 2222;

	die << "EOF"
chownat 0.08b-beta
usage: $0 [-d] <-c|-s> <local port> <dest host> [communication port]

	-d debug mode, two -d's for verbose debug mode
	-c client mode, you connect other applications to
	   localhost:local_port and it tunnels to the dest host
	-s server mode, anyone who connects to you gets tunneled
	   to whatever is already running on localhost:local_port

	<local port>	local port to listen on or connect to
			depending on if -c or -s is used
	<dest host>	 destination host to connect to or
			allow connections from
	[comm port]	 port to communicate on, default of 2222


example:
  on machine \"nat1\" running an ssh server behind a nat:
	nat1\$ ./chownat.pl -d -s 22 nat2.com

  on machine \"nat2\" behind another nat:
	nat2\$ ./chownat.pl -d -c 1234 nat1.com

  nat2 can now run `ssh -p 1234 username\@localhost` to ssh as 'username'
  to nat1 and break straight through both NATs / firewalls

EOF
	if	@ARGV != 4 ||
		$ARGV[0] !~ /^-[sc][hx]*$/ ||
		$ARGV[1] !~ /^\d+$/ ||
		$ARGV[2] =~ /[^a-zA-Z\d.-]/ ||
		$ARGV[3] !~ /^\d+$/;

	return ($debug, @ARGV);
}

# server side -- accepts and establishes a connection with the remote chownat
sub server_chownat_connect
{
	my $data;
	while (1)
	{
		&debug("Connecting..");
		send(CHOWNAT, chr(3), 0, $_[0]); #"03\n"
		eval
		{
			$SIG{ALRM}=sub{die};
			alarm(1);
			recv(CHOWNAT,$data,3,0);
			#debug("[1a = $data]=".($data ne "L\0").",".($data ne "L\0\n").",".($data ne "L\0\0"));
			alarm(0);
		};

		# we're connected
		if (ord($data) == 3)
		{
			&debug("REMOTE: Connection opened to remote end");
			$closed = 0;
			$disconnectMe = 0;
			$disconnectThem = 0;
			last;
		}
	}
	return 1;
}

# client side -- establishes a connection with the remote chownat
sub client_chownat_connect
{
	my $data;

	# open up a connection to the remote side
	&debug("Opening a connection to the remote end");
	while (1)
	{
		&debug("8Attempting to connect..");

		# open the connection
		send(CHOWNAT, chr(1), 0, $remoteport);
		eval
		{
			$SIG{ALRM}=sub{die};
			alarm(1);
			recv(CHOWNAT,$data,$size,0);
			alarm(0);
		};

		# we're connected
		if (ord($data) == 3)
		{
			chowout(chr(3));
			$closed = 0;
			$disconnectMe = 0;
			$disconnectThem = 0;
			&debug("REMOTE: Connection opened to remote end");
			last;
		}
		select(undef, undef, undef, 0.02);
	}

	return 1;
}

# client side -- binds a socket to allow local connections
sub client_bind
{
	my $localport = shift;
	&debug("Binding a new socket to $localport");

	socket(WAITCLI, PF_INET, SOCK_STREAM, getprotobyname('tcp'));
	setsockopt(WAITCLI, SOL_SOCKET, SO_REUSEADDR, pack("l", 1));
	bind(WAITCLI, sockaddr_in($localport, INADDR_ANY)) || die "Cannot bind to $localport: $!\n"; 
	listen(WAITCLI, 1);
}


# disconnect from remote chownat
sub chownat_disconnect
{
	my $data;
	&debug("9Attempting to disconnect");

	# let the remote NAT know we're disconnecting
	&debug("Trying to disconnect..");
	send(CHOWNAT, chr(2), 0, $_[0]);
}



# check for required modules for this to work
sub check_modules
{
        my @check = @_;
        my @notfound;

        foreach my $module (@check)
        {
                my $string;
                $string = substr("$module ............................", 0, 30);

                my $version = eval "use $module; \$${module}::VERSION";
                if ($version)
                {
                        $string .= "found version $version\n";
			print STDERR $string;
                }
                else
                {
                        $string .= "** FAILED **\n";
			print STDERR $string;

                        push(@notfound, $module);
                }   
        }                

	print "\n";

	if (@notfound)
	{
		print "some required modules are missing\nshall i install them [y/n]: ";
		chomp(my $answer = <STDIN>);

		if ($answer !~ /^y/i)
		{
			die "\nmust install modules before continuing. have a nice day.\n";
		}

		print "\n";
	}

        foreach my $module (@notfound)
        {
                CPAN::Shell->install($module);
                print "\n";
        }

        # load all modules after we know we have them all installed
        foreach (@check)
        {
                eval("use $_");
        }            
}
