#!/usr/bin/env python2
#
# ./find_dns.py -l IPs.txt -t 500 -o dnsservers.txt
#
# dns-server finder by dash
# 
#
#./find_dns.py -l rIP.txt -t 100 
#[*] Found 1001 entries
#[*] Entries 1001 in queue
#[*] Running with 100 threads
#==================================================
#IP          NAME
#==================================================
#91.x.x.x   (x.info)
#191.x.x.x  (191.x.br)
#67.x.x.x   (name.info)
#==================================================
#[*] Done
#

import os
import sys
import time
import Queue
import struct
import socket
import random
import argparse
import threading

global rQ
rQ = Queue.Queue()

def openFile(hostList):
	fr = open(hostList,'r')
	rBuf = fr.readlines()
	return rBuf

def openWriteFile(outfile):
	fw = open(outfile,'wb')
	return fw

def parseDomain(domain):
	do = domain.split('.')
	if len(do) != 2:
		print '[!] Sorry, unknown domain type: %s\nExample:google.com' % (domain)
		return False
	tld = do[1]
	tld_len = struct.pack('>B', len(tld))
	tld_sub = do[0]
	tld_sub_len = struct.pack('>B', len(tld_sub))
	dom_pay = '%c%s%c%s' % (tld_sub_len,tld_sub,tld_len,tld)
	return dom_pay
	
	

def checkDNS(payload,host,resolv,debug,version):
	# settimeout so recv is not block
	rBuf_len = -1
	try:
		s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		s.settimeout(5)
		s.connect((host,53))
		s.send(payload)
		rBuf = s.recv(1024)
		rBuf_len = len(rBuf)
		name = ''
		# default we resolve IPs as long as -n is not choosen
		if resolv:
			try:
				name = socket.gethostbyaddr(host)[0]
			except socket.herror,e:
				pass

		if version:
			# FEFE packet!
			ver_req = '\xfe\xfe\x01 \x00\x01\x00\x00\x00\x00\x00\x01\x07version\x04bind\x00\x00\x10\x00\x03\x00\x00)\x10\x00\x00\x00\x00\x00\x00\x00'
			try:
				s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
				s.settimeout(3)
				s.connect((host,53))
				s.send(ver_req)
				vBuf = s.recv(1024)
			except socket.error,e:
				vBuf = ''
				pass
				

		if name == '':
			if debug:
				print '%s\t%d\t%s\t%s' % (host,rBuf_len,repr(rBuf),repr(vBuf))
				data = '%s\t%d\t%s\t%s\n' % (host,rBuf_len,repr(rBuf),repr(vBuf))
			else:
				print '%s\t%d' % (host,rBuf_len)
				data = '%s\t%d\n' % (host,rBuf_len)
		else:
			if debug:
				print '%s\t(%s) %d\t%s' % (host,name,rBuf_len,repr(rBuf))
				data = '%s\t(%s) %d\t%s\n' % (host,name,rBuf_len,repr(rBuf))
			else:
				print '%s\t(%s) %d' % (host,name,rBuf_len)
				data = '%s\t(%s) %d\n' % (host,name,rBuf_len)

		rQ.put(data)
	except socket.error,e:
#		print e
		pass
	return
		
def run(args):
	""" mighty mighty function """

	if not args.thrCnt:
		thrCnt=50
	else:
		thrCnt = int(args.thrCnt)
	
	if args.outfile:
		fw = openWriteFile(args.outfile)

	dom_pay = parseDomain(args.domain)
	payload = 'J\x8e\x01\x00\x00\x01\x00\x00\x00\x00\x00\x00%s\x00\x00\x01\x00\x01' % (dom_pay)
	
	hostList = args.hostList

	q = Queue.Queue()
	rBuf = openFile(hostList)
	print '[*] Found %d entries' % len(rBuf)
	for r in rBuf:
		r = r.rstrip('\n')
		r = r.rstrip('\r')
		q.put(r)

	print '[*] Entries %d in queue' % q.qsize()
	print '[*] Running with %d threads' % thrCnt
	print '='*50
	if args.resolv:
		print 'IP\t\tNAME\tPAYLEN'
	else:
		print 'IP\t\tPAYLEN'

	print '='*50
	thrList = []
	org_qlen = float(q.qsize())
	while True:
		
		#TODO percents calc
		#qlen = q.qsize()
		#cur_cnt = (qlen / org_qlen) * 100
		#cur_cnt = int(100 - cur_cnt)
		#if cur_cnt % 5 == 0 and cur_cnt != 0:
			#print '='*20+' %d ' % (cur_cnt)+'='*20

		if len(thrList) < thrCnt and q.qsize()>0:

			# enable random transaction ids
			if args.randTrans:
				rd = random.randint(0,65535)
				rd_pack = struct.pack('>H',rd)
				payload = '%s%s' % (rd_pack,payload[2:]) 

			thrDns = threading.Thread(target = checkDNS, args = (payload,q.get(),args.resolv,args.debug,args.version))
			thrDns.daemon = True
			thrDns.start()
			thrList.append(thrDns)
		
		for entry in thrList:
			if entry.isAlive()==False:
				entry.join()
				thrList.remove(entry)

		if args.outfile and rQ.qsize()>0:
			i = rQ.get()
			data = "%s" % (i)
			fw.write(data)
			fw.flush()
		else:
			if rQ.qsize()>0:
				rQ.get()

		if q.qsize()==0 and len(thrList) == 0:
			break
	
	if args.outfile:
		fw.close()
	print '='*50
	print '[*] Done'
	print '='*50
	

def main():
	parser_desc = 'dns server finder, by dash'
	prog_desc = 'find_dns.py'
	parser = argparse.ArgumentParser(	prog = prog_desc, description = parser_desc)
	parser.add_argument("-l",action='store',required=True,help='host list with ips',dest='hostList')
	parser.add_argument('-t',action='store',required=False,help='thread count', dest='thrCnt')
	parser.add_argument('-o',action='store',required=False,help='write found data to file', dest='outfile')
	parser.add_argument('-n',action='store_false',default=True,required=False,help='do not resolve ips', dest='resolv')
	parser.add_argument('-d',action='store',default='google.com',required=False,help='choose the domain for the dns request', dest='domain')
	parser.add_argument('-r',action='store_false',default=True,required=False,help='deactivate random transaction ids', dest='randTrans')
	parser.add_argument('-v',action='store_true',default=False,required=False,help='grab version from dns server enable debug mode for it! (experimental!)', dest='version')
	parser.add_argument('-V',action='store_true',default=False,required=False,help='print version information', dest='versinfo')
	parser.add_argument('--debug',action='store_true',default=False,required=False,help='debug output', dest='debug')

	args = parser.parse_args()
	# add some more info here sometime
	if args.versinfo:
		print desc
		sys.exit(23)

	run(args)

if __name__ == "__main__":
	main()
