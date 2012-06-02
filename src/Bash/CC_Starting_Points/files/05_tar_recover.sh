#!/bin/sh

USERNAME="remoteuser"
REMOTEHOST="remotehost.example.org"
SRCDIR="/path/to/testdir"
OUTDIR="/remote/path/here"

# Format is "path bytecount"
LOCALFORMATFLAG="-f"  # Mac OS X
LOCALFORMAT="%N %z"  # Mac OS X
REMOTEFORMATFLAG="-c" # Linux
REMOTEFORMAT="%n %s"  # Linux

OUTDIRQUOTED="$(echo "$OUTDIR" | sed 's/"/\\"/g')"

IFS="
"

BACKUPLIST=""

cd "$SRCDIR"

# Generate a list of files and their length in bytes on the local
# and local machines.
LOCALFILELIST="$(cd "$SRCDIR" ; find . -exec stat "$LOCALFORMATFLAG" \
    "$LOCALFORMAT" {}  \; | sort)"
REMOTEFILELIST="$(ssh $USERNAME@$REMOTEHOST "cd \"$OUTDIRQUOTED\" ; \
    find . -exec stat "$REMOTEFORMATFLAG" '$REMOTEFORMAT' {}  \; | sort")"

# echo "RFL: $REMOTEFILELIST"

