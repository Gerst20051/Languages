<?php

$host = "127.0.0.1";
$port = "8888";

set_time_limit(0);

print "Starting Socket Server...\n";

$sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);

socket_bind($sock, $host, $port);
socket_listen($sock, 4)

$childSocket = socket_accept($sock);
$incomingData = socket_read($childSocket, 2048);

if (substr($incomingData, 0, -2) == "are you hungry?") {
	$response = "Server Response > I could eat!\n";
	socket_write($childSocket, $response, strlen($response));
}

socket_close($childSocket);
socket_close($sock);

?>