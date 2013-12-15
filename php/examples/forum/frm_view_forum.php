<?php
require_once 'db.member.inc.php';
require_once 'frm_functions.inc.php';
require_once 'frm_http.inc.php';

if (!isset($_GET['f'])) redirect('frm_index.php');
require_once 'frm_header.inc.php';

$forumid = $_GET['f'];
$forum = getForum($forumid);

echo breadcrumb($forumid, "F");

if (isset($_GET['page'])) {
$page = $_GET['page'];
} else {
$page = 1;
}

$limit = $admin['pageLimit']['value'];

if ($limit == "") $limit = 25;
$start = (($page - 1) * $admin['pageLimit']['value']);

$sql = "CREATE TEMPORARY TABLE tmp ( ".
"topic_id INT(11) NOT NULL DEFAULT 0, ".
"postdate datetime NOT NULL default '0000-00-00 00:00:00');";
mysql_query($sql) or die(mysql_error() . "<br />" . $sql);

$sql = "LOCK TABLES forum_users READ, forum_posts READ;";
mysql_query($sql) or die(mysql_error() . "<br />" . $sql);

$sql = "INSERT INTO tmp SELECT topic_id, MAX(date_posted) ".
"FROM forum_posts ".
"WHERE forum_id = $forumid ".
"AND topic_id > 0 ".
"GROUP BY topic_id;";
mysql_query($sql) or die(mysql_error() . "<br />" . $sql);

$sql = "UNLOCK TABLES";
mysql_query($sql) or die(mysql_error() . "<br />" . $sql);

// die('stop');
$sql = "SELECT SQL_CALC_FOUND_ROWS " .
"t.id as topic_id, t.subject as t_subject, " .
"u.name as t_author, count(p.id) as numreplies, " .
"t.date_posted as t_posted, tmp.postdate as re_posted " .
"FROM forum_users u " .
"JOIN forum_posts t " .
"ON t.author_id = u.id " .
"LEFT JOIN tmp " .
"ON t.id = tmp.topic_id " .
"LEFT JOIN forum_posts p " .
"ON p.topic_id = t.id " .
"WHERE t.forum_id = $forumid " .
"AND t.topic_id = 0 " .
"GROUP BY t.id " .
"ORDER BY re_posted DESC " .
"LIMIT $start, $limit";
$result = mysql_query($sql) or die(mysql_error() . "<br />" . $sql);
$numrows = mysql_num_rows($result);

if ($numrows == 0) {
$msg = "There are currently no posts.Would you like to be the first person to create a thread?";
$title = "Welcome to " . $forum['name'];
$dest = "compose.php?forumid=" . $forumid;
$sev = "Info";
$message = msgBox($msg, $title, $dest, $sev);

echo $message;
} else {
if (isset($_SESSION['user_id'])) {
echo topicReplyBar(0, $_GET['f'], "right");
}

echo "<table class='forumtable' cellspacing='0' cellpadding='2'><tr>";
echo "<th class='thread'>Thread</th>";
echo "<th class='author'>Author</th>";
echo "<th class='replies'>Replies</th>";
echo "<th class='lastpost'>Last Post</th>";
echo "</tr>";

while ($row = mysql_fetch_array($result)) {
$rowclass = ($rowclass == "row1" ? "row2" : "row1");

if ($row['re_posted'] == "") {
$lastpost = $row['t_posted'];
} else {
$lastpost = $row['re_posted'];
}

if ((isset($_SESSION['user_id'])) and ($_SESSION['last_login'] < $lastpost)) {
$newpost = true;
} else {
$newpost = false;
}

echo "<tr class='$rowclass'>";
echo "<td class='thread'>" . ($newpost ? NEWPOST . "&nbsp;" : "");
echo "<a href='frm_view_topic.php?t=" . $row['topic_id'] . "'>" . $row['t_subject'] . "</a></td>";
echo "<td class='author'>" . $row['t_author'] . "</td>";
echo "<td class='replies'>" . $row['numreplies'] . "</td>";
echo "<td class='lastpost'>" . $lastpost . "</td>";
echo "</tr>\n";
}

echo "</table>";
echo paginate($limit);
echo "<p>" . NEWPOST . " = New Post(s)</p>";
}
$sql = "DROP TABLE tmp;";
mysql_query($sql) or die(mysql_error() . "<br />" . $sql);

require_once 'frm_footer.inc.php';
?>