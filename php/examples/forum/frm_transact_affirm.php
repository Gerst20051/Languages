<?php
require_once 'db.member.inc.php';
require_once 'frm_functions.inc.php';
require_once 'frm_http.inc.php';
require_once 'hd.inc.php';
?>
<script type='text/javascript'>
<!--
function deletePost(id,redir) {
if (id > 0) {
window.location = "frm_transact_post.php?action=delete&post=" + id + "&r=" + redir;
} else {
history.back();
}
}

function deleteForum(id) {
if (id > 0) {
window.location = "frm_transact_admin.php?action=deleteForum&f=" + id;
} else {
history.back();
}
}
//-->
</script>
<?php
switch (strtoupper($_REQUEST['action'])) {
case "DELETEPOST":
$sql = "SELECT * FROM posts WHERE id=" . $_REQUEST['id'];
$result = mysql_query($sql);
$row = mysql_fetch_array($result);

if ($row['topic_id'] > 0) {
$msg = "Are you sure you wish to delete the post<br>".
"<em>" . $row['subject'] . "</em>?";
$redir = htmlspecialchars("frm_view_topic.php?t=" . $row['topic_id']);
} else {
$msg = "If you delete this post, all replies will be deleted as well. Are you sure you wish to delete the entire thread<br><em>" . $row['subject'] . "</em>?";
$redir = htmlspecialchars("frm_view_forum.php?f=" . $row['forum_id']);
}

echo "<div id='requestConfirmWarn'>";
echo "<h2>DELETE POST?</h2>\n";
echo "<p>" . $msg . "</p>";
echo "<p><input class='confirm' type='button' value='Delete' onclick='deletePost(" . $row['id'] . ",\"" . $redir . "\"); ' />";
echo "<input class='confirm' type='button' value='Cancel' onclick='history.back(); ' /></p>";
echo "</div>";
break;

case "DELETEFORUM":
$sql = "SELECT * FROM forum WHERE id=" . $_REQUEST['f'];
$result = mysql_query($sql);
$row = mysql_fetch_array($result);
$msg = "If you delete this forum, all topics and replies will be deleted as well. Are you sure you wish to delete the entire forum<br><em>" . $row['forum_name'] . "</em>?";

echo "<div id='requestConfirmWarn'>";
echo "<h2>DELETE FORUM?</h2>\n";
echo "<p>" . $msg . "</p>";
echo "<p><input class='confirm' type='button' value='Delete' onclick='deleteForum(" . $_REQUEST['f'] . "); ' />";
echo "<input class='confirm' type='button' value='Cancel' onclick='history.back(); ' /></p>";
echo "</div>";

default:
}

require_once 'ft.inc.php';
?>