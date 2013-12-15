<?php
header('Access-Control-Allow-Origin: *');
$api_dev_key = '5b76f2366bad23eff2f23b781b9389f5';
$api_user_name = urlencode('gerst20051');
$api_user_password = urlencode('comwiz05');
$api_paste_name = urlencode('Server IP Address');
$api_results_limit = '50';

$ch = curl_init('http://pastebin.com/api/api_login.php');
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_POSTFIELDS, 'api_dev_key='.$api_dev_key.'&api_user_name='.$api_user_name.'&api_user_password='.$api_user_password.'');
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($ch, CURLOPT_VERBOSE, 1);
curl_setopt($ch, CURLOPT_NOBODY, 0);
$api_user_key = curl_exec($ch);

$ch = curl_init('http://pastebin.com/api/api_post.php');
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_POSTFIELDS, 'api_option=list&api_user_key='.$api_user_key.'&api_dev_key='.$api_dev_key.'&api_results_limit='.$api_results_limit.'');
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($ch, CURLOPT_VERBOSE, 1);
curl_setopt($ch, CURLOPT_NOBODY, 0);
$xml = curl_exec($ch);

preg_match('/<paste_key(.*)?>(.*)?<\/paste_key>/', $xml, $paste_key);
preg_match('/<paste_title(.*)?>(.*)?<\/paste_title>/', $xml, $paste_title);

foreach ($paste_key as $key=>$value) {
	if ($value == "Server IP Address") {
		echo 'http://pastebin.com/raw.php?i='.$paste_key[$key];
	}
}

/*
<paste>

<paste_key>UA0MGqUX</paste_key>

<paste_date>1312084241</paste_date>

<paste_title>HnS API Server IP</paste_title>

<paste_size>4790</paste_size>

<paste_expire_date>0</paste_expire_date>

<paste_private>0</paste_private>

<paste_format_long>PHP</paste_format_long>

<paste_format_short>php</paste_format_short>

<paste_url>http://pastebin.com/UA0MGqUX</paste_url>

<paste_hits>19</paste_hits>

</paste>

<paste>

<paste_key>PgeB6Mxu</paste_key>

<paste_date>1312080934</paste_date>

<paste_title>Server IP Address</paste_title>

<paste_size>13</paste_size>

<paste_expire_date>0</paste_expire_date>

<paste_private>1</paste_private>

<paste_format_long>PHP</paste_format_long>

<paste_format_short>php</paste_format_short>

<paste_url>http://pastebin.com/PgeB6Mxu</paste_url>

<paste_hits>0</paste_hits>

</paste>

<paste>

<paste_key>AvyFsSwk</paste_key>

<paste_date>1309675637</paste_date>

<paste_title>HnS API Stuff</paste_title>

<paste_size>836</paste_size>

<paste_expire_date>0</paste_expire_date>

<paste_private>0</paste_private>

<paste_format_long>PHP</paste_format_long>

<paste_format_short>php</paste_format_short>

<paste_url>http://pastebin.com/AvyFsSwk</paste_url>

<paste_hits>49</paste_hits>

</paste>

<paste>

<paste_key>n7VWyV0B</paste_key>

<paste_date>1309531909</paste_date>

<paste_title>Ruben's Link Clicks</paste_title>

<paste_size>881</paste_size>

<paste_expire_date>0</paste_expire_date>

<paste_private>0</paste_private>

<paste_format_long>JavaScript</paste_format_long>

<paste_format_short>javascript</paste_format_short>

<paste_url>http://pastebin.com/n7VWyV0B</paste_url>

<paste_hits>51</paste_hits>

</paste>
*/
?>