<?php
$auth_realm = 'My realm';
require_once 'auth.php';
echo "You've logged in as {$_SESSION['username']}<br>";
echo '<p><a href="?action=logOut">LogOut</a></p>';
?>