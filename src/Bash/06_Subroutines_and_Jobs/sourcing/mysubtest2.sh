#!/bin/sh
MYVAR=4
. mysub.sh
echo "MYVAR INITIALLY $MYVAR"
mysub "This is an arg"
echo "MYVAR STILL $MYVAR"
