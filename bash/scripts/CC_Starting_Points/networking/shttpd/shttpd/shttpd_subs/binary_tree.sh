#!/bin/sh

# /*!
#     @header
#         A binary tree algorithm written in a shell script.  The main
#         functions of interest are {@link newTree}, {@link deleteTree},
#         {@link insertKey}, {@link insertKeyNumeric}, {@link treeSearch},
#         {@link treeSearchNumeric}, {@link iterateTree}, and
#         {@link mergeTrees}.
#
#         This is a minimal binary tree implementation that does not support
#         removing existing values from the tree once inserted.  However, such
#         functionality can be trivially retrofitted on top by adding or
#         clearing a "deleted" attribute on nodes using {@link setTreeField} if
#         desired.
#
#         To use this shell script, source it after setting DISABLE_TESTS to
#         "true".  To run tests, execute the script directly.
#  */

# /*! @group Global Variables
#         Variables used internally.  No user-serviceable parts inside.
#  */

# /*!
#     @abstract The starting object ID.  This is an internal counter.
#  */
OID=0

# /*!
#     @abstract A newline character.
#  */
NEWLINE="
"

# /*!
#     @abstract
#         Field separator.  Do not change.
#  */
IFS="$NEWLINE"

# /*! @group Node Functions
#         Functions that operate on a single node in the tree.
#  */

# /*!
#     @abstract Retrieves the key associated with a node object.
#     @result
#         Returns the key via <code>stdout</code>.
#     @param NODE
#         The node object.
#  */
treeKey()
{
	local NODE="$1"

	eval echo "\$$NODE"_KEY
}

# /*!
#     @abstract
#         Retrieves the left subtree for a node in the tree.
#     @result
#         Returns the node name of the left subtree via <code>stdout</code>.
#     @discussion
#         This is mainly an internal function, though you can use
#         it for debugging purposes.
#     @param NODE
#         The node object.
#  */
treeLeft()
{
	local NODE="$1"

	eval echo "\$$NODE"_LEFT
}

# /*!
#     @abstract
#         Sets the left subtree for a node in the tree.
#     @discussion
#         This is an internal function.  Do not call it directly.  Use
#         {@link insertKey} or {@link insertKeyNumeric} instead.
#     @param NODE
#         The node object.
#     @param VAL
#         The new left value.
#  */
setTreeLeft()
{
	local NODE="$1"
	local VAL="$2"

	eval "$NODE"_LEFT=\"$VAL\"
}

# /*!
#     @abstract
#         Retrieves the right subtree for a node in the tree.
#     @result
#         Returns the node name of the right subtree via <code>stdout</code>.
#     @discussion
#         This is mainly an internal function, though you can use
#         it for debugging purposes.
#     @param NODE
#         The node object.
#  */
treeRight()
{
	local NODE="$1"

	eval echo "\$$NODE"_RIGHT
}

# /*!
#     @abstract
#         Sets the right subtree for a node in the tree.
#     @discussion
#         This is an internal function.  Do not call it directly.  Use
#         {@link insertKey} or {@link insertKeyNumeric} instead.
#     @param NODE
#         The node object.
#     @param VAL
#         The new right value.
#  */
setTreeRight()
{
	local NODE="$1"
	local VAL="$2"

	eval "$NODE"_RIGHT=\"$VAL\"
}

# /*!
#     @abstract
#         Retrieves a field value for a node in the tree.
#     @result
#         Returns the requested field value via <code>stdout</code> or
#         an empty string.
#     @seealso setTreeField
#     @param NODE
#         The node object.
#     @param FIELDNAME
#         The field name.
#  */
treeField()
{
	local NODE="$1"
	local FIELDNAME="$2"

	eval echo "\$$NODE"_DATAFIELD_"$FIELDNAME"
}

