#!/bin/bash

# Parts of this code use BASH extensions.  These
# parts are noted with comments.

function jobidfromstring()
{
	local STRING;
	local RET;

	STRING=$1;
	RET="$(echo $STRING | sed 's/^[^0-9]*//' | sed 's/[^0-9].*$//')"

	echo $RET;
}

function delayprint()
{
	local TIME;
	TIME=$1
	echo "Sleeping for $TIME seconds."
	sleep $TIME
	echo "Done sleeping for $TIME seconds."
}

# Use the job ID for this one.
# The jobs builtin is a BASH extension.
delayprint 3 &
DP3=`jobidfromstring $(jobs %%)`

# Use the process ID this time.
# This should work with any bourne shell.
delayprint 5 &
DP5=$!

# The jobs builtin is a BASH extension.
delayprint 7 &
DP7=`jobidfromstring $(jobs %%)`

echo "Waiting for job $DP3";
wait %$DP3

echo "Waiting for process ID $DP5";
wait $DP5

echo "Waiting for job $DP7";
wait %$DP7

echo "Done."

