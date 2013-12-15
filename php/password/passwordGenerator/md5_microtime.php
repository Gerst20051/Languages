<?php
$mc=microtime();
$mc=md5($mc);
$e= substr('$mc', 0,8);
echo $e;
?>