# /*!
#     @abstract
#         Sets a field value for a node in the tree.
#     @discussion
#         This function allows you to store arbitrary attributes in a tree node.
#         If a value already exists for the specified field name, the value is
#         overwritten.
#     @param NODE
#         The node object.
#     @param FIELDNAME
#         The field name.
#     @param VAL
#         The new field value.
#  */
setTreeField()
{
	local NODE="$1"
	local FIELDNAME="$2"
	local VAL="$3"

	eval "$NODE"_DATAFIELD_"$FIELDNAME"=\"$VAL\"
	local DATAFIELDS="$(eval echo "\$$NODE"_DATAFIELDS)"
	eval "$NODE"_DATAFIELDS="\"$DATAFIELDS$NEWLINE$FIELDNAME\""
}


# /*! @group General Tree Functions
#         Operations that create, delete, iterate, and merge trees.
#  */

# /*!
#     @abstract
#         Iterates through a subtree, calling a function for each node.
#     @discussion
#         For each node in the tree (in sorted order), the function
#         specified by ACTION is called with a single parameter
#         containing the node name of the node being traversed.
#     @param TREE
#         The tree to traverse.
#     @param ACTION
#         The function to call on each node.
#     @param CALLONROOT
#         Set to 1 if you want to also call ACTION on the (bogus) root node.
#         This is usually only set for debug printing purposes.
#  */
iterateTree()
{
	local TREE="$1"
	local ACTION="$2"
	local CALLONROOT="$3"

	# echo "NAME IS $TREE"

	if [ "$CALLONROOT" = "1" ] ; then
		eval "$ACTION" "$TREE"
	fi

	iterateSubtree "$(treeLeft "$TREE")" "$ACTION"
}

# /*!
#     @abstract
#         Copies all of the keys in one tree into another.
#     @discussion
#         For each key in TREE_SRC, an equivalent key is
#         inserted in TREE_DST, including any field values
#         associated with it.  In the event of a collision
#         for a given key, the resulting set of field values
#         for that key is the union of the two sets of field
#         values, with the new values from TREE_SRC taking
#         precedence.
#     @param TREE_SRC
#         The source tree to copy.
#     @param TREE_DST
#         The destination tree into which the source tree is copied.
#  */
mergeTrees()
{
	local TREE_SRC="$1"
	local TREE_DST="$2"

	# echo "Here SRC: $TREE_SRC (left is $(treeLeft "$TREE_SRC"))" 1>&2
	# echo "     DST: $TREE_DST" 1>&2

	iterateSubtree "$(treeLeft "$TREE_SRC")" reinsert
}

# /*!
#     @abstract
#         Deletes a binary tree.
#     @param TREE
#         The name of the tree to delete.
#  */
deleteTree()
{
	local TREE="$1"

	if [ "$TREE" = "" ] ; then
		return;
	fi

	deleteTree "$(treeLeft "$TREE")"
	deleteTree "$(treeRight "$TREE")"
	deleteNode "$TREE"
}

# /*!
#     @abstract
#         Creates a new binary tree.
#     @result
#         Obtain the name of the tree using {@link getLastNodeName}.
#     @param TREE
#         The name of the tree to create.
#  */
newTree()
{
	local TREE="$1"

	newTreeNode "" "" "" "$TREE"
}

# /*! @group Search Functions
#         Functions used for searching for a key in a tree.  Be sure to
#         choose whether you want to use numerical or string key comparisons
#         for the search and choose the appropriate function accordingly.
#         The comparison type usde for searching must match the comparison
#         type used during insertion or the results are undefined.
#  */

# /*!
#     @abstract
#         Searches a binary tree for a given key.
#     @discussion
#         This tree search uses string comparisons.  You must use
#         {@link insertKey} with this function (and not
#         {@link insertKeyNumeric}.  For numeric searches, use
#         {@link treeSearchNumeric}.
#     @result
#         Returns the node name of the matching node through <code>stdout</code>
#         if found or an empty string otherwise.
#     @param TREE
#         The tree to search.
#     @param KEY
#         The key to search for.
#  */
treeSearch()
{
	local TREE="$1"
	local KEY="$2"

	subtreeSearch "$(treeLeft "$TREE")" "$KEY"
}

