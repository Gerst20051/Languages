<?php
function create_password($pw_length = 8, $use_caps = true, $use_numeric = true, $use_specials = true) {
	$caps = array();
	$numbers = array();
	$num_specials = 0;
	$reg_length = $pw_length;
	$pws = array();
	for ($ch = 97; $ch <= 122; $ch++) $chars[] = $ch; // create a-z
	if ($use_caps) for ($ca = 65; $ca <= 90; $ca++) $caps[] = $ca; // create A-Z
	if ($use_numeric) for ($nu = 48; $nu <= 57; $nu++) $numbers[] = $nu; // create 0-9
	$all = array_merge($chars, $caps, $numbers);
	if ($use_specials) {
		$reg_length =  ceil($pw_length*0.75);
		$num_specials = $pw_length - $reg_length;
		if ($num_specials > 5) $num_specials = 5;
		for ($si = 33; $si <= 47; $si++) $signs[] = $si;
		$rs_keys = array_rand($signs, $num_specials);	
		foreach ($rs_keys as $rs) {
			$pws[] = chr($signs[$rs]);
		}
	} 
	$rand_keys = array_rand($all, $reg_length);
	foreach ($rand_keys as $rand) {
		$pw[] = chr($all[$rand]);
	}	
	$compl = array_merge($pw, $pws);	
	shuffle($compl);
	return implode('', $compl);
}
?>