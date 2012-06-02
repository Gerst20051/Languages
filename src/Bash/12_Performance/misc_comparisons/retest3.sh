#!/bin/sh

function1()
{
    LOOP=0
    echo "elif"
    while [ $LOOP -lt 1000 ] ; do
	VAL=`expr $LOOP % 10`

	if [ $VAL -eq 0 ] ; then
		echo "ZERO" > /dev/null
	elif [ $VAL -eq 1 ] ; then
		echo "ONE" > /dev/null
	elif [ $VAL -eq 2 ] ; then
		echo "TWO" > /dev/null
	elif [ $VAL -eq 3 ] ; then
		echo "THREE" > /dev/null
	elif [ $VAL -eq 4 ] ; then
		echo "FOUR" > /dev/null
	elif [ $VAL -eq 5 ] ; then
		echo "FIVE" > /dev/null
	elif [ $VAL -eq 6 ] ; then
		echo "SIX" > /dev/null
	elif [ $VAL -eq 7 ] ; then
		echo "SEVEN" > /dev/null
	elif [ $VAL -eq 8 ] ; then
		echo "EIGHT" > /dev/null
	elif [ $VAL -eq 9 ] ; then
		echo "NINE" > /dev/null
	fi

	LOOP=$((LOOP + 1))
    done
}
function2()
{
    LOOP=0
    echo "if"
    while [ $LOOP -lt 1000 ] ; do
	VAL=`expr $LOOP % 10`
	if [ $VAL -eq 0 ] ; then
		echo "ZERO" > /dev/null
	fi
	if [ $VAL -eq 1 ] ; then
		echo "ONE" > /dev/null
	fi
	if [ $VAL -eq 2 ] ; then
		echo "TWO" > /dev/null
	fi
	if [ $VAL -eq 3 ] ; then
		echo "THREE" > /dev/null
	fi
	if [ $VAL -eq 4 ] ; then
		echo "FOUR" > /dev/null
	fi
	if [ $VAL -eq 5 ] ; then
		echo "FIVE" > /dev/null
	fi
	if [ $VAL -eq 6 ] ; then
		echo "SIX" > /dev/null
	fi
	if [ $VAL -eq 7 ] ; then
		echo "SEVEN" > /dev/null
	fi
	if [ $VAL -eq 8 ] ; then
		echo "EIGHT" > /dev/null
	fi
	if [ $VAL -eq 9 ] ; then
		echo "NINE" > /dev/null
	fi

	LOOP=$((LOOP + 1))
    done
}
function3()
{
    LOOP=0
    echo "case"
    while [ $LOOP -lt 1000 ] ; do
	VAL=`expr $LOOP % 10`
	case "$VAL" in
		( 0 ) echo "ZERO" > /dev/null ;;
		( 1 ) echo "ONE" > /dev/null ;;
		( 2 ) echo "TWO" > /dev/null ;;
		( 3 ) echo "THREE" > /dev/null ;;
		( 4 ) echo "FOUR" > /dev/null ;;
		( 5 ) echo "FIVE" > /dev/null ;;
		( 6 ) echo "SIX" > /dev/null ;;
		( 7 ) echo "SEVEN" > /dev/null ;;
		( 8 ) echo "EIGHT" > /dev/null ;;
		( 9 ) echo "NINE" > /dev/null ;;
	esac

	LOOP=$((LOOP + 1))
    done
}

function print_0()
{
	echo "ZERO" > /dev/null
}
function print_1()
{
	echo "ONE" > /dev/null
}
function print_2()
{
	echo "TWO" > /dev/null
}
function print_3()
{
	echo "THREE" > /dev/null
}
function print_4()
{
	echo "FOUR" > /dev/null
}
function print_5()
{
	echo "FIVE" > /dev/null
}
function print_6()
{
	echo "SIX" > /dev/null
}
function print_7()
{
	echo "SEVEN" > /dev/null
}
function print_8()
{
	echo "EIGHT" > /dev/null
}
function print_9()
{
	echo "NINE" > /dev/null
}

function4()
{
    LOOP=0
    echo "eval"
    while [ $LOOP -lt 1000 ] ; do
	VAL=`expr $LOOP % 10`

	eval "print_$VAL"

	LOOP=$((LOOP + 1))
    done
}

time function1
time function2
time function3
time function4


