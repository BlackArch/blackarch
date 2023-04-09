#!/bin/bash
# baraction.sh for spectrwm status bar
# From http://wiki.archlinux.org/index.php/Scrotwm

SLEEP_SEC=5
#loops forever outputting a line every SLEEP_SEC secs
while :; do

 	eval $(cat /proc/acpi/battery/BAT0/state | awk '/charging state/ {printf "BAT_CHGSTATE=%s;", $3}; /remaining capacity/ {printf "BAT_REMNG=%s;",$3}; /present rate/ {printf "BAT_RATE=%s;",$3};' -)
 	eval $(cat /proc/acpi/battery/BAT0/info | awk '/present/ {printf "BAT_PRESENT=%s;", $2}; /last full capacity/ {printf "BAT_LASTFULL=%s;",$4};' -)
 
	BAT_REMNG_WH=`echo "scale=1; a=($BAT_REMNG+50)/1000; print a" | bc -l`
	BAT_RATE_W=`echo "scale=1; a=($BAT_RATE+50)/1000; print a" | bc -l`
	BCSCRIPT="scale=0; a=(100*$BAT_REMNG / $BAT_LASTFULL); print a"
	BAT_PCT=`echo $BCSCRIPT | bc -l`%

	case $BAT_PRESENT in
		no)
		POWER_STR="AC, NO BATTERY"
		;;
		yes)

		case $BAT_CHGSTATE in
			charged)
			#on ac
			AC_STATUS="ON AC"
			TIME_REMNG="N/A"
			POWER_STR="$AC_STATUS $BAT_CHGSTATE $BAT_PCT"
			;;
			charging)
			#on ac
			AC_STATUS="ON AC"
			BCSCRIPT="scale=1; a=(60*($BAT_LASTFULL - $BAT_REMNG) / $BAT_RATE); print a"
			TIMETOFULL_MIN=`echo $BCSCRIPT | bc -l`
			POWER_STR="$AC_STATUS $BAT_CHGSTATE $BAT_PCT C="$BAT_REMNG_WH"Wh Rate="$BAT_RATE_W"W TTF="$TIMETOFULL_MIN"min"
			;;
			discharging)
			AC_STATUS="ON BATT"
			TIME_REMNG_MIN=`echo "scale=0; a=(60*$BAT_REMNG / $BAT_RATE); print a" | bc -l`
			TIME_REMNG_HH=`echo "scale=0; a=($BAT_REMNG / $BAT_RATE); if (a<10) {print "0"; print a} else {print a}" | bc -l`

			TIME_REMNG_MM=`echo "scale=0; a=($TIME_REMNG_MIN-60*$TIME_REMNG_HH); if (a<10) {print "0"; print a} else {print a}" | bc -l`
			POWER_STR="$AC_STATUS $BAT_PCT C="$BAT_REMNG_WH"Wh P="$BAT_RATE_W"W R="$TIME_REMNG_HH":"$TIME_REMNG_MM
			;;
			*)
			POWER_STR=$BAT_CHGSTATE
			;;
		esac

		;;
		*)
		POWER_STR="error"
		;;
	esac

	#spectrwm bar_print can't handle UTF-8 characters, such as degree symbol
	#Core 0:      +67.0°C  (crit = +100.0°C)
	eval $(sensors 2>/dev/null | sed s/[°+]//g | awk '/^Core 0/ {printf "CORE0TEMP=%s;", $3}; /^Core 1/ {printf "CORE1TEMP=%s;",$3}; /^fan1/ {printf "FANSPD=%s;",$2};' -)
	TEMP_STR="Tcpu=$CORE0TEMP,$CORE1TEMP F=$FANSPD"

	WLAN_ESSID=$(iwconfig wlan0 | awk -F "\"" '/wlan0/ { print $2 }')
	eval $(cat /proc/net/wireless | sed s/[.]//g | awk '/wlan0/ {printf "WLAN_QULTY=%s; WLAN_SIGNL=%s; WLAN_NOISE=%s", $3,$4,$5};' -)
	BCSCRIPT="scale=0;a=100*$WLAN_QULTY/70;print a"
	WLAN_QPCT=`echo $BCSCRIPT | bc -l`
	WLAN_POWER=`iwconfig 2>/dev/null| grep "Tx-Power"| awk {'print $4'}|sed s/Tx-Power=//`
	WLAN_STR="$WLAN_ESSID: Q=$WLAN_QPCT% S/N="$WLAN_SIGNL"/"$WLAN_NOISE"dBm T="$WLAN_POWER"dBm"

	CPUFREQ_STR=`echo "Freq:"$(cat /proc/cpuinfo | grep 'cpu MHz' | sed 's/.*: //g; s/\..*//g;')`
	CPULOAD_STR="Load:$(uptime | sed 's/.*://; s/,//g')"

	eval $(awk '/^MemTotal/ {printf "MTOT=%s;", $2}; /^MemFree/ {printf "MFREE=%s;",$2}' /proc/meminfo)
	MUSED=$(( $MTOT - $MFREE ))
	MUSEDPT=$(( ($MUSED * 100) / $MTOT ))
	MEM_STR="Mem:${MUSEDPT}%"

	echo -e "$POWER_STR  $TEMP_STR  $CPUFREQ_STR  $CPULOAD_STR  $MEM_STR  $WLAN_STR"
        #alternatively if you prefer a different date format
        #DATE_STR=`date +"%H:%M %a %d %b`
	#echo -e "$DATE_STR   $POWER_STR  $TEMP_STR  $CPUFREQ_STR  $CPULOAD_STR  $MEM_STR  $WLAN_STR"

	sleep $SLEEP_SEC
done
