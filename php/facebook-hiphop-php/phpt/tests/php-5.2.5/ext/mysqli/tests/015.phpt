--TEST--
mysqli autocommit/commit/rollback with myisam 
--SKIPIF--
<?php	
	require_once('skipif.inc');
	include "connect.inc";
	$link = mysqli_connect($host, $user, $passwd);
	$result = mysqli_query($link, "SHOW VARIABLES LIKE 'have_innodb'");
	$row = mysqli_fetch_row($result);
	mysqli_free_result($result);
	mysqli_close($link);
	
	if ($row[1] == "NO") {
		printf ("skip innodb support not installed.");
	}
?>
--FILE--
<?php
	include "connect.inc";
	
	$link = mysqli_connect($host, $user, $passwd);

	mysqli_select_db($link, "test");

	mysqli_autocommit($link, TRUE);

  	mysqli_query($link,"DROP TABLE IF EXISTS ac_01");

	mysqli_query($link,"CREATE TABLE ac_01(a int, b varchar(10))");

	mysqli_query($link, "INSERT INTO ac_01 VALUES (1, 'foobar')");
	mysqli_autocommit($link, FALSE);

	mysqli_query($link, "DELETE FROM ac_01");
	mysqli_query($link, "INSERT INTO ac_01 VALUES (2, 'egon')");

	mysqli_rollback($link);

	$result = mysqli_query($link, "SELECT * FROM ac_01");
	$row = mysqli_fetch_row($result);
	mysqli_free_result($result);

	var_dump($row);

	mysqli_query($link, "DELETE FROM ac_01");
	mysqli_query($link, "INSERT INTO ac_01 VALUES (2, 'egon')");
	mysqli_commit($link);

	$result = mysqli_query($link, "SELECT * FROM ac_01");
	$row = mysqli_fetch_row($result);
	mysqli_free_result($result);

	var_dump($row);

	mysqli_close($link);
?>
--EXPECT--
array(2) {
  [0]=>
  string(1) "2"
  [1]=>
  string(4) "egon"
}
array(2) {
  [0]=>
  string(1) "2"
  [1]=>
  string(4) "egon"
}
