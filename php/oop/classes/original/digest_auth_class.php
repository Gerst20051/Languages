<?php
// explode the digest with multibrowser support
public function explodethedigest($instring) {
	$quote = '"';
	$equal = '=';
	$comma = ',';
	$space = ' ';
	$a = explode( $comma, $instring);
	$ax = explode($space, $a[0]);
	$b = explode($equal, $ax[1], 2);
	$c = explode($equal, $a[1], 2);
	$d = explode($equal, $a[2], 2);
	$e = explode($equal, $a[3], 2);
	$f = explode($equal, $a[4], 2);
	$g = explode($equal, $a[5], 2);
	$h = explode($equal, $a[6], 2);
	$i = explode($equal, $a[7], 2);
	$j = explode($equal, $a[8], 2);
	$k = explode($equal, $a[9], 2);
	$l = explode($equal, $a[10], 2);
	$parts = array(trim($b[0])=>trim($b[1], '"'), trim($c[0])=>trim($c[1], '"'), trim($d[0])=>trim($d[1], '"'), trim($e[0])=>trim($e[1], '"'), trim($f[0])=>trim($f[1], '"'), trim($g[0])=>trim($g[1], '"'), trim($h[0])=>trim($h[1], '"'), trim($i[0])=>trim($i[1], '"'), trim($j[0])=>trim($j[1], '"'), trim($k[0])=>trim($k[1], '"'), trim($l[0])=>trim($l[1], '"'));
	return $parts;
}
?>