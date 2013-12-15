<?php
// Set time limit to indefinite execution
set_time_limit (0);
// Set the ip and port we will listen on
$address = 'localhost';
$port = 10000;
$max_clients = 10;
// Array that will hold client information
$client = Array();
// Create a TCP Stream socket
$sock = socket_create(AF_INET, SOCK_STREAM, 0);
// Bind the socket to an address/port
socket_bind($sock, $address, $port) or die('Could not bind to address');
// Start listening for connections
socket_listen($sock);
echo "Waiting for connections...\r\n";
// Loop continuously
while (true) {
	// Setup clients listen socket for reading
	$read[0] = $sock;
	for ($i = 0; $i < $max_clients; $i++) {
		if (isset($client[$i]['sock']))
			$read[$i + 1] = $client[$i]['sock'];
	}
	// Set up a blocking call to socket_select()
	if (socket_select($read, $write = NULL, $except = NULL, $tv_sec = 5) < 1)
		continue;
	/* if a new connection is being made add it to the client array */
	if (in_array($sock, $read)) {
		for ($i = 0; $i < $max_clients; $i++) {
			if (empty($client[$i]['sock'])) {
				$client[$i]['sock'] = socket_accept($sock);
				echo "New client connected $i\r\n";
				break;
			}
			elseif ($i == $max_clients - 1)
				echo "Too many clients...\r\n";
		}
	} // end if in_array
	// If a client is trying to write - handle it now
	for ($i = 0; $i < $max_clients; $i++) { // for each client
		if (isset($client[$i]['sock'])) {
			if (in_array($client[$i]['sock'], $read)) {
				$input = socket_read($client[$i]['sock'], 1024);
				if ($input == null) {
					echo "Client disconnecting $i\r\n";
					// Zero length string meaning disconnected
					unset($client[$i]);
				} else {
					echo "New input received $i\r\n";
					// send it to the other clients
					for ($j = 0; $j < $max_clients; $j++) {
						if (isset($client[$j]['sock']) && $j != $i) {
							echo "Writing '$input' to client $j\r\n";
							socket_write($client[$j]['sock'], $input, strlen($input));
						}
					}
					if ($input == 'exit') {
						// requested disconnect
						socket_close($client[$i]['sock']);
					}
				}
			} else {
				echo "Client disconnected $i\r\n";
				// Close the socket
				socket_close($client[$i]['sock']);
				unset($client[$i]);
			}
		}
	}
} // end while
// Close the master sockets
socket_close($sock);
?>