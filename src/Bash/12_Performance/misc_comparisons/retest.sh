#!/bin/sh

function1()
{
    LOOP=0
    while [ $LOOP -lt 1000 ] ; do
	VAL=`expr $LOOP % 2`
	if [ $VAL -eq 1 ] ; then
		echo "FOO" > /dev/null
	else
		echo "BAR" > /dev/null
	fi

	echo "This is a test." > /dev/null

	if [ $VAL -eq 1 ] ; then
		echo "ENDFOO" > /dev/null
	else
		echo "ENDBAR" > /dev/null
	fi
	LOOP=$((LOOP + 1))
    done
}
function2()
{
    LOOP=0
    while [ $LOOP -lt 1000 ] ; do
	VAL=`expr $LOOP % 2`
	if [ $VAL -eq 1 ] ; then
		echo "FOO" > /dev/null
		echo "This is a test." > /dev/null
		echo "ENDFOO" > /dev/null
	else
		echo "BAR" > /dev/null
		echo "This is a test." > /dev/null
		echo "ENDBAR" > /dev/null
	fi
	LOOP=$((LOOP + 1))
    done
}
function3()
{
    LOOP=0
    while [ $LOOP -lt 1000 ] ; do
	VAL=`expr $LOOP % 2`
	if [ -f retest.sh ] ; then
		echo "FOO" > /dev/null
	else
		echo "BAR" > /dev/null
	fi

	echo "This is a test." > /dev/null

	if [ -f retest.sh ] ; then
		echo "ENDFOO" > /dev/null
	else
		echo "ENDBAR" > /dev/null
	fi
	LOOP=$((LOOP + 1))
    done
}
function4()
{
    LOOP=0
    while [ $LOOP -lt 1000 ] ; do
	VAL=`expr $LOOP % 2`
	if [ -f retest.sh ] ; then
		echo "FOO" > /dev/null
		echo "This is a test." > /dev/null
		echo "ENDFOO" > /dev/null
	else
		echo "BAR" > /dev/null
		echo "This is a test." > /dev/null
		echo "ENDBAR" > /dev/null
	fi
	LOOP=$((LOOP + 1))
    done
}

time function1
time function2
time function3
time function4


