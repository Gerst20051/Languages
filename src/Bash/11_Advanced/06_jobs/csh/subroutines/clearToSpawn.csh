#!/bin/csh

set MAXJOBS = $1
set JOBCOUNT=`jobs | grep -c .`
if ( $JOBCOUNT < $MAXJOBS ) then
        echo 1;
        exit 1;
endif
    
echo 0;
exit 0;

