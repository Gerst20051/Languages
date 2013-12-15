<?php
session_start();
include ("db.inc.php");

if (!isset($_SESSION['logged']) || ($_SESSION['logged'] != 1)) {
header('refresh: 4; url=index.php');
?>
<!DOCTYPE html>
<html lang="en" dir="ltr">

<head>
<title>Social HnS | Welcome to Homenet Spaces!</title>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1, windows-1252" />
<meta name="author" content="Homenet Spaces Andrew Gerst" />
<meta name="copyright" content="© Homenet Spaces" />
<meta name="keywords" content="Homenet, Spaces, HnS, Desktop, OS, Web, WebOS, Webtop, Online, Operating, System, Applications, Application, Apps, App, Services, Internet, The, Place, To, Be, Creative, Andrew, Gerst, Free, Profile, Profiles" />
<meta name="description" content="Welcome to Homenet Spaces | This is the place to be creative! Feel free to add yourself to our wonderful community by registering! " />
<script type="text/javascript" src="jquery.js"></script>
<script type="text/javascript" src="javascript.php"></script>
<base target="_top" />
<style type="text/css">
div#pagecontent {
height: 100%;
overflow: hidden;
user-select: none;
-khtml-user-select: none;
-moz-user-select: none;
-webkit-user-select: none;
width: 100%;
}

div#auth {
height: 109px;
left: 20%;
position: absolute;
top: 41%;
width: 60%;
}
</style>
<script type="text/javascript">
var milisec = 0;
var seconds = 5; // add 1 to absolute value
$("span#counter").html(seconds);

function display() {
	if (milisec <= 0) {
		milisec = 10;
		seconds -= 1;
	}

	if (seconds <= -1) {
		milisec = 0;
		seconds += 1;
	} else {
		milisec -= 1;
		$("span#counter").html(seconds);
		setTimeout("display()", 110);
	}
}

display();
</script>
</head>

<body>
<!-- Begin page content -->
<div id="pagecontent">
<div id="auth">
<div class="dialog"><div class="content"><div class="heading">
<p><strong style="color: #ff3333; font-weight: bold">You will be redirected to the login page in <span id="counter"></span> seconds.</strong></p>
<p>If your browser doesn't redirect you properly automatically, <a href="index.php">click here</a>.</p>
</div></div></div>
</div>
</div>
<!-- End page content -->
</body>

</html>
<?php
die();
}
?>