<?php
if (isset($_POST['message_wall'])) {
	include('config.php');
	$message = mysql_real_escape_string(strip_tags($_POST['message_wall']));
	$sql = 'INSERT INTO WALL (message) VALUES ( "'.$message.'")';
	mysql_query($sql);
	echo $message;
} else { echo '0'; }
?>