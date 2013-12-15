<?php 
if (isset($_SERVER['HTTP_AUTHORIZATION'])) {
	$auth_params = explode(":", base64_decode(substr($_SERVER['HTTP_AUTHORIZATION'], 6)));
	$_SERVER['PHP_AUTH_USER'] = $auth_params[0];
	unset($auth_params[0]);
	$_SERVER['PHP_AUTH_PW'] = implode('', $auth_params);
}
?> 