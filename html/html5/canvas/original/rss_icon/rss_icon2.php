<?php
session_start();
chdir("/xampp/HomenetSpaces/hnsdesktop/");

include ("db.member.inc.php");
include ("login.inc.php");

if (isset($_SESSION['logged']) && ($_SESSION['logged'] == 1)) { // user is logged in
include ("users_online.loggedin.inc.php"); // include db.om.inc if this gets removed
include ("check_session.inc.php");
} else {
include ("users_online.notloggedin.inc.php"); // include db.om.inc if this gets removed
include ("check_session.inc.php");
}
?>
<!DOCTYPE html>
<html lang="en" dir="ltr">

<head>
<title>Homenet Spaces OS | HTML5 Canvas!</title>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1, windows-1252" />
<meta name="author" content="Homenet Spaces Andrew Gerst" />
<meta name="copyright" content="© HnS Desktop" />
<meta name="keywords" content="Homenet, Spaces, HnS, OS, Web, Desktop, The, Place, To, Be, Creative, Andrew, Gerst, Free, Profile, Profiles, Apps, Applications" />
<meta name="description" content="Welcome to Homenet Spaces OS | This is the place to be creative! Feel free to add yourself to our wonderful community by registering! HnS Desktop" />
<script type="text/javascript" src="/hnsdesktop/jquery.js"></script>
<script type="text/javascript" src="/hnsdesktop/jquery.corners.js"></script>
<base target="_top" />
<!--[if IE]>
<script type="text/javascript" src="excanvas.js"></script>
<![endif]--> 
<script type="text/javascript">
/*
function draw() {
	var canvas = document.getElementById('rss1');
	var canvas2 = document.getElementById('rss2');
	
	if (canvas.getContext && canvas2.getContext) {
		var ctx = canvas.getContext('2d');
		var ctx2 = canvas2.getContext('2d');
		ctx.scale(1,1);
		ctx2.scale(1,1);
		
		var gradient = ctx.createLinearGradient(0,0,24,24);
		gradient.addColorStop(0,'rgb(236,138,68)');
		gradient.addColorStop(0.5,'rgb(252,158,60)');
		gradient.addColorStop(1,'rgb(220,98,44)');
		
		ctx.fillStyle = gradient;
		ctx.fillRect(0,0,24,24);
		ctx.strokeStyle = "#ffffff";
		ctx.lineWidth = 3
		ctx.beginPath();
		ctx.arc(4,21,15,((Math.PI * 3) / 2),0,false);
		ctx.moveTo(4,11)
		ctx.arc(4,21,10,((Math.PI * 3) / 2),0,false);
		ctx.moveTo(8,18);
		ctx.arc(7,18,1,(Math.PI * 2),0,false);
		ctx.stroke();
		
		ctx2.fillStyle = gradient;
		ctx2.beginPath();
		ctx2.moveTo(0,5);
		ctx2.quadraticCurveTo(0,0,5,0);
		ctx2.lineTo(19,0);
		ctx2.quadraticCurveTo(24,0,24,5);
		ctx2.lineTo(24,19);
		ctx2.quadraticCurveTo(24,24,19,24);
		ctx2.lineTo(5,24);
		ctx2.quadraticCurveTo(0,24,0,19);
		ctx2.fill();
		ctx2.strokeStyle = "#ffffff";
		ctx2.lineWidth = 3
		ctx2.beginPath();
		ctx2.arc(4,21,15,((Math.PI * 3) / 2),0,false);
		ctx2.moveTo(4,11)
		ctx2.arc(4,21,10,((Math.PI * 3) / 2),0,false);
		ctx2.moveTo(8,18);
		ctx2.arc(7,18,1,(Math.PI * 2),0,false);
		ctx2.stroke();
	}
}
*/

// OR

function draw() {
	for (i = 1; i < 3; i++) {
		var canvas = document.getElementById('rss' + i);
	
		if (canvas.getContext) {
			var ctx = canvas.getContext('2d');
			ctx.scale(1,1);

			var gradient = ctx.createLinearGradient(0,0,24,24);
			gradient.addColorStop(0,'rgb(236,138,68)');
			gradient.addColorStop(0.5,'rgb(252,158,60)');
			gradient.addColorStop(1,'rgb(220,98,44)');

			ctx.fillStyle = gradient;

			if (i == 1) {
				ctx.fillRect(0,0,24,24);
			} else {
				ctx.beginPath();
				ctx.moveTo(0,5);
				ctx.quadraticCurveTo(0,0,5,0);
				ctx.lineTo(19,0);
				ctx.quadraticCurveTo(24,0,24,5);
				ctx.lineTo(24,19);
				ctx.quadraticCurveTo(24,24,19,24);
				ctx.lineTo(5,24);
				ctx.quadraticCurveTo(0,24,0,19);
				ctx.fill();
			}

			ctx.strokeStyle = "#ffffff";
			ctx.lineWidth = 3
			ctx.beginPath();
			ctx.arc(4,21,15,((Math.PI * 3) / 2),0,false);
			ctx.moveTo(4,11)
			ctx.arc(4,21,10,((Math.PI * 3) / 2),0,false);
			ctx.moveTo(8,18);
			ctx.arc(7,18,1,(Math.PI * 2),0,false);
			ctx.stroke();
		}
	}
}
</script>
</head>

<body scroll="no" onload="draw();">
<!-- Begin page content -->
<div id="main">
<canvas id="rss1" width="24" height="24"></canvas>
<canvas id="rss2" width="24" height="24"></canvas>
</div>
<!-- End page content -->
<?php
include ("tracking_scripts.inc.php");
?>
</body>

</html>
<?php
mysql_close($db);
?>