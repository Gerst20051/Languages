#!/bin/csh
printf "What is your name?  -> "
set LINE = "$<"
set NAME = `echo "$LINE" | cut -f 1 -d ' '`
set Age = `echo "$LINE" | cut -f 2 -d ' '`
echo "Hello, ${NAME}.  Nice to meet you.  You are ${Age} years old.  Man, that's young."
