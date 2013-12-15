#!/usr/local/bin/php -q
<?php
// Change the address and port variables to suit your setup and execute.
// telnet 192.168.1.53 10000
session_start();
error_reporting(E_ALL);
set_time_limit(0); // Allow the script to hang around waiting for connections.
ob_implicit_flush(); // Turn on implicit output flushing so we see what we're getting as it comes in.

$address = $_SERVER['SERVER_ADDR'];
$port = 10000;

if (($sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) === false) {
	echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
}

if (socket_bind($sock, $address, $port) === false) {
	echo "socket_bind() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
}

if (socket_listen($sock, 5) === false) {
	echo "socket_listen() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
}

do {
	if (($msgsock = socket_accept($sock)) === false) {
		echo "socket_accept() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
		break;
	}

	$msg = "\nWelcome to the PHP Test Server. \n\n";
	$msg .= "To quit, type 'quit'. To shut down the server type 'shutdown'.\n";
	socket_write($msgsock, $msg, strlen($msg));

	do {
		if (($buf = socket_read($msgsock, 2048, PHP_NORMAL_READ)) === false) {
			echo "socket_read() failed: reason: " . socket_strerror(socket_last_error($msgsock)) . "\n";
			break 2;
		}

		if (!$buf = trim($buf)) continue;
		if ($buf == 'quit') break;
		if ($buf == 'shutdown') { socket_close($msgsock); break 2; }

		$talkback = "PHP: You said '$buf'.";
		if (isset($_SESSION['logged'])) $talkback = "PHP: Hello " . $_SESSION['username'] . ", You said '$buf'.\n";
		else $talkback = "PHP: You said '$buf'.\n";
		socket_write($msgsock, $talkback, strlen($talkback) + 10);
		echo "$buf\n";
	} while (true);
	socket_close($msgsock);
} while (true);

socket_close($sock);
?>