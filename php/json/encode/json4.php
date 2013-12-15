<?php
$dataIn = (object)array(
    "assoc" => array("cow"=>"moo"),
    "object" => (object)array("cat"=>"miao"),
);
/*
== IN
object(stdClass)#2 (2) {
  ["assoc"]=>
  array(1) {
    ["cow"]=>
    string(3) "moo"
  }
  ["object"]=>
  object(stdClass)#1 (1) {
    ["cat"]=>
    string(4) "miao"
  }
}

== JSON
{"assoc":{"_PHP_ASSOC":{"cow":"moo"}},"object":{"cat":"miao"}}

== OUT
object(stdClass)#4 (2) {
  ["assoc"]=>
  array(1) {
    ["cow"]=>
    string(3) "moo"
  }
  ["object"]=>
  object(stdClass)#7 (1) {
    ["cat"]=>
    string(4) "miao"
  }
}
*/
?>