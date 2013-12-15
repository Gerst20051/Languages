<?php
session_start();

include ("db.member.inc.php");

$query = 'SELECT user_id FROM info';
$result = mysql_query($query, $db) or die(mysql_error($db));

while ($row = mysql_fetch_array($result)) {
$query2 = 'SELECT user_id, fullname FROM info WHERE user_id = ' . $row['user_id'];
$result2 = mysql_query($query2, $db) or die(mysql_error($db));
$row2 = mysql_fetch_assoc($result2);
extract($row2);
mysql_free_result($result2);

list($firstname, $lastname) = split(' ', $row2['fullname']);
$query3 = 'UPDATE info SET
firstname = "' . mysql_real_escape_string($firstname, $db) . '",
lastname = "' . mysql_real_escape_string($lastname, $db) . '"
WHERE
user_id = ' . $row['user_id'];
mysql_query($query3, $db) or die(mysql_error());
}
?>