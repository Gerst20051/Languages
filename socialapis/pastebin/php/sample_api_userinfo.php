<?php 
$api_dev_key = '5b76f2366bad23eff2f23b781b9389f5';
$api_user_key = '';
$url = 'http://pastebin.com/api/api_post.php';
$ch = curl_init($url);

curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_POSTFIELDS, 'api_option=userdetails&api_user_key='.$api_user_key.'&api_dev_key='.$api_dev_key.'');
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($ch, CURLOPT_VERBOSE, 1);
curl_setopt($ch, CURLOPT_NOBODY, 0);

$response = curl_exec($ch);
echo $response;

/* Sample Response
<user>
	<user_name>wiz_kitty</user_name>
	<user_format_short>text</user_format_short>
	<user_expiration>N</user_expiration>
	<user_avatar_url>http://pastebin.com/cache/a/1.jpg</user_avatar_url>
	<user_watching>0/<user_watching>
	<user_watchers>72</user_watchers>
	<user_total_pastes>2</user_total_pastes>
	<user_private>1</user_private>
	<user_website>http://myawesomesite.com</user_website>
	<user_email>oh@dear.com</user_email>
	<user_location>New York</user_location>
	<user_account_type>0</user_account_type>
	<user_bio>I like turtles</user_bio>
</user>
*/
?>