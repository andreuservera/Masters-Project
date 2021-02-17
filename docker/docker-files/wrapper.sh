#!/bin/bash

service mysql start
#!/bin/sh
SERVICE=mysql;

if ps ax | grep -v grep | grep $SERVICE > /dev/null
then
    echo "$SERVICE service running, everything is fine"
else
    echo "$SERVICE is not running"
fi

while sleep 60; do
	if ps ax | grep -v grep | grep $SERVICE = /dev/null
	then
		echo "MySQL stopped..."		
		exit 1
	fi
done

