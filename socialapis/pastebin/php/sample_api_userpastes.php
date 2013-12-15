<?php 
$api_dev_key = '5b76f2366bad23eff2f23b781b9389f5';
$api_user_key = '';
$api_results_limit = '100';
$url = 'http://pastebin.com/api/api_post.php';
$ch = curl_init($url);

curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_POSTFIELDS, 'api_option=list&api_user_key='.$api_user_key.'&api_dev_key='.$api_dev_key.'&api_results_limit='.$api_results_limit.'');
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($ch, CURLOPT_VERBOSE, 1);
curl_setopt($ch, CURLOPT_NOBODY, 0);

$response = curl_exec($ch);
echo $response;

/* Sample Response
<paste>
	<paste_key>0b42rwhf</paste_key>
	<paste_date>1297953260</paste_date>
	<paste_title>javascript test</paste_title>
	<paste_size>15</paste_size>
	<paste_expire_date>1297956860</paste_expire_date>
	<paste_private>0</paste_private>
	<paste_format_long>JavaScript</paste_format_long>
	<paste_format_short>javascript</paste_format_short>
	<paste_url>http://pastebin.com/0b42rwhf</paste_url>
	<paste_hits>15</paste_hits>
</paste>
<paste>
	<paste_key>0C343n0d</paste_key>
	<paste_date>1297694343</paste_date>
	<paste_title>Welcome To Pastebin V3</paste_title>
	<paste_size>490</paste_size>
	<paste_expire_date>0</paste_expire_date>
	<paste_private>0</paste_private>
	<paste_format_long>None</paste_format_long>
	<paste_format_short>text</paste_format_short>
	<paste_url>http://pastebin.com/0C343n0d</paste_url>
	<paste_hits>65</paste_hits>
</paste>
*/
?>