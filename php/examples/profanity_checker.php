<?php

function is_profanity($q,$json=0) {
	$q=urlencode(preg_replace('/[\W+]/',' ',$q));
	$p=file_get_contents('http://www.wdyl.com/profanity?q='.$q);
	if ($json) { return $p; }
	$p=json_decode($p);
	return ($p->response=='true')?1:0;
}

$q=isset($_REQUEST['q'])?$_REQUEST['q']:'';
echo is_profanity($q);

?>