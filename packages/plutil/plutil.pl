# plutil.pl 1.6 - implementation of binary/UTF-8 (text) XML conversion of plist files
# does not (yet) support OS X plutil command line syntax

# 1.0 - first public release
# 1.1 - added support for date Type 3; fixed real Type 2
# 1.2 - fixed handling of empty arrays
# 1.3 - fixed handling of large strings and empty keys
# 1.4 - write utf8 in XML, convert as necessary on read
# 1.5 - read/write 8 byte integers and negative integers; handle special XML chars in dictionary keys
#     - now requires Math::BigInt
# 1.6 - read/write data fields (type 4); also handles funny filenames better
#     - now requires MIME::Base64
# 1.7 - correct binary to XML conversion

# Copyright 2007-2008 Starlight Computer Wizardry

use strict;

use Encode qw(decode encode);
use Fcntl qw(:seek);
use Time::Local 'timegm_nocheck';
use POSIX ();
use Math::BigInt;
use MIME::Base64;

my $VERSION = "v1.5";

   sub nice_string {
       join("",
         map { $_ > 255 ?                  # if wide character...
               sprintf("\\x{%04X}", $_) :  # \x{...}
               chr($_) =~ /[[:cntrl:]]/ ?  # else if control character ...
               sprintf("\\x%02X", $_) :    # \x..
               quotemeta(chr($_))          # else quoted or as themselves
         } unpack("U*", $_[0]));           # unpack Unicode characters
   }


my @Offsets;
my ($OffsetSize, $ObjRefSize, $NumObjects, $TopObject, $OffsetTableOffset);
my $OVal;

my ($InX, %Strings);
my $SNum = 0;
my $SLen = 0; # string lens
my $ILen = 0; # int lens
my $MLen = 0; # Misc lens
my $OLen = 0; # object lens

sub GetToken {
    $InX =~ s/^\s+//; # skip whitespace

#    print "from ",substr($InX, 1, 20)," => ";

    if ($InX =~ s/<([^>]+)>//) {
#	print "Token: $1\n";
	return $1;
    }
    else {
	die "Can't find token at " . substr($InX, 0, 80);
    }
}

sub GetNonToken {
    my ($t) = @_;

    if ($InX =~ s{([^<]*)</$t>}{}) {
	return $1;
    }
    else {
	die "can't find end of <$t> at " . substr($InX, 0, 80);
    }
}

sub PushToken {
    my ($t) = @_;
    $InX = "<$t>" . $InX;
}

sub ReadXinteger {
    my $s = +GetNonToken("integer");

    my $il = 1;
    $il += 1 if ($s > 255);
    $il += 2 if ($s > 65535);
    $il += 4 if ($s > Math::BigInt->new("4294967295"));
    $il += 7 if ($s < 0);
    $ILen += $il+1;
#    print "ILen (int $s) += ", $il+1, "\n";

    ++$NumObjects;

    return ["integer", $s];
}

sub ReadXreal {
    my $s = +GetNonToken("real");

    $MLen += 9;
#    print "MLen (real $s) += 9\n";

    ++$NumObjects;

    return ["real", +$s];
}

sub ReadXdate {
    my $s = +GetNonToken("date");

    $MLen += 9;
#    print "MLen (date $s) += 9\n";

    ++$NumObjects;

    return ["date", +$s];
}

sub CountIntSize {
    my ($num) = @_;

    my $ans = 0;

    $ans += 2 if $num > 14;
    $ans += 1 if $num > 255;
    $ans += 2 if $num > 65535;

    return $ans;
}

sub CountX {
    my ($s) = @_;
    if (not defined $Strings{$s}) {
	++$NumObjects;
	$Strings{$s} = ++$SNum;
#	print "SLen ('$s' ",length($s),") += ",length($s)+1 + CountIntSize(length($s)),"\n";
	$SLen += length($s)+1;
	$ILen += CountIntSize(length($s));
    }
}


sub ReadXdata {
    my $s = +GetNonToken("data");


    ++$NumObjects;

    my $buf = decode_base64($s);
    $MLen += length($buf)+1;
    print "MLen (data ",unpack("H*",$buf),") += ",length($buf)+1,"\n";
    $ILen += CountIntSize(length($buf));
    print "ILen (data(",length($buf),")) += ", CountIntSize(length($buf)),"\n";

#    print "buf=",unpack("H*",$buf),"\n";

    return ["data", $buf];
}

