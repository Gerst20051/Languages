<?php

$PORT = 20222;			//chat port
$ADDRESS = "localhost";	//adress
$sock;		//chat socket
$uin;		//user input file descriptor

$sock = stream_socket_client("tcp://$ADDRESS:$PORT");		//creating the client socket

echo "connection established\n";

$uin = fopen("php://stdin", "r");		//opening a standart input file stream

$conOpen = true;	//we run the read loop until other side closes connection
while($conOpen) {	//the read loop

	$r = array($sock, $uin);		//file streams to select from
	$w = NULL;	//no streams to write to
	$e = NULL;	//no special stuff
	$t = NULL;	//no timeout for waiting
	
	if(0 < stream_select($r, $w, $e, $t)) {	//if select didn't throw an error
		foreach($r as $i => $fd) {	//checking every socket in list to see who's ready
			if($fd == $uin) {		//the stdin is ready for reading
				$text = fgets($uin);
				fwrite($sock, $text);
			}
			else {					//the socket is ready for reading
				$text = fgets($sock);
				if($text == "") {	//a 0 length string is read -> connection closed
					echo "Connection closed by peer\n";
					$conOpen = false;
					fclose($sock);
					break;
				}
				echo "[Server says] ".$text;
			}
		}
	}
}
?>
