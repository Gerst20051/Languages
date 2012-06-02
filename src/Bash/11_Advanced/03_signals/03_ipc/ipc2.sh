#!/bin/sh

## Save this as ipc2.sh

function hup_handler()
{
	echo "SIGHUP RECEIVED."
	exit 0
}

trap hup_handler SIGHUP

while true ; do
	sleep 1
done

