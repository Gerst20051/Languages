#!/bin/sh

TEXT="This is a test, this is only a test."

function trunc_field()
{
    local STR=$1
    local CHARS=$2
    local EXP=""
    local COUNT=0
    while [ $COUNT -lt $CHARS ] ; do
	EXP="$EXP."
	COUNT=`expr $COUNT + 1`
    done
    echo $STR | sed "s/^\($EXP\).*$/\1/"
}

printf "%10s | something\n" "`trunc_field "$TEXT" 20`"
