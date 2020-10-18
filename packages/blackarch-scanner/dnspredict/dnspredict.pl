#! /usr/bin/perl
# scrape names from google predict and check if dns names exist
# for education purposes only

use LWP::UserAgent; # network stuff
use IO::Socket;
use Socket;
use Net::hostent;
use strict; 		# always use strict!
use Carp::Heavy; 	# needed to compile on windows...
use Getopt::Long;  # command line parsing

my ($version)="0.0.2";

# CHANGELOG
# 
# 0.0.1 Initial Release
# 0.0.2 bug fix & report 

my ($debuglevel)=1;
my ($debuglog)="debug.log";
my ($dnsfile)="dns.txt"; # dns info file
my ($baseurl) = "http://labs.google.com/sets?hl=en"; # the base url 
my ($checktimeout)=40; # timeout for proxies in seconds
my ($maxtries)=3; # how should I try to get the proxy page?
my ($dieonerror)=1; # abort script on download fail
my ($downloadproxy)=""; # the proxy for downloading the proxylist
my ($page) = 1;
my ($domain);
my ($item1);
my ($item2);

my ($googleURL);

# do not edit below here...

my (@dnsentries);
my ($dns);
my (@googleContent);
my ($googleLine);
my (@proxyok);
my ($count)=0;
my ($okcount)=0;
my ($help);


GetOptions( "domain:s"  => \$domain,     # --verbose
				"item1:s" => \$item1,		# --output=string        
            "item2:s" => \$item2,		# --output=string
            "help"    => \$help,       # --Debug
            "loglevel:i" => \$debuglevel,		# --output=string
            "retries:i" => \$maxtries, # --retries=i
            "file:s" => \$dnsfile, # the file to write the working proxies to
            "proxy:s" =>\$downloadproxy, # the proxy to use for downloading
            "dieonerror:i" => \$dieonerror, # if set to 1 (the tool dies on fatal erros) 0 overrides this behaviour
            "timeout:i" => \$checktimeout );# --timeout=integer    

print ("DNS Predictor by Jimmy Neutron ".$version."\n\n");
logMe ("Starting up....",$debuglevel,$debuglog);

	if ((length($domain)<2)||(length($item1)<1)||(length($item2)<1))
	{
		print ("Argument Error \n\n");
		help();
		exit(-1);
	}
	
	if (length($downloadproxy)<1)
	{
		print("Warning: You aren't using a proxy. Remeber that this program is doing a direct g00gle scraping")
	}
	
	# ok we got parameters!
	# we only expect one parameter, so parsing is fairly easy...
	
	my (@paramdata) = split (/=/,$ARGV[0]);
	logMe ("Input: ".$ARGV[0],$debuglevel,$debuglog);
	$paramdata[0] =~ s/--//;
	logMe ("Parameters: ".$paramdata[0]." - ".$paramdata[1],$debuglevel,$debuglog);
	

		$googleURL = buildGoogleURL($baseurl,$item1,$item2);
		logMe ("Setting url to: ".$googleURL,$debuglevel,$debuglog);		
	
	if ($help)
	{
		help();
	}

logMe ("Setting url to: ".$googleURL,$debuglevel,$debuglog);	
@googleContent=getURLasArray ($googleURL,$maxtries,$dieonerror,$downloadproxy);

foreach $googleLine (@googleContent)
{
	if ($googleLine=~ /<a href.+?><center>([^\s]+?)<\/center>/)
	{
		my ($ip);
		print ($1."\t"."Resolving ".$1.".".$domain."\t");
		$ip=getIP($1.".".$domain);
		if (!($ip eq "-"))
		{
			
			push (@dnsentries,$1.".".$domain.";".$ip);
		}
		print ($ip."\n");
	}
}

#create new proxy file...
open (DNSFILE, ">".$dnsfile) or die "Can't open $dnsfile: $!";;
close (DNSFILE);
open (DNSFILE, ">>".$dnsfile) or die "Can't open $dnsfile: $!";;
foreach $dns (@dnsentries)
{	
	print (DNSFILE $dns."\n") ;
}
close(DNSFILE);

exit (0);

###################################
#                                 #
# SUB: getURLasArray($url)        #
#                                 #
# read a webpage from a URL       #
# and store it into an array      #
#                                 #
###################################

