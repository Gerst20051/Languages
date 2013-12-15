<?php
include ("validip.inc.php");

$t_stamp = time();
$timeout = ($t_stamp - 600);
$users_id = $_SESSION['user_id'];
$username = $_SESSION['username'];

if (isset($_SERVER['QUERY_STRING']) && $_SERVER['QUERY_STRING'] != null) $phpself = $_SERVER['PHP_SELF'] . "?" . $_SERVER['QUERY_STRING'];
else $phpself = $_SERVER['PHP_SELF'];

if (isset($_SESSION['logged']) && ($_SESSION['logged'] == 1)) mysql_query("INSERT INTO users_online VALUES ('$t_stamp','$ip','$users_id','$username','$phpself')", $db);
else mysql_query("INSERT INTO guests_online VALUES ('$t_stamp','$ip','guest','$phpself')", $db);

mysql_query("DELETE FROM users_online WHERE timestamp < $timeout", $db);
mysql_query("DELETE FROM guests_online WHERE timestamp < $timeout", $db);
?>