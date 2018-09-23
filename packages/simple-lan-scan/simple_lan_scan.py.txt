#!/usr/bin/python

# !!! Install package python-scapy before you use it !!!

# Simple LAN Scanner
# by Valentin Hoebel (valentin@xenuser.org)
# Version 1.0 (08th January 2011)

# ASCII FOR BREAKFAST

# ---------- [Description]
# A very simple LAN scanner which shows MAC and IP addresses.

# ---------- [Features]
# - Shows you the MAC and IP of a running system.
# - Creates a small log file.

# ---------- [Usage example]
# sudo ./simple_lan_scan.py --network=192.168.1.0/24 

# ---------- [Tested with]
# - Python 2.6.5

# ---------- [Notes]
# - This tool was developed using a Python 2.6.5 interpreter.
# - Modify, distribute, share and copy this code in any way you like!
# - Please note that this tool was created and published for educational purposes only.

# Power to the cows!

import sys,  getopt
from scapy.all import srp,Ether,ARP,conf
from time import gmtime, strftime

def print_usage():
    print_banner()
    print "[!] Wrong argument and parameter passed. Use --help for more information."
    print "[!] Usage: sudo ./simple_lan_scan.py --network=<your network>"
    print "[i] Usage Example: sudo ./simple_lan_scan.py --network=192.168.1.0/24"
    print ""
    print ""
    sys.exit()
    return
    
def print_help():
    print_banner()
    print ""
    print "[Description]"
    print "The Simple LAN Scanner tries to show you"
    print "the MAC and IP addresses of all running systems"
    print "in your local network."
    print ""
    print "[Usage]"
    print "sudo ./simple_lan_scan.py --network=<your network>"
    print ""
    print "[Usage example]"
    print "sudo ./simple_lan_scan.py --network=192.168.1.0/24"
    print ""
    print "[Feature list]"
    print "- Shows you the MAC and IP of a running system."
    print "- Creates a small log file."
    print ""
    print "[Installation]"
    print "- Requires package python-scapy."
    print ""
    print "[Some notes]"
    print "- Tested with Python 2.6.5."
    print "- Modify, distribute, share and copy the code in any way you like!"
    print "- Please note that this tool was created for educational purposes only."
    print "- Do not use this tool in an illegal way. Know and respect your local laws."
    print "- Power to teh c0ws!"
    print ""
    print ""
    sys.exit()    
    return
    
def print_banner():
    print ""
    print ""
    print "Simple LAN Scanner"
    print "by Valentin Hoebel (valentin@xenuser.org)"
    print ""
    print "Version 1.0 (08th January 2011)"
    print "____________________________________________________"
    print ""
    return
    
def scan_lan(scan_network):
    conf.verb=0
    ans,unans=srp(Ether(dst="ff:ff:ff:ff:ff:ff")/ARP(pdst=scan_network),timeout=2)
    print ""
    
    log_file=strftime("%d_%b_%Y_%H:%M:%S_+0000", gmtime()) + "_-_scan.log"
    FILE = open(log_file,  "w")
    FILE.write("Simple LAN Scanner - Log File\n")
    FILE.write("----------------------------------------\n")
    FILE.write("[+] Scanned network: " + scan_network + "\n")
    FILE.write("[+] Scan time: " + strftime("%d %b %Y, %H:%M:%S +0000 GMT", gmtime()) + "\n")
    FILE.write("\n")
    
    for snd,rcv in ans:
        mac_address=rcv.sprintf("%Ether.src%")
        ip_address=rcv.sprintf("%ARP.psrc%")
        
        print rcv.sprintf("[+] Found a system! MAC: %Ether.src% IP: %ARP.psrc% ")
        FILE.write(ip_address + ", " + mac_address + "\n")
    
    FILE.write("\n")
    FILE.close
    print ""
    print "[i] Completed the scan. Exiting now!"
    print ""
    print ""
    return
    

def main(argv):
    scan_network=""
    
    try:
        opts,  args = getopt.getopt(sys.argv[1:],  "",  ["help",  "network="])
    except getopt.GetoptError   :
        print_usage()
        sys.exit(2)
    
    for opt,  arg in opts:
        if opt in ("--help"):
            print_help()
            break
            sys.exit(1)
        elif opt in ("--network") :
            scan_network=arg
            
    if len(scan_network) < 1:
        print_usage()
        sys.exit()
        
    print_banner()
    print "[i] Provided network to scan: " + scan_network
    scan_lan(scan_network)

if __name__ == "__main__":
    main(sys.argv[1:])
### EOF ###
