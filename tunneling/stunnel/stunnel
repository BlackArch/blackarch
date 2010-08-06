#!/bin/bash

. /etc/rc.conf
. /etc/rc.d/functions

PID=`pidof -o %PPID /usr/bin/stunnel`
case "$1" in
  start)
    stat_busy "Starting stunnel"
    [ -z "$PID" ] && /usr/bin/stunnel
    if [ $? -gt 0 ]; then
      stat_fail
    else
      add_daemon stunnel
      stat_done
    fi
    ;;
  stop)
    stat_busy "Stopping stunnel"
    [ ! -z "$PID" ]  && kill $PID &> /dev/null
    if [ $? -gt 0 ]; then
      stat_fail
    else
      rm_daemon stunnel
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