sub getURLasArray($)
{
	my ($url) = @_[0];
	my ($maxerror)= @_[1];
	my ($dieOnFail)= @_[2];
	my ($proxy)=@_[3];
	# my ($maxerror)= 3;
	my ($content)="";
	my (@content);
	my ($errorcount)=0;
	my ($seconds)=5;
	# my ($debuglevel)="";
	# my ($debuglog)="";
	my ($response);
	my $ua = LWP::UserAgent->new(env_proxy => 0,
                              keep_alive => 1,
                              timeout => 300,
                              agent => "Mozilla/5.001 (windows; U; NT4.0; en-us)"
                             );
 	if (length($proxy)>0)
 	{
 		$ua->proxy(['http', 'ftp'], $proxy);
 		logMe ("Setting Proxy ".$proxy,$debuglevel,$debuglog);
 	}

	
	logMe ("fetching ".$url."",$debuglevel,$debuglog);
	
	# $content = get($url);
	$response = $ua->get($url);
	$content=$response->content;
	@content=split(/\n/,$content);
	while (! $response->is_success())
	{
		# if we got an error remove all proxies possibly the proxy failed ...
		$ua->proxy(['http', 'ftp'], '');
		$errorcount++;
		warn ("Could not fetch url \n".$url." \n attempt ".$errorcount."\n");
		logMe ("Could not fetch url ".$url." attempt ".$errorcount.".",2,"debug.log");
		logMe ("Could not fetch url ".$url." attempt ".$errorcount.".",2,"debug.log");
		$response = $ua->get($url);
		$content=$response->content;
		@content=split(/\n/,$content);
		
		if ($errorcount==$maxerror) # on maximum error
		{
			
			
			logMe ("Permanent error fetching url: ".$url,$debuglevel,$debuglog);
			logMe ("Permanent error fetching url: ".$url,2,"debug.log");
			if ($dieOnFail==1)
			{
				die "Permanent error fetching url:\n".$url."\n";
			}
			else
			{
				warn ("\a\a\a\a\a Permanent error fetching url:\n".$url."\n");
				return(-1);
			}
		}
		
		# increase delay to fool stupid webserver...
		logMe ("going to sleep for ".$seconds*$errorcount." seconds",$debuglevel,$debuglog);
		sleep ($seconds*$errorcount);	
		logMe ("woke up!");;
	}
	logMe ("got ".scalar(@content)." lines",$debuglevel,$debuglog);
	return (@content);
}


sub getRandomProxy($)
{
	my (@proxyList)=@_[0];
	my ($max)=scalar(@proxyList);
	my ($proxynum)=int( rand($max));;
	#generate random number ...
	logMe ("Random number is: ".$proxynum,2,"debug.log");
	return(getProxy($proxynum,@proxyList));	
}

###################################
#                                 #
# SUB: getProxy()			          #
#                                 #
# return a proxy from list        #
#                                 #
###################################

sub getProxy($)
{
	my (@proxyList)=@_[0];
	my ($proxyID)=@_[1];
	$proxyID++;
	if ($proxyID >= scalar(@proxyList))
	{
		$proxyID=0;
	}	
	return ($proxyList[$proxyID])
}




sub checkProxy ($)
{
	my ($proxy)=@_[0];
	my ($timeout)=@_[1];
	my ($checkurl)="http://www.microsoft.com"; # our reference site ....
	my ($ua);
	my ($response);
	my $ua = LWP::UserAgent->new(env_proxy => 0,
                              keep_alive => 1,
                              timeout => $timeout,
                              agent => "Mozilla/5.001 (Windows; U; NT4.0; en-us)"
                             );
 		$ua->proxy('http', "http://".$proxy."/");
 		$response = $ua->get($checkurl);
 		if ($response->is_success())
 		{
 			# print ("OK");
 			return (1)
 		}
 		else
 		{
 			# print ("NO");
 			return(0);
		}
}

sub getINIdata($)
{
	my ($ini);
	my($inientry)=@_[0];
	my(@ini)=@_[1];
	logMe ("searching for ".$inientry,$debuglevel,$debuglog);
	my(@parts);
	foreach $ini (@ini)
	{
		
		logMe ("checking ".$ini,$debuglevel,$debuglog);
		
		if ($ini=~/$inientry/)
		{
			logMe ("found",$debuglevel,$debuglog);
			@parts=split(/=/,$ini);
			close (INIFILE);
			return(@parts[1]);
		}
	}
	close (INIFILE);	
	return(@parts[1]);
}

sub logMe($)
{
	my ($text) = @_[0];
	my ($level)= @_[1];
	my ($filename)= @_[2];
	if ($level==0)
	{
		return(0);
	}
	my ($logfile);
	my ($now);
	$logfile=">>".$filename;	
	
	open (LOGFILE, $logfile) or die "Can't open $logfile: $!";
	$now=localtime(); 
	print (LOGFILE $now." - ".$text."\n");
	if ($level==2)
	{
		print ($now." - ".$text."\n");
	}
	close (LOGFILE);	
	return(1);
}


sub help()
{
	print ("DNS Predict by JN\n");
	print ("\n");
	print ("Syntax: dnspredic.pl \n\n");
	print ("--help : Shows this screen\n");
	print ("--domain : the domain to search (REQUIRED) \n");
	print ("--item1 : first argument for g00gle predict (REQUIRED) \n");
	print ("--item2 : second argument for g00gle predict (REQUIRED) \n");
	print ("--loglevel=level : The venbosity level\n");
	print ("--timeout=seconds : http timeout\n");
	print ("--file=filename : the output file for resolved hosts\n");
	print ("--dieonerror=int : if set to 1 (the tool dies on fatal erros) 0 overrides this behaviour\n");
	print ("--proxy=host:port : the proxy used for contacting g00gle\n");
	exit(0);
	
}

# parameters:
# $googleURL = buildGoogleURL($baseurl,$domain,$item1,$item2);
# http://labs.google.com/sets?hl=en&q1=homer&q2=flanders&q3=&q4=&q5=&btn=Large+Set

sub buildGoogleURL($)
{
	
	my ($base) = @_[0];
	my ($item1) = @_[1];
	my ($item2) = @_[2];
	

	my ($url);


	$url = $base . "&q1=" . $item1 ."&q2=".$item2."&q3=&q4=&q5=&btn=Large+Set";
	$url =~ s/ //g;
	
	return ($url);
}

sub getIP {
		my ($host) =$_[0];
		my ($h);
		$h = gethostbyname($host);

		if (defined($h))
		{
			return(inet_ntoa($h->addr)); 
		}
		else
		{
			return ("-");
		}
		
	}