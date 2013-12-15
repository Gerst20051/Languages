<?php
if (isset($_GET['forum'])) {
$action = "Edit";
} else {
$action = "Add";
}

$pageTitle = "$action Forum";

require_once 'hd.inc.php';

$forum = 0;
$fname = '';
$fdesc = '';
$fmod = '';
$userid = 0;

if (isset($_GET['forum'])) {
$forum = $_GET['forum'];
$sql = "SELECT forum_name, forum_desc, u.name, u.id FROM forum f LEFT JOIN login u ON f.forum_moderator = u.id WHERE f.id = $forum";
$result = mysql_query($sql) or die(mysql_error());

if ($row = mysql_fetch_array($result)) {
$fname = $row['forum_name'];
$fdesc = $row['forum_desc'];
$fmod = $row['name'];
$userid = $row['id'];
}
}

echo "<h2>$action forum</h2>";
?>
<form name="forumedit" action="transact_admin.php" method="post">
<table class="forumtable" cellspacing='0'>
<tr><th colspan='2'>General Forum Settings</th></tr>
<tr>
<td>Forum Name</td>
<td>
<input type='text' name='forumname' value="<?php echo $fname; ?>" />
</td>
</tr>
<tr>
<td>Forum Description</td>
<td>
<input type='text' name='forumdesc' size='75' value="<?php echo $fdesc; ?>" />
</td>
</tr>
<tr>
<td>Forum Moderator</td>
<td>
<select id="moderator" name="forummod[]">
<option value='0'>unmoderated</option>
<?php
$sql = "SELECT * FROM login WHERE access_level > 1";
$result = mysql_query($sql) or die(mysql_error());

while ($row = mysql_fetch_array($result)) {
echo "<option value='" . $row['id'] . "'";

if ($userid == $row['id']) echo " selected='selected'";

echo ">" . $row['name'] . "</option>";
}
?>
</select>
</td>
</tr>
<tr>
<td colspan='2'>
<input class="submit" type="submit" name="action" value="<?php echo $action; ?> Forum" />
</td>
</table>
<input type="hidden" name="forum_id" value="<?php echo $forum; ?>" />
</form>
<?php require_once 'ft.inc.php'; ?>