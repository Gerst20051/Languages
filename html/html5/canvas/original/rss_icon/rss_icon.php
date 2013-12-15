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
window.onload = function() {
	for (i = 2; i < 4; i++) {
		var canvas = document.getElementById('rsscanvas' + i);

		if (canvas.getContext) {
			var ctx = canvas.getContext('2d');
			ctx.scale(2,2)
			var verlauf = ctx.createLinearGradient(0,0,24,24);
			verlauf.addColorStop(0,'rgb(236,138,68)');
			verlauf.addColorStop(0.5,'rgb(252,158,60)');
			verlauf.addColorStop(1,'rgb(220,98,44)');
			ctx.fillStyle = verlauf;
			
			if (i < 3) {
				ctx.fillRect (0, 0, 24, 24);
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

<body scroll="no">
<!-- Begin page content -->
<div id="main">
<canvas id="rsscanvas2" width="48" height="48"></canvas>
<canvas id="rsscanvas3" width="48" height="48"></canvas>
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