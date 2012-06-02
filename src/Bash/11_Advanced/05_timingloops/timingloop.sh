#!/bin/sh

ONE_SECOND=1000

function read_test()
{
    COUNT=0
    local ONE_SECOND=1000                       # ensure this never trips!
    while [ $COUNT -lt 200 ] ; do
        CHAR=`./getch`
        if [ $1 = "rot" ] ; then
                CHAR=","
        fi
        case "$CHAR" in
		( "q" | "Q" )
			CONT=0;
			GAMEOVER=1
		;;
		( "" )
			# Silently ignore empty input.
		;;
		( * )
			echo "Unknown key $CHAR"
		;;
	esac
        COUNT=`expr $COUNT '+' 1`
        while [ $COUNT -ge $ONE_SECOND ] ; do
                COUNT=`expr $COUNT - $ONE_SECOND`
                MODE="clear";
                draw_cur $ROT;
                VPOS=`expr $VPOS '+' 1`
                MODE="apple";
                draw_cur $ROT
        done
    done
}

function calibrate_timers()
{
    2>/tmp/readtesttime time $0 -readtest
    local READ_DUR=`grep real /tmp/readtesttime | sed 's/real.*//' | tr -d ' '`
    # echo "READ_DUR: $READ_DUR"

    local READ_SINGLE=`echo "scale=20; ($READ_DUR / 200)" | bc`
    ONE_SECOND=`echo "scale=0; 1.0  / $READ_SINGLE" | bc`

    # echo "READ_SINGLE: $READ_SINGLE";
    # exit

    echo "One second is about $ONE_SECOND cycles."
}

if [ "x$1" = "x-readtest" ] ; then
	read_test
	exit
fi

echo "Calibrating.  Please wait."
calibrate_timers

echo "Done calibrating.  You should see a message about once per second.  Press 'q' to quit."
stty -icanon -isig

GAMEOVER=0
COUNT=0
# Start the game loop.
while [ $GAMEOVER -eq 0 ] ; do
        # echo -n "Enter a character: "
        CHAR=`./getch`
        case "$CHAR" in
		( "q" | "Q" )
			CONT=0;
			GAMEOVER=1
		;;
		( "" )
			# Silently ignore empty input.
		;;
		( * )
			echo "Unknown key $CHAR"
		;;
        esac
        COUNT=`expr $COUNT '+' 1`
        while [ $COUNT -ge $ONE_SECOND ] ; do
                COUNT=`expr $COUNT - $ONE_SECOND`
		echo "One second elapsed (give or take)."
        done
done

stty sane

