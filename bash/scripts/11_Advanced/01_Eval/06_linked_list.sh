#!/bin/sh

VAR1_VALUE="7"
VAR1_NEXT="VAR2"

VAR2_VALUE="11"
VAR2_NEXT="VAR3"

VAR3_VALUE="42"

HEAD="VAR1"
POS=$HEAD
while [ "x$POS" != "x" ] ; do
	echo "POS: $POS"
	VALUE="$(eval echo '$'$POS'_VALUE')"
	echo "VALUE: $VALUE"
	POS="$(eval echo '$'$POS'_NEXT')"
done
