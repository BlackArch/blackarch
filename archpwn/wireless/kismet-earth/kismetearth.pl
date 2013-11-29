#!/usr/bin/perl

#
# Kismet logs to .kml (Google Earth) converter v0.00002
#
# acidbits NO_SPAM_PLEASE gmail.com Jan-2007
#
# Reads kismet log files (.gps .xml) from a directory and creates a .kml file. There are two modes. The normal
# mode creates a classic wardriving map in this way:
#
#  - Picks best lat/lon from all logs, based in signal strengh
#  - Drops bad lat/lon data
#  - Removes duplicated BSSID
#  - Removes strange characters from SSID
#
# The single BSSID GPS data mapping mode, is for visualization of all GPS data on a given BSSID. It adds some
# elevation to map points in realtion with the signal strength. Best signal position linked with "node_closed.png",
# others linked to "node_open.png".
#
#  - Picks all lat/lon for the given BSSID
#  - Drops bad lat/lon data
#  - Drops duplicated GPS data for a same position
#
#  .kml output heavily based on kismet2kml converter at http://www.perrygeo.net/wordpress/?p=55
#

#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#

# CHANGELOG
#
# v0.00001: [01-2007]
#    - First release
#
# v0.00002: [07-2007]
#    - Added fix to parse correctly negative (-) Lat/Lon [thanks to Binary Traveler]
#


use XML::DOM;
use Getopt::Std;
use strict;

my $result='klog2kml.kml';
my $duped=0;
my $BSSIDcount=0;
my $BSSIDnoGPS=0;
my $parser = XML::DOM::Parser->new();
my $GPScount=0;
my $GPSmod=0;
my $GPSzero=0;
my $GPSbestsig=0;
my ( $dir, $lat, $lon, $sig, $sig1, $BSSID, $SSID, $TBSSID, $f, $icon );
my (@files, @macs );
my ( %options, %GPS_sig, %GPS_lat, %GPS_lon );

print "\nKismet logs to kml (Google Earth) converter v0.00002\nacidbits NO_SPAM_PLEASE gmail.com 07-2007\n";

getopts("ehb:f:",\%options);
$dir=$ARGV[0];
if( !$dir || $options{h} ){ usage(); }
if( $dir!~/\/$/ ) { $dir = $dir."/"; };
if( $options{f} ) { $result=$options{f}; }
if( $options{b} ) { $TBSSID=uc($options{b}); }

# Get .gps filenames
opendir(DIR, $dir) or die "\nCan't open $dir!\n\n";
while (defined($f = readdir(DIR))) {
    if( $f!~ /^\./ && $f=~/\.gps$/ ) { push(@files,$f); }
}
closedir(DIR);

