#!/bin/sh

# Create two FIFOs (named pipes)
INFIFO="/tmp/infifo.$$"
OUTFIFO="/tmp/outfifo.$$"
mkfifo "$INFIFO"
mkfifo "$OUTFIFO"

# Mac OS X and recent *BSD sed uses -l for line-buffered mode.
BUFFER_FLAG="-l"

# GNU sed uses -u for "unbuffered" mode (really line-buffered).
if [ "x$(sed --version 2>&1 | grep GNU)" != "x" ] ; then
	BUFFER_FLAG="-u"
fi

# Set up a sed substitution input from the input fifo otput to 
sed $BUFFER_FLAG 's/a test/not a test/' < $INFIFO > $OUTFIFO &
PID=$!

# Open a file descriptor (#8) to write to the input FIFO
exec 8> $INFIFO

# Open a file descriptor (#9) to read from the output FIFO.
exec 9<> $OUTFIFO

# Send two lines of text to the running copy of sed.
echo "This is a test." >&8
echo "This is maybe a test." >&8

# Read the first two lines from sed's output.
read -u9 A
echo "Result 1: $A"
read -u9 A
echo "Result 2: $A"

# Send another line of text to the running copy of sed.
echo "This is also a test." >&8

# Read it back.
read  -u9 A
echo "Result 3: $A"

# Show that sed is still running.
ps -p $PID

# Close the pipes to terminate sed.
exec 8>&-
exec 9<&-

# Show that sed is no longer running.
ps -p $PID

# Clean up the FIFO files in /tmp
rm "$INFIFO"
rm "$OUTFIFO"

