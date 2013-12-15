#!/bin/sh

COUNTER=0
VALUE="-1"
echo "Enter a series of lines of test.  Enter a blank line to end."

while [ "x$VALUE" != "x" ] ; do
	read VALUE
	eval ARRAY_$COUNTER=$VALUE
	eval export ARRAY_$COUNTER
	COUNTER=$(expr $COUNTER '+' 1) # More on this in Paint by Numbers
done
COUNTER=$(expr $COUNTER '-' 1) # Subtract one for the blank value at the end.

# print the exported variables.
COUNTERB=0;

echo "Printing values."
while [ $COUNTERB -lt $COUNTER ] ; do
	echo "ARRAY[$COUNTERB] = $(eval echo '$'ARRAY_$COUNTERB)"
	COUNTERB=$(expr $COUNTERB '+' 1) # More on this in Paint by Numbers
done
