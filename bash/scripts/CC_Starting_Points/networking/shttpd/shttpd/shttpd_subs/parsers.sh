#!/bin/sh

# /*!
#     @header
#         Data parsers and routines for working with HTTP headers.
#  */

# /*! @group HTTP Request Parsing */

# /*!
#     @abstract
#         Parses an HTTP request line.
#     @param REQUEST
#         The request line.  For example, "GET / HTTP/1.1"
#  */
parseRequest()
{
	local REQUEST="$(echo "$1" | tr -d '\r')"

	TYPE="$(echo "$REQUEST" | cut -f 1 -d ' ')"
	URL="$(echo "$REQUEST" | cut -f 2 -d ' ')"
	VERSION="$(echo "$REQUEST" | cut -f 3 -d ' ')"

	echo "GOT REQUEST: $REQUEST" 1>&2
}

# /*!
#     @abstract
#         Parses CGI output and forwards it to the client.
#     @discussion
#         This function starts by parsing the headers sent by the CGI script.
#
#         Upon reaching the end of the headers, it interprets the Status
#         header, then sends the HTTP status line with the correct status code.
#
#         Finally, it resends the data from the CGI script to the client
#         until the CGI script exits or closes its standard output.
#
#         This function expects to be run in a pipe.  It reads data from
#         the CGI script through its standard input and sends data to
#         the client through its standard output.
#     @param REQUEST
#         The request line.  For example, "GET / HTTP/1.1"
#  */ 
parseCGIOutput()
{
	STDIN=0
	STDOUT=1

	# ALLDATA="$(cat <&$STDIN)"
	# echo "ALLDATA: \"$ALLDATA\"" 2>&1

	parseHeaders $STDIN "CGIHEADERS"
	CGIHEADERS="$(getLastNodeName)"

	# echo "DATA IS $DATA"

        STATUSOBJ="$(treeSearch "$CGIHEADERS" "Status")"

	STATUS=200
	if [ "$STATUSOBJ" != "" ] ; then
		STATUSLINE="$(treeField "$STATUSOBJ" "Contents")"
	fi

	# echo "Here" 1>&2

	# echo "CGIH: $CGIHEADERS" 1>&2

	# Default content type if none is specified.
	addHeader "Content-Type" "text/plain" "$RESPONSEHEADERTREE"

	mergeTrees "$CGIHEADERS" "$RESPONSEHEADERTREE"

	# printTree "$RESPONSEHEADERTREE"

	HEADERDATA="$(getHeadersToSend)"
	# echo "Sending headers \"$HEADERDATA\"" 1>&2

	# Output status and headers.
	sendData "$STDOUT" ""

	# Pass the CGI script data on to the client.
	cat <&$STDIN >&$STDOUT

	deleteTree "$CGIHEADERS"
}

# /*!
#     @abstract
#         Receives headers from the client and parses them.
#     @discussion
#         This function reads lines from the client until it reaches the
#         end of the headers, then returns control to the caller to continue
#         reading data from the client if applicable.
#
#         It stores the headers into a global (or scope-local) variable
#         that by default is called HEADERTREE.
#     @param FD
#         The file descriptor for sending data to {@link //apple_ref/doc/man/1/nc nc}.
#     @param TREENAME
#         The name of the binary tree to create with the data.  If blank,
#         the name "HEADERTREE" is used.
#  */
parseHeaders()
{
	local FD="$1"
	local TREENAME="$2"
	local HEADERLINE

	if [ "$TREENAME" = "" ] ; then
		TREENAME="HEADERTREE"
	fi

	# Creates a new tree head object with the specified name.
	newTree "$TREENAME"
	eval $TREENAME=\"\$\(getLastNodeName\)\"

	# echo "TN: $TREENAME" 1>&2

	# Reads headers from the specified file descriptor until
	# it gets a blank line, pasing each one to a parser..
	while true ; do
		eval read -u$FD HEADERLINE
		HEADERLINE="$(echo "$HEADERLINE" | tr -d '\r')"
		# echo "GOT HEADER LINE: \"$HEADERLINE\"" 1>&2

		if [ "$HEADERLINE" = "" ] ; then
			# End of headers reached.
			# echo "End of headers" 1>&2
			break;
		fi

		addHeaderLine "$HEADERLINE" "$TREENAME"
	done
	LAST_TREE_NODE_INSERTED="$TREENAME"
}

