#!/bin/sh

# Demonstrates scoping rules.

changevalue()
{
	NAME="$1"

	eval "$NAME=\"\$(expr \"\$$NAME\" \"+\" \"1\")\""
	eval echo "\$$NAME"
}

localchange()
{
	local X=17

	printf "Local variable X: $X + 1 is: "
	changevalue X
	echo "which is also $X"
}

A=3
printf "$A + 1 is "
changevalue A
echo "which is also $A"

B=3
printf "$B + 1 is "
RESULT="$(changevalue B)"
echo $RESULT
echo "which is NOT $B"

localchange
echo "X in a global context is \"$X\""