# /*!
#     @abstract
#         Searches a binary tree for a given key.
#     @result
#         Returns the node name of the matching node through <code>stdout</code>
#         if found or an empty string otherwise.
#     @discussion
#         This tree search uses numeric comparisons.  You must use
#         {@link insertKeyNumeric} with this function (and not
#         {@link insertKey}.  For string searches, use {@link treeSearch}.
#     @param TREE
#         The tree to search.
#     @param KEY
#         The key to search for.
#  */
treeSearchNumeric()
{
	local TREE="$1"
	local KEY="$2"

	subtreeSearchNumeric "$(treeLeft "$TREE")" "$KEY"
}

# /*! @group Insertion Functions
#         Functions used for inserting a key into a tree.  Be sure to
#         choose whether you want to use numerical or string key comparisons
#         during insertion and choose the appropriate function accordingly.
#
#         After inserting, you can use {@link getLastNodeName} to get the
#         node name of the resulting node if desired.
#  */

# /*!
#     @abstract
#         Retrieves the last node inserted.
#     @result
#         Returns the node name of the last node inserted via
#         <code>stdout</code>.
#     @discussion
#         After creating a new node with {@link insertKey} or a
#         new tree with {@link newTree}, call this to obtain its
#         note ID.
#  */
getLastNodeName()
{
	echo "$LAST_TREE_NODE_INSERTED"
}

# /*!
#     @abstract
#         Inserts a new key into a binary tree.
#     @discussion
#         If a node already exists with this value, the
#         existing node is returned.
#
#         This tree insertion uses string comparisons.  You must use
#         {@link treeSearch} with this function (and not
#         {@link treeSearchNumeric}.  For numeric searches, use
#         {@link insertKeyNumeric}.
#     @result
#         Obtain the node name of the newly created node using
#         {@link getLastNodeName}.
#     @param TREE
#         The name of the binary tree.
#     @param KEY
#         The key to insert.
#  */
insertKey()
{
	local TREE="$1"
	local KEY="$2"

	local LASTTREE="$TREE"
	local DIRECTION="LEFT"

	while [ "$TREE" != "" -a "$LASTTREE" != "" ] ; do
		if [ $DIRECTION = "LEFT" ] ; then
			TREE="$(treeLeft "$TREE")"
		else 
			TREE="$(treeRight "$TREE")"
		fi
		local TREEKEY="$(treeKey "$TREE")"

		if [ "$TREE" != "" ] ; then
			if [ "$KEY" \< "$TREEKEY" ] ; then
				DIRECTION="LEFT"
				LASTTREE="$TREE"
			elif [ "$KEY" \> "$TREEKEY" ] ; then
				DIRECTION="RIGHT"
				LASTTREE="$TREE"
			else
				# Matching node already exists.  Return its name.
				LAST_TREE_NODE_INSERTED="$NODE"
				return
			fi
		fi
	done
	newTreeNode "" "" "$KEY"
	local NODE="$(getLastNodeName)"

	if [ $DIRECTION = "LEFT" ] ; then
		setTreeLeft "$LASTTREE" "$NODE"
	else
		setTreeRight "$LASTTREE" "$NODE"
	fi
}

