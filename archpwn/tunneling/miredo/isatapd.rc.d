#!/bin/bash

. /etc/rc.conf
. /etc/rc.d/functions

PID=`pidof -o %PPID /usr/sbin/isatapd`
case "$1" in
  start)
    stat_busy "Starting userspace ISATAP client"
    if [ -z "$PID" ]; then 
       /usr/sbin/isatapd
    fi
    if [ ! -z "$PID" -o $? -gt 0 ]; then
      stat_fail
    else
      add_daemon isatapd
      stat_done
    fi
    ;;
  stop)
    stat_busy "Stopping userspace ISATAP client"
    [ ! -z "$PID" ] && kill $PID &>/dev/null
    if [ $? -gt 0 ]; then
      stat_fail
    else
      rm_daemon isatapd
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
