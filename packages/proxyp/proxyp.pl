#!/usr/bin/env perl
use v5.14.2;
use LWP::Simple;
use Net::Ping;
use Time::HiRes;
use Getopt::Std;
use threads;
use threads::shared;
use Thread::Queue;
use IP::Country::Fast;
use Socket qw(:DEFAULT :crlf);
use strict;
use warnings;

printf "%s pid: %d as %s\n", $^O, $$, $< ? 'user' : 'root';

sub prox{
	print <<'END';
             __               __	
            / /       /\      \ \
           / /       /  \      \ \
          / <       / /\ \      > \
         / ^ \     / /__\ \    / ^ \
        /_/ \_\   / / __ \ \  /_/ \_\
        \ \__  __/_| |__| |_\__  _/ /
         > __)/ ___) |  | (___ \(__ <
        ( (_ ( (_| _\ \/ /_ |_) ) _) )
         > _) \__ (___||___) __/ (_ <
        ( (__   _) )       ( (_  __) )
         \__ \ (__|         |__)/ __/
            ) )                ( (
          (_/A_r_c_h_S_e_p_t_e_m_\_)

"No man is free who is not master of himself" --Epictetus

Proxyp: Multithreaded Proxy Enumeration Utility
OpenSource (GNU GPLv3) 2013 ArchSeptem "SevenWolf"
Usage: proxyp [Option(s)] {target specification}
OPTIONS:
  [-h]: help menu
  [-v]: verbosity (shows live with -e)
  [-x]: verbosity (shows live, dead, & threads with -e)
  [-t] <threads>: concurrency (for use with -e)
ENUMERATE:
  [-e]: enumerate/test live addresses
  [-c] <country>: exclusive enumeration by country (US, RU, CN, BR, etc.)
  [-d] <sec>: response duration ex. -d 0.1 or -d 10 (default 5)
  [-f] <file>: crawl file for ip's
  [-i] <website>: crawl website for ip's
  [-z]: server, port, & geolocation enum (time consuming, for use with -e)
  [-b]: supress banner/server enum (use with -z, saves time)
  [-p] <port>: @pool = (<port>, 80, 3128, 8080, 1080)
OUTPUT:
  [-o], [-a] <file>: output/truncate or append results w/o stats
EXAMPLES:
  proxyp -f /etc/proxychains.conf -exd 0.1 -t 10
  proxyp -i http://spys.ru/en/ -ezxt 20 -p 81 -a /etc/proxychains.conf
END
}

prox, exit if scalar @ARGV < 1;

my %live :shared;
my %opts;
my $addr;
my $prxy = Thread::Queue->new();
my $reg = IP::Country::Fast->new();
my %prot :shared;
my %serv :shared;
my @pool = (3128, 80, 8080, 1080);
my $cont = 'N/A';

getopts('hvxt:ec:d:i:f:zbp:o:a:', \%opts);
prox, exit if $opts{h};
die prox, "\n[-] please use -i or -f to designate address or path to file!\n" unless $opts{i} || $opts{f};
die prox, "\n[-] Choose either a website or a file!\n" if $opts{i} && $opts{f};
die prox, "\n[-] Choose either output or append!\n" if $opts{o} && $opts{a};

unshift @pool, $opts{p} if $opts{p};

sub these{$live{$a} <=> $live{$b} or $a cmp $b}

unless($opts{f}){
	my $pider = get($opts{i}) or die $!;

	foreach(split /\s+/, $pider){
		$prxy->enqueue($1) if /(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})/;
	}
}else{
	open FILE, "<$opts{f}";
	while(<FILE>){
		foreach(split /\s+/){
			$prxy->enqueue($1) if /(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})/;
		}
	}
	close FILE;
}
$opts{i} ? print "\n[*] Crawling $opts{i}...\n\n" : print "\n[*] Crawling $opts{f}...\n\n";

for(my $i=0; $i < $prxy->pending(); ++$i){
	printf "%s\n", $prxy->peek($i);
}
die prox, "\n[-] Output is designed to be used in conjunction with -e to utilize live hosts" if ($opts{o} || $opts{a}) && !$opts{e};

