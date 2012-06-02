#!/bin/sh
trap "" SIGINT

echo "This program will sleep for 10 seconds and cannot be killed with"
echo "control-c."
sleep 10

