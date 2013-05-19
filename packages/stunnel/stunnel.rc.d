#!/bin/bash

. /etc/rc.conf
. /etc/rc.d/functions

pidfile=/var/run/stunnel/stunnel.pid
if [[ -r $pidfile ]]; then
  read -r PID < "$pidfile"
  if [[ $PID && ! -d /proc/$PID ]]; then
    # stale pidfile
    unset PID
    rm -f "$pidfile"
  fi
fi

case "$1" in
  start)
    stat_busy "Starting stunnel"
    if [[ -z $PID ]] && /usr/bin/stunnel; then
      add_daemon stunnel
      stat_done
    else
      stat_fail
      exit 1
    fi
    ;;
  stop)
    stat_busy "Stopping stunnel"
    if [[ $PID ]] && kill $PID &>/dev/null; then
      rm_daemon stunnel
      stat_done
    else
      stat_fail
      exit 1
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
