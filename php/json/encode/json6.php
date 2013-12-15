<?php
// If you need to force an object (ex: empty array) you can also do: 
json_encode( (object)$arr );
// which acts the same as 
json_encode($arr, JSON_FORCE_OBJECT);

// Anybody having empty arrays and needing the JSON_FORCE_OBJECT option but not using 5.3 yet, you can substitute assigning an empty object:
if (empty($array)) $array = (object) null;
$return = json_encode($array);
?>