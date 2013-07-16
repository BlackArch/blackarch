#!/bin/bash

. /etc/rc.conf
. /etc/rc.d/functions

PID=`pidof -o %PPID /usr/sbin/radiusd`
case "$1" in
  start)
    stat_busy "Starting radiusd Server"

    [ -d /var/run/radiusd ] || mkdir -p /var/run/radiusd
    [ -f /var/run/radiusd/radiusd.pid ] || touch /var/run/radiusd/radiusd.pid
    chown -R radiusd.radiusd /var/run/radiusd

    [ -z "$PID" ] && /usr/sbin/radiusd
    if [ $? -gt 0 ]; then
      stat_fail
    else
      echo $PID > /var/run/radiusd.pid
      add_daemon radiusd
      stat_done
    fi
    ;;
  stop)
    stat_busy "Stopping radiusd Server"
    [ ! -z "$PID" ]  && kill $PID &> /dev/null
    if [ $? -gt 0 ]; then
      stat_fail
    else
      rm /var/run/radiusd.pid
      rm_daemon radiusd
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
