
# /*!
#     @header
#         Routines specific to CGI handling (environment variable exports,
#         spawning scripts, filename comparison, etc.).
#  */

# /*! @abstract Not a CGI script/binary. */
SPAWNTYPE_NONE=0
# /*! @abstract A CGI script/binary or shell script. */
SPAWNTYPE_CGI=1
# /*! @abstract A Perl script. */
SPAWNTYPE_PERL=2
# /*! @abstract A PHP script. */
SPAWNTYPE_PHP=3


# Disable Perl and PHP if no executable was set in the config file.
if [ "$PERL_EXECUTABLE" = "" ] ; then
	SPAWNTYPE_PERL=$SPAWNTYPE_NONE
fi
if [ "$PHP_CGI_EXECUTABLE" = "" ] ; then
	SPAWNTYPE_PHP=$SPAWNTYPE_NONE
fi

# /*! @group Environment Variable Functions
#         These functions export and unset environment variables for the
#         CGI environment.
#  */
# /*!
#     @abstract
#         Internal function.
#     @discussion
#         Used by {@link unexportCGIVars} to unexport a single environment
#         variable associated with an HTTP header.
#  */
unexportHTTPVar()
{
	local NODE="$1"

	if [ "$NODE" = "" ] ; then
		return;
	fi

	local NAME="$(toupper "$(treeKey "$NODE")")"
	NAME="$(print "$NAME" | sed 's/-/_/g' | sed 's/[^a-zA-Z0-9_-]/_/')"

	if [ "$NAME" = "" ] ; then
		return;
	fi

	eval unset $NAME
}

# /*!
#     @abstract
#         Internal function.
#     @discussion
#         Used by {@link unexportCGIVars} to unexport a single environment
#         variable associated with an HTTP header.
#  */
exportHTTPVar()
{
	local NODE="$1"

	if [ "$NODE" = "" ] ; then
		return;
	fi

	local VALUE="$(treeField "$NODE" "Contents")"
	local NAME="$(toupper "$(treeKey "$NODE")")"
	NAME="$(print "$NAME" | sed 's/-/_/g' | sed 's/[^a-zA-Z0-9_-]/_/')"

	if [ "$NAME" = "" ] ; then
		return;
	fi

	eval export $NAME=\"$VALUE\"
}

# /*!
#     @abstract
#         Exports the environment variables necessary for CGI compliance.
#     @discussion
#         Uses {@link exportHTTPVar} to export HTTP headers as environment variables.
#  */
exportCGIVars()
{
	local HEADERS="$1"
	local HOSTNAME="$2"
	local PORT="$3"
	local METHOD="$4" # GET/POST
	local SCRIPT_PATH="$5"
	local SCRIPT_REMAINDER="$6"
	local PARMPART="$7"
	local ROOT="$8"
	# local CTYPE="$8" # Used for POST
	# local CLEN="$9" # Used for POST

	iterateTree "$HEADERS" "exportHTTPVar"

	export SERVER_SOFTWARE="shttpd/$SHTTPD_VERSION"
	export SERVER_NAME="$HOSTNAME"
	export GATEWAY_INTERFACE="CGI/1.1"
	export SERVER_PROTOCOL="HTTP/1.1"
	export SERVER_PORT="$PORT"
	export REQUEST_METHOD="$METHOD"
	export SERVER_ROOT="$ROOT"
	export DOCUMENT_ROOT="$ROOT"

	export PATH_INFO="$SCRIPT_REMAINDER"

	# This is what the HTTP spec says:
	# export PATH_TRANSLATED="$SERVER_ROOT/$SCRIPT_REMAINDER"

	# This is what PHP requires:
	export PATH_TRANSLATED="$SERVER_ROOT/$SCRIPT_PATH"

	export SCRIPT_NAME="$SCRIPT_PATH"
	export QUERY_STRING="$PARMPART"

	# export REMOTE_HOST   # Can't get from netcat.
	# export REMOTE_ADDR   # Can't get from netcat.
	# export AUTH_TYPE     # HTTP auth not worth supporting.
	# export REMOTE_USER   # HTTP auth not worth supporting.
	# export REMOTE_IDENT  # HTTP auth not worth supporting.
	# export CONTENT_TYPE="$CTYPE" # This comes from the headers.
	# export CONTENT_LENGTH="$CLEN" # This comes from the headers.
}

# /*!
#     @abstract
#         Removes all of the custom environment variables exported
#         by {@link exportHTTPVar}.
#  */
unexportCGIVars()
{
	local HEADERS="$1"

	iterateTree "$HEADERS" "unexportHTTPVar"

}

# /*! @group CGI handling Functions
#         These functions check to see if URLs are CGI scripts and launch
#         CGI scripts.
#  */