sub UnfixXMLString {
    my ($s) = @_;

    $s =~ s/&lt;/</g;
    $s =~ s/&gt;/>/g;
    $s =~ s/&amp;/&/g;

    return $s;
}

sub ReadXstring {
    my $s = UnfixXMLString(GetNonToken("string"));

    CountX($s);
    return ["string", $s];
}

sub ReadXustring {
    my $s = GetNonToken("ustring");

    $s = pack "U0C*", unpack "C*", $s;

    if (not defined $Strings{$s}) {
	++$NumObjects;
	$Strings{$s} = ++$SNum;
#	print "SLen ('",nice_string($s),"' ",length($s)*2,") += ",length($s)*2+1 + CountIntSize(length($s)),"\n";
	$SLen += length($s)*2+1;
	$ILen += CountIntSize(length($s));
    }

# now treat as utf8 so binary output will work

    return ["ustring", $s];
}

sub ReadXarray {
    my @array;

    my $j = 0;

    while (1) {
	my $t = GetToken();
	last if ($t eq "/array");
	PushToken($t);
	my $v = ReadXObject();
	++$j;
	push @array,$v;
	++$OLen; # count each array member
    }

    ++$NumObjects;
    ++$MLen; # count array type byte
    $ILen += CountIntSize(scalar @array);

    return ["array", \@array];
}

sub ReadXdict {
    my %dict;

    my $t = GetToken();
    while ($t eq "key") {
	my $k = UnfixXMLString(GetNonToken("key"));
	CountX($k);
	my $v = ReadXObject();
	$dict{$k} = $v;
	$t = GetToken();
	$OLen += 2;
    }
    die "Missing /dict at $t" if ($t ne "/dict");

    ++$NumObjects;
    ++$MLen; # count dict type byte
    $ILen += CountIntSize(scalar keys %dict);
    return ["dict", \%dict];
}

sub ReadXtrue_ {
    ++$NumObjects;
    $MLen += 1; # type
    return ["true", 1];
}

sub ReadXfalse_ {
    ++$NumObjects;
    $MLen += 1; # type
    return ["false", 0];
}

sub ReadXObject {
    my $t = GetToken();
    $t =~ s{/$}{_};
#    print "ReadX$t()\n";
    my $ans = eval "ReadX" . $t . "()";
    die "$@ in ReadXObject" if $@;

    return $ans;
}

#my ($OffsetSize, $ObjRefSize, $NumObjects, $TopObject, $OffsetTableOffset);
#my @Offsets;
my @VObjects;
my $ObjNum = 0;
my %WStrings;

sub MakeInt {
    my ($anInt) = @_;
    my $ans = "";

    if ($anInt > 65535) { # 4 byte int
	$ans = "\x12" . pack("N", $anInt);
    }
    elsif ($anInt > 255) { # 2 byte int
	$ans = "\x11" . pack("n", $anInt);
    }
    else {
	$ans = "\x10" . pack("C", $anInt);
    }

    return $ans;
}

sub MakeAType {
    my ($typ, $len) = @_;
    my $ans = "";
#    print "MakeAType($typ,$len)\n";

    my $optint = "";

    if ($len < 15) {
	$typ .= sprintf("%x", $len);
    }
    else {
	$typ .= "f";
	$optint = MakeInt($len);
    }
#    print "typ=$typ\n";
    $ans = pack("H*", $typ) . $optint;

    return $ans;
}

sub WriteBtrue {
    my $oVal = $OVal;
    my $objNum = $ObjNum++;
    my $t = "\x09";
    $VObjects[$objNum] = $t;

#    print "#$objNum -> true\n";
    return $objNum;
}

sub WriteBfalse {
    my $oVal = $OVal;
    my $objNum = $ObjNum++;
    my $t = "\x08";
    $VObjects[$objNum] = $t;

#    print "#$objNum -> false\n";
    return $objNum;
}

