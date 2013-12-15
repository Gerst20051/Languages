<?php
session_start();

include ("db.inc.php");
include ("login.inc.php");

/*
define('FACEBOOK_APP_ID', 'your application id');
define('FACEBOOK_SECRET', 'your application secret');

function get_facebook_cookie($app_id, $application_secret) {
	$args = array();
	parse_str(trim($_COOKIE['fbs_' . $app_id], '\\"'), $args);
	ksort($args);
	$payload = '';

	foreach ($args as $key => $value) {
		if ($key != 'sig') {
			$payload .= $key . '=' . $value;
		}
	}

	if (md5($payload . $application_secret) != $args['sig']) {
		return null;
	}

	return $args;
}

$cookie = get_facebook_cookie(FACEBOOK_APP_ID, FACEBOOK_SECRET);
*/
// <meta property="fb:app_id" content="87741124305" /> 
?>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" dir="ltr">

<head>
<title>Social HnS | Welcome to Homenet Spaces!</title>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<meta http-equiv="content-language" content="en" />
<meta name="author" content="Homenet Spaces Andrew Gerst" />
<meta name="copyright" content="Social HnS" />
<meta name="keywords" content="Social, Homenet, Spaces, HnS, The, Place, To, Be, Creative, Andrew, Gerst, Free, Profile, Profiles" />
<meta name="description" content="Welcome to Social HnS | This is the place to be creative! Feel free to add yourself to our wonderful community by registering! Social Homenet Spaces" />
<meta property="og:title" content="Social HnS" />
<meta property="og:type" content="website" />
<meta property="og:url" content="http://socialhns.tk" />
<meta property="og:site_name" content="Social HnS" />
<meta property="fb:admins" content="637458869" />
<meta property="og:description" content="Welcome to Social HnS | This is the place to be creative! Feel free to add yourself to our wonderful community by registering! Social Homenet Spaces" />
<noscript><meta http-equiv="X-Frame-Options" content="deny" /></noscript>
<script type="text/javascript" src="jquery.js"></script>
<script type="text/javascript" src="javascript.php"></script>
<base target="_top" />
</head>

<body id="main">
<div class="noscript">
We Are Sorry! HnS Desktop Requires A Browser That Supports HTML5, jQuery, and Javascript.
</div>
</body>

</html>
<?php mysql_close($db); ?>