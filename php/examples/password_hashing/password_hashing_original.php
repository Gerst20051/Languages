<?php
//Password hashing pepper (generated just for you)
define('PASSWORD_PEPPER','T4XYRpmUnWMcEHHoEUDeqpPdzb9pIDWh5xfpNUdh9XgX6qo81nNliW4iwJOGeAYK1q0mgi1Ua1U4nKuQsZHqUMP'
	. 'DkaNkJOaBb5l7PMtSpvgVvB4FoZSqNQlEeiFT84GOI7PToZkO7hLx7cClTmw044Tt2Xl2z1ky9K6mjPrRvDGe5N3DKm1wAWa2lUluVMINN'
	. '7k34ptsB3VyiY8WvOtfh7nrTCBmOHI79zoMA4mjTfZy5KPqMphoKgJqUu9MQ04yuHFOYSUv35XSqS3EbU2nn3oFz4rxNhi4ILdJ9rmgYEy'
	. 'oel33y2b4Oos513RrLF9WigOSB7TP6uSlSGgOgo30fLx5kZspjpinu4j6XreR9QA4IbUeE1LIhno5L3bbCikmW1J7sT3TUfkb3sE3iXLxb'
	. 'ClY7KjBWxsnq3vTFh62tXXGsrMZcB0tylPkHPGDIZ6CR9izANzADLdQJ8sy56w2LQoO4ZpSkfn6ooXKmns4AY5yqGdzDonqD1C5C150ZeL');

function pbkdf2($password, $salt) {
	//Make a giant final salt
	$goodSalt = PASSWORD_PEPPER . $salt . PASSWORD_PEPPER;

	//Hash with 1024 iterations and key length of 256 using sha512 for hash
	return pbkdf2_real("sha512", $password, $goodSalt, 1024, 256);
}

/*
 * PBKDF2 key derivation function as defined by RSA's PKCS #5: https://www.ietf.org/rfc/rfc2898.txt
 * Following functions thanks to https://defuse.ca/php-pbkdf2.htm
 * $algorithm - The hash algorithm to use. Recommended: SHA256
 * $password - The password.
 * $salt - A salt that is unique to the password.
 * $count - Iteration count. Higher = better. Recommended: At least 1024.
 * $key_length - The length of the derived key in BYTES.
 * Returns: A $key_length-byte key derived from the password and salt (in binary).
 *
 * Test vectors can be found here: https://www.ietf.org/rfc/rfc6070.txt
 */
function pbkdf2_real($algorithm, $password, $salt, $count, $key_length) {
	$algorithm = strtolower($algorithm);
	if(!in_array($algorithm, hash_algos(), true))
		die('PBKDF2 ERROR: Invalid hash algorithm.');
	if($count < 0 || $key_length < 0)
		die('PBKDF2 ERROR: Invalid parameters.');
	if($key_length > 4294967295)
		die('PBKDF2 ERROR: Derived key too long.');

	$hLen = strlen(hash($algorithm, "", true));
	$numBlocks = (int)ceil((double)$key_length / $hLen);

	$output = "";
	for($i = 1; $i <= $numBlocks; $i++)
			$output .= pbkdf2_f($password, $salt, $count, $i, $algorithm, $hLen);

	return substr($output, 0, $key_length);
}

/*
 * The pseudorandom function used by PBKDF2.
 * Definition: https://www.ietf.org/rfc/rfc2898.txt
 */
function pbkdf2_f($password, $salt, $count, $i, $algorithm, $hLen) {
	//$i encoded as 4 bytes, big endian.
	$last = $salt . chr(($i >> 24) % 256) . chr(($i >> 16) % 256) . chr(($i >> 8) % 256) . chr($i % 256);
	$xorsum = "";
	for($r = 0; $r < $count; $r++) {
		$u = hash_hmac($algorithm, $last, $password, true);
		$last = $u;
		if(empty($xorsum))
			$xorsum = $u;
		else
			for($c = 0; $c < $hLen; $c++)
				$xorsum[$c] = chr(ord(substr($xorsum, $c, 1)) ^ ord(substr($u, $c, 1)));
	}
	return bin2hex($xorsum);
} 

function displayErrors($errors) {
	echo "<table><tr><td colspan=2><b>Error</b></td></tr>";
	if(empty($errors))
		echo "<tr><td>None!</td></tr>";
	else
		foreach($errors as $listItem => $error)
			echo "<tr><td class='firstColumn'>$listItem</td><td class='secondColumn'>$error</td></tr>";
	echo "</table>";
}

function genSalt(){
	return hash("sha256", md5(mt_rand()) . hash("sha1",PASSWORD_PEPPER . time() . mt_rand()));
}

function genVerificationKey() {
	return sha1(sha1(mt_rand()) . substr(md5(mt_rand()),12));
}

function failNonUser() {
	global $USER;
	if($USER == null) {
msgbox ("You are currently not logged in","Error");
		echo "<p><a href='/user/login'>Click here to login</a>";
		return true;
	}
	return false;
}
?>