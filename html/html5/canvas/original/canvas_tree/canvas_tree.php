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
<script type="text/javascript" src="jquery-1.3.2.js"></script> 
<script type="text/javascript" src="modernizr-1.0.js"></script> 
<script type="text/javascript" src="SnakeCollection.js"></script> 
<script type="text/javascript" src="Snake.js"></script>
<base target="_top" />
<script type="text/javascript"> 
			function randHex() {
				var num = Math.round(Math.random() * 255).toString(16);
				if (num.length == 1)
					num = "0"+num;
				return num;
			}
			
			jQuery(function() {
				if (Modernizr.canvas === true)
					jQuery("#canvas-warning").hide();
				
				// Convenience
				$canvas = jQuery("#canvas");
				canvas = $canvas[0];
				context = canvas.getContext("2d");
				
				// Dimensions
				var width = $canvas.width();
				var height = $canvas.height();
				
				// Set actual canvas size to match css
				$canvas.attr("width", width);
				$canvas.attr("height", height);
				
				// Information
				jQuery("#info").html("Size: "+canvas.width+"x"+canvas.height);
				
				// Frame rate
				var frame = 0;
				
				// Snakes
				var n = 2+Math.random()*3;
				var initialRadius = width/50;
				snakes = new SnakeCollection();
				for (var i=0 ; i<n ; i++) {
					var snake = new Snake(canvas);
					snake.x = width/2 - initialRadius + i*initialRadius*2/n;
					snake.radius = initialRadius;
					snakes.add(snake);
				}
				
				// Frame drawer
				var interval = setInterval(function() {
					snakes.next();
					
					frame++;
				}, 0);
				
				// fps
				var fpsInterval = setInterval(function() {
					jQuery("#fps").html(frame+" fps<br/>"+snakes.snakes.length+" branches running");
					frame = 0;
					if (snakes.snakes.length == 0) {
						clearInterval(interval);
						clearInterval(fpsInterval);
						var delay = 1500;
						jQuery("#info-container").fadeOut(1500, function(){
							jQuery("#info-container").html("Refresh for more delicious trees :)").fadeIn(delay, function() {
								setTimeout(function() {
									jQuery("#info-container").fadeOut(delay);
								}, delay*3)
							});
						});
					}
				}, 1000);
			});
		</script> 
		<style> 
			body {
				text-align: center;
/* 				background-color: #abcdef; */
			}
			#canvas {
				position: relative;
				margin-left: auto;
				margin-right: auto;
/* 				width: 1500px; */
/* 				height: 800px; */
				width: 750px;
				height: 400px;
/* 				border: 1px solid red; */
				border-bottom: 1px solid black;
			}
			a {
				color: black;
			}
		</style>
</head>

<body scroll="no">
<!-- Begin page content -->
<div id="main">
<div id="canvas-warning">I don't think your browser supports the HTML5 canvas, please use a different browser like <a 
href="http://www.mozilla-europe.org/en/firefox/">Firefox</a>, 
<a href="http://www.opera.com/mini/download/">Opera</a>, <a href="http://www.google.com/chrome">Chrome</a>, or <a href="http://www.apple.com/safari/download/">Safari</a></div> 
		<canvas id="canvas"></canvas> 
		<div><a href="http://kennethjorgensen.com/blog">blog</a></div> 
		<div id="info-container"> 
			<div id="info"></div> 
			<div id="fps"></div> 
		</div> 
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