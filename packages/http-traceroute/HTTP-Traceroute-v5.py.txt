#!/usr/bin/env python

import getopt, sys, re, urllib2, urllib, BaseHTTPServer
from urllib2 import Request, urlopen, URLError, HTTPError

################## HEADER ###################################

#
# Traceroute-like HTTP scanner
# Using the "Max-Forwards" header
# RFC 2616 - HTTP/1.1 - Section 14.31
# RFC 3261 - SIP - Section 8.1.1.6
#

#
# By Nicolas Gregoire (nicolas.gregoire@agarri.fr)
#
# 0.5 : First public release
# 0.4 : Private release, looking for bugs - More heuristics
# 0.3 : A lot more options - More verbosity levels - Some heuristics
#
# By Julien Cayssol (tools@aqwz.com)
#
# 0.2 : Add extract of headers
# 0.1 : Initial version
#

#
# Heuristics :
# - Status Codes :
#	- HTTP Status Code == 502
#	- HTTP Status Code == 483
# - Specific data in body or headers :
#	- X-Forwarded-For in body when using TRACE
#	- Via or X-Via in headers
# - Differences between hops :
#	- HTTP Status Codes
#	- Server headers
#	- Content-Type headers
#	- Via headers
#	- HTML titles
#	- HTML <address> tags
#	- X-Forwarded-For values when using TRACE 
#

############## GLOBAL VARIABLES ###################################

global_data = { 'StatusCode':{}, 'Server':{} , 'Content-Type':{}, 'Title':{}, 'Address':{}, 'X-Fwd':{}, 'Via':{} }
score = 0
verbosity = 0
scheme = 'http'
host = '127.0.0.1'
port = '80'
path = '/'
method = 'TRACE'
body_content = None
max_fwds = 3
userAgent = 'Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.8.0.2) Gecko/20060502 Firefox/1.5.0.2'
contentType = 'text/html'

############## FUNCTIONS ###################################

# Pretty printing
def zprint(string, flag = '=='):
	print '[' + flag + '] ' + string

# Increment the heuristic score
def inc_score():
	global score
	score = score + 1
	if verbosity:
		zprint('Score : ' + str(score), '!!')

# Help
def showUsage():
    print 'Usage : ' + sys.argv[0] + ' [-h] [-m method] [-s scheme] [-t target] [-p port] [-P path] [-v 0|1|2] [-f forwards]'
    print '\t[-h] Help (this text)'
    print '\t[-m] HTTP Method : default is "TRACE"'
    print '\t[-s] Scheme : default is "http"'
    print '\t[-t] Target host : default is "127.0.0.1"'
    print '\t[-p] Port : default is "80"'
    print '\t[-P] Path : default is "/"'
    print '\t[-f] Max # of forwards : default is "3"'
    print '\t[-v] Verbosity : 0 = default, 1 = verbose, 2 = debug'
    print 'Examples :'
    print sys.argv[0] + ' -t www.example.org'
    print '  => TRACE /'
    print sys.argv[0] + ' -t www.example.org -m GET -s https -p 443 -v 1'
    print '  => GET / on a SSL host'
    print sys.argv[0] + ' -t www.example.org -m POST -P /axis2/checkacc -v 2 -f 5'
    print '  => Debug mode on a specific end-point'
    sys.exit(1)

# Parse CLI args
def getArguments():
    try:
	if len(sys.argv) < 2:
		zprint('No arguments ? Probably a bad choice. Use "-h" ...', '!!')
		sys.exit(1)
        optlist, list = getopt.getopt(sys.argv[1:], 'hm:s:t:p:P:v:f:')
    except getopt.GetoptError:
        showUsage()
    for opt in optlist:
        if opt[0] == '-h':
            showUsage()
        if opt[0] == '-m':
            global method
            method = opt[1]
        if opt[0] == '-s':
            global scheme
            scheme = opt[1]
        if opt[0] == '-t':
            global host
            host = opt[1]
        if opt[0] == '-p':
            global port
            port = opt[1]
        if opt[0] == '-P':
            global path
            path = opt[1]
        if opt[0] == '-v':
            global verbosity
            verbosity = int(opt[1])
        if opt[0] == '-f':
            global max_fwds
            max_fwds = int(opt[1])

