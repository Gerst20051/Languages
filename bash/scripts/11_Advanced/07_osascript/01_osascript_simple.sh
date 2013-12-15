#!/bin/sh

POEM="$PWD/../poem.txt"

cat << EOF | osascript -l AppleScript
launch application "TextEdit"
tell application "TextEdit"
	open "$POEM"
end tell
EOF
