<?php
// Beware of index arrays :

echo json_encode(array("test","test","test"));
echo json_encode(array(0=>"test",3=>"test",7=>"test"));

// Will give :

["test","test","test"]
{"0":"test","3":"test","7":"test"}

// arrays are returned only if you don't define index.

// Be careful about one thing: 
// With a string key Php will consider it's an object: 

echo json_encode(array('id'=>'testtext')); 
echo json_encode(array('testtext')); 

// Will give: 

// {"id":"testtext"} 
// ["testtext"] 

// Beware of the string keys!
?>