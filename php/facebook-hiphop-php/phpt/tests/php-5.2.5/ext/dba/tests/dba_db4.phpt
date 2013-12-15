--TEST--
DBA DB4 handler test
--SKIPIF--
<?php 
	$handler = 'db4';
	require_once('skipif.inc');
?>
--FILE--
<?php
	$handler = 'db4';
	require_once('test.inc');
	$lock_flag = ''; // lock in library
	require_once('dba_handler.inc');
?>
===DONE===
--EXPECT--
database handler: db4
3NYNYY
Content String 2
Content 2 replaced
Read during write: allowed
Content 2 replaced 2nd time
The 6th value
array(3) {
  ["key number 6"]=>
  string(13) "The 6th value"
  ["key2"]=>
  string(27) "Content 2 replaced 2nd time"
  ["key5"]=>
  string(23) "The last content string"
}
===DONE===
