#!/usr/bin/python
"""Extract list of URLs in a web page

This program is part of "Dive Into Python", a free Python book for
experienced programmers.  Visit http://diveintopython.org/ for the
latest version.
"""

__author__ = "Mark Pilgrim (mark@diveintopython.org)"
__version__ = "$Revision: 1.2 $"
__date__ = "$Date: 2004/05/05 21:57:19 $"
__copyright__ = "Copyright (c) 2001 Mark Pilgrim"
__license__ = "Python"

from sgmllib import SGMLParser
import sys

if len(sys.argv) != 2:
	print "\n\n+++++++++++++++++++++++++++++++++++++++++++++++++++++"
	print "Extract links form webpage - v.0.1            "
	print "+++++++++++++++++++++++++++++++++++++++++++++++++++++"
	print "\nUsage : ./list-urls.py <web-page>            "
	print "Eg: ./list-urls.py http://www.whoppix.net          "
	print "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++"
	sys.exit(1)



class URLLister(SGMLParser):
	def reset(self):
		SGMLParser.reset(self)
		self.urls = []

	def start_a(self, attrs):
		href = [v for k, v in attrs if k=='href']
		if href:
			self.urls.extend(href)

if __name__ == "__main__":

	import urllib
	print "\n##########################################################"
	print "# 							#"
	print "#	     Extract URLS from a web page		#"
	print "#		muts@whitehat.co.il			#"
	print "# 							#"
	print "##########################################################\n"
	link = sys.argv[1]
	try:
		usock = urllib.urlopen(link)
		parser = URLLister()
		parser.feed(usock.read())
		parser.close()
		usock.close()
		for url in parser.urls: print url
	except: 
		print "Could not reach "+ sys.argv[1]+ " !"
		print "Did you remember to put an http:// before the domain name?"
