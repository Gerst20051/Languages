#!/bin/csh

# Note: most csh variants do NOT respect IFS.

echo "Setting IFS doesn't work."

set IFS = ":"
set LIST = "a:b:c d"
foreach i ( $LIST )
        echo $i
end

echo "Try it the CSH way:"

# Repeat until you get an empty field.  This only works if
# you know you should never encounter an empty field.  Otherwise,
# you must know the number of fields.
# 
# See expr/02_count_leter_i.csh for an easy way to count the
# number of field separators.

set IFS = ":"
set LIST = "a:b:c d"
set POS = 1
set i = `echo "${LIST}" | cut -f ${POS} -d ':'`
while ( "x${i}" != "x" )
	echo $i
	set POS = `expr ${POS} '+' 1`
	set i = `echo "${LIST}" | cut -f ${POS} -d ':'`
end