# /*!
#     @abstract
#         Parses a single header line and stores the result into a binary tree.
#     @param HEADERLINE
#         The raw line from the header.
#     @param TREENAME
#         The name of the binary tree to insert this header into.
#  */
addHeaderLine()
{
	local HEADERLINE="$1"
	local TREE="$2"

	local FIELDNAME="$(echo "$HEADERLINE" | cut -f 1 -d ':')"
	local FIELDVALUE="$(echo "$HEADERLINE" | cut -f 2- -d ':' | sed 's/^[[:space:]]//g')"

	addHeader "$FIELDNAME" "$FIELDVALUE" "$TREE"
}

# /*!
#     @abstract
#         Adds a single header into a binary tree.
#     @param FIELDNAME
#         The name of the header field.
#     @param FIELDVALUE
#         The value of the header field.
#     @param TREENAME
#         The name of the binary tree to insert this header into.
#  */
addHeader()
{
	local FIELDNAME="$1"
	local FIELDVALUE="$2"
	local TREE="$3"

	# echo "Inserting $FIELDNAME with value $FIELDVALUE into $TREE" 1>&2
	insertKey "$TREE" "$FIELDNAME"
	NODE="$(getLastNodeName)"
	setTreeField "$NODE" "Contents" "$FIELDVALUE"
}

# /*!
#     @abstract
#         Tests the request parser.
#     @param REQUEST
#         The request to test.
#     @param EXP_TYPE
#         The expected <code>TYPE</code> that the parser should return.
#     @param EXP_URL
#         The expected <code>URL</code> that the parser should return.
#     @param EXP_VERSION
#         The expected <code>VERSION</code> that the parser should return.
#  */
testRequest()
{
	REQUEST="$1"
	EXP_TYPE="$2"
	EXP_URL="$3"
	EXP_VERSION="$4"

	parseRequest "$REQUEST"

	if [ "$TYPE" != "$EXP_TYPE" ] ; then
		echo "Type parsed incorrectly.  Expected \"$EXP_TYPE\" got \"$TYPE\"."
	else
		echo "OK ($TYPE = $EXP_TYPE)"
	fi
	if [ "$URL" != "$EXP_URL" ] ; then
		echo "URL parsed incorrectly.  Expected \"$EXP_URL\" got \"$URL\"."
	else
		echo "OK ($URL = $EXP_URL)"
	fi
	if [ "$VERSION" != "$EXP_VERSION" ] ; then
		echo "Version parsed incorrectly.  Expected \"$EXP_VERSION\" got \"$VERSION\"."
	else
		echo "OK ($VERSION = $EXP_VERSION)"
	fi

}

# /*!
#     @abstract
#         Sends a file to the client.
#     @discussion
#         If the HTTP status and headers have not been sent, this function
#         sends them first.
#     @param FD
#         The file descriptor for sending data to {@link //apple_ref/doc/man/1/nc nc}.
#     @param LOCALFILE
#         The complete local path of the file to send.
#     @param URL
#         The URL associated with the request.
#  */
sendFile()
{
	FD="$1"
	LOCALFILE="$2"
	URL="$3"

	if [ ! -r "$LOCALFILE" ] ; then
		echo "403 Sendfile" 1>&2
		STATUS="403"
		handleError "$FD" "$URL"
		return;
	fi

	sendStatus "$FD"
	sendHeaders "$FD"
	eval cat \"$LOCALFILE\" \>\&$FD
}

# /*!
#     @abstract
#         Sends a string to the client.
#     @discussion
#         If the HTTP status and headers have not been sent, this function
#         sends them first.
#     @param FD
#         The file descriptor for sending data to {@link //apple_ref/doc/man/1/nc nc}.
#     @param DATA
#         The string to send.
#  */
sendData()
{
	FD="$1"
	DATA="$2"

	sendStatus "$FD"
	sendHeaders "$FD"

	eval printf \"%s\" \"\$DATA\" \>\&$FD
}

