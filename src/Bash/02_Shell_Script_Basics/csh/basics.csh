#!/bin/csh

set VARIABLE = "Silly"
if ( "x${VARIABLE}" == "xSilly" ) then
    echo "Silly human, scripts are for kiddies."
endif

if ( -d "/System/Library/Frameworks" ) then
    echo "/System/Library/Frameworks is a directory."
endif

