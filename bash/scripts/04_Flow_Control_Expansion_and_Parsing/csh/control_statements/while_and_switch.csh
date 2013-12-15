#!/bin/csh

set LOOP = 0

while ( ${LOOP} <= 20 )
	set VAL = `expr ${LOOP} % 10`
	switch (${VAL})
		case 0:
			echo "ZERO" ; breaksw
		case 1:
			echo "ONE" ; breaksw
		case 2:
			echo "TWO" ; breaksw
		case 3:
			echo "THREE" ; breaksw
		case 4:
			echo "FOUR" ; breaksw
		case 5:
			echo "FIVE" ; breaksw
		case 6:
			echo "SIX" ; breaksw
		case 7:
			echo "SEVEN" ; breaksw
		case 8:
			echo "EIGHT" ; breaksw
		case 9:
			echo "NINE" ; breaksw
		default:
			echo "This shouldn't happen."
	endsw

	set LOOP = `expr ${LOOP} + 1`

end

