#!/bin/sh

LOOP=0

while [ $LOOP -lt 20 ] ; do
	VAL=`expr $LOOP % 10`
	case "$VAL" in
		( 0 ) echo "ZERO" ;;
		( 1 ) echo "ONE" ;;
		( 2 ) echo "TWO" ;;
		( 3 ) echo "THREE" ;;
		( 4 ) echo "FOUR" ;;
		( 5 ) echo "FIVE" ;;
		( 6 ) echo "SIX" ;;
		( 7 ) echo "SEVEN" ;;
		( 8 ) echo "EIGHT" ;;
		( 9 ) echo "NINE" ;;
		( * ) echo "This shouldn't happen." ;;
	esac

	LOOP=$((LOOP + 1))
done

