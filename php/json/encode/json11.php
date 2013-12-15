<?php
// I had trouble putting the results of mysql_fetch_assoc() through json_encode: numbers being returned from the query were being quoted in the JSON output (i.e., they were being treated as strings). In order to fix this, it is necessary to explicitly cast each element of the array before json_encode() is called.
// The following code uses metadata from a MySQL query result to do this casting.

$mysql = mysql_connect('localhost', 'user', 'password');
mysql_select_db('my_db');

$query = 'select * from my_table';
$res = mysql_query($query);

// iterate over every row
while ($row = mysql_fetch_assoc($res)) {
// for every field in the result..
for ($i=0; $i < mysql_num_fields($res); $i++) {
	$info = mysql_fetch_field($res, $i);
	$type = $info->type;

	// cast for real
	if ($type == 'real')
	$row[$info->name] = doubleval($row[$info->name]);
	// cast for int
	if ($type == 'int')
	$row[$info->name] = intval($row[$info->name]);
}

$rows[] = $row;
}

// JSON-ify all rows together as one big array
echo json_encode($rows);
    
mysql_close($mysql);
?>