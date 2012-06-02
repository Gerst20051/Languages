#!/bin/sh

function1()
{
    FROM=1
    while [ $FROM -lt 1000 ] ; do
	TO=`expr $FROM + 3`
	eval "FOO_$TO=\$FOO_$FROM"
	FROM=`expr $FROM + 1`
    done
}

function2()
{
    FROM=1
    TO=`expr $FROM + 3`
    while [ $FROM -lt 1000 ] ; do
	eval "FOO_$TO=\$FOO_$FROM"
	FROM=`expr $FROM + 1`
	TO=`expr $TO + 1`
    done
}

function3()
{
    FROM=1
    while [ $FROM -lt 1000 ] ; do
	TO=$(( $FROM + 3 ))
	eval "FOO_$TO=\$FOO_$FROM"
	FROM=$(( $FROM + 1 ))
    done
}

function4()
{
    FROM=1
    TO=$(( $FROM + 3 ))
    while [ $FROM -lt 1000 ] ; do
	eval "FOO_$TO=\$FOO_$FROM"
	FROM=$(( $FROM + 1 ))
	TO=$(( $TO + 1 ))
    done
}

function5()
{
    FROM=1
    while [ $FROM -lt 1000 ] ; do
	TO=`echo $FROM + 3 | bc`
	eval "FOO_$TO=\$FOO_$FROM"
	FROM=`echo $FROM + 1 | bc`
    done
}

function6()
{
    FROM=1
    TO=$(( $FROM + 3 ))
    while [ $FROM -lt 1000 ] ; do
	eval "FOO_$TO=\$FOO_$FROM"
	FROM=`echo $FROM + 1 | bc`
	TO=`echo $TO + 1 | bc`
    done
}

time function1
time function2
time function3
time function4
time function5
time function6

