<?php
define('MYSQL_HOST','localhost');
define('MYSQL_USER','root');
define('MYSQL_PASSWORD','comwiz05');

$db = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD) or die ("<h2>Unable to connect to database Members. Check your connection parameters.</h2>");
mysql_select_db("members", $db) or die(mysql_error($db));

$message_db = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD, true) or die ("<h2>Unable to connect to database Messages. Check your connection parameters.</h2>");
mysql_select_db("messages", $message_db) or die(mysql_error($message_db));

$comment_db = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD, true) or die ("<h2>Unable to connect to database Comments. Check your connection parameters.</h2>");
mysql_select_db("comments", $comment_db) or die(mysql_error($comment_db));

$group_db = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD, true) or die ("<h2>Unable to connect to database Groups. Check your connection parameters.</h2>");
mysql_select_db("groups", $group_db) or die(mysql_error($group_db));
?>