sub WriteBinteger {
    my $oVal = $OVal;
    my $objNum = $ObjNum++;
    my $il = 0;
    if ($oVal > 255) {
	++$il;
    }
    if ($oVal > 65535) {
	++$il;
    }
    if ($oVal > Math::BigInt->new("4294967295")) {
	++$il;
    }
    $il = 3 if ($oVal < 0);

    my $t = MakeAType("1", $il);
    my $buf;
    if ($il < 3) {
	$buf = pack((($il == 0) ? "C" : (($il == 1) ? "n" : "N" )), $oVal);
    }
    else {
	if ($oVal < 0) {
	    $oVal += Math::BigInt->new(2)->bpow(64);
	}
	my $hw = Math::BigInt->new($oVal);
	$hw->brsft(32);
	my $lw = Math::BigInt->new($oVal);
	$lw->band(Math::BigInt->new("4294967295"));
	$buf = pack("N", $hw) . pack("N", $lw);
    }
    $VObjects[$objNum] = $t . $buf;
#    print "($il) VObjects[$objNum] = ",unpack("H*", $VObjects[$objNum]),"  :  ";

#    print "#$objNum -> $oVal\n";
    return $objNum;
}

sub WriteBreal {
    my $oVal = $OVal;
    my $objNum = $ObjNum++;

    my $t = MakeAType("2", 3);
    $VObjects[$objNum] = $t . reverse(pack("d", $oVal));

#    print "#$objNum -> $oVal\n";
    return $objNum;
}

sub WriteBdate {
    my $oVal = $OVal;
    my $objNum = $ObjNum++;

    my ($year,$mon,$mday,$hour,$min,$sec) = unpack "A4xA2xA2xA2xA2xA*",$oVal;
    $sec =~ s/Z$//;
    my ($fsec,$isec) = POSIX::modf($sec);
#    print "Write date $year-$mon-${mday}T$hour:$min:${sec}Z\n";
    $oVal = timegm_nocheck($isec,$min,$hour,$mday,$mon-1,$year-1900)-978307200 + $fsec;

    my $t = MakeAType("3", 3);
    $VObjects[$objNum] = $t . reverse(pack("d", $oVal));

#    print "#$objNum -> $oVal\n";
    return $objNum;
}

sub WriteBdata {
    my $oVal = $OVal;
    my $objNum = $ObjNum++;

    my $t = MakeAType("4", length($oVal));
#    print "data(",length($oVal),")\n";
    $VObjects[$objNum] = $t . $oVal;

#    print "#$objNum -> ",unpack("H*",$oVal),"\n";
    return $objNum;
}

sub WriteBstring {
    my $oVal = $OVal;
    my $objNum;

    if (not defined $WStrings{$oVal}) {
	$objNum = $ObjNum++;
	$WStrings{$oVal} = $objNum;
	my $t = MakeAType("5", length($oVal));
	$VObjects[$objNum] = $t . $oVal;
#	print "#$objNum -> $oVal\n";
    }
    else {
	$objNum = $WStrings{$oVal};
    }

    return $objNum;
}

sub WriteBustring {
    my $oVal = $OVal;
    my $objNum;

    if (not defined $WStrings{$oVal}) {
	$objNum = $ObjNum++;
	$WStrings{$oVal} = $objNum;
	my $t = MakeAType("6", length($oVal));
	$VObjects[$objNum] = $t . pack("C*", unpack("U*", encode("UTF-16BE", $oVal)));
#	print "#$objNum -> ", nice_string($oVal), "\n";
    }
    else {
	$objNum = $WStrings{$oVal};
    }

    return $objNum;
}

sub WriteBarray {
    my $oVal = $OVal;
    my $objNum = $ObjNum++;
    my $t = MakeAType("a", scalar @$oVal);

    for (my $j = 0; $j < @$oVal; ++$j) {
	my $vref = WriteB($oVal->[$j]);
#	print "[$j] = $vref\n";
	$t .= PackIn($ObjRefSize, $vref);
    }

    $VObjects[$objNum] = $t;
#    print "#$objNum = [", unpack("H*", $t), "]\n";
    return $objNum;
}

sub WriteBdict {
    my $oVal = $OVal;
    my $objNum = $ObjNum++;
    my $t = MakeAType("d", scalar keys %$oVal);

    foreach my $k (sort keys %$oVal) {
	my $kref = WriteB(["string", $k]);
	$t .= PackIn($ObjRefSize, $kref);
    }

    foreach my $k (sort keys %$oVal) {
	my $vref = WriteB($oVal->{$k});
	$t .= PackIn($ObjRefSize, $vref);
    }

    $VObjects[$objNum] = $t;
#    print "#$objNum = {", unpack("H*", $t), "}\n";
    return $objNum;
}

