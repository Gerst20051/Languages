<?php
function trimBody($theText, $lmt = 100, $s_chr = "@@@", $s_cnt = 1) {
$pos = 0;
$trimmed = FALSE;

for ($i = 1; $i <= $s_cnt; $i++) {
if ($tmp = strpos($theText, $s_chr, $pos)) {
$pos = $tmp;
$trimmed = TRUE;
} else {
$pos = strlen($theText);
$trimmed = FALSE;

break;
}
}

$theText = substr($theText, 0, $pos);

if (strlen($theText) > $lmt) {
$theText = substr($theText, 0, $lmt);
$theText = substr($theText, 0, strrpos($theText,' '));
$trimmed = TRUE;
}

if ($trimmed) $theText .= '...';
return $theText;
}

function msgBox($m, $t, $d = "frm_index.php", $s = "Info") {
$theMsg = "<div id='requestConfirm" . $s . "'>";
$theMsg .= "<h2>" . $t . "</h2>\n";
$theMsg .= "<p>" . $m . "</p>";
$theMsg .= "<p><a href='" . $d . "' ";
$theMsg .= "class='buttonlink'>";
$theMsg .= "Yes</a>";
$theMsg .= "<a href='frm_index.php' class='buttonlink'>";
$theMsg .= "No</a></p>";
$theMsg .= "</div>";

return $theMsg;
}

function getForum($id) {
$sql = "SELECT forum_name as name, forum_desc as description, forum_moderator as mod FROM forum WHERE id = " . $id;
$result = mysql_query($sql) or die(mysql_error() . "<br />" . $sql);
$row = mysql_fetch_array($result);

return $row;
}

function getForumID($topicid) {
$sql = "SELECT forum_id FROM posts WHERE id = $topicid";
$result = mysql_query($sql) or die(mysql_error() . "<br />" . $sql);
$row = mysql_fetch_array($result);

return $row['forum_id'];
}

function breadcrumb($id, $getfrom = "F") {
$sep = "<span class='bcsep'>";
$sep .= " &middot; ";
$sep .= "</span>";

if ($getfrom == "P") {
$sql = "SELECT forum_id, subject FROM posts WHERE id = " . $id;
$result = mysql_query($sql) or die(mysql_error() . "<br />" . $sql);
$row = mysql_fetch_array($result);
$id = $row['forum_id'];
$topic = $row['subject'];
}

$row = getForum($id);
$bc = "<a href='frm_index.php'>Home</a>$sep";

switch ($getfrom) {
case "P":
$bc .= "<a href='frm_view_forum.php?f=$id'>" . $row['name'] . "</a>$sep" . $topic;
break;

case "F":
$bc .= $row['name'];
break;

default:
}

return "<h4 class='breadcrumb'>" . $bc . "</h4>";
}

function showTopic($topicid, $showfull = TRUE) {
global $conn;
global $userid;
global $limit;

echo breadcrumb($topicid, "P");

if (isset($_GET['page'])) {
$page = $_GET['page'];
} else {
$page = 1;
}

if ($limit == "") $limit = 25;
$start = (($page - 1) * $limit);

if (isset($_SESSION['user_id'])) {
echo topicReplyBar($topicid, getForumID($topicid), "right");
}

$sql = "SELECT SQL_CALC_FOUND_ROWS " .
"p.id, p.subject, p.body, p.date_posted, " .
"p.date_updated, u.username as author, u.id as author_id, " .
"c.post_count as postcount, " .
"p.forum_id as forum_id, f.forum_moderator as mod, " .
"p.update_id, u2.username as updated_by " .
"FROM forum f " .
"JOIN posts p " .
"ON f.id = p.forum_id " .
"JOIN login u " .
"ON u.id = p.author_id " .
"LEFT JOIN login u2 " .
"ON u2.id = p.update_id " .
"LEFT JOIN info c " .
"ON u.id = c.user_id " .
"WHERE (p.topic_id = $topicid OR p.id = $topicid) " .
"ORDER BY p.topic_id, p.date_posted " .
"LIMIT $start, $limit";
$result = mysql_query($sql, $db) or die(mysql_error() . "<br />" . $sql);
$pagelinks = paginate($limit);

if (mysql_num_rows($result) == 0) {
$msg = "There are currently no posts.Would you like to be the first person to create a thread?";
$title = "No Posts...";
$dest = "./frm_compose.php?forumid=" . $forumid;
$sev = "Info";
$message = msgBox($msg, $title, $dest, $sev);

echo $message;
} else {
echo "<table class='forumtable' cellspacing='0' ";
echo "cellpadding='2'><tr>";
echo "<th class='author'>Author</th>";
echo "<th class='post'>Post</th>";
echo "</tr>";

while ($row = mysql_fetch_array($result)) {
$lastupdate = "";
$editlink = "";
$dellink = "";
$replylink = "&nbsp;";
$pcount = "";
$pdate = "";
$sig = "";

if ($showfull) {
$body = $row['body'];

if (isset($_SESSION['user_id'])) {
$replylink = "<a href='frm_compose.php?forumid=" . $row['forum_id'] . "&topicid=$topicid&reid=" . $row['id'] . "' class='buttonlink'>REPLY</a>&nbsp;";
} else {
$replylink = "";
}

if ($row['update_id'] > 0) {
$lastupdate = "<p class='smallNote'>Last updated: " . $row['date_updated'] . " by " . $row['updated_by'] . "</p>";
}

if (($userid == $row['author_id']) or ($userid == $row['mod']) or ($_SESSION['access_lvl'] > 1)) {
$editlink = "<a href='frm_compose.php?a=edit&post=" . $row['id'] . "' class='buttonlink'>EDIT</a>&nbsp;";
$dellink = "<a href='frm_transact_affirm.php?action=deletepost&id=" . $row['id'] . "' class='buttonlink'>DELETE</a>&nbsp;";
}

$pcount = "<br /><span class='textsmall'>Posts: " . ($row['postcount'] == "" ? "0" : $row['postcount']) . "</span>";
$pdate = $row['date_posted'];
$sig = ($row['sig'] != "" ? "<p class='sig'>" . nl2br($row['sig']) : "") . "</p>";
} else {
$body = trimBody($body);
}

$rowclass = ($rowclass == "row1" ? "row2" : "row1");

echo "<tr class='$rowclass'>";
echo "<td class='author'>" . $row['author'];
echo $pcount;
echo "</td><td class='post'><p>";

if (isset($_SESSION['user_id']) and ($_SESSION['last_login'] < $row['date_posted'])) {
echo NEWPOST . " ";
}

if (isset($_GET['page'])) $pagelink = "&page=" . $_GET['page'];
echo "<a name='post" . $row['id'] . "' href='frm_view_topic.php?t=" . $topicid . $pagelink . "#post" . $row['id'] . "'>" . POSTLINK . "</a>";

if (isset($row['subject'])) {
echo " <strong>" . $row['subject'] . "</strong>";
}

echo "</p><p>" . nl2br(htmlspecialchars($body)) . "</p>";
echo $sig;
echo $lastupdate;
echo "</td></tr>";
echo "<tr class='$rowclass'><td class='authorfooter'>";
echo $pdate . "</td><td class='threadfooter'>";
echo $replylink;
echo $editlink;
echo $dellink;
echo "</td></tr>\n";
}

echo "</table>";
echo $pagelinks;
echo "<p>" . NEWPOST . " = New Post&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
echo POSTLINK . " = Post link (use to bookmark)</p>";
}
}

