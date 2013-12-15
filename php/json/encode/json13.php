<?php
// Note that this function does not always produce legal JSON.

$json = json_encode('foo');
var_dump($json);
//string(5) ""foo""

$json = json_encode(23);
var_dump($json);
//string(2) "23"

// According to the JSON spec, only objects and arrays can be represented; the JSON_FORCE_OBJECT flag available since PHP 5.3 does not change this behaviour. If you're using this to produce JSON that will be exchanged with other systems, adjust your output accordingly.

$json = preg_replace('/^([^[{].*)$/', '[$1]', $json);

// The json_decode function accepts these JSON fragments without complaint.
?>