#!/bin/sh

while true; do
    response=$(/usr/bin/mysqladmin -uUNKNOWN_USER ping 2>&1) && break
    echo "$response" | grep -q "mysqld is alive" && break
    echo "$response" | grep -q "Access denied for user" && break 
    sleep 1
done