# /*!
#     @abstract
#         Sends the HTTP status line to the client.
#     @discussion
#         If the status has already been sent, this function returns silently.
#
#         This function relies on two global variables:
#
#         If {@link //apple_ref/shell/var/STATUSLINE STATUSLINE} is set to
#         a complete status line (by PHP), the entire line is sent after
#         the HTTP version code.
#
#         Otherwise, the numerical status code is obtained from
#         {@link //apple_ref/shell/var/STATUS STATUS} if set
#         or 200 if it is not set, and the status string is generated from
#         that value by calling {@link //apple_ref/shell/func/getStatusText getStatusText}.
#         
#     @param FD
#         The file descriptor for sending data to {@link //apple_ref/doc/man/1/nc nc}.
#  */
sendStatus()
{
	FD="$1"

	if [ "$STATUSSENT" = "1" ] ; then
		return;
	fi
	STATUSSENT="1"

	if [ "$STATUSLINE" != "" ] ; then
		eval printf \"HTTP/1.1 $STATUSLINE\\n\" \>\&$FD
	else
		if [ "$STATUS" = "" ] ; then
			STATUS="200"
		fi
		STATUSTEXT="$(getStatusText $STATUS)"
		eval printf \"HTTP/1.1 $STATUS $STATUSTEXT\\n\" \>\&$FD
	fi
}

# /*!
#     @abstract
#         Sends a single HTTP header to the client.
#     @param NODE
#         A binary tree node name associated with the header.
#  */
printHTTPHeader()
{
	# Inherit FD from parent context.
	NODE="$1"

	if [ "$NODE" = "" ] ; then
		return;
	fi

	NAME="$(treeKey "$NODE")"
	VALUE="$(treeField "$NODE" "Contents")"

	if [ "$NAME" != "" ] ; then
		echo "$NAME: $VALUE"
	fi
}

# /*!
#     @abstract
#         Obtains a string containing HTTP headers to send to the client.
#     @discussion
#         Uses the global or context-local variable
#         {@link //apple_ref/shell/var/RESPONSEHEADERTREE RESPONSEHEADERTREE}.
#     @param NODE
#         A binary tree node name associated with the header.
#  */
getHeadersToSend()
{

	SENDHEADERS="Connection: close$NEWLINE"
	SENDHEADERS="$SENDHEADERS$(iterateTree "$RESPONSEHEADERTREE" printHTTPHeader)"
	# echo "HEADERS: \"$SENDHEADERS\"" 1>&2

	if [ "$SENDHEADERS" != "" ] ; then
		SENDHEADERS="$SENDHEADERS$NEWLINE"
	fi
	echo "$SENDHEADERS"
}

# /*!
#     @abstract
#         Obtains a string containing HTTP headers to send to the client.
#     @discussion
#         Uses the global or context-local variable 
#         {@link //apple_ref/shell/var/RESPONSEHEADERTREE RESPONSEHEADERTREE}.
#     @param FD
#         The file descriptor for sending data to {@link //apple_ref/doc/man/1/nc nc}.
#     @param ALLOWCONTINUATION
#         Set to 1 if the headers should be allowed to continue.  This was used
#         before proper CGI response parsing was implemented and is no longer useful.
#  */
sendHeaders()
{
	local FD="$1"
	local ALLOWCONTINUATION="$2"

	# echo "INSH" 1>&2

	if [ "$HEADERSSENT" = "1" ] ; then
		return;
	fi
	HEADERSSENT="1"

	# if [ "$ALLOWCONTINUATION" = "1" ] ; then
		# addHeader "X-CGI-Continuing-Headers" "true" "$RESPONSEHEADERTREE"
	# fi

	SENDHEADERS="$(getHeadersToSend)"
	eval printf \"%s\\n\" \"\$SENDHEADERS\" \>\&$FD
	# eval printf \"X-CGI-Continuing-Headers: true\\n\" \>\&$FD

	if [ "$ALLOWCONTINUATION" != "1" ] ; then
		eval printf \"\\n\" \>\&$FD
	fi
}

if [ "$DISABLE_TESTS" != "true" ] ; then
	SCRIPT="$(which $0)"
	SCRIPTDIR="$(dirname "$SCRIPT")"

	DISABLE_TESTS=true
	. "$SCRIPTDIR"/binary_tree.sh

	testRequest "GET /" "GET" "/" ""
	testRequest "GET / HTTP/1.0" "GET" "/" "HTTP/1.0"
	testRequest "GET /photos/index.php HTTP/1.1" "GET" "/photos/index.php" "HTTP/1.1"

fi

