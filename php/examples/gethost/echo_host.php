<?php
    $ip = $_SERVER['REMOTE_ADDR'];
    $fullhost = gethostbyaddr($ip);
    $host = preg_replace("/^[^.]+./", "*.", $fullhost);
?>

IP address <?=$ip?> | Host: <?=$host?>