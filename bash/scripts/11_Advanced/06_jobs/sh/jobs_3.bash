#!/bin/bash

# This example depends on the jobs builtin, which is a
# BASH extension.  It is also available on most other
# Bourne shell variants, including zsh, ash, and dash,
# but may not be completely portable.

MAXJOBS=3

function spawnjob()
{
	echo $1 | bash
}

function clearToSpawn
{
    local JOBCOUNT="$(jobs -r | grep -c .)"
    if [ $JOBCOUNT -lt $MAXJOBS ] ; then
        echo 1;
        return 1;
    fi
    
    echo 0;
    return 0;
}

JOBLIST=""

COMMANDLIST='ls
echo "sleep 3"; sleep 3; echo "sleep 3 done"
echo "sleep 10"; sleep 10 ; echo "sleep 10 done"
echo "sleep 1"; sleep 1; echo "sleep 1 done"
echo "sleep 5"; sleep 5; echo "sleep 5 done"
echo "sleep 7"; sleep 7; echo "sleep 7 done"
echo "sleep 2"; sleep 2; echo "sleep 2 done"
'

IFS="
"

for COMMAND in $COMMANDLIST ; do
	while [ `clearToSpawn` -ne 1 ] ; do
		sleep 1
	done
	spawnjob $COMMAND &
	LASTJOB=$!
	JOBLIST="$JOBLIST $LASTJOB"
done

IFS=" "

for JOB in $JOBLIST ; do
	wait $JOB
	echo "Job $JOB exited with status $?"
done

echo "Done."

