#!/bin/sh

function1()
{
    LOOP=0
    echo "elif"
    while [ $LOOP -lt 1000 ] ; do
	VAL=`expr $LOOP % 50`

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
	elif [ $VAL -eq 10 ] ; then
		echo "ZERO" > /dev/null
	elif [ $VAL -eq 11 ] ; then
		echo "ONE" > /dev/null
	elif [ $VAL -eq 12 ] ; then
		echo "TWO" > /dev/null
	elif [ $VAL -eq 13 ] ; then
		echo "THREE" > /dev/null
	elif [ $VAL -eq 14 ] ; then
		echo "FOUR" > /dev/null
	elif [ $VAL -eq 15 ] ; then
		echo "FIVE" > /dev/null
	elif [ $VAL -eq 16 ] ; then
		echo "SIX" > /dev/null
	elif [ $VAL -eq 17 ] ; then
		echo "SEVEN" > /dev/null
	elif [ $VAL -eq 18 ] ; then
		echo "EIGHT" > /dev/null
	elif [ $VAL -eq 19 ] ; then
		echo "NINE" > /dev/null
	elif [ $VAL -eq 20 ] ; then
		echo "ZERO" > /dev/null
	elif [ $VAL -eq 21 ] ; then
		echo "ONE" > /dev/null
	elif [ $VAL -eq 22 ] ; then
		echo "TWO" > /dev/null
	elif [ $VAL -eq 23 ] ; then
		echo "THREE" > /dev/null
	elif [ $VAL -eq 24 ] ; then
		echo "FOUR" > /dev/null
	elif [ $VAL -eq 25 ] ; then
		echo "FIVE" > /dev/null
	elif [ $VAL -eq 26 ] ; then
		echo "SIX" > /dev/null
	elif [ $VAL -eq 27 ] ; then
		echo "SEVEN" > /dev/null
	elif [ $VAL -eq 28 ] ; then
		echo "EIGHT" > /dev/null
	elif [ $VAL -eq 29 ] ; then
		echo "NINE" > /dev/null
	elif [ $VAL -eq 30 ] ; then
		echo "ZERO" > /dev/null
	elif [ $VAL -eq 31 ] ; then
		echo "ONE" > /dev/null
	elif [ $VAL -eq 32 ] ; then
		echo "TWO" > /dev/null
	elif [ $VAL -eq 33 ] ; then
		echo "THREE" > /dev/null
	elif [ $VAL -eq 34 ] ; then
		echo "FOUR" > /dev/null
	elif [ $VAL -eq 35 ] ; then
		echo "FIVE" > /dev/null
	elif [ $VAL -eq 36 ] ; then
		echo "SIX" > /dev/null
	elif [ $VAL -eq 37 ] ; then
		echo "SEVEN" > /dev/null
	elif [ $VAL -eq 38 ] ; then
		echo "EIGHT" > /dev/null
	elif [ $VAL -eq 39 ] ; then
		echo "NINE" > /dev/null
	elif [ $VAL -eq 40 ] ; then
		echo "ZERO" > /dev/null
	elif [ $VAL -eq 41 ] ; then
		echo "ONE" > /dev/null
	elif [ $VAL -eq 42 ] ; then
		echo "TWO" > /dev/null
	elif [ $VAL -eq 43 ] ; then
		echo "THREE" > /dev/null
	elif [ $VAL -eq 44 ] ; then
		echo "FOUR" > /dev/null
	elif [ $VAL -eq 45 ] ; then
		echo "FIVE" > /dev/null
	elif [ $VAL -eq 46 ] ; then
		echo "SIX" > /dev/null
	elif [ $VAL -eq 47 ] ; then
		echo "SEVEN" > /dev/null
	elif [ $VAL -eq 48 ] ; then
		echo "EIGHT" > /dev/null
	elif [ $VAL -eq 49 ] ; then
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
	VAL=`expr $LOOP % 50`
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
	if [ $VAL -eq 10 ] ; then
		echo "ZERO" > /dev/null
	fi
	if [ $VAL -eq 11 ] ; then
		echo "ONE" > /dev/null
	fi
	if [ $VAL -eq 12 ] ; then
		echo "TWO" > /dev/null
	fi
	if [ $VAL -eq 13 ] ; then
		echo "THREE" > /dev/null
	fi
	if [ $VAL -eq 14 ] ; then
		echo "FOUR" > /dev/null
	fi
	if [ $VAL -eq 15 ] ; then
		echo "FIVE" > /dev/null
	fi
	if [ $VAL -eq 16 ] ; then
		echo "SIX" > /dev/null
	fi
	if [ $VAL -eq 17 ] ; then
		echo "SEVEN" > /dev/null
	fi
	if [ $VAL -eq 18 ] ; then
		echo "EIGHT" > /dev/null
	fi
	if [ $VAL -eq 19 ] ; then
		echo "NINE" > /dev/null
	fi
	if [ $VAL -eq 20 ] ; then
		echo "ZERO" > /dev/null
	fi
	if [ $VAL -eq 21 ] ; then
		echo "ONE" > /dev/null
	fi
	if [ $VAL -eq 22 ] ; then
		echo "TWO" > /dev/null
	fi
	if [ $VAL -eq 23 ] ; then
		echo "THREE" > /dev/null
	fi
	if [ $VAL -eq 24 ] ; then
		echo "FOUR" > /dev/null
	fi
	if [ $VAL -eq 25 ] ; then
		echo "FIVE" > /dev/null
	fi
	if [ $VAL -eq 26 ] ; then
		echo "SIX" > /dev/null
	fi
	if [ $VAL -eq 27 ] ; then
		echo "SEVEN" > /dev/null
	fi
	if [ $VAL -eq 28 ] ; then
		echo "EIGHT" > /dev/null
	fi
	if [ $VAL -eq 29 ] ; then
		echo "NINE" > /dev/null
	fi
	if [ $VAL -eq 30 ] ; then
		echo "ZERO" > /dev/null
	fi
	if [ $VAL -eq 31 ] ; then
		echo "ONE" > /dev/null
	fi
	if [ $VAL -eq 32 ] ; then
		echo "TWO" > /dev/null
	fi
	if [ $VAL -eq 33 ] ; then
		echo "THREE" > /dev/null
	fi
	if [ $VAL -eq 34 ] ; then
		echo "FOUR" > /dev/null
	fi
	if [ $VAL -eq 35 ] ; then
		echo "FIVE" > /dev/null
	fi
	if [ $VAL -eq 36 ] ; then
		echo "SIX" > /dev/null
	fi
	if [ $VAL -eq 37 ] ; then
		echo "SEVEN" > /dev/null
	fi
	if [ $VAL -eq 38 ] ; then
		echo "EIGHT" > /dev/null
	fi
	if [ $VAL -eq 39 ] ; then
		echo "NINE" > /dev/null
	fi
	if [ $VAL -eq 40 ] ; then
		echo "ZERO" > /dev/null
	fi
	if [ $VAL -eq 41 ] ; then
		echo "ONE" > /dev/null
	fi
	if [ $VAL -eq 42 ] ; then
		echo "TWO" > /dev/null
	fi
	if [ $VAL -eq 43 ] ; then
		echo "THREE" > /dev/null
	fi
	if [ $VAL -eq 44 ] ; then
		echo "FOUR" > /dev/null
	fi
	if [ $VAL -eq 45 ] ; then
		echo "FIVE" > /dev/null
	fi
	if [ $VAL -eq 46 ] ; then
		echo "SIX" > /dev/null
	fi
	if [ $VAL -eq 47 ] ; then
		echo "SEVEN" > /dev/null
	fi
	if [ $VAL -eq 48 ] ; then
		echo "EIGHT" > /dev/null
	fi
	if [ $VAL -eq 49 ] ; then
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
	VAL=`expr $LOOP % 50`
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
		( 10 ) echo "ZERO" > /dev/null ;;
		( 11 ) echo "ONE" > /dev/null ;;
		( 12 ) echo "TWO" > /dev/null ;;
		( 13 ) echo "THREE" > /dev/null ;;
		( 14 ) echo "FOUR" > /dev/null ;;
		( 15 ) echo "FIVE" > /dev/null ;;
		( 16 ) echo "SIX" > /dev/null ;;
		( 17 ) echo "SEVEN" > /dev/null ;;
		( 18 ) echo "EIGHT" > /dev/null ;;
		( 19 ) echo "NINE" > /dev/null ;;
		( 20 ) echo "ZERO" > /dev/null ;;
		( 21 ) echo "ONE" > /dev/null ;;
		( 22 ) echo "TWO" > /dev/null ;;
		( 23 ) echo "THREE" > /dev/null ;;
		( 24 ) echo "FOUR" > /dev/null ;;
		( 25 ) echo "FIVE" > /dev/null ;;
		( 26 ) echo "SIX" > /dev/null ;;
		( 27 ) echo "SEVEN" > /dev/null ;;
		( 28 ) echo "EIGHT" > /dev/null ;;
		( 29 ) echo "NINE" > /dev/null ;;
		( 30 ) echo "ZERO" > /dev/null ;;
		( 31 ) echo "ONE" > /dev/null ;;
		( 32 ) echo "TWO" > /dev/null ;;
		( 33 ) echo "THREE" > /dev/null ;;
		( 34 ) echo "FOUR" > /dev/null ;;
		( 35 ) echo "FIVE" > /dev/null ;;
		( 36 ) echo "SIX" > /dev/null ;;
		( 37 ) echo "SEVEN" > /dev/null ;;
		( 38 ) echo "EIGHT" > /dev/null ;;
		( 39 ) echo "NINE" > /dev/null ;;
		( 41 ) echo "ONE" > /dev/null ;;
		( 42 ) echo "TWO" > /dev/null ;;
		( 43 ) echo "THREE" > /dev/null ;;
		( 44 ) echo "FOUR" > /dev/null ;;
		( 45 ) echo "FIVE" > /dev/null ;;
		( 46 ) echo "SIX" > /dev/null ;;
		( 47 ) echo "SEVEN" > /dev/null ;;
		( 48 ) echo "EIGHT" > /dev/null ;;
		( 49 ) echo "NINE" > /dev/null ;;
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
function print_10()
{
	echo "ZERO" > /dev/null
}
function print_11()
{
	echo "ONE" > /dev/null
}
function print_12()
{
	echo "TWO" > /dev/null
}
function print_13()
{
	echo "THREE" > /dev/null
}
function print_14()
{
	echo "FOUR" > /dev/null
}
function print_15()
{
	echo "FIVE" > /dev/null
}
function print_16()
{
	echo "SIX" > /dev/null
}
function print_17()
{
	echo "SEVEN" > /dev/null
}
function print_18()
{
	echo "EIGHT" > /dev/null
}
function print_19()
{
	echo "NINE" > /dev/null
}
function print_20()
{
	echo "ZERO" > /dev/null
}
function print_21()
{
	echo "ONE" > /dev/null
}
function print_22()
{
	echo "TWO" > /dev/null
}
function print_23()
{
	echo "THREE" > /dev/null
}
function print_24()
{
	echo "FOUR" > /dev/null
}
function print_25()
{
	echo "FIVE" > /dev/null
}
function print_26()
{
	echo "SIX" > /dev/null
}
function print_27()
{
	echo "SEVEN" > /dev/null
}
function print_28()
{
	echo "EIGHT" > /dev/null
}
function print_29()
{
	echo "NINE" > /dev/null
}
function print_30()
{
	echo "ZERO" > /dev/null
}
function print_31()
{
	echo "ONE" > /dev/null
}
function print_32()
{
	echo "TWO" > /dev/null
}
function print_33()
{
	echo "THREE" > /dev/null
}
function print_34()
{
	echo "FOUR" > /dev/null
}
function print_35()
{
	echo "FIVE" > /dev/null
}
function print_36()
{
	echo "SIX" > /dev/null
}
function print_37()
{
	echo "SEVEN" > /dev/null
}
function print_38()
{
	echo "EIGHT" > /dev/null
}
function print_39()
{
	echo "NINE" > /dev/null
}
function print_40()
{
	echo "ZERO" > /dev/null
}
function print_41()
{
	echo "ONE" > /dev/null
}
function print_42()
{
	echo "TWO" > /dev/null
}
function print_43()
{
	echo "THREE" > /dev/null
}
function print_44()
{
	echo "FOUR" > /dev/null
}
function print_45()
{
	echo "FIVE" > /dev/null
}
function print_46()
{
	echo "SIX" > /dev/null
}
function print_47()
{
	echo "SEVEN" > /dev/null
}
function print_48()
{
	echo "EIGHT" > /dev/null
}
function print_49()
{
	echo "NINE" > /dev/null
}

function4()
{
    LOOP=0
    echo "eval"
    while [ $LOOP -lt 1000 ] ; do
	VAL=`expr $LOOP % 50`

	eval "print_$VAL"

	LOOP=$((LOOP + 1))
    done
}

time function1
time function2
time function3
time function4


