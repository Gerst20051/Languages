<?
// If you're using a server on Windows, this is faster
function getisp($ip='') {
   if ($ip=='') $ip = $_SERVER['REMOTE_ADDR'];
   $longisp = @gethostbyaddr($ip);
   $isp = explode('.', $longisp);
   $isp = array_reverse($isp);
   $tmp = $isp[1];
   if (preg_match("/\<(org?|com?|net)\>/i", $tmp)) {
       $myisp = $isp[2].'.'.$isp[1].'.'.$isp[0];
   } else {
       $myisp = $isp[1].'.'.$isp[0];
   }
   if (preg_match("/[0-9]{1,3}\.[0-9]{1,3}/", $myisp))
      return 'ISP lookup failed.';
   return $myisp;
}

// If your server is on a *nix system, this is faster
function gethost ($ip) {
 $host = `host $ip`;
 return (($host ? end ( explode (' ', $host)) : $ip));
}

// be warned, however, that gethost() will issue a warning
// if safe mode is on with the use of backticked variables

?>
<?
function getisp($ip='') {
    if ($ip=='') $ip = $_SERVER['REMOTE_ADDR'];
    $longisp = @gethostbyaddr($ip);
    $isp = explode('.', $longisp);
    $isp = array_reverse($isp);
    $tmp = $isp[1];
    if (preg_match("/\<(org?|com?|net)\>/i", $tmp)) {
        $myisp = $isp[2].'.'.$isp[1].'.'.$isp[0];
    } else {
        $myisp = $isp[1].'.'.$isp[0];
    }
    preg_match("/[0-9]{1,3}\.[0-9]{1,3}/", $myisp) ? return 'ISP lookup failed.' : return $myisp;
}
?>