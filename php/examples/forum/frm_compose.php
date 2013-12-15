<?php
require_once 'db.member.inc.php';
require_once 'frm_functions.inc.php';
require_once 'hd.inc.php';

$subject = '';
$topicid = $_GET['topicid'];
$forumid = $_GET['forumid'];
$reid = $_GET['reid'];
$body = '';
$post = '';
$authorid = $_SESSION['user_id'];
$edit_mode = FALSE;

if (isset($_GET['a']) and $_GET['a'] == 'edit' and isset($_GET['post']) and $_GET['post']) {
$edit_mode = TRUE;
}

require_once 'hd.inc.php';

if (!isset($_SESSION['user_id'])) {
echo "<div class='notice'>You must be logged in to post. Please <a href='login.php'>Log in</a> before posting a message.</div>";
} elseif ($edit_mode and $_SESSION['user_id'] != $authorid) {
echo "<div class='noauth'>" . "You are not authorized to edit this post. Please contact your administrator.</div>";
} else {
if ($edit_mode) {
$sql = "SELECT * FROM posts p, forum f WHERE p.id = " . $_GET['post'] . " AND p.forum_id = f.id";
$result = mysql_query($sql,$conn) or die('Could not retrieve post data; ' . mysql_error());
$row = mysql_fetch_array($result);

$subject = $row['subject'];
$topicid = $row['topic_id'];
$forumid = $row['forum_id'];
$body = $row['body'];
$post = $_GET['post'];
$authorid = $row['author_id'];
} else {
if ($topicid == "") {
$topicid = 0;
$topicname = "New Topic";
} else {
if ($reid != "") {
$sql = "SELECT subject FROM forum_posts WHERE id = " . $reid;
$result = mysql_query($sql, $conn) or die('Could not retrieve topic; ' . mysql_error());

if (mysql_num_rows($result) > 0) {
$row = mysql_fetch_array($result);
$re = preg_replace("/(re: )/i", "", $row['subject']);
}
}

$sql = "SELECT subject FROM posts WHERE id = ";
$sql .= $topicid . " AND topic_id = 0 AND forum_id = $forumid;";
$result = mysql_query($sql, $conn) or die('Could not retrieve topic; ' . mysql_error());

if (mysql_num_rows($result) > 0) {
$row = mysql_fetch_array($result);
$topicname = "Reply to <em>" . $row['subject'] . "</em>\n";
$subject = ($re == "" ? "" : "Re: " . $re);
} else {
$topicname = "Reply";
$topicid = 0;
}
}
}

if ($forumid == "" or $forumid == 0) $forumid = 1;
$sql = "SELECT forum_name FROM forum WHERE id = '";
$sql .= $forumid . "'";
$result = mysql_query($sql, $conn) or die('Could not retrieve forum name; ' . mysql_error());
$row = mysql_fetch_array($result);
$forumname = $row['forum_name'];
?>
<form id="forumpost" method="post" action="frm_transact_post.php">
<h3><?php echo $edit_mode ? "Edit Post" : "$forumname: $topicname"; ?>
</h3>
<p>
Subject:<br />
<input type="text" class="subject" name="subject" maxlength="255" value="<?php echo $subject; ?>" />
</p>
<p>
Body:<br />
<textarea class="body" name="body" rows="10" cols="60"><?php echo $body; ?></textarea>
</p>
<p>
<?php
if ($edit_mode) {
echo '<input type="submit" class="submit" name="action" ' . "value=\"Save Changes\" />\n";
} else {
echo '<input type="submit" class="submit" name="action" ' . "value=\"Submit New Post\" />\n";
}
?>
</p>
<?php
echo "<input type='hidden' name='post' value='$post' />\n";
echo "<input type='hidden' name='topic_id' value='$topicid' />\n";
echo "<input type='hidden' name='forum_id' value='$forumid' />\n";
echo "<input type='hidden' name='author_id' value='$authorid' />\n";
echo "</form>\n";
}

require_once 'ft.inc.php';
?>