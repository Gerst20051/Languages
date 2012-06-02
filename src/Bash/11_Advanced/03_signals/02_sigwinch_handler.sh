#!/bin/sh

function fixrows()
{
	echo "Window size changed."
}

echo "Adjust the size of your window now."
trap fixrows SIGWINCH

COUNT=0
while [ $COUNT -lt 60 ] ; do
	COUNT=$(($COUNT + 1))
	sleep 1
done
