#!/bin/sh

## Save this as ipc1.sh

./ipc2.sh &

PID=$!

sleep 1 # Give it time to launch.

kill -HUP $PID

