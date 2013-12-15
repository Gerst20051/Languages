<?
header("Expires: Sat, 01 Jan 2000 00:00:00 GMT");
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
session_cache_limiter("public, no-store");
session_start();

function http_auth() {
	$_SESSION['AUTH'] = 1;
	header('HTTP/1.0 401 Unauthorized');
	header('WWW-Authenticate: Basic realm="sn4g auth system"');
	// The actions to be done when the user clicks on 'cancel'
	exit();
}

if (!isset($_SERVER['PHP_AUTH_USER']) or @$_SESSION['AUTH'] != 1) {
	http_auth();
	exit();
}

// Actions do be done when the user has logged
// rest, must clean the session array
$_SESSION = array();
session_destroy();
?>