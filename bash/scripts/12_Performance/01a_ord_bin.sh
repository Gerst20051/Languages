#!/bin/sh

# Convert an int to an octal value.
function inttooct()
{
        echo $(echo "obase=8; $1" | bc)
}

function split_str()
{
	STR="$1"
	NUM="$2"
	SPLIT="$(eval "echo \"\$SPLIT_$NUM\"")"
	LEFT="$(echo "$STR" | sed "s/^\\($SPLIT\\).*$/\\1/")"
	RIGHT="$(echo "$STR" | sed "s/^$SPLIT//")"
}

# Initializer for the scary shell ord function.
function ord_init()
{
    I=1
    ORDSTRING=""
    while [ $I -lt 256 ] ; do
        # local HEX=$(inttohex $I);
        local OCT=$(inttooct $I);
        # The following should work with GNU sed, but
        # Mac OS X's sed doesn't support \x.
        # local CH=$(echo ' ' | sed -E "s/ /\\x$HEX/")
        # How about this?
        # local CH=$(perl -e  "\$/=undef; \$x = ' '; \$x =~ s/ /\x$HEX/g; print \$x;")
        # Yes, that works, but it's cheating.  Here's a better one.
        local CH=$(echo ' ' | tr ' ' "\\$OCT");
        ORDSTRING="$ORDSTRING$CH"
        I=$(($I + 1)) # or I=$(expr $I '+' 1)
        # echo "ORDSTRING: $ORDSTRING"
    done
}
# Initializer for the scary shell ord function.
function ord_init2()
{
    I=1
    ORDSTRING=""
    while [ $I -lt 256 ] ; do
        # local HEX=$(inttohex $I);
        local OCT=$(inttooct $I);
        # The following should work with GNU sed, but
        # Mac OS X's sed doesn't support \x.
        # local CH=$(echo ' ' | sed -E "s/ /\\x$HEX/")
        # How about this?
        # local CH=$(perl -e  "\$/=undef; \$x = ' '; \$x =~ s/ /\x$HEX/g; print \$x;")
        # Yes, that works, but it's cheating.  Here's a better one.
        local CH=$(echo ' ' | tr ' ' "\\$OCT");
        ORDSTRING="$ORDSTRING$CH"
        I=$(($I + 1)) # or I=$(expr $I '+' 1)
        # echo "ORDSTRING: $ORDSTRING"
    done
    SPLIT=128
    while [ $SPLIT -ge 1 ] ; do
	COUNT=$SPLIT
	STRING=""
	while [ $COUNT -gt 0 ] ; do
		STRING="$STRING""."
		COUNT=$((COUNT - 1))
	done
	eval "SPLIT_$SPLIT=\"$STRING\"";
	SPLIT=$((SPLIT / 2))
    done
}
function ord()
{
    local CH="$1"
    local STRING=""
    local OCCOPY="$ORDSTRING"
    local COUNT=0;
 
    # Delete the first character from a copy of ORDSTRING if that
    # character doesn't match the one we're looking for.  Loop
    # until we don't have any more leading characters to delete.
    # The count will be the ASCII character code for the letter.
    CONT=1;
    while [ $CONT = 1 ]; do
        # Copy the string so we know if we've stopped finding
        # nonmatching characters.
        OCTEMP="$OCCOPY"
 
        # echo "CH WAS $CH"
        # echo "ORDSTRING: $ORDSTRING"
 
        # If it's a close bracket, quote it; we don't want to
        # break the regexp.
        if [ "x$CH" = "x]" ] ; then
                CH='\]'
        fi
 
        # Delete a character if possible.
        OCCOPY="$(echo "$OCCOPY" | sed "s/^[^$CH]//")";
 
        # On error, we're done.
        if [ $? != 0 ] ; then CONT=0 ; fi
 
        # If the string didn't change, we're done.
        if [ "x$OCTEMP" = "x$OCCOPY" ] ; then CONT=0 ; fi
 
        # Increment the counter so we know where we are.
        COUNT=$((COUNT + 1)) # or COUNT=$(expr $COUNT '+' 1)
        # echo "COUNT: $COUNT"
    done
 
    COUNT=$(($COUNT + 1)) # or COUNT=$(expr $COUNT '+' 1)
    # If we ran out of characters, it's a null (character 0).
    if [ "x$OCTEMP" = "x" ] ; then COUNT=0; fi
 
    # echo "ORD IS $COUNT";
 
    # Return the ord of the character in question....
    echo $COUNT
    # exit 0
}

