<?php
function dns_timeout($ip) {
 $res=`nslookup -timeout=3 -retry=1 $ip`;
 if (preg_match('/\nName:(.*)\n/', $res, $out)) {
   return trim($out[1]);
 } else {
   return $ip;
 }
}
?>