# Extract some interesting data from the headers
def analyse_headers(data):

	if verbosity:
		print
		zprint('Analyzing headers', '**')

	wanted_headers = [
		'Server',
		'Via',
		'X-Via',
		'Set-Cookie',
		'X-Forwarded-For',
		'Content-Type',
		'Content-Length',
		'Last-Modified',
		'Location',
		'Date',
	]

	for h_name in wanted_headers:
		h_value = data.getheader(h_name)
		if h_value != None:
			# Print the value
			if verbosity:
				zprint(h_value, h_name)
			# Add it to the global structure if needed
			if h_name == 'Server' or h_name == 'Content-Type':
				global_data[h_name][hop] = h_value
			# Some heuristics
			if h_name == 'Via' or h_name == 'X-Via':
				zprint('"Via" header : Probably a reverse proxy', '++')
				global_data['Via'][hop] = h_value
				inc_score()

# Extract some interesting data from the body
def analyse_body(data):

	if verbosity:
		print 
		zprint('Analyzing body', '**')

	wanted_patterns = [
		'<title>(.*)</title>',
		'<address>(.*)</address>',
		'Reason: <strong>(.*)</strong>',
		'X-Forwarded-For: (.*)',
	]

	for p_name in wanted_patterns:
		# Case insensitive search
		p_value = re.search(p_name, data, re.IGNORECASE)
		if p_value != None:
			# Only the 1st group, without newlines
			value =  p_value.groups()[0].strip('\r\n')
			if verbosity:
				zprint(value, p_name)
			# Add it to the global structure if needed
			if p_name == '<title>(.*)</title>':
				global_data['Title'][hop] = value
			if p_name == '<address>(.*)</address>':
				global_data['Address'][hop] = value
			# Some heuristics
			if re.search('X-Forwarded-For:' , p_name):
				global_data['X-Fwd'][hop] = value
				if method == 'TRACE':
					zprint('"X-Forwarded-For" in body when using TRACE : Probably a reverse proxy', '++')
					inc_score()

# Analyse the data returned by urllib2.*open()
def debug_and_parse(data):

	# Get data
	headers = data.info()
	body = data.read()

	# Debug
	if verbosity == 2:
		zprint(str(headers), 'DEBUG HEADERS')
		zprint(str(body), 'DEBUG BODY')

	# Extract some intersting info
	codes = BaseHTTPServer.BaseHTTPRequestHandler.responses
	global_data['StatusCode'][hop] =  str(data.code) + ' ' + codes[data.code][0]
	analyse_headers(headers)
	analyse_body(body)

############## SCAN ###################################

# Init
getArguments()

# Current target
url = scheme + '://' + host + ':' + port + path
zprint('Target URL : ' + url)
zprint('Used method : ' + method)
zprint('Max number of hops : ' + str(max_fwds))

# Scan
for hop in range(0, max_fwds):

	# Create the request object
	request = urllib2.Request(url)
	request.get_method = lambda: method
	request.add_data(body_content)
	request.add_header('Content-Type', contentType)
	request.add_header('User-agent', userAgent)

	# Add the 'Max-Forwards' header
	request.add_header('Max-Forwards', hop)
	if verbosity:
		print('-' * 80)
		zprint('Current value of "Max-Forwards" = ' + str(hop) + ' [' + '-' * 20 + ']', '-' * 19)
		print('-' * 80)

	try:
		# Do the HTTP request
		opener = urllib2.build_opener(urllib2.HTTPHandler)
		result = opener.open(request)

		# Found something
		if verbosity:
			zprint('Status Code => HTTP 200: OK', '**')

		# Analyse it
		debug_and_parse(result)

	# Not a 200 OK
	except HTTPError, e:
		if verbosity:
			zprint('Status Code => ' + str(e), '**')
		# Some heuristics
		if e.code == 502:
			zprint('HTTP 502 : Probably a reverse proxy', '++')
			inc_score()
		if e.code == 483:
			zprint('HTTP 483 : Probably a reverse proxy (SIP ?)', '++')
			inc_score()

		# Analyse it
		debug_and_parse(e)

	# Network problem
	except URLError, e:
		zprint('Network problem !', '!!')
		zprint('Reason : ' + str(e.reason), '!!')
		break

############## REPORT ###################################

print('-' * 80)
zprint('Heuristic Report [' + '-' * 31 + ']', '-' * 27)
print('-' * 80)

# For each key
for k in global_data.keys():
	string = k + ':\n'
	previous = 'Undef'
	# For each hop
	ok = 0
	for i in range(0, max_fwds):
		# Try this key
		try:
			current = global_data[k][i]
			# We got a value !
			ok = 1
		except KeyError:
			current = 'Undef'

		# Some heuristics
		if previous != current and i > 0:
			inc_score()

		# Then add it to the current string
		string = string + '\tHop #' + str(i) + " : " + current + '\n'
		previous = current 

	# Display this key only if values were found
	if ok:
		print string

# Final score
if score == 0:
	zprint('No reverse proxy', '--')
else:
	zprint('Found a reverse proxy, score is ' + str(score), '++')

