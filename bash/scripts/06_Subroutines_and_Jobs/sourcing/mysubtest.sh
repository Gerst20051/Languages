#!/bin/sh
MYVAR=4
source mysub.sh
echo "MYVAR INITIALLY $MYVAR"
mysub "This is an arg"
echo "MYVAR STILL $MYVAR"
