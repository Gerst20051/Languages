#!/bin/bash

# This is an extension that is supported in
# bash, zsh, and many other recent sh variants,
# but is not always valid.
#
# Usage: for5.sh <number>

for (( i = 1 ; i <= $1 ; i++ )) ; do
        echo "I is $i"
done


# This is a safer way to write this.

i=1
while [ $i -le $1 ] ; do
    echo "I is $i"
    i=`expr $i '+' 1`
done
