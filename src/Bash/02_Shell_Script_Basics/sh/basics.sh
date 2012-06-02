#!/bin/sh

VARIABLE="Silly"
if [ "x$VARIABLE" = "xSilly" ] ; then
    echo "Silly human, scripts are for kiddies."
fi

if [ -d "/System/Library/Frameworks" ] ; then
    echo "/System/Library/Frameworks is a directory."
fi