# /*!
#     @abstract
#         Inserts a new key into a binary tree.
#     @discussion
#         If a node already exists with this value, the
#         existing node is returned.
#
#         This tree insertion uses string comparisons.  You must use
#         {@link treeSearch} with this function (and not
#         {@link treeSearchNumeric}.  For numeric searches, use
#         {@link insertKeyNumeric}.
#     @result
#         Obtain the node name of the newly created node using
#         {@link getLastNodeName}.
#     @param TREE
#         The name of the binary tree.
#     @param KEY
#         The key to insert.
#  */
insertKeyNumeric()
{
	local TREE="$1"
	local KEY="$2"

	# echo "IN INSNUM"

	local LASTTREE="$TREE"
	local DIRECTION="LEFT"

	while [ "$TREE" != "" -a "$LASTTREE" != "" ] ; do
		if [ $DIRECTION = "LEFT" ] ; then
			TREE="$(treeLeft "$TREE")"
		else 
			TREE="$(treeRight "$TREE")"
		fi
		local TREEKEY="$(treeKey "$TREE")"

		if [ "$TREE" != "" ] ; then
			if [ "$KEY" -lt "$TREEKEY" ] ; then
				DIRECTION="LEFT"
				LASTTREE="$TREE"
			elif [ "$KEY" -gt "$TREEKEY" ] ; then
				DIRECTION="RIGHT"
				LASTTREE="$TREE"
			else
				# Matching node already exists.  Return its name.
				LAST_TREE_NODE_INSERTED="$NODE"
				return
			fi
		fi
	done
	newTreeNode "" "" "$KEY"
	local NODE="$(getLastNodeName)"

	if [ $DIRECTION = "LEFT" ] ; then
		setTreeLeft "$LASTTREE" "$NODE"
	else
		setTreeRight "$LASTTREE" "$NODE"
	fi
}


# /*! @group Debug Functions
#         Functions that print debug information about binary trees,
#         tree nodes, and so on.
#  */

# /*!
#     @abstract
#         Prints a node structure for debugging purposes.
#     @param NODE
#         The node to print.
#  */
printNode()
{
	local NODE="$1"

	echo "NAME:  $NODE"
	echo "KEY:   $(treeKey "$NODE")"
	echo "LEFT:  $(treeLeft "$NODE")"
	echo "RIGHT: $(treeRight "$NODE")"
	echo "DATA:"

	local DATAFIELDS="$(eval echo "\$$NODE"_DATAFIELDS)"
	local FIELDNAME
	for FIELDNAME in $DATAFIELDS ; do
		# Skip the empty first field.
		if [ "$FIELDNAME" != "" ] ; then
			eval echo "     $NODE""_DATAFIELD_$FIELDNAME"":" \
				"\$$NODE""_DATAFIELD_$FIELDNAME"
		fi
	done
	echo "-=-=-=-=-=-=-=-=-=-=-=-"
}

# /*!
#     @abstract
#         Prints out the contents of a tree for debugging purposes.
#  */
printTree()
{
	local TREE="$1"

	# echo "NAME IS $TREE"
	iterateTree "$TREE" "printNode" 1
}

# /*!
#     @abstract
#         Prints a line of text in red letters.
#  */
echored()
{
	printf "\e[1;31m%s\e[0;30m\n" $@
}

# /*!
#     @abstract
#         Prints a line of text in green letters.
#  */
echogreen()
{
	printf "\e[1;32m%s\e[0;30m\n" $@
}

# /*!
#     @abstract
#         Prints a line of text in blue letters.
#  */
echoblue()
{
	printf "\e[1;34m%s\e[0;30m\n" $@
}


# /*! @group Internal Functions
#         No user-serviceable parts inside.  These functions are used
#         internally by the other functions and should generally not
#         be called from outside unless you really know what you are
#         doing.
#  */

# /*!
#     @abstract
#         Iterates through a subtree, calling a function for each node.
#     @discussion
#         Do not call this directly.  Call {@link iterateTree} instead.
#  */
iterateSubtree()
{
	local TREE="$1"
	local ACTION="$2"

	if [ "$TREE" = "" ] ; then
		return;
	fi
	# echo "IN IST: TREE $TREE" 1>&2

	iterateSubtree "$(treeLeft "$TREE")" "$ACTION"
	eval "$ACTION $TREE"
	iterateSubtree "$(treeRight "$TREE")" "$ACTION"

}

