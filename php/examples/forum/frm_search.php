<?php
require_once 'db.member.inc.php';
require_once 'frm_functions.inc.php';
require_once 'hd.inc.php';

$result = NULL;

if (isset($_GET['keywords'])) {
$sql = "SELECT *, MATCH (subject,body) " .
"AGAINST ('" . $_GET['keywords'] . "') AS score " .
"FROM posts " .
"WHERE MATCH (subject,body) " .
"AGAINST ('" . $_GET['keywords'] . "') " .
"ORDER BY score DESC";
$result = mysql_query($sql, $db) or die('Could not perform search; ' . mysql_error());
}

echo "<table class='forumtable' width='100%' cellspacing='0'>\n";
echo "<tr><th class='searchHeader'>Search Results</th></tr>\n";

if ($result and !mysql_num_rows($result)) {
echo "<tr class='row1'><td>No articles found that match the ";
echo "search term(s) '<strong>" . $_GET['keywords'] . "</strong>'";

if ($access_level > 1) echo "<p>SQL: $sql</p>";

echo "</td></tr>\n";
} else {
while ($row = mysql_fetch_array($result)) {
$rowclass = ($rowclass == "row1" ? "row2" : "row1");

echo "<tr class='$rowclass'>\n<td>\n";

$topicid = ($row['topic_id'] == 0 ? $row['id'] : $row['topic_id']);

echo "<p class='searchSubject'>\n<a href='frm_view_topic.php?t=" . $topicid . "#post" . $row['id'] . "'>" . $row['subject'] . "</a>\n";
echo "</p>\n";
echo "<p class='searchBody'>\n";
echo htmlspecialchars(trimBody($row['body']));

if ($access_lvl > 1) {
echo "<br /><br />relevance: " . $row['score'];
}

echo "\n</p>\n";
echo "</td>\n</tr>\n\n";
}
}

echo "</table>";

require_once 'ft.inc.php';
?>