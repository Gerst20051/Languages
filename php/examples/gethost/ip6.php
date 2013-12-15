<?php
function gethostbyaddr6($ip6) {
  $ipv6calc = "/bin/ipv6calc";
  $dig = "/usr/bin/dig";
  $file = popen($ipv6calc." --in ipv6addr --out revnibbles.int ".escapeshellarg($ip6), r);
  $ip = fread($file, 128);
  pclose($file);
  if ((substr($ip, 0, 5) == "Error") || (!$ip)) return "Address is not a valid IPv6 address";
  $file = popen($dig." ptr ".$ip, r);                
   while (!feof ($file)) {
    $buffer = fgets($file, 128);
    if (substr($buffer, 0, 1) == ";") continue;
    $buffer = explode(" ", $buffer);
    if ($buffer[3] == "PTR") {
     $host = substr(trim($buffer[4]), 0, -1);
     pclose($file);
     return $host;
    }
   }
  pclose($file);
  return $ip6;
 }
 ?>