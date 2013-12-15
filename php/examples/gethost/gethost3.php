<?
function gethost($ipaddress)
{
   $host = trim( `getent hosts $ipaddress` );
   $host=explode(" ",$host);
   if(isset($host[1])) return $host[1];
   else return "";
}
?>