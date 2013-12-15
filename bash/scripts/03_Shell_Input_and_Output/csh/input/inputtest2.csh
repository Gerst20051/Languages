#!/bin/csh
printf "Type three numbers separated by 'q'. -> "

set LINE = "$<"

set NUMBER1 = `echo "$LINE" | cut -f 1 -d 'q'`
set NUMBER2 = `echo "$LINE" | cut -f 2 -d 'q'`
set NUMBER3 = `echo "$LINE" | cut -f 3 -d 'q'`

echo "You said: $NUMBER1, $NUMBER2, $NUMBER3"