# Loop until there are no more local files to check.
while true ; do
    LNFILES="$(echo "$LOCALFILELIST" | grep -c .)"
    LNFM1="$(expr "$LNFILES" '-' '1')"
    RNFILES="$(echo "$REMOTEFILELIST" | grep -c .)"
    RNFM1="$(expr "$RNFILES" '-' '1')"

    # echo "@TOP LNFM1: $LNFM1 RNFM1 $RNFM1"

    # If there are no more local files, break out of the outer loop.
    # Otherwise, pop the first filename from the list.
    if [ $LNFM1 -lt 0 ] ; then
        break;
    else
        LOCALLINE="$(echo "$LOCALFILELIST" | head -n 1)"
        LOCALFILE="$(echo "$LOCALLINE" | sed 's/ [0-9][0-9]*$//')"
        LOCALQUOTED="$(echo "$LOCALFILE" | sed 's/"/\\"/g')"
        LOCALLENGTH="$(echo "$LOCALLINE" | sed 's/.* \([0-9][0-9]*\)$/\1/')"
        LOCALFILELIST="$(echo "$LOCALFILELIST" | tail -n $LNFM1)"
    fi

    # If there are no more remote files, every local file must
    # be added to the list of files to copy.
    # Otherwise, pop the first filename from the list.
    if [ $RNFM1 -lt 0 ] ; then
        REMOTELINE=""
        REMOTEFILE=""
        REMOTELENGTH=0
        REMOTEFILELIST=""
    else
        REMOTELINE="$(echo "$REMOTEFILELIST" | head -n 1)"
        REMOTEFILE="$(echo "$REMOTELINE" | sed 's/ [0-9][0-9]*$//')"
        REMOTELENGTH="$(echo "$REMOTELINE" | sed 's/.* \([0-9][0-9]*\)$/\1/')"
        REMOTEFILELIST="$(echo "$REMOTEFILELIST" | tail -n $RNFM1)"
    fi

    # echo "OLOOP LOCALFILE: $LOCALFILE REMOTEFILE: $REMOTEFILE"
    # echo "LOCALFILELIST: $LOCALFILELIST"
    # echo "REMOTEFILELIST: $REMOTEFILELIST"

    # If the filenames do not match, then the local file does
    # not exist on the remote server (because the lists are sorted).
    if [ "$LOCALFILE" != "$REMOTEFILE" ] ; then

        # Until they do match, keep adding files to the list of stuff to copy.
        while [ "$LOCALFILE" != "$REMOTEFILE" -a "$LOCALFILE" != "" ] ; do
            # echo "NOMATCHLOOP LOCALFILE: $LOCALFILE REMOTEFILE: $REMOTEFILE"

            # echo "ADDED \"$LOCALQUOTED\" TO BACKUP LIST"

            BACKUPLIST="$BACKUPLIST \"$LOCALQUOTED\""

            # If it is a directory, adding the directory to the archive
            # adds everything in it, so skip everything in it.
            if [ -d "$LOCALFILE" ] ; then
                # echo "ISDIR"

                DIRLOOP=1
                LList2="$LOCALFILELIST"

                # Loop until we run out of files or the names do not match.
                while [ $DIRLOOP = 1 ] ; do
                    LOCALFILE="$(echo "$LOCALFILE" | sed 's/\/$//')"
                    LOCALQUOTED="$(echo "$LOCALFILE" | sed 's/"/\\"/g')"

                    LNFILES2="$(echo "$LList2" | grep -c .)"
                    LNFM1_2="$(expr "$LNFILES2" '-' '1')"

                    # echo "LList2: $LList2"
                    if [ $LNFM1_2 -lt 0 ] ; then
                        # We ran out of files, so stop looking for files in
                        # the directory.

                        LLine2=""
                        LF2=""
                        LLen2=0
                        LList2=""
                        DIRLOOP=0
                    else
                        # Grab the next file in the list.
                        LLine2="$(echo "$LList2" | head -n 1)"
                        LF2="$(echo "$LLine2" | sed 's/ [0-9][0-9]*$//')"
                        LLen2="$(echo "$LLine2" | sed 's/.* \([0-9][0-9]*\)$/\1/')"
                        LList2="$(echo "$LList2" | tail -n $LNFM1_2)"

                        # echo "INDIRLOOP: FILE IS $LF2"

                        # Repeatedly strip off the last part of the path
                        # until it matches or the path is empty.
                        INDIR="NO"
                        while [ "$LF2" != "" -a "$LF2" != "." ] ; do
                            # echo "LF2: \"$LF2\""
                            LF2="$(dirname "$LF2" | sed 's/\/$//')";
                            if [ "$LF2" = "$LOCALFILE" ] ; then
                                # It matches.  The file is in the directory.
                                INDIR="YES"; LF2="";
                            fi
                        done
                        if [ $INDIR = "YES" ] ; then
                            # Because this file is in the directory, commit
                            # the changes to the local file list (thus
                            # removing this file from the list).

                            # echo "INDIR"
                            LOCALFILELIST="$LList2"
                        else 
                            # This file is not in the directory.  Don't take
                            # it off the list, and stop looking for files in
                            # the directory.

                            # echo "NOTINDIR"
                            DIRLOOP=0
                        fi
                    fi
                done

                # Recount the number of files in the local list because it may
                # have changed significantly.
                LNFILES="$(echo "$LOCALFILELIST" | grep -c .)"
                LNFM1="$(expr "$LNFILES" '-' '1')"

            else
                # It is not a directory.  Pop the file from the list.

                # echo "@BOTTOM LOCALFILELIST: $LOCALFILELIST"

                # Recount the number of files in the local list.
                LNFILES="$(echo "$LOCALFILELIST" | grep -c .)"
                LNFM1="$(expr "$LNFILES" '-' '1')"

                # echo "@BOTTOM LNFM1: $LNFM1 RNFM1 $RNFM1"

                # Grab the next file.  This is the middle loop iterator
                # testing to see if the filename matches.
                if [ $LNFM1 -lt 0 ] ; then
                    LOCALLINE=""
                    LOCALFILE=""
                    LOCALQUOTED=""
                    LOCALLENGTH=0
                    LOCALFILELIST=""
                else
                    LOCALLINE="$(echo "$LOCALFILELIST" | head -n 1)"
                    LOCALFILE="$(echo "$LOCALLINE" | sed 's/ [0-9][0-9]*$//')"
                    LOCALQUOTED="$(echo "$LOCALFILE" | sed 's/"/\\"/g')"
                    LOCALLENGTH="$(echo "$LOCALLINE" | sed 's/.* \([0-9][0-9]*\)$/\1/')"
                    LOCALFILELIST="$(echo "$LOCALFILELIST" | tail -n $LNFM1)"
                fi

            fi
        done
    fi

    # When the script reaches this point, 
    if [ "$LOCALFILE" = "$REMOTEFILE" -a "$LOCALFILE" != "" \
            -a $LOCALLENGTH != $REMOTELENGTH ] ; then
        if [ ! -d "$LOCALFILE" ] ; then
            # echo "ADDED \"$LOCALQUOTED\" TO BACKUP LIST"
            BACKUPLIST="$BACKUPLIST \"$LOCALQUOTED\""
        fi
    fi
done

echo "BACKUPLIST $BACKUPLIST"

if [ "$BACKUPLIST" != "" ] ; then
        eval tar -czf - $BACKUPLIST  | ssh $USERNAME@$REMOTEHOST \
                "cd \"$OUTDIRQUOTED\" ; tar -xzf -"
fi


