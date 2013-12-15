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
<script type="text/javascript" src="/hnsdesktop/exp/html5_canvas/processing/processing-0.9.4.js"></script>
<script type="text/javascript" src="/hnsdesktop/exp/html5_canvas/processing/processing.init.js"></script> 
<base target="_top" />
</head>

<body scroll="no">
<!-- Begin page content -->
<div id="main">
<div id="ide"> 
<div style="float: left; padding-left: 10px; text-align: left"> 
<div id="canvasbox"> 
<canvas id="canvas" style="width: 250px; height: 250px"></canvas>
</div> 
<button type="button" id="run">Run</button>
<button type="button" id="dl">Get Source</button>
<button type="button" id="pp" class="pause">Pause</button>
</div>

<script type="text/javascript" id="sketch">
void setup() {
	size(250,250);
	frameRate(20);
	strokeWeight(2);
}

void draw() {
	background(50);
	stroke(0,156,255);
	line(random(width),random(height),random(width),random(height));
}
</script>
<script type="text/javascript">
(function() {
	var init = function() {
		var canvas = document.getElementById("canvas");
		var sketch = document.getElementById("sketch").text;
		Processing(canvas,sketch);
	}

addEventListener("DOMContentLoaded", init, false);
})();

/*
// button functions
window.Processing.data = {};
window.Processing.data.state = 1; // 1 for play, 0 for pause

$(document).ready(function() {
// play / pause
$("#pp").click(function() {
if ($(this).hasClass("pause")) {
	$(this).removeClass("pause").addClass("play").html('&nbsp;&nbsp;Play&nbsp;&nbsp;');
	window.Processing.data.state = 0;
} else {
	$(this).removeClass("play").addClass("pause").html('Pause');
	window.Processing.data.state = 1;
}
});

// run script
$("#run").click(function() {
$("#canvas").fadeOut("normal",function() { // fade canvas out
$(this).remove(); // remove it to stop 'layering' of processing script
$("#canvasbox").html('<canvas id="display" style="width:250px;height:250px;display:none;"></canvas>');

// setup the play pause stuff
var source = $("#source").val().replace(/void[\s]*draw[\s]*\([a-zA-Z\,0-9]*\)[\s]*\{/g,'void draw(){if(window.Processing.data.state==0){return;}');

if (window.Processing.data.state == 0) { // press play if it's paused
	$("#pp").click();
}

// run processing and fade back in
Processing($("#canvas")[0],source);
$("#canvas").fadeIn("slow");
});

return false;
}).click(); // click for initial setup
}
*/
</script>
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