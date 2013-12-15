#!/bin/sh

DO_LONG=""

# Start processing options at index 1.
OPTIND=1
# OPTERR=1
OUTPUT_FILE=""
while getopts ":hlo:" VALUE "$@" ; do

	echo "GOT FLAG $VALUE"

	if [ "$VALUE" = "h" ] ; then
		echo "Usage: $0 [-l] [-o outputfile] [path ...]"
		exit 1
	fi
	if [ "$VALUE" = "l" ] ; then
		DO_LONG="-l"
	fi
	if [ "$VALUE" = "o" ] ; then
		echo "Set output file to \"$OPTARG\""
		OUTPUT_FILE="$OPTARG"
	fi

	# The getopt routine returns a colon when it encounters
	# a flag that should have an argument but doesn't.  It
	# returns the errant flag in the OPTARG variable.
	if [ "$VALUE" = ":" ] ; then
		echo "Flag -$OPTARG requires an argument."
		echo "Usage: $0 [-l] [-o outputfile] [path ...]"
		exit 1
	fi
	# The getopt routine returns a question mark when it
	# encounters an unknown flag.  It returns the unknown
	# flag in the OPTARG variable.
	if [ "$VALUE" = "?" ] ; then
		echo "Unknown flag -$OPTARG detected."
		echo "Usage: $0 [-l] [-o outputfile] [path ...]"
		exit 1
	fi
done

# The first non-flag argument is at index $OPTIND, so shift one fewer
# to move it into $1
shift `expr $OPTIND - 1`

if [ "$OUTPUT_FILE" = "" ] ; then
	ls $DO_LONG "$@"
else
	ls $DO_LONG "$@" > $OUTPUT_FILE
fi

exit $?
