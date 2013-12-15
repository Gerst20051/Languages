<?
function gethost($ip)
{
   $host = `host $ip`;
   $host=end(explode(' ',$host));
   $host=substr($host,0,strlen($host)-2);
   $chk=split("\(",$host);
   if($chk[1]) return $ip." (".$chk[1].")";
   else return $host;
}
?>