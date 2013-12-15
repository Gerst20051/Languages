<?php
function var_dump_pre($mixed = null) {
	echo '<pre>';
	var_dump($mixed);
	echo '</pre>';
	return null;
}

// This one returns the value of var_dump instead of outputting it.
function var_dump_ret($mixed = null) {
	ob_start();
	var_dump($mixed);
	$content = ob_get_contents();
	ob_end_clean();
	return $content;
}

// If contains html code
function var_dump_html($var){
	echo '<pre>'; // This is for correct handling of newlines
	ob_start();
	var_dump($var);
	$a=ob_get_contents();
	ob_end_clean();
	echo htmlspecialchars($a,ENT_QUOTES); // Escape every HTML special chars (especially > and < )
	echo '</pre>';
}

function var_dump_to_string($var) {
	$output = "<pre>";
	_var_dump_to_string($var,$output);
	$output .= "</pre>";
	return $output;
}

function _var_dump_to_string($var,&$output,$prefix="") {
	foreach ($var as $key=>$value) {
		if (is_array($value)) {
			$output .= $prefix.$key.": \n";
			_var_dump_to_string($value,$output,"  ".$prefix);
		} else {
			$output .= $prefix.$key.": ".$value."\n";
		}
	}
}

function array_to_string($array, $options = array()) {
	$options = array_merge(array(
		"html"=>array("open"=>"<pre>","close"=>"</pre>"),
		"enclose"=>array("open"=>"[","close"=>"]"),
		"separator"=>", "
	), $options);
 
	$output = $options["html"][0];
	$output = $options["enclose"][0];
	$output = implode($options["separator"], $array);
	$output = $options["enclose"][1];
	$output = $options["html"][1];
	return $output;
}

function array_to_string2($array, $options = array()) {
	$options = array_merge(array(
		"html"=>array("open"=>"<pre>", "close"=>"</pre>"),
		"enclose"=>array("open"=>"'", "close"=>"'"),
		"separator"=>array("pointer"=>" => ", "array"=>",\n")
	), $options);
 
	$output = $options["html"][0]."\$pass_arr = array(\n";
	foreach ($array as $key=>$value) {
		$output .= $options["enclose"][0].$key.$options["enclose"][1].$options["separator"][0];
		$output .= $options["enclose"][0].$value.$options["enclose"][1].$options["separator"][1];
	}
	$output .= ");\n".$options["html"][1];
	return $output;
}
?>