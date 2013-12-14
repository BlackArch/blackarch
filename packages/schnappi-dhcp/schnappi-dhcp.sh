#!/bin/bash
#
# Copyright (C) 2009 Emanuele Gentili < emgent@remote-exploit.org >
#
# This program is released under the terms of the GNU General Public License
# (GPL), which is distributed with this software in the file "COPYING".
# The GPL specifies the terms under which users may copy and use this software.
#
# schnappi-dhcp V 0.1
#
 
show_help(){
  echo ""
  echo " 2009 (C) schnappi-dhcp alias Schnappi can fuck network with no DHCP"
  echo ""
  echo " --usage    show the software Usage"
  echo " --version  show the software Version"
  echo " --whois    show me who is schnappi"
  echo " --dnsadd   add openDNS in your system"
  echo " --help     show the Help"
  echo ""
  echo "Emanuele Gentili <emgent@remote-exploit.org>"
}

show_whois(){
  echo "Hello, I'm Schnappi"
  echo "Check Your browser."
firefox http://www.youtube.com/watch?v=Oe3FG4EOgyU & > /dev/null
}

show_version(){
  echo "snappy-dhcp 0.1"
}

show_usage(){
  echo "usage $0 --interface wlan0 --IPrange 192.168.1 --IPtest 192.168.1.4"
}

add_dnsadd(){
  echo "[-] Adding open DNS"
  echo "nameserver 208.67.222.222" > /etc/resolv.conf
  echo "nameserver 208.67.220.220" >> /etc/resolv.conf
  echo "[*] DNS Added."
}

# Bash
while [[ $# != 0 ]]; do
    arg_name=$1; shift
    case "$arg_name" in
      --help|-?|-h) show_help; exit 0;;
      --whois) show_whois; exit 0;;
      --version) show_version; exit 0;;
      --usage) show_usage; exit 0;;
      --dnsadd) add_dnsadd; exit 0;;
      --interface) interface=$1; shift;;
      --IPrange) IPrange=$1; shift;;
      --IPtest) IPtest=$1; shift;;
      *) echo "invalid option: $1"; show_help;exit 1;;
    esac
done

[ -z "$interface" ] && { show_help; exit 1; }


ifconfig $interface $IPtest up ;
for gw in $(nmap -n -sP $IPrange.{1..255} | awk '/appears to be up/{print $2}') $IPrange.{1..255}; do
route del -net default;
route add -net default gw "$gw";
echo ""
echo "PING TEST"
echo ""
ping -c 2 208.67.222.222 && break;
done
echo ""
echo "NETWORK RESUME"
echo "[*] YOUR IP: $IPtest"
echo "[*] YOUR Gateway IP: $gw"
echo ""
