#!/bin/sh

# Convert an int to an octal value.
function inttooct()
{
        echo $(echo "obase=8; $1" | bc)
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
        ORDSTRING=$ORDSTRING$CH
        I=$(($I + 1)) # or I=$(expr $I '+' 1)
        # echo "ORDSTRING: $ORDSTRING"
    done
}
function ord()
{
    local CH="$1"
    local STRING=""
    local OCCOPY=$ORDSTRING
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
        OCCOPY=$(echo "$OCCOPY" | sed "s/^[^$CH]//");
 
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
    local OCCOPY=$ORDSTRING
    local COUNT=0;
 
    # Delete the first character from a copy of ORDSTRING if that
    # character doesn't match the one we're looking for.  Loop
    # until we don't have any more leading characters to delete.
    # The count will be the ASCII character code for the letter.

    CONT=1
    BASE=0
    LAST="$OCCOPY"
    while [ $CONT = 1 ] ; do
	LAST=`echo "$OCCOPY" | sed 's/^\(................\)/\1/'`
	OCCOPY=`echo "$OCCOPY" | sed 's/^................//'`
	CONT=`echo "$OCCOPY" | grep -c "$CH"`
	BASE=`expr $BASE + 16`
    done
    BASE=`expr $BASE - 16`

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

function test1()
{
	ord k
}

function test2()
{
	ord2 k
}


echo "Initializing.  Please wait."
ord_init

echo "Timing."

time test1
time test2

