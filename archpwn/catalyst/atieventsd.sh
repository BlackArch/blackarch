#!/bin/bash

. /etc/rc.conf
. /etc/rc.d/functions

ATID_PID=`pidof -o %PPID /usr/sbin/atieventsd`

case "$1" in
  start)
    rc=0
    stat_busy "Starting ATI Events Daemon"
    [ -z "$ATID_PID" ] && /usr/sbin/atieventsd
    rc=$(($rc+$?))
    if [ $rc -gt 0 ]; then
      stat_fail
    else
      add_daemon atieventsd
      stat_done
    fi
    ;;
  stop)
    rc=0
    stat_busy "Stopping ATI Events Daemon"
    kill $ATID_PID &>/dev/null
    rc=$(($rc+$?))
    if [ $rc -gt 0 ]; then
      stat_fail
    else
      rm_daemon atieventsd
      stat_done
    fi
    ;;
  restart)
    $0 stop
    sleep 1
    $0 start
    ;;
  *)
    echo "usage: $0 {start|stop|restart}"  
esac
exit 0