sub WriteB {
    my ($obj) = @_;
    my $oType = $obj->[0];
    $OVal = $obj->[1];

    my $ans;
    if ($oType ne "") {
	$ans = eval "WriteB" . $oType . "()";
	die "$@ in WriteB" if $@;
    }
    else {
	die "$oType == ''";
    }

    return $ans;
}

sub TryName {
    my ($oldname,$fromtype, $totype, $tryext) = @_;


    if ($oldname =~ /\.$fromtype\.$tryext$/) {
	print "Found .$fromtype.$tryext\n";
	$oldname =~ s/\.$fromtype\.$tryext$/.$totype.$tryext/;
    }
    elsif ($oldname =~ /\.$tryext$/) {
	print "Found .$tryext\n";
	$oldname =~ s/\.$tryext$/.$totype.$tryext/;
    }

    print "TryName: $oldname\n";

    return $oldname;
}

sub NewName {
    my ($oldname, $fromtype, $totype) = @_;
    print "Old: $oldname, from: $fromtype, to: $totype\n";
    my $newname = TryName($oldname, $fromtype, $totype, "plist");

    $newname = TryName($newname, $fromtype, $totype, "strings");
    $newname .= ".$totype" if $newname !~ /$totype/;

    return $newname;
}

sub NumBytesNeeded {
    my ($num) = @_;

    my $ans = 0;

    while ($num >= 1) {
	++$ans;
	$num /= 256;
    }

    return $ans;
}

sub PackIn {
    my ($bytes, $num) = @_;

    my $fmt = ["C", "n", "N", "N"]->[$bytes-1];

    if ($bytes == 3) {
	my $ans =  substr(pack($fmt, $num), -3);
#	print "PackIn($bytes, $num) = ", unpack("H*", $ans),"\n";
	return $ans;
    }
    else {
	return pack($fmt, $num);
    }
}

sub ConvertXMLToBinary {
    my ($filename) = @ARGV;
    my $newname = NewName($filename, "text", "binary");

    print "XMLToBinary:\n";

    open(INF, "<", $filename) or die "can't open $filename for conversion"; # removed :utf8
    readline INF; # skip XML line
    readline INF; # skip DTD line
    my $verline = readline INF;
    chomp $verline;
    warn "Unknown plist version $verline" if $verline ne "<plist version=\"1.0\">";

    $InX = do { local $/; undef $/; <INF> };

# brute force remove unicode flag
#    $InX = pack "C*", unpack "U0C*", $InX;

    $NumObjects = 0;
    my $top = ReadXObject();

    print "NumObjects = $NumObjects\n";
    $ObjRefSize = NumBytesNeeded($NumObjects);
    print "ObjRefSize = $ObjRefSize\n";

    print "String Lengths = $SLen\n";
    print "Int Lengths = $ILen\n";
    print "Misc Lengths = $MLen\n";
    print "$OLen obj refs = ", $OLen * $ObjRefSize, "\n";
    my $fileSize = $SLen + $ILen + $MLen + $OLen * $ObjRefSize + 40;
    print "Object FileSize = $fileSize\n";
    $OffsetSize = NumBytesNeeded($fileSize);
    print "OffsetSize = $OffsetSize\n";
    print "Total Filesize = ",$fileSize + $NumObjects * $OffsetSize,"\n";
    $OffsetTableOffset = $fileSize - 32;
    print "OffsetTableOffset = $OffsetTableOffset\n\n";
    print "Unique strings = ", scalar keys %Strings,"\n";

    $TopObject = 0;

    close(INF);

    open(OUTF, ">$newname") or die "can't open $newname for output";
    binmode(OUTF);

# output magic header
    print OUTF "bplist00";

    WriteB($top);

    my $objOffset = 8;
    my @offsets;

    for (my $j = 0; $j < @VObjects; ++$j) {
	print OUTF $VObjects[$j];
	$offsets[$j] = $objOffset;
	$objOffset += length($VObjects[$j]);
    }

    for (my $j = 0; $j < @offsets; ++$j) {
	print OUTF PackIn($OffsetSize, $offsets[$j]);
    }

    print OUTF pack("x6CC", $OffsetSize, $ObjRefSize);
    print OUTF pack("x4N", $NumObjects);
    print OUTF pack("x4N", $TopObject);
    print OUTF pack("x4N", $OffsetTableOffset);

    close(OUTF);
}

my %RStrings;

