#!/bin/sh
echo "Enter variable name and value separated by a space"
read VARIABLE VALUE
echo Assigning the value $VALUE to variable $VARIABLE
eval $VARIABLE=$VALUE

# print the value
eval echo "$"$VARIABLE

# export the value
eval export $VARIABLE

# print the exported variables.
export
