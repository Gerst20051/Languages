<?php
include ("hd.inc.php");
?>
<script type="text/javascript">
<!--
function delForum(id) {
window.location = "frm_transact-affirm.php?action=deleteForum&f=" + id;
}
//-->
</script>
<?php
$sql = "SELECT access_level, access_name FROM forum_access_levels ORDER BY access_lvl DESC";
$result = mysql_query($sql) or die(mysql_error());

while ($row = mysql_fetch_array($result)) {
$a_users[$row['access_lvl']] = $row['access_name'];
}

$menuoption = "boardadmin"; // default

if (isset($_GET['option'])) $menuoption = $_GET['option'];

$menuItems = array(
"boardadmin" => "Board Admin",
"forums" => "Forums"
);

echo "<p class='menu'>|";

foreach ($menuItems as $key => $value) {
if ($menuoption != $key) {
echo "<a href='" . $_SESSION['PHP_SELF'] . "?option=$key'>";
}

echo " $value ";

if ($menuoption != $key) echo "</a>";
echo "|";
}

echo "</p>";

switch ($menuoption) {
case 'boardadmin':
?>
<h3>Board Administration</h3>
<form id='adminForm' method='post' action='frm_transact-admin.php'>
<table cellspacing='0' class='forumtable'>
<tr>
<th>Title</th><th>Value</th><th>Parameter</th>
</tr>
<?php
foreach ($admin as $k => $v) {
echo "<tr><td>". $v['title'] . "</td><td><input type='text' name='". $k . "' " . "value='" . $v['value'] . "' size='60'></td><td>$k</td></tr>\n";
}
?>
</table>
<p class='buttonBar'>
<input class='submit' type='submit' name="action" id="Update" value='Update'>
</p>
</form>
<?php
break;

case 'forums':
?>
<h2>Forum Administration</h2>
<table class='forumtable' cellspacing='0'>
<tr><th class="forum">Forum</th><th>&nbsp;</th><th>&nbsp;</th></tr>
<?php
$sql = "SELECT * FROM forum_forum";
$result = mysql_query($sql) or die(mysql_error());
while ($row = mysql_fetch_array($result)) {
echo "<tr><td><span class='forumname'>" . $row['forum_name'] . "</span><br><span class='forumdesc'>" . $row['forum_desc'] . "</span></td><td><a href='editforum.php?forum=" . $row['id'] . "'>Edit</a></td><td><a href='#' onclick='delForum(" . $row['id'] . ");'>Delete</a></td></tr>";
}
?>
</table>
<p class='buttonBar'>
<a href='editforum.php' class='buttonlink'>New Forum</a>
</p>
<?php
break;

default:
}
?>
</script>
<?php
include ("ft.inc.php");
?>