sub ReadBType0 {
    my ($objLen) = @_;
    my $ans;

    if ($objLen == 0) {
	$ans = ["null", 0];
    }
    elsif ($objLen == 8) {
	$ans = ["false", 0];
    }
    elsif ($objLen == 9) {
	$ans = ["true", 1];
    }
    elsif ($objLen == 15) {
	$ans = ["fill", 15];
    }

    ++$MLen;

#    print $ans->[0],"\n";
    return $ans;
}

sub ReadBType1 { # int
    my ($objLen) = @_;
    die "Integer > 8 bytes = $objLen" if ($objLen > 3);

    my $byteLen = 1 << $objLen;

    my ($buf, $val);
    read(INF, $buf, $byteLen);
    if ($objLen == 0) {
	$val = unpack("C", $buf);
    }
    elsif ($objLen == 1) {
	$val = unpack("n", $buf);
    }
    elsif ($objLen == 2) {
	$val = unpack("N", $buf);
    }
    elsif ($objLen == 3) {
#	print "buf=",unpack("H*",$buf),"\n";
	my ($hw,$lw) = unpack("NN", $buf);
	$val = Math::BigInt->new($hw)->blsft(32)->bior($lw);
	if ($val->bcmp(Math::BigInt->new(2)->bpow(63)) > 0) {
	    $val -= Math::BigInt->new(2)->bpow(64);
	}
#	print "8 byte val: hw; $hw, lw: $lw, val: $val\n";
#	$val = unpack("Q", $buf);
#	print "8 byte val (Q): $val\n";
    }
    $ILen += $byteLen+1;

#    print "$val\n";
    return ["int", $val];
}

sub ReadBType2 { # real
    my ($objLen) = @_;
    die "Real > 8 bytes" if ($objLen > 3);

    my $byteLen = 1 << $objLen;

    my ($buf, $val);
    read(INF, $buf, $byteLen);
    if ($objLen == 0) { # 1 byte float = error?
	die "1 byte real found";
    }
    elsif ($objLen == 1) { # 2 byte float???
	die "2 byte real found";
    }
    elsif ($objLen == 2) {
	$val = unpack("f", reverse $buf);
    }
    elsif ($objLen == 3) {
	$val = unpack("d", reverse $buf);
    }

    $MLen += 9;

#    print "$val\n";
    return ["real", $val];
}

sub ReadBType3 { # date
    my ($objLen) = @_;
    die "Date > 8 bytes" if ($objLen > 3);

    my $byteLen = 1 << $objLen;

    my ($buf, $val);
    read(INF, $buf, $byteLen);
    if ($objLen == 0) { # 1 byte NSDate = error?
	die "1 byte NSDate found";
    }
    elsif ($objLen == 1) { # 2 byte NSDate???
	die "2 byte NSDate found";
    }
    elsif ($objLen == 2) {
	$val = unpack("f", reverse $buf);
    }
    elsif ($objLen == 3) {
	$val = unpack("d", reverse $buf);
    }

    $MLen += 9;

#    print "$val\n";
    return ["date", $val];
}

sub ReadBType4 { # binary data
    my ($byteLen) = @_;

    my $buf;
    read(INF, $buf, $byteLen);

    $MLen += $byteLen+1;

#    print "buf($byteLen)=",unpack("H*",$buf),"\n";
    return ["data", $buf];
}


sub ReadBType5 { # byte (utf8?) string
    my ($objLen) = @_;

    my $buf;
    read(INF, $buf, $objLen);

    if (not defined $RStrings{$buf}) {
	$SLen += $objLen+1;
#	print "SLen ($buf) = ",$objLen+1,"\n";
	$RStrings{$buf} = 1;
    }
    else {
	$ILen -= CountIntSize($objLen);
    }

    $buf = pack "U0C*", unpack "C*", $buf; # mark as Unicode

#    print "$buf\n";
    return ["string", $buf];
}

sub ReadBType6 { # unicode string
    my ($objLen) = @_;

    my $buf;
    read(INF, $buf, 2*$objLen);

    if (not defined $RStrings{$buf}) {
	$SLen += 2*$objLen+1;
	$RStrings{$buf} = 1;
    }
    else {
	$ILen -= CountIntSize($objLen);
    }

#    print nice_string(decode("UTF-16BE", $buf));
    return ["ustring", decode("UTF-16BE", $buf)];
}

