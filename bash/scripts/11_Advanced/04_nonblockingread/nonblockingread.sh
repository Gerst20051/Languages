#!/bin/bash

stty -icanon -isig
while true ; do
        echo -n "Enter a character: "
        CHAR=`./getch`
        if [ "x$CHAR" = "x" ] ; then
                echo "NO DATA";
        else
        	if [ "x$CHAR" = "xq" ] ; then
			stty -cbreak
			exit
		fi
                echo "DATA: $CHAR";
        fi
        sleep 1;
done

# never reached
stty -cbreak

