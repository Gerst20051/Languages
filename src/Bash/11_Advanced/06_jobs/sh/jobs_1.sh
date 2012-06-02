#!/bin/sh

function delayprint()
{
	local TIME;
	TIME=$1
	echo "Sleeping for $TIME seconds."
	sleep $TIME
	echo "Done sleeping for $TIME seconds."
}

delayprint 3 &
delayprint 5 &
delayprint 7 &
wait

