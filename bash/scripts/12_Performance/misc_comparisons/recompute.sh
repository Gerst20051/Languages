#!/bin/sh

function1()
{
    LOOP=0
    while [ $LOOP -lt 1000 ] ; do
	FOO="$(echo "`expr $LOOP + 3` is a big number.")"
	BAR="$(echo "`expr $LOOP + 3` is stll a big number.")"
	LOOP=$((LOOP + 1))
    done
}
function2()
{
    LOOP=0
    while [ $LOOP -lt 1000 ] ; do
	local LOOP_PLUS_3=`expr $LOOP + 3`
	FOO="$(echo "$LOOP_PLUS_3 is a big number.")"
	BAR="$(echo "$LOOP_PLUS_3 is stll a big number.")"
	LOOP=$((LOOP + 1))
    done
}
function3()
{
    LOOP=0
    while [ $LOOP -lt 1000 ] ; do
	FOO="$(echo "$(( $LOOP + 3 )) is a big number.")"
	BAR="$(echo "$(( $LOOP + 3 )) is stll a big number.")"
	LOOP=$((LOOP + 1))
    done
}
function4()
{
    LOOP=0
    while [ $LOOP -lt 1000 ] ; do
	local LOOP_PLUS_3=$(( $LOOP + 3 ))
	FOO="$(echo "$LOOP_PLUS_3 is a big number.")"
	BAR="$(echo "$LOOP_PLUS_3 is stll a big number.")"
	LOOP=$((LOOP + 1))
    done
}

time function1
time function2
time function3
time function4


