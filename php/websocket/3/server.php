<?php
set_time_limit(0);
$address = "localhost";
$port = 4545;

$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);

socket_bind($socket, $address, $port);

socket_listen($socket);
$connection = socket_accept($socket);

$result = trim(socket_read($connection, 1024));

echo "Result received: '$result'\n";

socket_close($connection);

socket_shutdown($socket);
socket_close($socket);
?>