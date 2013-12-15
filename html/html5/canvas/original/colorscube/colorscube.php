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
<script type="text/javascript" src="colorscube.js"></script>
<base target="_top" />
<style>
  body * {
    font-family: sans-serif;
    font-size: 14px;
  }
  
  body.white {
    background-color: white;
    color: black;
  }
  
  body.black {
    background-color: black;
    color: white;
  }
  
  body.black a:link, body.black a:visited, body.black a:hover, body.black a:active {
    color: white;
  }
  
  body.white a:link, body.white a:visited, body.white a:hover, body.white a:active {
    color: black;
  }
  
  span.spaceyspan { margin-right: 20px; }
  div.centeredDiv { text-align: center; }
  li { list-style: none; }
  td { padding-right: 10px; }
</style>
</head> 
</head>

<body class="black" scroll="no">
<!-- Begin page content -->
<div id="main"> 
<div class="centeredDiv"> 
<canvas id="canvas" width="800" height="600"> 
  Sorry, this demo requires a web browser which supports HTML5 canvas!
</canvas> 
</div> 
 
<p> 
&copy; 2009 Dean McNamee (dean at gmail). Built with <a href="http://deanm.github.com/pre3d/">Pre3d</a>.
</p> 
 
<table> 
<tr><td>Press t</td><td>&rarr;</td><td>toggle background color</td></tr> 
<tr><td>Press p</td><td>&rarr;</td><td>pause</td></tr> 
<tr><td>Mouse</td><td>&rarr;</td><td>rotate around origin x and y axis</td></tr> 
<tr><td>Mouse + ctrl</td><td>&rarr;</td><td>pan x / y</td></tr> 
<tr><td>Mouse + shift</td><td>&rarr;</td><td>pan z</td></tr> 
<tr><td>Mouse + ctrl + shift</td><td>&rarr;</td><td>adjust focal length</td></tr> 
</table> 
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