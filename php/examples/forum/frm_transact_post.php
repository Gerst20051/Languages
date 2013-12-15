<?php
session_start();

require_once 'db.member.inc.php';
require_once 'frm_http.inc.php';

if (isset($_REQUEST['action'])) {
switch (strtoupper($_REQUEST['action'])) {
case 'SUBMIT NEW POST':
if (isset($_POST['subject']) and isset($_POST['body']) and isset($_SESSION['user_id'])) {
$sql = "INSERT INTO forum_posts VALUES (" .
"NULL, " . $_POST['topic_id'] .
", " . $_POST['forum_id'] .
", " . $_SESSION['user_id'] .
", 0" .
", '" . date("Y-m-d H:i:s", time()) .
"', 0" .
", '" . $_POST['subject'] .
"', '" . $_POST['body'] . "')";
mysql_query($sql, $conn) or die('Could not post: ' . mysql_error() . "<br>$sql");

$postid = mysql_insert_id();
$sql = "INSERT IGNORE INTO forum_postcount VALUES (" . $_SESSION['user_id'] . ", 0);";
mysql_query($sql, $conn) or die(mysql_error());

$sql = "UPDATE forum_postcount SET count = count + 1 WHERE user_id = " . $_SESSION['user_id'];
mysql_query($sql, $conn) or die(mysql_error());
}

$topicid = ($_POST['topic_id'] == 0 ? $postid : $_POST['topic_id']);

redirect('frm_view_topic.php?t=' . $topicid . '#post' . $postid);
break;

case 'NEW TOPIC':
redirect('frm_compose.php?f=' . $_POST['forum_id']);
break;

case 'EDIT':
redirect('frm_compose.php?a=edit&post=' . $_POST['topic_id']);
break;

case 'SAVE CHANGES':
if (isset($_POST['subject']) and isset($_POST['body'])) {
$sql = "UPDATE forum_posts " .
"SET subject='" . $_POST['subject'] .
"', update_id=" . $_SESSION['user_id'] .
", body='" . $_POST['body'] . "', date_updated='" .
date("Y-m-d H:i:s", time()) . "' " .
"WHERE id=" . $_POST['post'];

if (isset($_POST['author_id'])) {
$sql .= " AND author_id=" . $_POST['author_id'];
}

mysql_query($sql, $conn) or die('Could not update post; ' . mysql_error());
}

$redirID = ($_POST['topic_id'] == 0 ? $_POST['post']:
$_POST['topic_id']);

redirect('frm_view_topic.php?t=' . $redirID);
break;

case 'DELETE':
if ($_REQUEST['post']) {
$sql = "DELETE FROM forum_posts WHERE id = " . $_REQUEST['post'];
mysql_query($sql,$conn) or die('Could not delete post; ' . mysql_error());
}

redirect($_REQUEST['r']);
break;
}
} else {
redirect('frm_index.php');
}
?>