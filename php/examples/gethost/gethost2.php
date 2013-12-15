<?php 
function gethost ($ip) 
{ 
 //Make sure the input is not going to do anything unexpected 
 //IPs must be in the form x.x.x.x with each x as a number 
 $testar = explode('.',$ip); 
 if (count($testar)!=4) 
  return $ip; 
 for ($i=0;$i<4;++$i) 
  if (!is_numeric($testar[$i])) 
   return $ip; 

 $host = `host $ip`; 
 return (($host ? end ( explode (' ', $host)) : $ip)); 
} 
?>