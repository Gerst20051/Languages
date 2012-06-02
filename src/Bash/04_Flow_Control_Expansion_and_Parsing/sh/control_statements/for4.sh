#!/bin/sh

set IFS = ":"

# IFS should be ignored, so you should get
#
# a:b:c
# d
#
# However some early bourne shells may treat this
# just like for3.sh
for i in a:b:c d ; do
	echo $i
done

