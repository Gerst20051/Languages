<?php
echo memory_get_usage() . "\n";
$a = str_repeat("Hello", 4242);
echo memory_get_usage() . "\n";
unset($a);
echo memory_get_usage() . "\n";
?>