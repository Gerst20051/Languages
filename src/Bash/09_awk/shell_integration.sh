#!/bin/sh

# Store the output in a variable.
OUTPUT="$(awk 'BEGIN { \
	RS="\n"; \
	FS="\t"; \
     } \
     { \
	print $1 "\t" $3 "\t" $5; \
     }' tab_delimited_file)"

# Set the field separator to a newline so that
# the "for" statement below will put one line
# at a time in the "LINE" variable.
IFS="
"

# Parse and print the records.
RECORD=1
for LINE in $OUTPUT ; do
	# By default, cut uses tab as its delimiter,
	# so these commands take the first,
	# second, and third tab-delimited fields
	# from a single line of input, respectively.
	FIELD_1="$(echo "$LINE" | cut -f 1)"
	FIELD_2="$(echo "$LINE" | cut -f 2)"
	FIELD_3="$(echo "$LINE" | cut -f 3)"

	echo "RECORD $RECORD"
	echo "    FIELD 1: $FIELD_1"
	echo "    FIELD 2: $FIELD_2"
	echo "    FIELD 3: $FIELD_3"
	echo
	RECORD="$(expr $RECORD '+' 1)"
done

