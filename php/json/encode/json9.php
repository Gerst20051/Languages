<?php

//If you have problems with quotes when encoding numeric data retrieved from the database, you can just cast that value to integer and there will be no quotes: 

$testArr['key'] = '1'; 
print json_encode($testArr); 

// ===> {"key":"1"} 
 
$testArr['key'] = (int)'1'; 
print json_encode($testArr);  

// ===> {"key":1} 

// Don't forget that you have to deal with numbers, otherwise your string will be converted to 0.

?>