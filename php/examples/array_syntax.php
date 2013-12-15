<?php
// SYNTAX FOR ARRAYS
//considering 2 arrays
$foo1 = array ("a", "b", "c");
$foo2 = array ("d", "e", "f");

//and 2 variables that hold integers
$num = 1;
$cell = 2;

echo ${foo.$num}[$cell]; // outputs "c"

$num = 2;
$cell = 0;

echo ${foo.$num}[$cell]; // outputs "d"
?>