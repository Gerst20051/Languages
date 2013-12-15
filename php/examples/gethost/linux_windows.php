<?php
// For Linux...

function gethost ($ip) {
 $host = `host $ip`;
 return (($host ? end ( explode (' ', $host)) : $ip));
}

// For Win32...

function nslookup ($ip) {
 $host = split('Name:',`nslookup $ip`);
 return ( trim (isset($host[1]) ? str_replace ("\n".'Address:  '.$ip, '', $host[1]) : $ip));
}
?>