sub ReadBTypea { # array
    my ($objLen) = @_;

    my @array;

    # read objrefs
    my $buf;
    read(INF, $buf, $objLen * $ObjRefSize);
    my @objs = unpack(($ObjRefSize == 1 ? "C*" : "n*"), $buf);

    for (my $j = 0; $j < $objLen; ++$j) {
	my $obj = ReadBObjectAt($objs[$j]);
	$array[$j] = $obj;
    }

    ++$MLen;

#    print "array\n";
    return ["array", \@array];
}

sub ReadBTyped { # dictionary
    my ($objLen) = @_;
    my %dict;

#    print "dict\n";

    # read keys
    my $buf;
    read(INF, $buf, $objLen * $ObjRefSize);
    my @keys = unpack(($ObjRefSize == 1 ? "C*" : "n*"), $buf);

    # read objrefs
    read(INF, $buf, $objLen * $ObjRefSize);
    my @objs = unpack(($ObjRefSize == 1 ? "C*" : "n*"), $buf);

    for (my $j = 0; $j < $objLen; ++$j) {
#	print "\t= ";
	my $key = ReadBObjectAt($keys[$j])->[1];
#	print " -> ";
	my $obj = ReadBObjectAt($objs[$j]);
	$dict{$key} = $obj;
    }

    ++$MLen;

#    print "\n";
    return ["dict", \%dict];
}

sub ReadBObject {
    # get object type/size
    my $buf;
    if (read(INF, $buf, 1) != 1) {
	die "Didn't read type byte: $!";
    }
    my $objLen = unpack("C*", $buf) & 0xF;
    $buf = unpack("H*", $buf);
    my $objType = substr($buf, 0, 1);
    if ($objType ne "0" && $objLen == 15) {
	$objLen = ReadBObject()->[1];
    }

#   print "ReadBType $objType ( $objLen )\n";
    my $ans = eval "ReadBType" . $objType . "($objLen)";
    die "$@ in ReadBObject" if $@;

    return $ans;
}

sub ReadBObjectAt {
    my ($objNum) = @_;

#   print "#$objNum @(", $Offsets[$objNum],")= ";
    seek(INF, $Offsets[$objNum], SEEK_SET);
    return ReadBObject();
}

my ($Indent);

sub FixXMLString {
    my ($in) = @_;

    $in =~ s/&/&amp;/g;
    $in =~ s/</&lt;/g;
    $in =~ s/>/&gt;/g;

    return $in;
}

sub WriteXMLstring {
    my $oVal = $OVal;
    print OUTF "\t" x $Indent,"<string>",FixXMLString($oVal),"</string>\n";
}

sub WriteXMLtrue {
    print OUTF "\t" x $Indent,"<true/>\n";
}

sub WriteXMLfalse {
    print OUTF "\t" x $Indent,"<false/>\n";
}

sub WriteXMLint {
    my $oVal = $OVal;
    print OUTF "\t" x $Indent,"<integer>$oVal</integer>\n";
}

sub WriteXMLreal {
    my $oVal = $OVal;
    print OUTF "\t" x $Indent,"<real>";
    my $ans = sprintf "%.25f", $oVal;
    $ans =~ s/0+$//;
    print OUTF "$ans</real>\n";
}

sub WriteXMLdate { # note: CFDate is a double = # seconds since 1/1/2001 0:0:0 GMT
# Windows Perl epoch 1/1/1970 0:0:0 UTC so difference is 978307200
    my $oVal = $OVal;

    print "date $oVal\n";

    my ($fsec,$iVal) = POSIX::modf($oVal);

    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = gmtime(978307200+$iVal);
    print OUTF "\t" x $Indent,"<date>";
    printf OUTF "%04d-%02d-%02dT%02d:%02d:%.6fZ", $year+1900,$mon+1,$mday,$hour,$min,$sec+$fsec;
    print OUTF "</date>\n";
}

sub WriteXMLdata {
    my $oVal = $OVal;
    print "oVal=",unpack("H*",$oVal),"\n";
    my $ind = "\t" x $Indent;

    print OUTF "$ind<data>\n";
    my $buf = encode_base64($oVal,"");
    print "buf=\n$buf\n";
# limit lines to 76 chars, counting tabs as 8 chars
    my $len = 76 - 8 * $Indent;
    my @lines = unpack("(a$len)*",$buf);
    foreach (@lines) {
	print OUTF $ind,$_,"\n";
    }
    print OUTF $ind, "</data>\n";
}

