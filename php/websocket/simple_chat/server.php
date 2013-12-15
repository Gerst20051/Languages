<?php

$PORT = 20222;			//chat port
$ADDRESS = "localhost";	//adress
$ssock;		//server socket
$csock;		//chat socket
$uin;		//user input file descriptor

$ssock = stream_socket_server("tcp://$ADDRESS:$PORT");		//creating the server sock

echo "Waiting for client...\n";
$csock = stream_socket_accept($ssock);			//waiting for the client to connect
										//$csock will be used as the chat socket
echo "Connection established\n";

$uin = fopen("php://stdin", "r");		//opening a standart input file stream

$conOpen = true;	//we run the read loop until other side closes connection
while($conOpen) {	//the read loop

	$r = array($csock, $uin);		//file streams to select from
	$w = NULL;	//no streams to write to
	$e = NULL;	//no special stuff handling
	$t = NULL;	//no timeout for waiting
	
	if(0 < stream_select($r, $w, $e, $t)) {	//if select didn't throw an error
		foreach($r as $i => $fd) {	//checking every socket in list to see who's ready
			if($fd == $uin) {		//the stdin is ready for reading
				$text = fgets($uin);
				fwrite($csock, $text);
			}
			else {					//the socket is ready for reading
				$text = fgets($csock);
				if($text == "") {	//a 0 length string is read -> connection closed
					echo "Connection closed by peer\n";
					$conOpen = false;
					fclose($csock);
					break;
				}
				echo "[Client says] " .$text;
			}
		}
	}
}

fclose($ssock);
?>