# /*!
#     @abstract
#         Internal helper function.
#     @discussion
#         This function is used by {@link mergeTrees} to take a node from
#         one tree and duplicte it in another.
#  */
reinsert()
{
	local NODE="$1"

	# echo "GOT NODE \"$NODE\"" 1>&2

	# echo "TREE_DST: $TREE_DST" 1>&2

	if [ "$NODE" = "" ] ; then
		return;
	fi

	local VAL="$(treeKey "$NODE")"

	if [ "$VAL" = "" ] ; then
		return;
	fi

	# local NEWNODE="$(treeSearch "$TREE_DST" "$VAL")"
	# echo "NN1: $NEWNODE"

	insertKey "$TREE_DST" "$VAL"
	local NEWNODE="$(getLastNodeName)"

	# print "NN: $NEWNODE" 1>&2

	local DATAFIELDS="$(eval echo "\$$NODE"_DATAFIELDS)"
	local FIELDNAME

	for FIELDNAME in $DATAFIELDS ; do
		# Skip the empty first field.
		if [ "$FIELDNAME" != "" ] ; then
			# eval echo setting "$NEWNODE""_DATAFIELD_$FIELDNAME""=\"\$$NODE""_DATAFIELD_$FIELDNAME\"" 1>&2
			eval "$NEWNODE""_DATAFIELD_$FIELDNAME""=\
				\"\$$NODE""_DATAFIELD_$FIELDNAME\""
		fi
	done

	# printNode "$NODE"
}

# /*!
#     @abstract
#         Creates a new node in the tree.
#     @discussion
#         This is an internal function.  Do not call it directly.  Use
#         {@link insertKey} or {@link insertKeyNumeric} instead.
#     @param LEFT
#         The initial left value for the node (usually empty).
#     @param RIGHT
#         The initial right value for the node (usually empty).
#     @param KEY
#         The key for the new node.
#     @param TREE
#         The desired name for the node (usually empty).
#  */
newTreeNode()
{
	local LEFT="$1"
	local RIGHT="$2"
	local KEY="$3"
	local TREE="$4"

	if [ "$TREE" = "" ] ; then
		TREE="TREENODE_$OID"
		OID="$(expr "$OID" "+" "1")"
		# echo "$TREE"
	# else
		# echo "Using explicit name \"$TREE\"" 1>&2
	fi

	eval "$TREE"_LEFT=\"$LEFT\"
	eval "$TREE"_RIGHT=\"$RIGHT\"
	eval "$TREE"_KEY=\"$KEY\"
	LAST_TREE_NODE_INSERTED="$TREE"
}


# /*!
#     @abstract
#         Searches a binary tree for a given key.
#     @discussion
#         This is an internal function.  Do not call it directly.  Use
#         {@link treeSearch} instead.
#     @result
#         Returns the node name of the matching node through <code>stdout</code>
#         if found or an empty string otherwise.
#     @param TREE
#         The subtree to search.
#     @param KEY
#         The key to search for.
#  */
subtreeSearch()
{
	local TREE="$1"
	local KEY="$2"

	if [ "$TREE" = "" ] ; then
		return;
	fi

	local TREEKEY="$(treeKey "$TREE")"

	if [ "$KEY" \< "$TREEKEY" ] ; then
		subtreeSearch "$(treeLeft "$TREE")" "$KEY"
	elif [ "$KEY" \> "$TREEKEY" ] ; then
		subtreeSearch "$(treeRight "$TREE")" "$KEY"
	else
		echo $TREE
	fi
}

# /*!
#     @abstract
#         Searches a binary tree for a given key.
#     @discussion
#         This is an internal function.  Do not call it directly.  Use
#         {@link treeSearch} instead.
#     @result
#         Returns the node name of the matching node through <code>stdout</code>
#         if found or an empty string otherwise.
#     @param TREE
#         The subtree to search.
#     @param KEY
#         The key to search for.
#  */
subtreeSearchNumeric()
{
	local TREE="$1"
	local KEY="$2"

	if [ "$TREE" = "" ] ; then
		return;
	fi

	local TREEKEY="$(treeKey "$TREE")"

	if [ "$KEY" -lt "$TREEKEY" ] ; then
		subtreeSearchNumeric "$(treeLeft "$TREE")" "$KEY"
	elif [ "$KEY" -gt "$TREEKEY" ] ; then
		subtreeSearchNumeric "$(treeRight "$TREE")" "$KEY"
	else
		echo $TREE
	fi
}

