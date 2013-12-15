<?php
$ip = file_get_contents('http://whatismyip.org/');

$api_dev_key = '5b76f2366bad23eff2f23b781b9389f5';
$api_user_name = urlencode('gerst20051');
$api_user_password = urlencode('comwiz05');

$ch = curl_init('http://pastebin.com/api/api_login.php');
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_POSTFIELDS, 'api_dev_key='.$api_dev_key.'&api_user_name='.$api_user_name.'&api_user_password='.$api_user_password.'');
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($ch, CURLOPT_VERBOSE, 1);
curl_setopt($ch, CURLOPT_NOBODY, 0);

$api_user_key = curl_exec($ch);
$api_paste_private = '1';
$api_paste_expire_date = 'N';
$api_paste_format = 'php';
$api_paste_name = urlencode('Server IP Address');
$api_paste_code = urlencode($ip);

$ch = curl_init('http://pastebin.com/api/api_post.php');
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_POSTFIELDS, 'api_option=paste&api_user_key='.$api_user_key.'&api_paste_private='.$api_paste_private.'&api_paste_name='.$api_paste_name.'&api_paste_expire_date='.$api_paste_expire_date.'&api_paste_format='.$api_paste_format.'&api_dev_key='.$api_dev_key.'&api_paste_code='.$api_paste_code.'');
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($ch, CURLOPT_VERBOSE, 1);
curl_setopt($ch, CURLOPT_NOBODY, 0);
$response = curl_exec($ch);

$filename = 'pastebin.txt';
if (filesize($filename) > 0) {
	$fh = fopen($filename, 'r') or die("can't open file");
	$api_paste_key = fread($fh,filesize($filename));
	fclose($fh);
}
$fh = fopen($filename, 'w') or die("can't open file");
fwrite($fh, substr($response,20));
fclose($fh);

$ch = curl_init('http://pastebin.com/api/api_post.php');
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_POSTFIELDS, 'api_option=delete&api_user_key='.$api_user_key.'&api_dev_key='.$api_dev_key.'&api_paste_key='.$api_paste_key.'');
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($ch, CURLOPT_VERBOSE, 1);
curl_setopt($ch, CURLOPT_NOBODY, 0);

$response = curl_exec($ch);
?>