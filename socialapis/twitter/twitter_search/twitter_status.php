<html>
<head>
<style>
.twitter_container{
color:#444;
font-size:12px;
width:600px;
margin: 0 auto;
}
.twitter_container a{
color:#0066CC;
font-weight:bold;
}
.twitter_status{
height:60px;
padding:6px;
border-bottom:solid 1px #DEDEDE;
}
.twitter_image{
float:left;
margin-right:14px;
border:solid 2px #DEDEDE;
width:50px;
height:50px;
}
.twitter_posted_at{
font-size:11px;
padding-top:4px;
color:#999;
}
</style>
</head>
<body>
<div class="twitter_container">
<?php 
// require the twitter library
require "twitter.lib.php";

// your twitter username and password
$username = "your_username";
$password = "your_password";

// initialize the twitter class
$twitter = new Twitter($username, $password);

// fetch public timeline in xml format
$xml = $twitter->getPublicTimeline();

$twitter_status = new SimpleXMLElement($xml);
foreach($twitter_status->status as $status){
	foreach($status->user as $user){
		echo '<img src="'.$user->profile_image_url.'" class="twitter_image">';
		echo '<a href="http://www.twitter.com/'.$user->name.'">'.$user->name.'</a>: ';
	}
	echo $status->text;
	echo '<br/>';
	echo '<div class="twitter_posted_at">Posted at:'.$status->created_at.'</div>';
	echo '</div>';
}
?>
<div>
</body>
</html>