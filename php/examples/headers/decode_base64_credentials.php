<?php
// maybe we have caught authentication data in $_SERVER['REMOTE_USER']
if ((!$_SERVER['PHP_AUTH_USER'] || !$_SERVER['PHP_AUTH_USER']) && preg_match('/Basics+(.*)$/i', $_SERVER['REMOTE_USER'], $matches)) {
	list($name, $password) = explode(':', base64_decode($matches[1]));
	$_SERVER['PHP_AUTH_USER'] = strip_tags($name);
	$_SERVER['PHP_AUTH_PW'] = strip_tags($password);
}
?>