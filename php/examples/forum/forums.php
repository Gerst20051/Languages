<?php
require_once 'db.member.inc.php';
require_once 'frm_functions.inc.php';
require_once 'hd.inc.php';

$sql = <<< EOS
SELECT f.id as id, f.forum_name as forum, f.forum_desc as description, count(forum_id) as threads, u.username as mod
FROM forum f
LEFT JOIN posts p
ON f.id = p.forum_id
AND p.topic_id = 0
LEFT JOIN login u
ON f.forum_moderator = u.id
GROUP BY f.id
EOS;
$result = mysql_query($sql) or die(mysql_error());

if (mysql_num_rows($result) == 0) {
echo "<br />\n";
echo "There are currently no forums to view.\n";
} else {
echo "<table class='forumtable' cellspacing='0' ";
echo "cellspacing='0'><tr>";
echo "<th class='forum'>Forum</th>";
echo "<th class='threadcount'>Threads</th>";
echo "<th class='moderator'>Moderator</th>";
echo "</tr>";

while ($row = mysql_fetch_array($result)) {
$rowclass = ($rowclass == "row1" ? "row2" : "row1");

echo "<tr class='$rowclass'>";
echo "<td class='firstcolumn'><a href='viewforum.php?f=" . $row['id'] . "'>";
echo $row['forum'] . "</a><br />";
echo "<span class='forumdesc'>" . $row['description'];
echo "</span></td>";
echo "<td class='center'>" . $row['threads'] . "</td>";
echo "<td class='center'>" . $row['mod'] . "</td>";
echo "</tr>\n";
}
echo "</table>";
}

require_once 'ft.inc.php';
?>