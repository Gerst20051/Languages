<?php
    if ($db = new SQLiteDatabase('filename')) {
        $q = @$db->query('SELECT requests FROM tablename WHERE id = 1');
        if ($q === false) {
            $db->queryExec('CREATE TABLE tablename (id int, requests int, PRIMARY KEY (id)); INSERT INTO tablename VALUES (1,1)');
            $hits = 1;
        } else {
            $result = $q->fetchSingle();
            $hits = $result+1;
        }
        $db->queryExec("UPDATE tablename SET requests = '$hits' WHERE id = 1");
    } else {
        die($err);
    }
?>
<?php
$dbhandle = new SQLiteDatabase('sqlitedb');

$query = $dbhandle->query('SELECT name, email FROM users LIMIT 25'); // buffered result set
$query = $dbhandle->unbufferedQuery('SELECT name, email FROM users LIMIT 25'); // unbuffered result set

while ($entry = $query->fetch(SQLITE_ASSOC)) {
    echo 'Name: ' . $entry['name'] . '  E-mail: ' . $entry['email'];
}
?>
<?php
if ($db = sqlite_open('mysqlitedb', 0666, $sqliteerror)) { 
    sqlite_query($db, 'CREATE TABLE foo (bar varchar(10))');
    sqlite_query($db, "INSERT INTO foo VALUES ('fnord')");
    $result = sqlite_query($db, 'select bar from foo');
    var_dump(sqlite_fetch_array($result)); 
} else {
    die($sqliteerror);
}
?>
<?php
$dbhandle = sqlite_factory('sqlitedb');
$dbhandle->query('SELECT user_id, username FROM users');

/* functionally equivalent to: */

$dbhandle = new SQLiteDatabase('sqlitedb');
$dbhandle->query('SELECT user_id, username FROM users');

?>
<?php
function sqlite_data_seek($result, $numrow) {
  if ($numrow==0) {
    return sql_rewind($result);
  } else {
    return sql_seek($result, $numrow);
  }
}
?>
<?php
function md5_and_reverse($string) 
{
    return strrev(md5($string));
}

if ($dbhandle = sqlite_open('mysqlitedb', 0666, $sqliteerror)) {
    
    sqlite_create_function($dbhandle, 'md5rev', 'md5_and_reverse', 1);
    
    $sql  = 'SELECT md5rev(filename) FROM files';
    $rows = sqlite_array_query($dbhandle, $sql);
} else {
    echo 'Error opening sqlite db: ' . $sqliteerror;
    exit;
}
?>
<?php
$rows = sqlite_array_query($dbhandle, "SELECT php('md5', filename) from files");
?>
<?php

class sqlite_function {

    function md5($value)
    {
        return md5($value);
    }

}

$dbhandle = sqlite_open('SQLiteDB');

sqlite_create_function($dbhandle, 'md5', array('sqlite_function', 'md5'), 1);

// From now on, you can use md5 function inside your SQL statements

?>
<?php
$dbhandle = sqlite_open('mysqlitedb');
$query = sqlite_query($dbhandle, "UPDATE users SET email='jDoe@example.com' WHERE username='jDoe'");
if (!$query) {
    exit('Error in query.');
} else {
    echo 'Number of rows modified: ', sqlite_changes($dbhandle);
}
?>
<?php
$dbhandle = new SQLiteDatabase('mysqlitedb');
$query = $dbhandle->query("UPDATE users SET email='jDoe@example.com' WHERE username='jDoe'");
if (!$query) {
    exit('Error in query.');
} else {
    echo 'Number of rows modified: ', $dbhandle->changes();
}
?>
<?php
$dbhandle = sqlite_open('sqlitedb');
$result = sqlite_array_query($dbhandle, 'SELECT name, email FROM users LIMIT 25', SQLITE_ASSOC);
foreach ($result as $entry) {
    echo 'Name: ' . $entry['name'] . '  E-mail: ' . $entry['email'];
}
?>
<?php
$dbhandle = new SQLiteDatabase('sqlitedb');
$result = $dbhandle->arrayQuery('SELECT name, email FROM users LIMIT 25', SQLITE_ASSOC);
foreach ($result as $entry) {
    echo 'Name: ' . $entry['name'] . '  E-mail: ' . $entry['email'];
}
?>
<?php
$data = array(
   'one',
   'two',
   'three',
   'four',
   'five',
   'six',
   'seven',
   'eight',
   'nine',
   'ten',
   );
$dbhandle = sqlite_open(':memory:');
sqlite_query($dbhandle, "CREATE TABLE strings(a)");
foreach ($data as $str) {
    $str = sqlite_escape_string($str);
    sqlite_query($dbhandle, "INSERT INTO strings VALUES ('$str')");
}

function max_len_step(&$context, $string) 
{
    if (strlen($string) > $context) {
        $context = strlen($string);
    }
}

function max_len_finalize(&$context) 
{
    return $context;
}

sqlite_create_aggregate($dbhandle, 'max_len', 'max_len_step', 'max_len_finalize');

var_dump(sqlite_array_query($dbhandle, 'SELECT max_len(a) from strings'));

?>
<?php
$dbhandle = sqlite_open('sqlitedb');
sqlite_close($dbhandle);
?>
<?php
if ($db = sqlite_open('mysqlitedb', 0666, $sqliteerror)) { 
    sqlite_query($db, 'CREATE TABLE foo (bar varchar(10))');
    sqlite_query($db, "INSERT INTO foo VALUES ('fnord')");
    $result = sqlite_query($db, 'select bar from foo');
    var_dump(sqlite_fetch_array($result)); 
} else {
    die($sqliteerror);
}
?>
<?php
$db = sqlite_open('mysqlitedb');
$result = sqlite_query($db, "SELECT * FROM mytable WHERE name='John Doe'");
$rows = sqlite_num_rows($result);

echo "Number of rows: $rows";
?>
<?php
$db = new SQLiteDatabase('mysqlitedb');
$result = $db->query("SELECT * FROM mytable WHERE name='John Doe'");
$rows = $result->numRows();

echo "Number of rows: $rows";
?>
<?php
$dbhandle = sqlite_open('mysqlitedb');
$query = sqlite_exec($dbhandle, "UPDATE users SET email='jDoe@example.com' WHERE username='jDoe'", $error);
if (!$query) {
    exit("Error in query: '$error'");
} else {
    echo 'Number of rows modified: ', sqlite_changes($dbhandle);
}
?>
<?php
$dbhandle = new SQLiteDatabase('mysqlitedb');
$query = $dbhandle->queryExec("UPDATE users SET email='jDoe@example.com' WHERE username='jDoe'", $error);
if (!$query) {
    exit("Error in query: '$error'");
} else {
    echo 'Number of rows modified: ', $dbhandle->changes();
}
?>