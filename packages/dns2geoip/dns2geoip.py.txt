#!/usr/bin/env python
#Packaged dependencies

#apt-get install python-geoip python-dnspython python-argparse

#Unpackaged dependencies

#simplekml

#hg clone https://code.google.com/p/simplekml/

#mv simplekml simplekml.hg

#mv simplekml.hg/simplekml simplekml

#rm -Rf simplekml.hg

 

#City resolution GeoLite database

#wget http://geolite.maxmind.com/download/geoip/database/GeoLiteCity.dat.gz

#gunzip GeoLiteCity.dat.gz

#sudo mv GeoLiteCity.dat /usr/share/GeoIP/GeoIPCity.dat
#special thanks to andrew king for helping me turn a ugly hack that didn't work into clean code that carries out a useful function
#this tool geolocates subdomains for any given domain i reccomend you use it to scope out a network 
#to see which services they run on site and which they run offiste
import GeoIP

import dns.resolver

import argparse

import simplekml

 

class DnsDictSearch:

    def __init__(self, path_to_wordlist):

        self.kml = simplekml.Kml()

        self.path_to_wordlist = path_to_wordlist
        self.gi = GeoIP.open(path_to_dat,GeoIP.GEOIP_STANDARD)
    def to_kml(self, coords, out):

        for coord in coords:

            self.kml.newpoint(name=coord[0],

                         coords=[coord[1]]

                         )

        self.kml.save(out)

 

    def brute(self, domain, path_to_dat, path_to_output):

        coords = []

    

        with open(self.path_to_wordlist, "r") as lines:

            try:

                for line in lines.readlines():

                    fulldomain = line.rstrip() + "." + domain

                    try:

                        answers = dns.resolver.query(fulldomain, 'A')

                        if type(answers) == dns.resolver.Answer:

                            for rdata in answers:

                                ip = rdata.address

                                gi = GeoIP.open(path_to_dat,GeoIP.GEOIP_STANDARD)

                                go = gi.record_by_addr(ip)

                                coord = (go['latitude'], go['longitude'])

                                coords.append([fulldomain, coord])

                    except:

                        pass    

            except (dns.exception.DNSException):

                pass

        self.to_kml(coords, path_to_output)  

        

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description=''+\

        'brute forces subdomains and then geolocates the subdomains it finds')

    parser.add_argument('-D',

                         action='store',

                         dest='domain',

                         help='the domain you wish to analyze',

                         default='This is not a domain')

    parser.add_argument('-w',

                        action='store',

                        dest='path_to_wordlist',

                        help='the full path to the list of possible subdomains'+\

                        ' for bruteforce',

                         default='This is not a wordlist')

    parser.add_argument('-x',

                        action='store',

                        dest='path_to_dat',

                        help='the path to the max mind geoip dat file'+\

                        ' (/usr/share/GeoIP/GeoIPCity.dat being the default)',

                        default ='/usr/share/GeoIP/GeoIPCity.dat')

    parser.add_argument('-o',

                        action='store',

                        dest='path_to_output',

                        help='saves subdomain locations to a kml file',

                        default='output.kml')

    results = parser.parse_args()

    run_warnings = []

    if results.domain != 'This is not a domain':

        domainarg = results.domain

    else:

        run_warnings.append("You need to specify a domain with -D")

    if results.path_to_wordlist != 'This is not a wordlist':

        wordarg = results.path_to_wordlist

    else:

        run_warnings.append("You need to specify a wordlist with -w")

    datarg = results.path_to_dat

    kmlarg = results.path_to_output

    if run_warnings.__len__() > 0:

        print("You failed to supply some required arguments")

        for warn in run_warnings:

            print(warn)

    else:

        this = DnsDictSearch(wordarg)

        this.brute(domainarg,datarg,kmlarg)
