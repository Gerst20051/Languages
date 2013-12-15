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
<script type="application/processing"> 
/* 
PROCESSINGJS.COM - BASIC EXAMPLE  
Delayed Mouse Tracking
MIT License - Hyper-Metrix.com/F1LT3R
Native Processing compatible 
*/  

// Global variables
float radius = 50.0;
int X, Y;
int nX, nY;
int delay = 16;

// setup the processing canvas
void setup() {
size(200,200);
strokeWeight(10);
frameRate(15);
X = (width / 2);
Y = (width / 2);
nX = X;
nY = Y;
}

// main draw loop
void draw() {
radius = (radius + sin(frameCount / 4));

// track circle to new destination
X += ((nX-X) / delay);
Y += ((nY-Y) / delay);

// fill canvas grey
background(100);

// set fill-color to blue
fill(0,121,184);

// set stroke-color white
stroke(255);

// draw circle
ellipse(X,Y,radius,radius);
}

// set circle's next destination
void mouseMoved() {
nX = mouseX;
nY = mouseY;
}
</script>
<canvas height="200px" width="200px"></canvas>
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