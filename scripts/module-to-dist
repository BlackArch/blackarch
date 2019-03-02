#!/usr/bin/perl
use warnings;
use strict;
use v5.10;

package Modules;

use HTTP::Tiny qw();

sub cpan_provider
{
    my ($module) = @_;
    my $url = "http://cpanmetadb.plackperl.org/v1.0/package/$module";
    my $http = HTTP::Tiny->new;
    my $resp = $http->get($url);
    return undef unless $resp->{'success'};

    my ($cpanpath) = $resp->{'content'} =~ /^distfile: (.*)$/m
        or return undef;

    my $dist = $cpanpath;
    $dist =~ s{\A.+/}{};    # remove author directory
    $dist =~ s{-[^-]+\z}{}; # remove version and extension
	$dist =~ s/-/::/g;
    return ($dist eq 'perl' ? undef : $dist);
}

package main;

my %seen = ();
my @modules;

if (@ARGV == 0) {
	# no args? read from stdin
	@modules = <>;
} else {
	@modules = @ARGV;
}

for my $module (@modules) {
	chomp($module);
	next if ($module eq "");

	my $dist = Modules::cpan_provider $module;

	next unless $dist; # ignore undef
	next if $seen{$dist}++; # ignore duplicates

	say $dist;
}

