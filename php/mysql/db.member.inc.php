<?php
define('MYSQL_HOST','localhost');
define('MYSQL_USER','root');
define('MYSQL_PASSWORD','comwiz05');
define('MYSQL_DB','hns');

$db = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD) or die ("<h2>" . $TEXT['header-databaseerrors_member'] . "</h2>");
mysql_select_db(MYSQL_DB, $db) or die(mysql_error($db));

$message_db = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD, true) or die ("<h2>" . $TEXT['header-databaseerrors_messages'] . "</h2>");
mysql_select_db("messages", $message_db) or die(mysql_error($message_db));

$comment_db = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD, true) or die ("<h2>" . $TEXT['header-databaseerrors_comments'] . "</h2>");
mysql_select_db("comments", $comment_db) or die(mysql_error($comment_db));
?>