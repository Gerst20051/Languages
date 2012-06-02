#!/bin/sh

# Usage: ./head_c filename bytecount
FILENAME=$1
COUNT=$2

SCRIPT="$(mktemp '/tmp/head_c.XXXXXXXXXX')"

cat << EOF > "$SCRIPT"
BEGIN {
	FS=""; 
	my_string = ""
}
{
	my_string = my_string "\n" \$0;
}
END {
	# Start from character 2 to skip the bogus leading newline.
	print substr(my_string, 2, $COUNT);
}
EOF

awk -f "$SCRIPT" "$FILENAME"

rm "$SCRIPT"

