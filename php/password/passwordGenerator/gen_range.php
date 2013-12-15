<?php
function randCode($length = 5) {
	$ranges = array(range('a', 'z'), range('A', 'Z'), range(1, 9));
	$code = '';
	for($i = 0; $i < $length; $i++) {
		$rkey = array_rand($ranges);
		$vkey = array_rand($ranges[$rkey]);
		$code .= $ranges[$rkey][$vkey];
	}
	return $code;
}
?>