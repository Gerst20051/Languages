<?php
function getServerAddress() {
	if (isset($_SERVER["SERVER_ADDR"])) return $_SERVER["SERVER_ADDR"];
	else { // Running CLI
		if (stristr(PHP_OS, 'WIN')) {
			//  Rather hacky way to handle windows servers
			exec('ipconfig /all', $catch);
			foreach ($catch as $line) {
				if (eregi('IP Address', $line)) {
					// Have seen exec return "multi-line" content, so another hack.
					if (count($lineCount = split(':', $line)) == 1) {
						list($t, $ip) = split(':', $line);
						$ip = trim($ip);
					} else {
						$parts = explode('IP Address', $line);
						$parts = explode('Subnet Mask', $parts[1]);
						$parts = explode(': ', $parts[0]);
						$ip = trim($parts[1]);
					}
					if (ip2long($ip > 0)) {
						echo 'IP is '.$ip."\n";
						return $ip;
					} else
					; // TODO: Handle this failure condition.
				}
			}
		} else {
			$ifconfig = shell_exec('/sbin/ifconfig eth0');
			preg_match('/addr:([\d\.]+)/', $ifconfig, $match);
			return $match[1];
		}
	}
}
?>