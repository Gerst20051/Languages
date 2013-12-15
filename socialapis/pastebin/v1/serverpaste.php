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
$response = curl_exec($ch);

echo $response;
?>