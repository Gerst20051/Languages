<?php
$password_length = 9;
$alfa = "1234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
$token = "";

function make_seed() {
	list($usec, $sec) = explode(' ', microtime());
	return (float) $sec + ((float) $usec * 100000);
}

srand(make_seed());
for ($i = 0; $i < $password_length; $i ++) $token .= $alfa[rand(0, strlen($alfa))];
echo $token;
?>