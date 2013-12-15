<?php
// A note of caution: If you are wondering why json_encode() encodes your PHP array as a JSON object instead of a JSON array, you might want to double check your array keys because json_encode() assumes that you array is an object if your keys are not sequential.
// e.g.:

$myarray = Array('isa', 'dalawa', 'tatlo');
var_dump($myarray);

/* output
array(3) {
  [0]=>
  string(3) "isa"
  [1]=>
  string(6) "dalawa"
  [2]=>
  string(5) "tatlo"
}
*/

// As you can see, the keys are sequential; $myarray will be correctly encoded as a JSON array.

$myarray = Array('isa', 'dalawa', 'tatlo');
unset($myarray[1]);
var_dump($myarray);

/* output
array(2) {
  [0]=>
  string(3) "isa"
  [2]=>
  string(5) "tatlo"
}
*/

// Unsetting an element will also remove the keys. json_encode() will now assume that this is an object, and will encode it as such.

// SOLUTION: Use array_values() to re-index the array.

?>