if($opts{e}){
	$^O eq 'linux' ? system('clear') : system('cls');
	
	$opts{i} ? print "\n[*] Enumerating Proxies from $opts{i}...\n\n" : print "[*] Enumerating Proxies from $opts{f}...\n\n";
	
	my $dur = 0;
	$dur = $opts{d} if $opts{d};
	
	if($opts{t}){
		my @addr;
		push @addr, threads->create(\&enumerate, $dur)
			for 0..$opts{t}-1;
				
		$addr[$_]->join()
			for 0..$opts{t}-1;
	}else{
		&enumerate($dur);
	}
	die "no ip's in html\n" unless %live;
	
	print <<END;
==-=-=-=-=-=-=-=-=-=--
Results of Enumeration
--=-=-=-=-=-=-=-=-=-==

END
	my $i=0;
	foreach(sort these keys %live){
		printf "%d\t%s\t", ++$i, $_;
		printf "%d\t", $prot{$_} if $opts{z} && $prot{$_}; print "*****\t" if $opts{z} && !$prot{$_};
		$opts{z} ? printf "%.10f\t", $live{$_} : printf "%.10f\n", $live{$_};
		$cont = $reg->inet_atocc($_); $cont = 'N/A' unless $cont;
		if($opts{b}){printf "%s\n", $cont if $opts{z}}else{printf "%s\t", $cont if $opts{z};}
		unless($opts{b}){printf "%s\n", $serv{$_} if $opts{z} && $serv{$_}; print "Server: N/A\n" if $opts{z} && !$serv{$_};}
	}
}

if($opts{o} || $opts{a}){
	$opts{a} ? open ELIF, ">>$opts{a}" : open ELIF, ">$opts{o}";
	foreach(sort these keys %live){
		print ELIF "$_\n";
	}
	close ELIF;
}
sub enumerate{
	my $tid = threads->tid() if $opts{t};
	my $n = shift;
	
	while(my $enum = $prxy->dequeue_nb()){
		if($opts{c}){
			unless($reg->inet_atocc($enum) eq $opts{c}){
				next;
			}
		}
		my $p = Net::Ping->new('icmp');
		$p->hires();
		my ($ret, $duration, $ip) = $p->ping("$enum", $n);
		if($ret){
			print "$tid " if $opts{t} && $opts{x};
			print "ENUMERATING: $ip" if $opts{v} || $opts{x};
			if($duration < .2){print "\n*fast*: $duration\n" if $opts{v} || $opts{x}}else{print "\n*live*: $duration\n" if $opts{v} || $opts{x};} print "\n" unless $opts{z};
			if($opts{z}){
				SYN:
				foreach my $portifice(@pool){
					#print "$tid CURRENT PORT: $portifice\n" if $opts{x} && $opts{t};
					my $sS = Net::Ping->new("syn");
					$sS->port_number($portifice);
					$sS->ping("$enum");
					if(my ($host,$rtt,$ip) = $sS->ack){
						print "$tid ACK Recieved from port: $portifice\n" if $opts{x} && $opts{t};
						$prot{$ip} = $portifice;
						unless($opts{b}){
							$|=1;
							socket(BANGRAB, PF_INET, SOCK_STREAM, getprotobyname('tcp'))
								or die "[-] failed attempt to create socket: $!";
							
							setsockopt(BANGRAB, SOL_SOCKET, SO_RCVTIMEO, pack('jj', 10, 0)) or die $!;

							if(connect(BANGRAB, sockaddr_in($portifice, inet_aton($ip)))){
								send(BANGRAB, "HEAD / HTTP/1.1$CRLF$CRLF", 0);
								
								foreach(<BANGRAB>){
									when(/^Server:{1}\s/i){
										if($_ !~ /Apache|Microsoft/i){
											print if $opts{v} || $opts{x};
											chomp($serv{$ip} = $_);
											$prot{$ip} = $portifice;
											last SYN;
										}
									}
								}
							}
						}
						$serv{$ip} = 'Server: SOCKS4/5' if $prot{$ip} == 1080;
						close(BANGRAB) unless $opts{b};
					}
				}
			}
			print "$tid " if $opts{t} && ($opts{z} && $opts{x});
			$cont = $reg->inet_atocc($enum); $cont = 'N/A' unless $cont;
			printf "Geolocated to Country: %s\n\n", $cont if $opts{z} && ($opts{v} || $opts{x});
			$live{$enum} = $duration;
		}else{
			print "$tid " if $opts{t} && $opts{x};
			print "$enum -dead/slow-\n\n" if $opts{x};
		}
		$p->close();
	}
}

