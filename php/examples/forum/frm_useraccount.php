<?php
require_once 'frm_header.inc.php';

$userid = $username = $useremail = $password = $accesslvl = '';
$mode = "Create";
if (isset($_SESSION['user_id'])) {
$userid = $_SESSION['user_id'];
$mode = "Edit";
if (isset($_GET['user'])) {
if (($_SESSION['user_id'] == $_GET['user'])
|| ($_SESSION['access_lvl'] > 2)) {
$userid = $_GET['user'];
$mode = "Modify";
}
}
$sql = "SELECT * FROM forum_users WHERE id=$userid";
$result = mysql_query($sql)
or die('Could not look up user data; ' . mysql_error());

$row = mysql_fetch_array($result);
$username = $row['name'];
$useremail = $row['email'];
$accesslvl = $row['access_lvl'];
$signature = $row['signature'];
}

echo "<h3>$mode Account</h3>\n";
echo "<form method=\"post\" action=\"frm_transact_user.php\">\n";
?>

<p>
Full name:<br />
<input type="text" class=".phpinput" name="name" maxlength="100" value="<?php echo htmlspecialchars($username); ?>" />
</p>
<?php
if ($mode == "Edit") {
?>
<p>
Email Address:<br />
<input type="text" class=".phpinput" name="email" maxlength="255" value="<?php echo htmlspecialchars($useremail); ?>" />
</p>
<?php
}
if ($mode == "Modify") {
echo "<div><fieldset>\n";
echo "<legend>Access Level</legend>\n";

$sql = "SELECT * FROM forum_access_levels ORDER BY access_lvl DESC";
$result = mysql_query($sql,$conn)
or die('Could not list access levels; ' . mysql_error());

while ($row = mysql_fetch_array($result)) {
echo '<input type="radio" class="radio" id="acl_' .
$row['access_lvl'] . '" name="accesslvl" value="' .
$row['access_lvl'] . '" ';

if ($row['access_lvl'] == $accesslvl) {
echo 'checked="checked" ';
}
echo '/>' . $row['access_name'] . "<br />\n";
}
echo "</fieldset></div>";
}
if ($mode != "Modify") echo "<div id='passwords'>";
if ($mode == "Edit") {
if ($_GET['error'] == "nopassedit") {
echo "<span class='error'>Could not modify passwords.";
echo " Please try again.</span><br />";
}
?>
<p>
Old Password:<br />
<input type="password" id="oldpasswd" name="oldpasswd" maxlength="50" />
</p>
<?php
}
if ($mode != "Modify") {
?>
<p>
New Password:<br />
<input type="password" id="passwd" name="passwd" maxlength="50" />
</p>
<p>
Password Verification:<br />
<input type="password" id="passwd2" name="passwd2" maxlength="50" />
</p>
<?php }
if ($mode != "Modify") echo "</div>";
if ($mode != "Create") {
?>
<p>
Signature:<br />
<textarea name="signature" id="signature" cols=60 rows=5><?php echo $signature;?></textarea>
</p>
<?php } ?>
<p>
<input type="submit" class="submit" name="action" value="<?php echo $mode; ?> Account" />
</p>
<?php if ($mode == "Edit") {?>
<input type="hidden" name="accesslvl" value="<?php echo $accesslvl; ?>" />
<?php } ?>
<input type="hidden" name="userid" value="<?php echo $userid; ?>" />
</form>
<?php require_once 'frm_footer.inc.php'; ?>