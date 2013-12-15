<?php
if (isset($PHP_AUTH_USER)) {
	if (!isset($prev_user)) {
		header("Location: http://$HTTP_HOST$PHP_SELF?prev_user=$PHP_AUTH_USER");
		exit;
	} else {
		if ($PHP_AUTH_USER == $prev_user) {
			header('WWW-Authenticate: Basic realm="Secure"');
			header('HTTP/1.0 401 Unauthorized');
			exit;
		}
	}
} else {
	header('WWW-Authenticate: Basic realm="Secure"');
	header('HTTP/1.0 401 Unauthorized');
	exit;
}
?>