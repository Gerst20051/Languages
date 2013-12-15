<?php
function gethostbyaddrl($ip) {
    $rrs=dns_get_record(implode('.',array_reverse(explode('.', $ip))).'.in-addr.arpa.',DNS_PTR);
    $revnames=array();
    foreach($rrs as $rr) $revnames[]=$rr['target'];
    return (count($revnames)) ? $revnames : FALSE;
}
?>