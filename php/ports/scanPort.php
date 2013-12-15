<?php
function _scanPort ($portNumber) {
	$handle = fsockopen($this-> targetIP, $portNumber, &$errno, &$errstr, 2);

	if (!$handle) {
		echo "$errno : $errstr <br/>";
	}

	socket_set_timeout ($handle, $this-> timeout);
	$write = fwrite($handle,"\x00");
	if (!$write) {
		echo "error writing to port: $index.<br/>";
		next;
	}

	$startTime = time();
	$header = fread($handle, 1);
	$endTime = time();
	$timeDiff = $endTime - $startTime;

	if ($timeDiff >= $this-> timeout) {
		fclose($handle);
		return 1;
	} else {
		fclose($handle);
		return 0;
	}
}
?>