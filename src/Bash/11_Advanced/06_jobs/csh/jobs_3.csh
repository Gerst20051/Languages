#!/bin/csh

# This example depends on the jobs builtin, which is a
# BASH extension.  It is also available on most other
# Bourne shell variants, including zsh, ash, and dash,
# but may not be completely portable.

set MAXJOBS=3


set JOBLIST=""

# Must put list in a file because C shell doesn't allow newlines in variables.
set COMMANDLIST='commandlist'
set CMDCOUNT=`grep -c . commandlist`

set I=1

# echo "I: $I"
# echo "CMDCOUNT: $CMDCOUNT"

while (${I} <= ${CMDCOUNT})
	set COMMAND = `head -n $I $COMMANDLIST | tail -n 1`
	echo "CL: "${COMMANDLIST}

	# Can't use inline execution to run jobs builtin because
	# it would run in a separate shell.
	jobs > /tmp/joblist
	set JOBCOUNT=`cat /tmp/joblist | grep -c .`
	while ( $JOBCOUNT >= $MAXJOBS )
		sleep 1
		jobs > /tmp/joblist
		set JOBCOUNT=`cat /tmp/joblist | grep -c .`
	end
	echo "JC: $JOBCOUNT"
	source subroutines/spawnjob.csh "$COMMAND" &
	set LASTJOB=$!
	set JOBLIST="$JOBLIST $LASTJOB"
	set I=`expr $I '+' 1`
end

# for JOB in $JOBLIST ; do
	# wait $JOB
	# echo "Job $JOB exited with status $?"
# done
wait

echo "Done."

