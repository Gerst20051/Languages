<?php
$a = array('<foo>',"'bar'",'"baz"','&blong&');

echo "Normal: ", json_encode($a), "\n";
echo "Tags: ",   json_encode($a, JSON_HEX_TAG), "\n";
echo "Apos: ",   json_encode($a, JSON_HEX_APOS), "\n";
echo "Quot: ",   json_encode($a, JSON_HEX_QUOT), "\n";
echo "Amp: ",    json_encode($a, JSON_HEX_AMP), "\n";
echo "All: ",    json_encode($a, JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_QUOT | JSON_HEX_AMP), "\n\n";

$b = array();

echo "Empty array output as array: ", json_encode($b), "\n";
echo "Empty array output as object: ", json_encode($b, JSON_FORCE_OBJECT), "\n\n";

$c = array(array(1,2,3));

echo "Non-associative array output as array: ", json_encode($c), "\n";
echo "Non-associative array output as object: ", json_encode($c, JSON_FORCE_OBJECT), "\n\n";

$d = array('foo' => 'bar', 'baz' => 'long');

echo "Associative array always output as object: ", json_encode($d), "\n";
echo "Associative array always output as object: ", json_encode($d, JSON_FORCE_OBJECT), "\n\n";

/*
Normal: ["<foo>","'bar'","\"baz\"","&blong&"]
Tags: ["\u003Cfoo\u003E","'bar'","\"baz\"","&blong&"]
Apos: ["<foo>","\u0027bar\u0027","\"baz\"","&blong&"]
Quot: ["<foo>","'bar'","\u0022baz\u0022","&blong&"]
Amp: ["<foo>","'bar'","\"baz\"","\u0026blong\u0026"]
All: ["\u003Cfoo\u003E","\u0027bar\u0027","\u0022baz\u0022","\u0026blong\u0026"]

Empty array output as array: []
Empty array output as object: {}

Non-associative array output as array: [[1,2,3]]
Non-associative array output as object: {"0":{"0":1,"1":2,"2":3}}

Associative array always output as object: {"foo":"bar","baz":"long"}
Associative array always output as object: {"foo":"bar","baz":"long"}
*/
?>