print "\nProcessing GPS logs ...\nBSSID\tFix\tZero\tFile\n";
# Start processing GPS data
foreach my $f (@files){

    # kismet .gps logs use to be long. XML::DOM library works bad with large amount of data
    # Process .gps files reding data stright through
    open(KML,$dir.$f);
    my @cont=<KML>;
    close(KML);
    chomp(@cont);

    LOOP1: foreach my $line (@cont){
        print "$GPScount \t$GPSmod \t$GPSzero \t$dir$f  \r";
        # If not AP GPS data, process next line
        if($line !~ /gps-point/ || $line=~ /GP:SD:TR:AC:KL:OG/) { next LOOP1; }
        # Get GPS data from line
        $line =~ m{bssid=\"([\w|\d|\:]+)\"};  $BSSID=$1;
        $line =~ m{lat="([\w|\d|\-|\.]+)\"};  $lat=$1;
        $line =~ m{lon="([\w|\d|\-|\.]+)\"};  $lon=$1;
        $line =~ m{signal="([\w|\d|\.|-]+)\"};  $sig=$1;
        # Make sure lat/lon is valid
        if( abs($lon)==180 || abs($lat)==90 || !$lon || !$lat ) {  next LOOP1;  }

        # Normal mode or single BSSID full GPS data map mode?
        if( !$TBSSID ) {
            # Already have a position for this BSSID?
            if( grep(/$BSSID/,keys(%GPS_sig)) ) {
                # If new signal is better, store the new position
                if($sig>$GPS_sig{$BSSID}) {
                    if( $GPS_sig{$BSSID}<1 ) {$GPSzero--;} # Count BSSID with null signal
                    $GPS_sig{$BSSID}=$sig;
                    $GPS_lat{$BSSID}=$lat;
                    $GPS_lon{$BSSID}=$lon;
                    $GPSmod++; # Count number of modified poitions
                }
            } else {
                $GPScount++; # Count unique BSSID in .gps files
                if( $sig<1 ) {$GPSzero++;} # Count BSSID with null signal
                $GPS_sig{$BSSID}=$sig;
                $GPS_lat{$BSSID}=$lat;
                $GPS_lon{$BSSID}=$lon;
            }
        } else {
            # This is -b option BSSID full GPS data mapping
            # It stores all BSSID good GPS positions
            if( $TBSSID=~/$BSSID/ ) {
                # If new signal is better, store the new position
                if($sig>$GPSbestsig) {
                    $GPSbestsig=$sig;
                }
                # Already have this position?
                my $c=0;
                while( $GPS_lat{$c}!=$lat && $GPS_lon{$c}!=$lon && $c<$GPScount){ $c++;    }
                # If so, update signal if it is better
                if( $GPS_lat{$c}==$lat && $GPS_lon{$c}==$lon ){
                    if($GPS_sig{$c}<$sig) { $GPS_sig{$c}=$sig; $GPSmod++;}
                } else {
                    $GPS_sig{$GPScount}=$sig;
                    $GPS_lat{$GPScount}=$lat;               
                    $GPS_lon{$GPScount}=$lon;
                    $GPScount++; # Positions Counter       
                }
            }
        }
    }
}


# Get .xml filenames
unlink($dir.$result);
@files=();
opendir(DIR, $dir) or die "\n\nCan't open $dir!\n\n";
while (defined($f = readdir(DIR))) {
    if( $f!~ /^\./ && $f=~/\.xml$/ ) { push(@files,$f);  }
}
closedir(DIR);

open( KML,">".$dir.$result);
print KML "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<kml xmlns=\"http://earth.google.com/kml/2.0\">\n<Document>\n\t<name>Kismet Log </name>\n\t<Folder>\n\t\t<name> Kismet Log Points </name>\n";

print "\n\nProcessing XML logs ...\nBSSID\tDuped\tNoGPS\tFile\n";

# Normal mode or single BSSID full GPS data map mode?
if( !$options{b} ) {
    # Tracks BSSID processed to avoid duplicated ones in .kml
    @macs=();
    foreach my $f (@files){ dump_kml($dir.$f); }
} else {
        foreach my $f (@files){
            open(FH,$dir.$f); my @cont=<FH>; close(FH);
             if( grep(/$TBSSID/,@cont) ) { dump_kml($dir.$f); last; }
        }
}

print KML "\n\t</Folder>\n</Document>\n</kml>\n";
close(KML);
print "\n\n$dir$result created.\n\n";

exit;



# Reads data from .xml file and create .kml. Adds GPS data obtined before.
sub dump_kml()
{
    my $doc = $parser->parsefile($_[0]);
    foreach my $net ($doc->getElementsByTagName('wireless-network')){

        $BSSID=$net->getElementsByTagName('BSSID')->item(0)->getFirstChild->getNodeValue;

        # Single BSSID full GPS data Mapping mode
        if( $TBSSID && $TBSSID=~/$BSSID/) {
            # Sometimes kismet doesn't stores SSID data in .xml logs
            if( $net->toString =~ /<SSID>/) {
                # Remove strange characters, and max 32 chars length
                $SSID=$net->getElementsByTagName('SSID')->item(0)->getFirstChild->getNodeValue;
                $SSID=~ s/[<|>|&]+//;
                if( length($SSID)>32) { $SSID=substr($SSID,0,32); }
                $net->getElementsByTagName('SSID')->item(0)->getFirstChild->setNodeValue($SSID);
            } else {
                $SSID="UNKNOWN";
            }
            for(my $c=0;$c<$GPScount;$c++){
                $lon=$GPS_lon{$c};
                $lat=$GPS_lat{$c};
                $sig=$GPS_sig{$c};
                if( $sig==$GPSbestsig ) { $icon="node_closed.png"; } else { $icon="node_open.png"; }
                if( !$options{e} ) { $sig=0; } else { $sig1=$sig; }               
                print KML "\t\t<Placemark>\n\t\t\t<description><![CDATA[\n\t\t\t\t<p style=\"font-size:8pt;font-family:monospace;\">($lon, $lat)</p>\n\t\t\t\t<ul>\n\t\t\t\t            <li> BSSID : $BSSID </li>\n\t\t\t\t<li> Type : ".$net->getAttributeNode('type')->getValue." </li>\n\t\t\t\t<li> Channel : ".$net->getElementsByTagName('channel')->item(0)->getFirstChild->getNodeValue." </li>\n\t\t\t\t<li> Encrypt : ".$net->getElementsByTagName('encryption')->item(0)->getFirstChild->getNodeValue." </li>\n\t\t\t\t<li> Signal : $sig </li>\n\t\t\t\t</ul>\n\t\t\t\t]]>\n\t\t\t</description>\n\t\t\t<name>$SSID</name>\n\t\t\t<Style>\n\t\t\t<icon>$icon</icon>\n\t\t\t</Style>\n\t\t\t<Point>\n\t\t\t\t<extrude>1</extrude>\n\t\t\t\t<altitudeMode>relativeToGround</altitudeMode>\n\t\t\t\t<coordinates>$lon,$lat,$sig1</coordinates>\n\t\t\t</Point>\n\t\t</Placemark>\n";
            }
            return;

        }

        # Normal mode
        if( !$TBSSID ) {
            # BSSID already processed?
            if( grep(/$BSSID/,@macs)) {
                $duped++; # Count Dup BSSIDs
            } else {
                # Sometimes kismet doesn't stores SSID data in .xml logs
                if( $net->toString =~ /<SSID>/) {
                    # Remove strange characters, and max 32 chars length
                    $SSID=$net->getElementsByTagName('SSID')->item(0)->getFirstChild->getNodeValue;
                    $SSID=~ s/[<|>|&]+//;
                    if( length($SSID)>32) { $SSID=substr($SSID,0,32); }
                    $net->getElementsByTagName('SSID')->item(0)->getFirstChild->setNodeValue($SSID);
                } else {
                    $SSID="UNKNOWN";
                }
                # Select open or closed icon
                if($net->getElementsByTagName('encryption')->item(0)->getFirstChild->getNodeValue=~/^None$/){
                    $icon="node_open.png";
                } else {
                    $icon="node_closed.png";
                }
   
                # Pick GPS position
                $lon=$GPS_lon{$BSSID};
                $lat=$GPS_lat{$BSSID};
                $sig=$GPS_sig{$BSSID};
                # If no position for this BSSID in .gps logs ...
                if( !abs($lat) || !abs($lon) ) {
                    # Gets position from max-(lat|lon) in .xml
                    $lat=$net->getElementsByTagName('max-lat')->item(0)->getFirstChild->getNodeValue;
                    $lon=$net->getElementsByTagName('max-lon')->item(0)->getFirstChild->getNodeValue;
                    $sig=0;
                    # Make sure lat/lon is valid
                    if(abs($lon)==180||abs($lat)==90){$lon=0;$lat=0;}
                    # If no position for this BSSID in max.(lat|lon) ...
                    if( !abs($lat) || !abs($lon) ) {
                        # Gets position from min-(lat|lon) in .xml
                        $lat=$net->getElementsByTagName('min-lat')->item(0)->getFirstChild->getNodeValue;
                        $lon=$net->getElementsByTagName('min-lon')->item(0)->getFirstChild->getNodeValue;
                    # Make sure lat/lon is valid
                        if(abs($lon)==180||abs($lat)==90){$lon=0;$lat=0;$BSSIDnoGPS++;}
                    }
                }
                if( !$options{e} ) { $sig1=0; } else { $sig1=$sig; }
                # Have we got a valid position for this BSSID?
                if( abs($lat) && abs($lon) ) {
                    print KML "\t\t<Placemark>\n\t\t\t<description><![CDATA[\n\t\t\t\t<p style=\"font-size:8pt;font-family:monospace;\">($lon, $lat)</p>\n\t\t\t\t<ul>\n\t\t\t\t            <li> BSSID : $BSSID </li>\n\t\t\t\t<li> Type : ".$net->getAttributeNode('type')->getValue." </li>\n\t\t\t\t<li> Channel : ".$net->getElementsByTagName('channel')->item(0)->getFirstChild->getNodeValue." </li>\n\t\t\t\t<li> Encrypt : ".$net->getElementsByTagName('encryption')->item(0)->getFirstChild->getNodeValue." </li>\n\t\t\t\t<li> Signal : $sig </li>\n\t\t\t\t</ul>\n\t\t\t\t]]>\n\t\t\t</description>\n\t\t\t<name>$SSID</name>\n\t\t\t<Style>\n\t\t\t<icon>$icon</icon>\n\t\t\t</Style>\n\t\t\t<Point>\n\t\t\t\t<extrude>1</extrude>\n\t\t\t\t<altitudeMode>relativeToGround</altitudeMode>\n\t\t\t\t<coordinates>$lon,$lat,$sig1</coordinates>\n\t\t\t</Point>\n\t\t</Placemark>\n";

                    # Add BSSID to already processed list
                    push(@macs,$BSSID); $BSSIDcount++;
                }
            }
        }
        print "$BSSIDcount \t$duped \t$BSSIDnoGPS \t$_[0]  \r";
    } # Next BSSID

}



sub usage()
{

    die("\nReads all .xml and .gps files from a kismet log placed in a direcotory\nand builds a .kml file to view in GoogleEarth in this way:\n\n    - Picks best lat/lon from all logs, based in signal strengh\n    - Drops bad lat/lon data\n    - Removes duplicated BSSID\n    - Removes strange characters from SSID\n\nUsage:\n    ./klog2kml.pl [-e] [-f filename.kml] [-b BSSID] logs_dir\n\nOptions:\n    -f\tfilename to save\n    -e\tGives height\n    -b\tMaps all BSSID GPS data \n\nEg: \n    Standard mode. Generates a .kml from all kismet logs in /var/log/kismet.\n\n      ./klog2kml.pl /var/log/kismet\n\n    Single BSSID full GPS data mapping. Generates a .kml with all\n    11:22:33:44:55:66 GPS data. Gives height relative to signal strengh.\n\n      ./klog2kml.pl -e -b 11:22:33:44:55:66 /var/log/kismet\n\nnode_open.png and node_closed.png should be placed in the same directory where\n.kml file is.\n\n");

}