function ord2()
{
    local CH="$1"
    local STRING=""
    local OCCOPY="$ORDSTRING"
    local COUNT=0;
 
    # Delete the first character from a copy of ORDSTRING if that
    # character doesn't match the one we're looking for.  Loop
    # until we don't have any more leading characters to delete.
    # The count will be the ASCII character code for the letter.

    CONT=1
    BASE=0
    LAST="$OCCOPY"
    while [ $CONT = 1 ] ; do
	LAST="`echo "$OCCOPY" | sed 's/^\(................\)/\1/'`"
	OCCOPY="`echo "$OCCOPY" | sed 's/^................//'`"
	CONT="`echo "$OCCOPY" | grep -c "$CH"`"
	BASE=$(($BASE + 16))
    done
    BASE=$(($BASE - 16))

    CONT=1;
    while [ $CONT = 1 ]; do
        # Copy the string so we know if we've stopped finding
        # nonmatching characters.
        OCTEMP="$LAST"
 
        # echo "CH WAS $CH"
        # echo "ORDSTRING: $ORDSTRING"
 
        # If it's a close bracket, quote it; we don't want to
        # break the regexp.
        if [ "x$CH" = "x]" ] ; then
                CH='\]'
        fi
 
        # Delete a character if possible.
        LAST=$(echo "$LAST" | sed "s/^[^$CH]//");
 
        # On error, we're done.
        if [ $? != 0 ] ; then CONT=0 ; fi
 
        # If the string didn't change, we're done.
        if [ "x$OCTEMP" = "x$LAST" ] ; then CONT=0 ; fi
 
        # Increment the counter so we know where we are.
        COUNT=$((COUNT + 1)) # or COUNT=$(expr $COUNT '+' 1)
        # echo "COUNT: $COUNT"
    done
 
    COUNT=$(($COUNT + 1 + $BASE)) # or COUNT=$(expr $COUNT '+' 1)
    # If we ran out of characters, it's a null (character 0).
    if [ "x$OCTEMP" = "x" ] ; then COUNT=0; fi
 
    # echo "ORD IS $COUNT";
 
    # Return the ord of the character in question....
    echo $COUNT
    # exit 0
}

function ord3()
{
    local CH="$1"
    OCCOPY="$ORDSTRING"
    FIRST=1;
    LAST=257

    ord3_sub "$CH" "$ORDSTRING" $FIRST $LAST
}

function ord3_sub()
{
    local CH="$1"
    OCCOPY="$2"
    FIRST=$3
    LAST=$4

    # echo "FIRST: $FIRST, LAST: $LAST"

    if [ $FIRST -ne $(($LAST - 1)) ] ; then
	SPLITWIDTH=$((($LAST - $FIRST) / 2))
	split_str "$OCCOPY" $SPLITWIDTH
	if [ $(echo "$LEFT" | grep -c "$CH") -eq 1 ] ; then
		# echo "left"
		ord3_sub "$CH" "$LEFT" $FIRST $(( $FIRST + $SPLITWIDTH ))
	else
		# echo "right"
		ord3_sub "$CH" "$RIGHT" $(( $FIRST + $SPLITWIDTH )) $LAST
	fi
    else
	echo $(( $FIRST + 1 ))
    fi
}

function test1()
{
	ord k
}

function test2()
{
	ord2 k
}

function test3()
{
	ord3 k
}


echo "Initializing.  Please wait."
time ord_init
time ord_init2

# echo "STRING 16: $SPLIT_16"

echo "Timing."

time test1
time test2
time test3

