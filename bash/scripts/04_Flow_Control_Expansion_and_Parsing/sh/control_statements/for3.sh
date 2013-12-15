#!/bin/sh

IFS=":"
LIST="a:b:c d"
for i in $LIST ; do
	echo $i
done

