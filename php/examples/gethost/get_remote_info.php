<?php
function getRemoteInfo () {
    $proxy="";
    $IP = "";
    if (isSet($_SERVER)) {
        if (isSet($_SERVER["HTTP_X_FORWARDED_FOR"])) {
            $IP = $_SERVER["HTTP_X_FORWARDED_FOR"];
            $proxy  = $_SERVER["REMOTE_ADDR"];
        } elseif (isSet($_SERVER["HTTP_CLIENT_IP"])) {
            $IP = $_SERVER["HTTP_CLIENT_IP"];
        } else {
            $IP = $_SERVER["REMOTE_ADDR"];
        }
    } else {
        if ( getenv( 'HTTP_X_FORWARDED_FOR' ) ) {
            $IP = getenv( 'HTTP_X_FORWARDED_FOR' );
            $proxy = getenv( 'REMOTE_ADDR' );
        } elseif ( getenv( 'HTTP_CLIENT_IP' ) ) {
            $IP = getenv( 'HTTP_CLIENT_IP' );
        } else {
            $IP = getenv( 'REMOTE_ADDR' );
        }
    }
    if (strstr($IP, ',')) {
        $ips = explode(',', $IP);
        $IP = $ips[0];
    }
    $RemoteInfo[0]=$IP;
    $RemoteInfo[1]=@GetHostByAddr($IP);
    $RemoteInfo[2]=$proxy;
    return $RemoteInfo;
}
?>