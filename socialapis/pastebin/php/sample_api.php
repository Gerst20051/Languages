<?php 
$api_dev_key = '5b76f2366bad23eff2f23b781b9389f5'; // your api_developer_key
$api_paste_code = 'just some random text you :)'; // your paste text
$api_paste_private = '1'; // 0=public 1=private
$api_paste_name = 'justmyfilename.js'; // name or title of your paste
$api_paste_expire_date = '10M';
$api_paste_format = 'php';
$api_user_key = ''; // if invalid key or no key is used, the paste will be create as a guest
$api_paste_name = urlencode($api_paste_name);
$api_paste_code = urlencode($api_paste_code);
$url = 'http://pastebin.com/api/api_post.php';

$ch = curl_init($url);
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_POSTFIELDS, 'api_option=paste&api_user_key='.$api_user_key.'&api_paste_private='.$api_paste_private.'&api_paste_name='.$api_paste_name.'&api_paste_expire_date='.$api_paste_expire_date.'&api_paste_format='.$api_paste_format.'&api_dev_key='.$api_dev_key.'&api_paste_code='.$api_paste_code.'');
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($ch, CURLOPT_VERBOSE, 1);
curl_setopt($ch, CURLOPT_NOBODY, 0);
$response = curl_exec($ch);

echo $response;

/*
Creating A New Paste, [Required Parameters]
Include all the following POST parameters when you request the URL:

1. api_dev_key - which is your unique API Developers Key.
2. api_option - set as 'paste', this will indicate you want to create a new paste.
3. api_paste_code - this is the text that will be written inside your paste.

Leaving any of these parameters out will result in an error.
Creating A New Paste, [Optional Parameters]
These parameters are not required when you create a new paste, but are possible to add:

1. api_user_key - this paramater is part of the login system, which is explained further down the page.
2. api_paste_name - this will be the name / title of your paste.
3. api_paste_format - this will be the syntax highlighting value, which is explained in detail further down the page.
4. api_paste_private - this makes a paste public or private, public = 0, private = 1
5. api_paste_expire_date - this sets the expiration date of your paste, the values are explained futher down the page.
*/
?>