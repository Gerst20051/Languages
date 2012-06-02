#!/bin/sh

ITERATIONS=1000
SCALE=6

# Prevent sed from caring about high ASCII characters not
# being valid UTF-8 sequences
export LANG="C"

# Set FAST to "slow", "medium", or "fast".  This controls
# which ord() function to use.
# 
# slow-use a combination of perl, awk, and shell methods
# medium-use only perl and awk methods.
# fast-use only perl

# FAST="slow"
# FAST="medium"
FAST="slow"

# 100 iterations - FAST
# real    0m9.850s
# user    0m2.162s
# sys     0m8.388s

# 100 iterations - MEDIUM
# real    0m10.362s
# user    0m2.375s
# sys     0m8.726s

# 100 iterations - SLOW
# real    2m25.556s
# user    0m32.545s
# sys     2m12.802s

# Calculate the distance from point 0,0 to point X,Y.
# In other words, calculate the hypotenuse of a right
# triangle whose legs are of length X and Y.
distance()
{
    local X=$1
    local Y=$2

    DISTANCE=$(echo "sqrt(($X ^ 2) + ($Y ^ 2))" | bc)

    echo $DISTANCE
}

# Convert an int to a hex value.  (Not used.)
inttohex()
{
	echo $(echo "obase=16; $1" | bc)
}

# Convert an int to an octal value.
inttooct()
{
	echo $(echo "obase=8; $1" | bc)
}

# Initializer for the scary shell ord function.
ord_init()
{
    I=1
    ORDSTRING=""
    while [ $I -lt 256 ] ; do
	# local HEX=$(inttohex $I);
	local OCT=$(inttooct $I);
	# The following should work with GNU sed, but
	# Mac OS X's sed doesn't support \x.
	# local CH=$(echo ' ' | sed "s/ /\\x$HEX/")
	# How about this?
	# local CH=$(perl -e  "\$/=undef; \$x = ' '; \$x =~ s/ /\x$HEX/g; print \$x;")
	# Yes, that works, but it's cheating.  Here's a better one.
	local CH=$(echo ' ' | tr ' ' "\\$OCT");
	ORDSTRING=$ORDSTRING$CH
	I=$(($I + 1)) # or I=$(expr $I '+' 1)
	# echo "ORDSTRING: $ORDSTRING"
    done
}

# This is a scary little lovely piece of shell script.
# It finds the ord of a character using only the shell,
# tr, and sed.  The variable ORDSTRING must be initialized
# prior to first use with a call to ord_init.  This string
# is not modified.
ord()
{
    local CH="$1"
    local STRING=""
    local OCCOPY=$ORDSTRING
    local COUNT=0;

    # Some shells can't handle NULL characters,
    # so this code gets an empty argument.
    if [ "x$CH" = "x" ] ; then
	echo 0
	return
    fi

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
	## Not needed.  It's the first character.
	## if [ "x$CH" = "x]" ] ; then
		## CH='\]'
	## fi

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

# If we're using the shell ord function, we need to
# initialize it on launch.  We also do a quick sanity
# check just to make sure it is working.
if [ "x$FAST" = "xslow" ] ; then
	echo "Initializing Bourne ord function."
	ord_init

	# Test our ord function
	echo "Testing ord function"
	ORDOFA=$(ord "a")
	# That better be 97.
	if [ "$ORDOFA" != "97" ] ; then
		echo "Shell ord function broken.  Try fast mode."
	fi

	echo "ord_init done"
fi

COUNT=0
IN=0

# For the Monte Carlo method, we check to see if a random point between
# 0,0 and 1,1 lies within a unit circle distance from 0,0.  This allows
# us to approximate pi.
while [ $COUNT -lt $ITERATIONS ] ; do
	# Read four random bytes.
	RAWVAL1="$(dd if=/dev/random bs=1 count=1 2> /dev/null)"
	RAWVAL2="$(dd if=/dev/random bs=1 count=1 2> /dev/null)"
	RAWVAL3="$(dd if=/dev/random bs=1 count=1 2> /dev/null)"
	RAWVAL4="$(dd if=/dev/random bs=1 count=1 2> /dev/null)"

	# ord "$RAWVAL4";
	# exit 0;

	# The easy method for doing an ord() of a character: use Perl.
	XVAL0=$(echo "$RAWVAL1" | perl -e '$/ = undef; my $val = <STDIN>; print ord($val);')
	XVAL1=$(echo "$RAWVAL2" | perl -e '$/ = undef; my $val = <STDIN>; print ord($val);')

	# The not-so-easy way using awk (but still almost as fast as perl)
	if [ "x$FAST" != "xfast" ] ; then
	    # Run this for FAST = medium or slow.
	    echo "AWK ord"
	    # Fun little awk program for calculating ord of a letter.
	    YVAL0=$(echo "$RAWVAL3" | awk '{
		RS="\n"; ch=$0;
		# print "CH IS ";
		# print ch;
		if (!length(ch)) { # must be the record separator. 
			ch="\n"
		};
		s="";
		for (i=1; i<256; i++) {
			l=sprintf("%c", i);
			ns = (s l); s = ns;
		};
		pos = index(s, ch); printf("%d", pos)
	    }')
	    # Fun little shell script for calculating ord of a letter.
	else
	    YVAL0=$(echo "$RAWVAL3" | perl -e '$/ = undef; my $val = <STDIN>; print ord($val);')
	fi

	# The evil way---slightly faster than looking it up by hand....
	if [ "x$FAST" = "xslow" ] ; then
	    # Run this ONLY for FAST = slow.  This is REALLY slow!
	    YVAL1=$(ord "$RAWVAL4")
	else 
	    YVAL1=$(echo "$RAWVAL4" | perl -e '$/ = undef; my $val = <STDIN>; print ord($val);')
	fi

	# echo "YV3: $VAL3"
	# YVAL1="0"

	# We basically want to get an unsigned 16-bit number out of
	# two raw bytes.  Earlier, we got the ord() of each byte.
	# Now, we figure out what that unsigned value would be by
	# multiplying the high order byte by 256 and adding the
	# low order byte.  We don't really care which byte is which,
	# since they're just random numbers.
	XVAL=$(( ($XVAL0 * 256) + $XVAL1 ))   # use expr for older shells.
	YVAL=$(( ($YVAL0 * 256) + $YVAL1 ))   # use expr for older shells.

	# This doesn't work well, since we can't seed awk's PRNG
	# in any useful way.
	# YVAL=$(awk '{printf("%d", rand() * 65535)}')

	# Calculate the difference.
	DISTANCE=$(distance $XVAL $YVAL)
	echo "X: $XVAL, Y: $YVAL, DISTANCE: $DISTANCE"

	if [ $DISTANCE -le 65535 ] ; then # use expr for older shells
		echo "In circle.";
		IN=$(($IN + 1))
	else
		echo "Outside circle.";
	fi

	COUNT=$(($COUNT + 1))                # use expr for older shells.
done

# Calculate PI.
PI=$(echo "scale=$SCALE; ($IN / $ITERATIONS) * 4" | bc)

# Print the results.
echo "IN: $IN, ITERATIONS: $ITERATIONS"
echo "PI is about $PI"

