<?php
 
require_once 'facebook.php';
 
$appapikey = 'xxxx';
$appsecret = 'xxxx';
 
$facebook = new Facebook($appapikey, $appsecret);
 
$user_id = $facebook->require_login();
 
$friends = $facebook->api_client->friends_get();
 
echo "<p>Hello <fb:name uid=\"$user_id\" useyou=\"false\" linked=\"false\" firstnameonly=\"true\"></fb:name>, you have ".count($friends)." friends";
 
foreach($friends as $friend){
     $infos.=$friend.",";
}
 
$infos = substr($infos,0,strlen($infos)-1);
 
$gender=$facebook->api_client->users_getInfo($infos,'sex');
 
$gender_array = array(); 
 
foreach($gender as $gendervalue){
     $gender_array[$gendervalue[sex]]++;
}
 
$male = round($gender_array[male]*100/($gender_array[male]+$gender_array[female]),2);
$female = 100-$male;
 
echo "<ul><li>Males: $male%</li><li>Females: $female%</li></ul>";
 
?>