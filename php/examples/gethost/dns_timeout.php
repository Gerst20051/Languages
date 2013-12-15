<?php 
function dns_timeout($ip) 
{ 
    $res=`nslookup -timeout=2 -retry=1 $ip`; 
    if (preg_match('/name = (.*).\n/', $res, $out)) 
    { 
        return $out[1]; 
    } else 
    { 
        return $ip; 
    } 
} 
?>