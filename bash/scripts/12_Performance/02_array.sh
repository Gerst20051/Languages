#!/bin/sh

export FOO_1_1=1
export FOO_1_2=0
export FOO_1_3=0
export FOO_1_4=1
export FOO_2_1=0
export FOO_2_2=1
export FOO_2_3=1
export FOO_2_4=0
export FOO_3_1=1
export FOO_3_2=0
export FOO_3_3=1
export FOO_3_4=0
export FOO_4_1=0
export FOO_4_2=1
export FOO_4_3=0
export FOO_4_4=1

function test1()
{
	X=1; XA=0
	while [ $X -lt 5 ] ; do
		Y=1;
		while [ $Y -lt 5 ] ; do
			eval "FOO_$X""_$Y=FOO_$XA""_$Y"
			Y=`expr $Y + 1`
		done
		X=`expr $X + 1`
		XA=`expr $XA + 1`
	done
}

# Note: this function shifts things in the opposite direction
# from the others.  It is also rather slow.
function test2()
{
	local SEMI=""
	LIST=`export | grep "FOO_" | sed 's/export//'`
	LISTB=""
	P1="\"\""
	P2="\"\""
	P3="\"\""
	P4="\"\""
	for LINE in $LIST ; do
		# echo $LINE
		VAR=`echo $LINE | sed 's/=.*//'`
		LISTB="$LISTB$SEMI$VAR=$P4"
		SEMI=";"
		P4=$P3
		P3=$P2
		P2=$P1
		P1=`echo $LINE | sed 's/.*=//'`
	done
	# echo "B:"
	# echo $LISTB
	eval $LISTB
}

function test3()
{
	local SEMI=""
	X=1; XA=0
	LIST=""
	while [ $X -lt 5 ] ; do
		Y=1;
		while [ $Y -lt 5 ] ; do
			LIST="$LIST$SEMI""FOO_$X""_$Y=\$FOO_$XA""_$Y"
			SEMI=";"
			Y=`expr $Y + 1`
		done
		X=`expr $X + 1`
		XA=`expr $XA + 1`
	done
	# echo $LIST
	eval $LIST
}

function test4()
{
	LIST=$1
	# echo $LIST
	eval $LIST
}


time test1
time test2
time test3

X=1; XA=0
while [ $X -lt 5 ] ; do
	Y=1;
	while [ $Y -lt 5 ] ; do
		LIST="$LIST$SEMI""FOO_$X""_$Y=\$FOO_$XA""_$Y"
		SEMI=";"
		Y=`expr $Y + 1`
	done
	X=`expr $X + 1`
	XA=`expr $XA + 1`
done
time test4 "$LIST"