# /*!
#     @abstract
#         Deletes a node in a tree.
#     @discussion
#         This algorithm does not support deleting arbitrry nodes.
#         This is an internal function that is used by {@link deleteTree}.
#     @param NODE
#         The node to delete.
#  */
deleteNode()
{
	local NODE="$1"

	local DATAFIELDS="$(eval echo "\$$NODE"_DATAFIELDS)"

	local FIELDNAME
	for FIELDNAME in $DATAFIELDS ; do
		# Skip the empty first field.
		if [ "$FIELDNAME" != "" ] ; then
			eval unset "$NODE"_DATAFIELD_$FIELDNAME
		fi
	done
	eval unset "$NODE"_LEFT
	eval unset "$NODE"_RIGHT
}


#############################################################################
#  End of functions.                                                        #
#############################################################################

if [ "$DISABLE_TESTS" != "true" ] ; then

	echoblue "Node tests:"

	FAIL=0

	printf "Create node: "
	newTreeNode "leftval" "rightval" "42" "TESTNODE"
	TESTNODE="$(getLastNodeName)"

	if [ "$TESTNODE" != "TESTNODE" ] ; then
		echored "Name wrong.  EXPECTED TESTNODE GOT $TESTNODE"
		FAIL=1
	else
		echogreen OK
	fi

	printf "Compare left node: "
	if [ "$(treeLeft "$TESTNODE")" != "leftval" ] ; then
		echored "Left node wrong." \
			"EXPECTED \"leftval\"" \
			"GOT \"$(treeLeft "$TESTNODE")\""
		FAIL=1
	else
		echogreen OK
	fi

	printf "Compare right node: "
	if [ "$(treeRight "$TESTNODE")" != "rightval" ] ; then
		echored "Right node wrong." \
			"EXPECTED \"rightval\"" \
			"GOT \"$(treeRight "$TESTNODE")\""
		FAIL=1
	else
		echogreen OK
	fi

	printf "Compare key: "
	if [ "$(treeKey "$TESTNODE")" != "42" ] ; then
		echored "Key wrong.  EXPECTED 42 GOT $(treeKey "$TESTNODE")"
		FAIL=1
	else
		echogreen OK
	fi

	printf "setTreeLeft: "
	setTreeLeft "$TESTNODE" "newleft"
	if [ "$(treeLeft "$TESTNODE")" != "newleft" ] ; then
		echored "Update of left node failed.  EXPECTED newleft GOT" \
			"$(treeLeft "$TESTNODE")"
		FAIL=1
	else
		echogreen OK
	fi

	printf "setTreeRight: "
	setTreeRight "$TESTNODE" "newright"
	if [ "$(treeRight "$TESTNODE")" != "newright" ] ; then
		echored "Update of right node failed.  EXPECTED newright GOT" \
			"$(treeRight "$TESTNODE")"
		FAIL=1
	else
		echogreen OK
	fi

	printf "setTreeField: "
	setTreeField "$TESTNODE" "four" "4"
	FOURSEARCHFIELD="$(treeField "$TESTNODE" "four")"
	if [ "$FOURSEARCHFIELD" != "4" ] ; then
		echored "Set field failed.  Field four: expected 4 GOT:" \
			"$FOURSEARCHFIELD"
		FAIL=1
	else
		echogreen OK
	fi

	echo
	echo
	echoblue "Numeric tree tests:"