# /*!
#     @abstract
#         Returns the type of CGI script associated with a URL.
#     @result
#         Returns {@link SPAWNTYPE_CGI} if the URL is a shell script or CGI binary,
#         {@link SPAWNTYPE_PERL} if the URL is a Perl script, {@link SPAWNTYPE_PHP}
#         if the URL is a PHP script, or {@link SPAWNTYPE_NONE} if it is not a CGI
#         script.
#     @param FULLPATH
#         The path to check.
#  */
cgiMatch()
{
	FULLPATH="$1"

	ENDING="$(tolower "$(echo "$FULLPATH" | sed 's/.*\///' | sed 's/.*\././')")"

	# echo "ENDING: $ENDING" 1>&2

	case "$ENDING" in
		.sh | .cgi )
			if [ -x "$FULLPATH" ] ; then
				# echo "CGI script" 1>&2
				return $SPAWNTYPE_CGI
			fi ;;
		.pl | .perl )
			# echo "Perl script ($SPAWNTYPE_PERL)" 1>&2
			return $SPAWNTYPE_PERL ;;
		.php | .php4 | .php5 )
			# echo "PHP script" 1>&2
			return $SPAWNTYPE_PHP ;;
	esac

	# echo "Not a script ($ENDING)" 1>&2
	return $SPAWNTYPE_NONE
}

# /*!
#     @abstract
#         Starts a CGI script.
#     @param FD
#         The file descriptor for sending data to {@link //apple_ref/doc/man/1/nc nc}.
#     @param READFD
#         The file descriptor for reading data from {@link //apple_ref/doc/man/1/nc nc}.
#     @param SERVER_ROOT
#         The base path for the server root for this domain.
#     @param SCRIPT_PATH
#         The path of the script relative to <code>SERVER_ROOT<code>.
#     @param SCRIPT_REMAINDER
#         If the script name is followed by additional path parts,
#         this variable contains the additional path parts as a string, otherwise empty.
#         For example, if the URL is "http://example.org/script.cgi/foo/bar", the value
#         of SCRIPT_REMAINDER would be "/foo/bar".
#     @param PARMPART
#         The query string after the question mark, or an empty string.
#     @param HOST
#         The hostname:port combination against which the request was made.
#     @param METHOD
#         The request method used (GET, POST, and so on).
#     @param HEADERTREE
#         A binary tree of headers sent by the client.  See
#         {@link //apple_ref/shell/func/parseHeaders parseHeaders} for more information.
#  */
spawn_cgi()
{
	FD="$1"
	READFD="$2"
	SERVER_ROOT="$3"
	SCRIPT_PATH="$4"
	SCRIPT_REMAINDER="$5"
	PARMPART="$6"
	HOST="$7"
	METHOD="$8"
	HEADERTREE="$9"

	LOCALFILE="$SERVER_ROOT$SCRIPT_PATH"

	HOSTNAME="$(echo "$HOST" | sed 's/:.*$//')"
	PORT="$(echo "$HOST" | sed 's/^.*://')"

	exportCGIVars "$HEADERTREE" "$HOSTNAME" "$PORT" "$METHOD" "$SCRIPT_PATH" "$SCRIPT_REMAINDER" "$PARMPART" "$SERVER_ROOT"

	# export >&$FD

	# printTree "$HEADERTREE" >&8

	# sendStatus "$FD"
	# sendHeaders "$FD" "1"

	# echo "ST_P: $SPAWNTYPE_PHP"

	# export REDIRECT_REQUEST="$FULLURL"
	# export REDIRECT_URL="$FULLURL"
	# export REDIRECT_STATUS="404"

	OWD="$PWD"
	cd "$DOCUMENT_ROOT"

	cgiMatch "$LOCALFILE"
	case "$?" in
		$SPAWNTYPE_CGI)
			"$LOCALFILE" <&$READFD | parseCGIOutput >&$FD ;;
		$SPAWNTYPE_PERL)
			"$PERL_EXECUTABLE" "./$SCRIPT_PATH" <&$READFD | parseCGIOutput >&$FD ;;
		$SPAWNTYPE_PHP)
			# echo \"$PHP_CGI_EXECUTABLE\" -d \"cgi.force_redirect=0\" -d \"doc_root=$SERVER_ROOT\" \"$SCRIPT_PATH\" 1>&2

			# For debugging
			# export

			export PHP_DOCUMENT_ROOT="$SERVER_ROOT"
			"$PHP_CGI_EXECUTABLE" -d "cgi.force_redirect=0" -d "doc_root=$DOCUMENT_ROOT" "$SCRIPT_PATH" <&$READFD | parseCGIOutput >&$FD
			unset PHP_DOCUMENT_ROOT
			 ;;
	esac
	cd "$OWD"

	unexportCGIVars "$HEADERTREE"
}


