#!/bin/csh

set OUTPUT_FILE=""
set DO_LONG=""

set argv=`getopt "hlo:" $*`

if ( $status != 0 ) then
	echo "Usage: $0 [-l] [-o outputfile] [path ...]"
	exit 1
endif

while ( "$1" != "--" )
        echo "GOT FLAG $1"
	switch($1)
		case "-h":
			echo "Usage: $0 [-l] [-o outputfile] [path ...]"
			exit 1
		case "-o":
			set OUTPUT_FILE="$2"
			shift
			breaksw
		case "-l":
			set DO_LONG="-l"
			breaksw
	endsw
	shift
end

shift # remove trailing --

# echo "ARGS: $*"

if ( "$OUTPUT_FILE" == "" ) then
	ls $DO_LONG $*
else
	ls $DO_LONG $* > $OUTPUT_FILE
endif

exit $status

