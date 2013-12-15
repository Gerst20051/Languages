<?php
$remove = "ccop.jpg";

//
// get the string from the database
//

$arr = unserialize($str);
// $arr = json_decode($str);

foreach($arr as $key => $value) {
	if ($value == $remove) unset($arr[$key]);
}

$str = serialize($arr);
// $str = json_encode($arr);

//
// save the string back to the database
//
?>