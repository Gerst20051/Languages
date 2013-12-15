<?php
session_start();

if (empty($_SESSION['session_id'])) {
	session_regenerate_id();
	$_SESSION['session_id'] = session_id();
	header("Location: /auth/" . $_SESSION['session_id'] . "/", TRUE, 301);
}

$url_action = (empty($_REQUEST['action'])) ? 'HomePage' : $_REQUEST['action'];
if (isset($url_action)) {
	if (is_callable($url_action)) call_user_func($url_action);
	else print 'Function does not exist, request terminated';
}

function HomePage() {
	print '<h1>Homepage</h1>';
	print '<p><a href="?action=LogIn">LogIn</a></p>';
	print '<p><a href="?action=LogOut">LogOut</a></p>';
	print '<p><a href="?action=SecureContent">Secure Content</a></p>';
}

function LogIn($url='') {
	$session_id = $_SESSION['session_id'];
	while (!IsAuthenticated()) {
		header('WWW-Authenticate: Basic realm="' . $session_id . '"'); 
		header('HTTP/1.1 401 Unauthorized');
		die('Authorization Required');
	} 
	if (!empty($url)) return TRUE;
	else header("Location: /auth/" . $_SESSION['session_id'] . "/", TRUE, 301);
}

function LogOut() {
	session_destroy(); 
	session_unset($_SESSION['session_id']);
	header("Location: /auth/", TRUE, 301);
}

function SecureContent() {
	if (LogIn("SecureContent")) {
		print '<h1>Secure Content</h1>';
		print '<p>This is secure content</p>';
		print '<p><a href="/auth/' . $_SESSION['session_id'] . '/?action=HomePage">Home Page</a></p>';
	} else print '<h1>Not Authorized</h1>';
}

function IsAuthenticated() {
	if (isset($_SERVER['PHP_AUTH_USER']) && isset($_SERVER['PHP_AUTH_PW'])) {
		$httpd_username = filter_var($_SERVER['PHP_AUTH_USER'], FILTER_SANITIZE_STRING, FILTER_FLAG_ENCODE_HIGH|FILTER_FLAG_ENCODE_LOW);
		$httpd_password = filter_var($_SERVER['PHP_AUTH_PW'], FILTER_SANITIZE_STRING, FILTER_FLAG_ENCODE_HIGH|FILTER_FLAG_ENCODE_LOW);
		if ($httpd_username == "test" && $httpd_password == "test") return TRUE;
		else return FALSE;
	}
	return FALSE;
}

?>