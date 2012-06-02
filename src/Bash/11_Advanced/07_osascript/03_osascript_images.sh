#!/bin/sh

# Get an absolute path for the poem.txt file.

MAXLONG=480
MAXSHORT=320

URL="http://images.apple.com/macpro/images/design_smartdesign_hero20080108.png"
FILE="$PWD/my design_smartdesign_hero20080108.png"
OUTFILE="$PWD/my design_smartdesign_hero20080108-mini.png"

if [ ! -f "$FILE" ] ; then
	curl "$URL" > "$FILE"
fi

# Tell the shell not to mangle newline characters, tabs, or whitespace.
IFS=""

# Obtain image information
DIM="$(cat << EOF | osascript -l AppleScript
tell application "Image Events"
	launch
	set this_image to open "$FILE"
	copy dimensions of this_image to {W, H}
	close this_image
end tell
return W & H
EOF
)"

W="$(echo "$DIM" | sed 's/ *, *.*//' )"
H="$(echo "$DIM" | sed 's/.* *, *//' )"

echo WIDTH: $W HEIGHT: $H

if [ $W -gt $H ] ; then
	LONG=$W
	SHORT=$H
else
	LONG=$H
	SHORT=$W
fi

# echo "LONG: $LONG SHORT: $SHORT"
# echo "MAXLONG: $MAXLONG MAXSHORT: $MAXSHORT"

NEWLONG=$LONG
NEWSHORT=$SHORT
# NEWSCALE=1

if [ $NEWLONG -gt $MAXLONG ] ; then
	# Long direction is too big.
	NEWLONG="$(echo "scale=20; $LONG * ($MAXLONG/$LONG)" | bc | sed 's/\..*//')";
	NEWSHORT="$(echo "scale=20; $SHORT * ($MAXLONG/$LONG)" | bc | sed 's/\..*//')";
	NEWSCALE="$(echo "scale=20; ($MAXLONG/$LONG)" | bc)";
fi

# echo "PART 1: NEWLONG: $NEWLONG NEWSHORT: $NEWSHORT"

if [ $NEWSHORT -gt $MAXSHORT ] ; then
	# Short direction is till too big.
	NEWLONG="$(echo "scale=20; $LONG * ($MAXSHORT/$SHORT)" | bc | sed 's/\..*//')";
	NEWSHORT="$(echo "scale=20; $SHORT * ($MAXSHORT/$SHORT)" | bc | sed 's/\..*//')";
	NEWSCALE="$(echo "scale=20; ($MAXSHORT/$SHORT)" | bc)";
fi

# echo "PART 2: NEWLONG: $NEWLONG NEWSHORT: $NEWSHORT"

if [ $W -gt $H ] ; then
	NEWWIDTH=$NEWLONG
	NEWHEIGHT=$NEWSHORT
else
	NEWHEIGHT=$NEWLONG
	NEWWIDTH=$NEWSHORT
fi

echo "DESIRED WIDTH: $NEWWIDTH NEW HEIGHT: $NEWHEIGHT (SCALE IS $NEWSCALE)"

cp "$FILE" "$OUTFILE"

DIM="$(cat << EOF | osascript -l AppleScript
tell application "Image Events"
	launch
	set this_image to open "$OUTFILE"
	scale this_image by factor $NEWSCALE
	save this_image with icon
	copy dimensions of this_image to {W, H}
	close this_image
end tell
return W & H
EOF
)"

GOTW="$(echo "$DIM" | sed 's/ *, *.*//' )"
GOTH="$(echo "$DIM" | sed 's/.* *, *//' )"

echo "NEW WIDTH: $GOTW NEW HEIGHT: $GOTH"


