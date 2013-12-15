<?php
include ('classes/udpPortScanner.inc'); // can't find

$udpScanner = new udpPortScanner("$REMOTE_ADDR");
$ports = $udpScanner-> doScan();

if (count($ports) == 0) {
	echo "no open udp ports detected.<br/>";
} else {
	echo "open udp ports:<br/>";
	foreach ($ports as $portNumber => $service) {
		echo "$portNumber ($service)<br/>";
	}
}
?>