<?php
define('HOST','localhost');
define('USER','');
define('PASS','');
define('DB','members');
$con=mysql_connect(HOST,USER,PASS) or die(mysql_error());
mysql_query("CREATE DATABASE IF NOT EXISTS ".DB,$con);
mysql_select_db(DB) or die(mysql_error());
if (!$con) die('Could not connect: '.mysql_error());
mysql_select_db(DB,$con);
$sql="CREATE TABLE users
(
first_name VARCHAR(255) NOT NULL,
user_email VARCHAR(255) NOT NULL
)";
mysql_query($sql,$con);
mysql_close($con)
?>