#!/bin/sh

SECONDS_TO_RUN=5
SECONDS_TO_PAUSE=20
handler() {
    killall -CONT ftpd
    exit 0
}

trap handler SIGHUP SIGTERM SIGQUIT SIGINT
# This must be run as root or the ftp user.
while true ; do
    killall -STOP ftpd
    sleep $SECONDS_TO_PAUSE
    killall -CONT ftpd
    sleep $SECONDS_TO_RUN
done

