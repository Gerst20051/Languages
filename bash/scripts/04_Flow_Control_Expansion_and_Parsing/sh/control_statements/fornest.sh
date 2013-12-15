#!/bin/sh

# A simple example of continue with a numerical argument.
# This code prompts you with the letter "a", "b", or "c",
# then expects you to type something in reply.
#
# If you enter any word or phrase other than the word "skip",
# the script prints the same letter again because of the inner
# "while" loop.
#
# If you press return without entering anything, it prints the
# words "not skipped" and then prints the next letter because
# of the outer "for" loop.
#
# If you type the word "skip", the "continue 2" statement
# causes the outer "for" loop to continue, not the inner while
# loop.  Thus, the statement that would otherwise echo "not
# skipped" is, in fact, skipped.  Then, the script prints the
# following letter (or exits if you are already on the final
# letter).

for i in a b c ; do
	echo $i
	read temp
	while [ "x$temp" != "x" ] ; do
		if [ "x$temp" = "xskip" ] ; then
			continue 2
		fi
		echo $i
		read temp
	done
	echo not skipped
done