sub WriteXMLustring {
    my $oVal = $OVal;
    $oVal =~ s/&/&amp;/g;
    $oVal =~ s/</&lt;/g;
    $oVal =~ s/>/&gt;/g;
    print OUTF "\t" x $Indent,"<ustring>$oVal</ustring>\n";
}

sub WriteXMLarray {
    my $oVal = $OVal;
    print OUTF "\t" x $Indent,"<array>\n";

    ++$Indent;
    foreach my $v (@$oVal) {
	WriteXML($v);
    }
    --$Indent;

    print OUTF "\t" x $Indent,"</array>\n";
}

sub WriteXMLdict {
    my $oVal = $OVal;

    print OUTF "\t" x $Indent,"<dict>\n";

    ++$Indent;
    foreach my $k (sort keys %$oVal) {
	print OUTF "\t" x $Indent,"<key>",FixXMLString($k),"</key>\n";
	WriteXML($oVal->{$k});
    }
    --$Indent;

    print OUTF "\t" x $Indent,"</dict>\n";
}

sub WriteXML {
    my ($obj) = @_;

    my $oType;
    ($oType, $OVal) = @$obj;

    if ($oType ne "") {
	eval "WriteXML" . $oType . "()";
	die "$@ in WriteXML" if $@;
    }
    else {
	die "$oType is null!";
    }
}

sub WriteTopXML {
    $Indent = 0;
    WriteXML(@_);
}

sub ConvertBinaryToXML {
    my ($filename) = @_;
    my $newname = NewName($filename, "binary", "text");

    print "BinaryToXML:\n";
    open(INF, $filename) or die "can't open $filename for conversion";
    binmode(INF);
    open(OUTF, ">:utf8", "$newname") or die "can't open $newname for output";

# get trailer
    seek(INF, -32, SEEK_END);
    my $buf;
    read(INF, $buf, 32);
    ($OffsetSize, $ObjRefSize, $NumObjects, $TopObject, $OffsetTableOffset) = unpack "x6CC(x4N)3", $buf;

    print "Offsets are $OffsetSize bytes\n";
    print "Object Refs are $ObjRefSize bytes\n";
    print "There are $NumObjects objects in the file\n";
    print "The top object is at $TopObject\n";
    print "The Offset Table is at offset $OffsetTableOffset\n\n";

# get the offset table
    seek(INF, $OffsetTableOffset, SEEK_SET);
    my $rawOffsetTable;
    my $readSize = read(INF, $rawOffsetTable, $NumObjects * $OffsetSize);
    if ($readSize != $NumObjects * $OffsetSize) {
      die "rawOffsetTable read $readSize expected ",$NumObjects * $OffsetSize;
    }

    @Offsets = unpack(["","C*","n*","(H6)*","N*"]->[$OffsetSize], $rawOffsetTable);
    if ($OffsetSize == 3) {
	@Offsets = map { hex($_) } @Offsets;
    }

    $ILen = 0;
    $MLen = 0;
    $SLen = 0;

    my $top = ReadBObjectAt($TopObject);

    print "Int Lengths = $ILen\n";
    print "Misc Lengths = $MLen\n";
    print "String Lengths = $SLen\n";
    print "Unique Strings = ",scalar keys %RStrings,"\n";

# write the XML header
    print OUTF "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    print OUTF "<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";
    print OUTF "<plist version=\"1.0\">\n";

# dump out the top object
    WriteTopXML($top);

# write the XML trailer
    print OUTF "</plist>\n";

    close(OUTF);
    close(INF);
}

sub Convert {

    print "plutil.pl $VERSION\n\n";

    my ($filename) = @ARGV;

    if (-s $filename < 8) {
	die "$filename is too short for a plist file";
    }

    # convert binary or text?

    open(INF, $filename) or die "can't open $filename";

    my $magic;
    read(INF, $magic, 8);
    close(INF);

    if (substr($magic, 0, 6) eq "bplist") { # convert binary plist to XML
	if (not ($magic =~ /00$/)) {
	    warn "Binary plist unknown version: " . substr($magic, -2);
	}
	ConvertBinaryToXML($filename);
    }
    else { # convert XML to binary plist
	ConvertXMLToBinary($filename);
    }
}

if ((@ARGV > 0) && (-e $ARGV[0])) {
    Convert($ARGV[0]);
}
else {
    print "usage: plutil file[.text|.binary][.plist|.strings|.*]\n";
}
