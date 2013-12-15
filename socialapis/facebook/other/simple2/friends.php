<?php

// Get these from http://developers.facebook.com
$api_key = '2c73ae8529134aaaaaaaaaaaaaaaaaaa';
$secret  = '3c041a7cbd797aaaaaaaaaaaaaaaaaaa';

include_once './facebook-platform/php/facebook.php';

$facebook = new Facebook($api_key, $secret);
$user = $facebook->require_login();

?>
<h1>Yummie Tester</h1>
Hello <fb:name uid='<?php echo $user; ?>' useyou='false' possessive='true' />! <br>
Your id : <?php echo $user; ?>.

You have several friends:<br>
<?
$friends = $facebook->api_client->friends_get();
?>

<ul>
<?
foreach($friends as $friend){
	echo "<li><fb:name uid=\"$friend\" useyou=\"false\"></li>";	
}
?>
</ul>