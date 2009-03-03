#!/bin/bash

. /etc/rc.conf
. /etc/rc.d/functions

PID=`pidof -o %PPID /usr/sbin/miredo-server`
case "$1" in
  start)
    stat_busy "Starting teredo protocol server"
    if [ -z "$PID" ]; then 
       /usr/sbin/miredo-server
    fi
    if [ ! -z "$PID" -o $? -gt 0 ]; then
      stat_fail
    else
      add_daemon miredo-server
      stat_done
    fi
    ;;
  stop)
    stat_busy "Stopping teredo protocol server"
    [ ! -z "$PID" ] && kill $PID &>/dev/null
    if [ $? -gt 0 ]; then
      stat_fail
    else
      rm_daemon miredo-server
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
