<?php
$memcache = new Memcache;
$memcache -> connect("my.memcached.server", 11211);
$arrayVals = $memcache -> get("My Identifier");

if (!$arrayVals) {
	$query = "SELECT * FROM myTable";
	$result = mysql_query($query);
	while ($row = mysql_fetch_array($result)) $arrayVals[] = $row;
	$memcache -> set("My Identifier", $arrayVals, false, 60*60*24);
}
 
foreach ($arrayVals as $val) { print_r($val); }
?>