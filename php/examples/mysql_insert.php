<?php
function mysql_insert($table, $inserts) {
    $values = array_map('mysql_real_escape_string', array_values($inserts));
    $keys = array_keys($inserts);
        
    return mysql_query('INSERT INTO `'.$table.'` (`'.implode('`,`', $keys).'`) VALUES (\''.implode('\',\'', $values).'\')');
}
?>

For example:

<?php

mysql_insert('cars', array(
    'make' => 'Aston Martin',
    'model' => 'DB9',
    'year' => '2009',
));
?>