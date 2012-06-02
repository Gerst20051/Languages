#!/bin/sh

# Print the shell variables for terminal size information.

echo "ROWS: $LINES"
echo "COLUMNS: $COLUMNS"

# Eek!  Those are blank in Mac OS X v10.3 and v10.4!  Well,
# try it this way:

MYLINES=`stty -a | grep rows | sed 's/ rows; .*;//' | sed 's/.* //'` # ROWS
MYCOLUMNS=`stty -a | grep col | sed 's/.*;.*; //' | sed 's/ .*//'` # COLUMNS

echo "ROWS (from stty): $MYLINES"
echo "COLUMNS (from stty): $MYCOLUMNS"

function fixrows()
{
	MYLINES=`stty -a | grep rows | sed 's/ rows; .*;//' | sed 's/.* //'` # ROWS
	MYCOLUMNS=`stty -a | grep col | sed 's/.*;.*; //' | sed 's/ .*//'` # COLUMNS
	echo "$MYLINES, $MYCOLUMNS"
}

echo "Adjust the size of your window and this code will tell you the new size."
trap fixrows SIGWINCH

COUNT=0
while [ $COUNT -lt 60 ] ; do
	COUNT=$(($COUNT + 1))
	sleep 1
done
