<?php
// fetch JavaScript files to compress
$jsfiles = array_keys($_GET);

$js = '';		// code to compress
$jscomp = '';	// compressed JS
$err = '';		// error string
$reduced = -1;	// compression saving

// fetch JavaScript files
for ($i = 0, $j = count($jsfiles); $i < $j; $i++) {

	$fn = $jsfiles[$i] . '.js';
	$jscode = @file_get_contents($fn);
	if ($jscode !== false) {
		$js .= $jscode . "\n";
	}
	else {
		$err .= $fn . '; ';
	}
}


if ($err != '') {

	// error: missing files
	$jscomp = "alert('The following JavaScript files could not be read:\\n$err');";
}
else if ($js != '') {

	// REST API arguments
	$apiArgs = array(
		'compilation_level' => 'ADVANCED_OPTIMIZATIONS',
		'output_format' => 'text',
		'output_info' => 'compiled_code'
	);
	
	$args = 'js_code=' . urlencode($js);
	foreach ($apiArgs as $key => $value) {
		$args .= '&' . $key . '=' . urlencode($value);
	}
	
	// API call using cURL
	$call = curl_init();
	curl_setopt_array($call, array(
		CURLOPT_URL => 'http://closure-compiler.appspot.com/compile',
		CURLOPT_POST => 1,
		CURLOPT_POSTFIELDS => $args,
		CURLOPT_RETURNTRANSFER => 1,
		CURLOPT_HEADER => 0,
		CURLOPT_FOLLOWLOCATION => 0
	));
	$jscomp = curl_exec($call);
	curl_close($call);
	
	// calculate compression saving
	$reduced = (strlen($js) - strlen($jscomp)) / strlen($js) * 100;
}

// output content
header('Content-type: text/javascript');
echo $jscomp;

// output saving
if ($reduced >= 0) {
	echo
		'document.write("<p>JavaScript size reduced by ', 
		round($reduced, 1),
		'%.</p>");';
}
?>