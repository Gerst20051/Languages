<?php
session_start();

include ("db.inc.php");
include ("users_online.inc.php");

if (isset($_SESSION['logged']) && ($_SESSION['logged'] == 1)) {
include ("auth.inc.php");
include ("check_session.inc.php");
}

if (isset($_COOKIE['hnsrememberme'])) {
if (isset($_SESSION['logged']) && ($_SESSION['logged'] == 1 || 0)) {
} else {
$username = $_COOKIE['hnsrememberme']['username'];
$password = $_COOKIE['hnsrememberme']['password'];

$query = 'SELECT * FROM login u JOIN info i ON u.user_id = i.user_id WHERE username = "' . mysql_real_escape_string($username, $db) . '" AND password = "' . mysql_real_escape_string($password, $db) . '"';
$result = mysql_query($query, $db) or die(mysql_error($db));

if (mysql_num_rows($result) > 0) {
$row = mysql_fetch_assoc($result);
extract($row);
mysql_free_result($result);

$_SESSION['logged'] = 1;
$_SESSION['username'] = $username;
$_SESSION['access_level'] = $row['access_level'];
$_SESSION['last_login'] = $row['last_login'];
$_SESSION['last_login_ip'] = $row['last_login_ip'];
$_SESSION['fullname'] = $row['firstname'] . ' ' . $row['middlename'] . ' ' . $row['lastname'];
$_SESSION['firstname'] = $row['firstname'];
$_SESSION['middlename'] = $row['middlename'];
$_SESSION['lastname'] = $row['lastname'];
$_SESSION['email'] = $row['email'];
$_SESSION['status'] = $row['status'];
$_SESSION['mood'] = $row['mood'];
$_SESSION['alarm'] = $row['alarm'];
$_SESSION['default_image'] = $row['default_image'];
$_SESSION['pref_song_astart'] = $row['pref_song_astart'];
$_SESSION['pref_psong_astart'] = $row['pref_psong_astart'];
$_SESSION['pref_upstyle'] = $row['pref_upstyle'];
$_SESSION['pref_pupstyle'] = $row['pref_pupstyle'];
$_SESSION['pref_upview'] = $row['pref_upview'];
$_SESSION['setting_vmode'] = $row['setting_vmode'];
$_SESSION['setting_theme'] = $row['setting_theme'];
$_SESSION['setting_language'] = $row['setting_language'];
$_SESSION['user_id'] = $row['user_id'];
$user_id = null;
$username = null;

$last_login = date('Y-m-d');
$logins++;

$query = 'UPDATE info SET logins = ' . $logins . ' WHERE user_id = ' . $_SESSION['user_id'];
mysql_query($query, $db) or die(mysql_error());

$query = 'UPDATE login SET
last_login = "' . $last_login . '",
last_login_ip = "' . $ip . '"
WHERE
user_id = ' . $_SESSION['user_id'];
mysql_query($query, $db) or die(mysql_error());
} else {
$time = time();

setcookie("hnsrememberme[username]", null, ($time - 3600));
setcookie("hnsrememberme[password]", null, ($time - 3600));

$_SESSION['logged'] = null;
$_SESSION['username'] = null;
$_SESSION['access_level'] = null;
$_SESSION['last_login'] = null;
$_SESSION['last_login_ip'] = null;
$_SESSION['fullname'] = null;
$_SESSION['firstname'] = null;
$_SESSION['middlename'] = null;
$_SESSION['lastname'] = null;
$_SESSION['email'] = null;
$_SESSION['status'] = null;
$_SESSION['mood'] = null;
$_SESSION['default_image'] = null;
$_SESSION['pref_song_astart'] = null;
$_SESSION['pref_psong_astart'] = null;
$_SESSION['pref_upstyle'] = null;
$_SESSION['pref_pupstyle'] = null;
$_SESSION['pref_upview'] = null;
$_SESSION['setting_vmode'] = null;
$_SESSION['setting_theme'] = null;
$_SESSION['setting_language'] = null;
$_SESSION['user_id'] = null;
$user_id = null;
$username = null;
}
}
}
?>