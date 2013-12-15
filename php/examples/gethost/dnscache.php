<?php
function gethostbyaddr_with_cache($a) {
  global $dns_cache;
  if ($dns_cache[$a]) {
    return $dns_cache[$a];
    } else {
    $temp = gethostbyaddr($a);
    $dns_cache[$a] = $temp;
    return $temp;
  }
}
?>