function isParent($page) {
$currentpage = $_SERVER['PHP_SELF'];

if (strpos($currentpage, $page) === false) {
return FALSE;
} else {
return TRUE;
}
}

function topicReplyBar($topicid, $forumid, $pos = "right") {
$html = "<p class='buttonBar" . $pos . "'>";

if ($topicid > 0) {
$html .= "<a href='frm_compose.php?forumid=$forumid&topicid=$topicid&reid=$topicid' class='buttonlink'>Reply to Thread</a>";
}

if ($forumid > 0) {
$html .= "<a href='frm_compose.php?forumid=$forumid' class='buttonlink'>New Thread</a>";
}

$html .= "</p>";

return $html;
}

function userOptionList($level) {
$sql = "SELECT id, name, access_lvl FROM forum_users WHERE access_lvl = " . $level . " ORDER BY name";
$result = mysql_query($sql) or die(mysql_error());

while ($row = mysql_fetch_array($result)) {
echo "<option value='" . $row['id'] . "'>" . htmlspecialchars($row['name']) . "</options>";
}
}

function paginate($limit = 10) {
global $admin;

$sql = "SELECT FOUND_ROWS();";
$result = mysql_query($sql) or die(mysql_error());
$row = mysql_fetch_array($result);
$numrows = $row[0];
$pagelinks = "<div class=pagelinks>";

if ($numrows > $limit) {
if (isset($_GET['page'])) {
$page = $_GET['page'];
} else {
$page = 1;
}

$currpage = $_SERVER['PHP_SELF'] . "?" . $_SERVER['QUERY_STRING'];
$currpage = str_replace("&page=" . $page, "", $currpage);

if ($page == 1) {
$pagelinks .= "<span class='pageprevdead'>&lt; PREV</span>";
} else {
$pageprev = ($page - 1);
$pagelinks .= "<a class='pageprevlink' href='" . $currpage . "&page=" . $pageprev . "'>&lt; PREV</a>";
}

$numofpages = ceil($numrows / $limit);
$range = 7;

if ($range == "" or $range == 0) $range = 7;

$lrange = max(1, ($page - (($range-1) / 2)));
$rrange = min($numofpages, ($page + (($range - 1) / 2)));

if (($rrange - $lrange) < ($range - 1)) {
if ($lrange == 1) {
$rrange = min($lrange + ($range - 1), $numofpages);
} else {
$lrange = max($rrange - ($range - 1), 0);
}
}

if ($lrange > 1) {
$pagelinks .= "..";
} else {
$pagelinks .= "&nbsp;&nbsp;";
}

for ($i = 1; $i <= $numofpages; $i++) {
if ($i == $page) {
$pagelinks .= "<span class='pagenumdead'>$i</span>";
} else {
if ($lrange <= $i and $i <= $rrange) {
$pagelinks .= "<a class='pagenumlink' href='" . $currpage . "&page=" . $i . "'>" . $i . "</a>";
}
}
}

if ($rrange < $numofpages) {
$pagelinks .= "..";
} else {
$pagelinks .= "&nbsp;&nbsp;";
}

if (($numrows - ($limit * $page)) > 0) {
$pagenext = ($page + 1);
$pagelinks .= "<a class='pagenextlink' href='" . $currpage . "&page=" . $pagenext . "'>NEXT &gt;</a>";
} else {
$pagelinks .= "<span class='pagenextdead'>NEXT &gt;</span>";
}
} else {
$pagelinks .= "<span class='pageprevdead'>&lt; PREV</span>&nbsp;&nbsp;";
$pagelinks .= "<span class='pagenextdead'>NEXT &gt;</span>&nbsp;&nbsp;";
}
$pagelinks .= "</div>";

return $pagelinks;
}
?>