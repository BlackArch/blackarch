#!/bin/bash

. /etc/rc.conf
. /etc/rc.d/functions

PID=`pidof -o %PPID /usr/sbin/miredo`
case "$1" in
  start)
    stat_busy "Starting teredo protocol client"
    if [ -z "$PID" ]; then 
       /usr/sbin/miredo
    fi
    if [ ! -z "$PID" -o $? -gt 0 ]; then
      stat_fail
    else
      add_daemon miredo
      stat_done
    fi
    ;;
  stop)
    stat_busy "Stopping teredo protocol client"
    [ ! -z "$PID" ] && kill $PID &>/dev/null
    if [ $? -gt 0 ]; then
      stat_fail
    else
      rm_daemon miredo
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
