#!/bin/sh

expr '(' '3' '*' '4' ')' '+' '2'

expr '3' '<' '-2'


COUNT=0
while [ $COUNT -lt '4' ] ; do
    echo "COUNT IS $COUNT"
    COUNT="$(expr "$COUNT" '+' '1')"
done

if [ 1 = 2 ] ; then
    echo "equal"
else
    echo "not equal"
fi

if [ 1 = "01" ] ; then
    echo "equal"
else
    echo "not equal"
fi

expr '1' '+' '2'
expr ' 1' '+' '2'
expr '2' '<' '1'
expr ' 2' '<' '1'

if [ ' 2' -gt '1' ] ; then
	echo "greater"
fi

MYRAWNUMBER=" 2" # Note this is a string, not a number.

# Strip off any characters that aren't in the range of 0-9.
MYNUMBER="$(echo "$MYRAWNUMBER" | sed 's/[^0-9]//g')"

expr "$MYNUMBER" '<' '1'

