<?php
session_start();

require_once 'db.member.inc.php';
require_once 'frm_http.inc.php';

if (isset($_REQUEST['action'])) {
switch ($_REQUEST['action']) {
case 'Add Forum':
if (isset($_POST['forumname']) and $_POST['forumname'] != "" and isset($_POST['forumdesc']) and $_POST['forumdesc'] != "") {
$sql = "INSERT IGNORE INTO forum_forum " .
"VALUES (NULL, '" .
htmlspecialchars($_POST['forumname'], ENT_QUOTES) .
"', '" .
htmlspecialchars($_POST['forumdesc'], ENT_QUOTES) .
"', " . $_POST['forummod'][0] . ")";
mysql_query($sql) or die(mysql_error());
}

redirect('frm_admin.php?option=forums');
break;

case 'Edit Forum':
if (isset($_POST['forumname']) and $_POST['forumname'] != "" and isset($_POST['forumdesc']) and $_POST['forumdesc'] != "") {
$sql = "UPDATE forum_forum " .
"SET forum_name = '" . $_POST['forumname'] .
"', forum_desc = '" . $_POST['forumdesc'] .
"', forum_moderator = " . $_POST['forummod'][0] .
" WHERE id = " . $_POST['forum_id'];
mysql_query($sql) or die(mysql_error());
}

redirect('frm_admin.php?option=forums');
break;

case 'Modify User':
redirect("frm_useraccount.php?user=" . $_POST['userlist'][0]);
break;

case 'Update':
foreach($_POST as $key => $value) {
if ($key != 'action') {
$sql = "UPDATE forum_admin SET value='$value' WHERE constant = '$key'";
mysql_query($sql) or die(mysql_error());
}
}

redirect('frm_admin.php');
break;

case "deleteForum":
$sql = "DELETE FROM forum_forum WHERE id=" . $_GET['f'];
mysql_query($sql) or die(mysql_error());
$sql = "DELETE FROM forum_posts WHERE forum_id=" . $_GET['f'];
mysql_query($sql) or die(mysql_error());

redirect('admin.php?option=forums');
break;

case "Add New":
$sql = "INSERT INTO forum_bbcode " .
"VALUES (NULL, '" .
htmlentities($_POST['bbcode-tnew'],ENT_QUOTES) . "', '" .
htmlentities($_POST['bbcode-rnew'],ENT_QUOTES) . "');";
mysql_query($sql) or die(mysql_error()."<br>" . $sql);

redirect('frm_admin.php?option=bbcode');
break;

case "deleteBBCode":
if (isset($_GET['b'])) {
$bbcodeid = $_GET['b'];
$sql = "DELETE FROM forum_bbcode WHERE id=" . $bbcodeid;
mysql_query($sql) or die(mysql_error());
}

redirect('frm_admin.php?option=bbcode');
break;

default:
redirect('frm_index.php');
}
} else {
redirect('frm_index.php');
}
?>