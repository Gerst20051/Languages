--TEST--
Bug #20087 (Assertion failure)
--SKIPIF--
<?php extension_loaded('mbstring') or die('skip mbstring not available'); ?>
--FILE--
<?php
ini_set('include_path','.');
include_once('common.inc');
$testmoo = "blah blah";
var_dump(mb_parse_str("testmoo"));
var_dump($testmoo);
var_dump(mb_parse_str("test=moo"));
var_dump($test);
?>
--EXPECT--
bool(true)
string(0) ""
bool(true)
string(3) "moo"