NUMSORTEDLIST="1
2
3
4
5
7
12
13
15
39"

	printf "newTree: "
	newTree "MYNUMTREE"
	TREE="$(getLastNodeName)"

	if [ "$TREE" != "MYNUMTREE" ] ; then
		echored "Name wrong.  EXPECTED MYNUMTREE GOT $TREE"
		FAIL=1
	else
		echogreen OK
	fi

	insertKeyNumeric "$TREE" 3
	insertKeyNumeric "$TREE" 1
	insertKeyNumeric "$TREE" 7
	insertKeyNumeric "$TREE" 39
	insertKeyNumeric "$TREE" 12
	insertKeyNumeric "$TREE" 4
	FOURNODE="$(getLastNodeName)"
	insertKeyNumeric "$TREE" 15
	insertKeyNumeric "$TREE" 12
	insertKeyNumeric "$TREE" 5
	insertKeyNumeric "$TREE" 2
	insertKeyNumeric "$TREE" 4
	insertKeyNumeric "$TREE" 13

	# printTree "$TREE"
	printf "Multiple inserts: "
	LIST="$(iterateTree "$TREE" "treeKey")"
	if [ "$LIST" != "$NUMSORTEDLIST" ] ; then
		echored "List does not match." \
			"EXPECTED $(echo "$NUMSORTEDLIST" | tr '\n' ' ')" \
			"GOT $(echo "$LIST" | tr '\n' ' ')."
		FAIL=1
	else
		echogreen "OK"
	fi

	printf "Set and retrieve values: "
	setTreeField "$FOURNODE" "four" "4"
	FOURSEARCH="$(treeSearchNumeric "$TREE" "4")"
	FOURSEARCHFIELD="$(treeField "$FOURSEARCH" "four")"

	if [ "$FOURSEARCHFIELD" != "4" ] ; then
		echored "Tree search failed.  Node was $FOURSEARCH."
		printNode "$FOURSEARCH"
		FAIL=1
	else
		echogreen "OK"
	fi

	deleteTree "$TREE"

	echo
	echo
	echoblue "String tree tests:"

STRSORTEDLIST="1
12
13
15
2
3
39
4
5
7"

	printf "newTree: "
	newTree "MYSTRTREE"
	TREE="$(getLastNodeName)"

	if [ "$TREE" != "MYSTRTREE" ] ; then
		echored "Name wrong.  EXPECTED MYSTRTREE GOT $TREE"
		FAIL=1
	else
		echogreen OK
	fi

	insertKey "$TREE" 3
	insertKey "$TREE" 1
	insertKey "$TREE" 7
	insertKey "$TREE" 39
	insertKey "$TREE" 12
	insertKey "$TREE" 4
	FOURNODE="$(getLastNodeName)"

	insertKey "$TREE" 15
	insertKey "$TREE" 12
	insertKey "$TREE" 5
	insertKey "$TREE" 2
	insertKey "$TREE" 4
	insertKey "$TREE" 13

	# printTree "$TREE"
	printf "Multiple inserts: "
	LIST="$(iterateTree "$TREE" "treeKey")"
	if [ "$LIST" != "$STRSORTEDLIST" ] ; then
		echored "List does not match." \
			"EXPECTED $(echo "$STRSORTEDLIST" | tr '\n' ' ')" \
			"GOT $(echo "$LIST" | tr '\n' ' ')."
		FAIL=1
	else
		echogreen "OK"
	fi

	printf "Set and retrieve values: "
	setTreeField "$FOURNODE" "four" "4"
	FOURSEARCH="$(treeSearch "$TREE" "4")"
	FOURSEARCHFIELD="$(treeField "$FOURSEARCH" "four")"

	if [ "$FOURSEARCHFIELD" != "4" ] ; then
		echored "Tree search failed.  Node was $FOURSEARCH."
		printNode "$FOURSEARCH"
		FAIL=1
	else
		echogreen "OK"
	fi

	deleteTree "$TREE"

	echo
	if [ $FAIL = 0 ] ; then
		echogreen "All tests passed."
	else
		echored "Some tests failed."
	fi
	echo
fi

