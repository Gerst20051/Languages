#!/bin/sh

# This is a handy way to find out the absolute path of the current script.
SCRIPT="$(which $0)"
if [ "x$(echo $SCRIPT | grep '^\/')" = "x" ] ; then
    SCRIPT="$PWD/$SCRIPT"
fi

function1()
{
    LOOP=0
    while [ $LOOP -lt 1000 ] ; do
	echo "This is a test." > /dev/null
	LOOP=$((LOOP + 1))
    done
}
function2()
{
    LOOP=0
    while [ $LOOP -lt 1000 ] ; do
	/bin/echo "This is a test." > /dev/null
	LOOP=$((LOOP + 1))
    done
}
function3()
{
    LOOP=0
    while [ $LOOP -lt 1000 ] ; do
	printf "This is a test." > /dev/null
	LOOP=$((LOOP + 1))
    done
}
function4()
{
    LOOP=0
    while [ $LOOP -lt 1000 ] ; do
	/usr/bin/printf "This is a test." > /dev/null
	LOOP=$((LOOP + 1))
    done
}

if [ "x$1" != "x" ] ; then
	eval "$1"
	exit
fi


# These have to be run externally because you can't use /usr/bin/time to
# compute the time of a function and most shells don't have a builtin version
# of "time".  In BASH, you can omit the "$SCRIPT" part and the timing will
# still work.

echo "echo (builtin)"
time "$SCRIPT" function1

echo
echo "echo (external)"
time "$SCRIPT" function2

echo
echo "printf (builtin)"
time "$SCRIPT" function3

echo
echo "printf (external)"
time "$SCRIPT" function4


