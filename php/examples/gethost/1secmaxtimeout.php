<?php
function ar_gethostbyaddr($ip) {
  $output = `host -W 1 $ip`;
  if (ereg('.*pointer ([A-Za-z0-9.-]+)\..*',$output,$regs)) {
    return $regs[1]; 
  }
  return $ip;
}
?>