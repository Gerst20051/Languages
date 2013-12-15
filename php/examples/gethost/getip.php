<?php 
function get_ip( $host ){ 
  $hostip = @gethostbyname( $host ); 
  $ip = ( $hostip == $host ) ? $host : long2ip( ip2long( $hostip ) ); 
  //echo sprintf("Resolved %s to %s", $host, $ip); 
  return $ip; 
} 
?>