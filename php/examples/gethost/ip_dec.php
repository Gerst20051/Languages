<?php
function ip2dec($ipaddr) {
  $base=explode(".",$ipaddr);
  $decimal=(double) $base[0]*16777216;
  $decimal+=$base[1]*65536;
  $decimal+=$base[2]*256;
  $decimal+=$base[3];
  if($decimal>2147483647) {
    $decimal-=4294967296;
  }
  return (int) $decimal;
}

function dec2ip($dec) {
  if($dec<0) {
    $dec=(double) 4294967296+$dec;
  }
  if($dec>16777215) {
    $ip=$dec-(intval($dec/256)*256);
    $dec=(double) intval($dec/256);
  } else $ip="0";
  if($dec>65535) {
    $ip=($dec-(intval($dec/256)*256)).".".$ip;
    $dec=(double) intval($dec/256);
  } else $ip="0.".$ip;
  if($dec>255) {
    $ip=($dec-(intval($dec/256)*256)).".".$ip;
    $dec=(double) intval($dec/256);
  } else $ip="0.".$ip;
  $ip=$dec.".".$ip;
  return (string) $ip;
}
?>