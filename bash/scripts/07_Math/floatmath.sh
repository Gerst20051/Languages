#!/bin/sh
PI=3.141592654
RAD=7
AREA=$(perl -e "print \"The value is \".($PI * ($RAD*$RAD)).\"\n\";")
echo $AREA

AREA=$(echo "$PI * ($RAD ^ 2)" | bc)
echo "The area is $AREA"

