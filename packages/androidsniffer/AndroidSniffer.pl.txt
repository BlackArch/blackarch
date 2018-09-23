#!/usr/bin/perl
###############################################
#   Version 0.1 Andr01d Magic dumper
#
#   Written by Sw1tCh
#
###############################################
# Process goes: Untar -> Search -> Dislay
#
#
##############
# You will need the following to work with
# SQLite3
#
#  $ perl -MCPAN -e shell   
#  cpan> install DBI        
#  cpan> install DBD::SQLite
##############################################
use strict;
#use warnings;
use Archive::Tar;
use Term::Menu;
use DBI;


print "#############################################################\n";
print "##   Android Harassment Tool    					    	 \n";
print "## 	Version 0.1											 \n";         
print "##   Written by Sw1tCh								     \n";
print "#############################################################\n";

#############Sub Routines#################


#// Untar the Files to the _temp dir
sub untar {
	my $tar = Archive::Tar->new();
	$tar->read( $ARGV[0] );
	`mkdir _temp`;
#// Extract into the directory	
	chdir('./_temp/') or die "$!";
	$tar->extract();
#// Reset when done
	chdir('../') or die "$!";
}

#// Dumps The Passwords that are stored for Third Party Apps:
sub thridpartyPasswords
{
	##// Connects to Database
 	my $dbh = DBI->connect( "dbi:SQLite:_temp/system/accounts.db" ) || die "Cannot connect: $DBI::errstr";	
	my $Accounts3p = $dbh->selectall_arrayref( q( SELECT * FROM accounts) );

	foreach( @$Accounts3p ) {
		foreach my $i (0..$#$_) {
			print "$_->[$i] \n";
		}
	#	print "\n";
	}

	$dbh->disconnect;
	print "\n\n\n\n";
}

#// Dumps the Phone Contacts by Connecting to the sqlite3 database 
sub DumpTheContacts
{

	
	print "\n\n\n Phone Contacts:\n Account Name - Display Name - Alt. Display Name - Displayed Name Source - Times Contacted \n-------------------------------------------------------------------------------------------------------\n";

	#// Connected to the database
 	my $dbh = DBI->connect( "dbi:SQLite:_temp/data/com.android.providers.contacts/databases/contacts2.db" ) || die "Cannot connect: $DBI::errstr";	
	my $calls = $dbh->selectall_arrayref( q( SELECT raw_contacts.account_name, raw_contacts.display_name, raw_contacts.display_name_alt, phone_lookup.normalized_number, raw_contacts.times_contacted  FROM raw_contacts, phone_lookup WHERE raw_contacts._id = phone_lookup.raw_contact_id;) );

	foreach( @$calls ) {
		foreach my $i (0..$#$_) {
			print "$_->[$i]       ";
		}
		print "\n";
	}

	$dbh->disconnect;
print "\n\n\n";
}


#// Dump Email Contacts from the contact2.db which is in sqlite3 format.
sub DumpEmailContacts
{
	print "\n\n\n Email Contacts:\n Account Name - Display Name - Alt. Display Name - Displayed Name Source - Times Contacted\n-------------------------------------------------------------------------------------------------------\n";


 	my $dbh = DBI->connect( "dbi:SQLite:_temp/data/com.android.providers.contacts/databases/contacts2.db" ) || die "Cannot connect: $DBI::errstr";	
	my $calls = $dbh->selectall_arrayref( q(SELECT account_name, display_name, display_name_alt, display_name_source, times_contacted, last_time_contacted  FROM raw_contacts) );

	foreach( @$calls ) {
		foreach my $i (0..$#$_) {
			print "$_->[$i]       ";
		}
		print "\n";
	}

	$dbh->disconnect;
print "\n\n\n";
}


#// Dumps out the contact groups.  99% of the time... Totally useless
sub DumpTheGroups
{
 	my $dbh = DBI->connect( "dbi:SQLite:_temp/data/com.android.providers.contacts/databases/contacts2.db" ) || die "Cannot connect: $DBI::errstr";	
	my $calls = $dbh->selectall_arrayref( q( SELECT account_name, title FROM groups) );

	print "\n";
	foreach( @$calls ) {
		foreach my $i (0..$#$_) {
			print "$_->[$i] ";
		}
		print "\n";
	}

	$dbh->disconnect;
print "\n\n\n";
}




#// Dumps Log of Calls Made from Phone
sub CallLog
{
 	my $dbh = DBI->connect( "dbi:SQLite:_temp/data/com.android.providers.contacts/databases/contacts2.db" ) || die "Cannot connect: $DBI::errstr";	
	my $calls = $dbh->selectall_arrayref( q( SELECT _id, number, name FROM calls) );

	foreach( @$calls ) {
		foreach my $i (0..$#$_) {
			print "$_->[$i] ";
		}
		print "\n";
	}

	$dbh->disconnect;
print "\n\n\n";

}


#// Dumps Contents of wpa_supplicant.conf, which contains the details on the wireless networks
##//     Really needs to be cleaned up / formatted...but w/e
sub DumpWirelessNetworks
{

	my $wirelessConf ="_temp/misc/wifi/wpa_supplicant.conf";
	open(DAT, $wirelessConf) || die("Could not open file!"); 
	my @rawWireless = <DAT>;
 	
 	foreach (@rawWireless){
 		print "$_";
 	}
	close (DAT);
	print "\n\n\n";
}



########### Main #######################

#// Validates Input
my $filename = $ARGV[0];
if ((! -e $filename) || !( $filename =~ m/tar$/ ) ) 
{
	print "##################################################################\n";
	print "##   Usage: #> perl AndroidMagic.pl <data.tar (from phone)>       \n";
	print "##     <data.tar - From Nandroid Dump >    \n";
	print "##################################################################\n\n\n\n\n";
	exit;
} 


#//Untars the Files
untar($ARGV[0]);

#// Menu

my $menu = new Term::Menu;
while (1)
{
	my $answer = $menu->menu(
        SearchPasswords    => ["Search for 3rd Party Passwords", '1'],
        DumpCallLog		   => ["Dump the Call Log", '2'],
        DumpContacts       => ["Dump Contacts", '3'],
        DumpEmailContacts  => ["Dump Email Contacts", '4'],
        DumpGroups  	   => ["Dump Contact Groups", '5'],
        WirelessNetworks   => ["Dump Wireless Configuration", '6'],
        quit			   => ["Exit", "q"],
	);
#	print "$answer\n";
	chomp ($answer);

	if($answer eq "SearchPasswords" ) {
		print "Search for Passwords\n\n";
		thridpartyPasswords();
	}
	elsif ($answer eq "DumpContacts")
	{
		print "Dump Contacts";
		DumpTheContacts();
	}
	elsif ($answer eq "DumpEmailContacts")
	{
		DumpEmailContacts();
	}
	elsif ($answer eq "DumpGroups")
	{
		DumpTheGroups();
	}
	elsif ($answer eq "DumpCallLog")
	{
		CallLog();
	}
	elsif ($answer eq "WirelessNetworks")
	{
		DumpWirelessNetworks();
	}
	elsif ($answer eq "quit")
	{
		print "Quiting --> \n\n";
	        `rm -rf _temp/`;
		exit;

	}
}
	print "failing out: should not see this\n\n";
