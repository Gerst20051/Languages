--TEST--
function test: mysqli_num_rows()
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php

	include "connect.inc";
	
	/*** test mysqli_connect 127.0.0.1 ***/
	$link = mysqli_connect($host, $user, $passwd);

	mysqli_select_db($link, "test");

	mysqli_query($link, "DROP TABLE IF EXISTS test_result");

	mysqli_query($link, "CREATE TABLE test_result (a int, b varchar(10))");

	mysqli_query($link, "INSERT INTO test_result VALUES (1, 'foo')");

	mysqli_real_query($link, "SELECT * FROM test_result");
	if (mysqli_field_count($link)) {
		$result = mysqli_store_result($link);
		$num = mysqli_num_rows($result);
		mysqli_free_result($result);
	}

	var_dump($num);

	mysqli_close($link);
?>
--EXPECT--
int(1)
