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
PROCESSINGJS.COM HEADER ANIMATION 
MIT License - F1lT3R/Hyper-Metrix
Native Processing Compatible
*/
 
// Set number of circles
int count = 20;
 
// Set maximum and minimum circle size
int maxSize = 100;
int minSize = 20;
 
// Build float array to store circle properties
float[][] e = new float[count][5];
 
// Set size of dot in circle center
float ds=2;
 
// Selected mode switch
int sel = 0;
 
// Set drag switch to false
boolean dragging=false;
 
// If use drags mouse...
void mouseDragged(){
  
  // Set drag switch to true
  dragging=true;
}
  
// If user releases mouse...
void mouseReleased(){
  
  // ..user is no-longer dragging
  dragging=false;
}
 
 
// Set up canvas
void setup(){
  
  // Frame rate
  frameRate(10);
  
  // Size of canvas (width,height)
  size(900,74);
  
  // Stroke/line/border thickness
  strokeWeight(1);
  
  // Initiate array with random values for circles
  for(int j=0;j< count;j++){
    e[j][0]=random(width); // X 
    e[j][1]=random(height); // Y
    e[j][2]=random(minSize,maxSize); // Radius        
    e[j][3]=random(-.5,.5); // X Speed
    e[j][4]=random(-.5,.5); // Y Speed    
  }
}
 
// Begin main draw loop (called 25 times per second)
void draw(){
   
   // Fill background black
   background(0);
  
  // Begin looping through circle array
  for (int j=0;j< count;j++){
    
    // Disable shape stroke/border
    noStroke();
    
    // Cache diameter and radius of current circle
    float radi=e[j][2];
    float diam=radi/2;
    
    // If the cursor is within 2x the radius of current circle...
    if( dist(e[j][0],e[j][1],mouseX,mouseY) < radi ){
      
      // Change fill color to green.
      fill(64,187,128,100);
      
      // Remember user has circle "selected"  
      sel=1;
      
      // If user has mouse down and is moving...
      if(dragging){
        
        // Move circle to circle position
        e[j][0]=mouseX;
        e[j][1]=mouseY;
      }
    } else {
      // Keep fill color blue
      fill(64,128,187,100);
      
      // User has nothing "selected"
      sel=0;
    }
    
    // Draw circle
    ellipse(e[j][0],e[j][1],radi,radi);
    
    // Move circle
    e[j][0]+=e[j][3];
    e[j][1]+=e[j][4];
            
    
    /* Wrap edges of canvas so circles leave the top
       and re-enter the bottom, etc... */ 
    if( e[j][0] < -diam      ){ e[j][0] = width+diam;  } 
    if( e[j][0] > width+diam ){ e[j][0] = -diam;       }
    if( e[j][1] < 0-diam     ){ e[j][1] = height+diam; }
    if( e[j][1] > height+diam){ e[j][1] = -diam;       }
    
    // If current circle is selected...
    if(sel==1){
      
      // Set fill color of center dot to white..
      fill(255,255,255,255);
      
      // ..and set stroke color of line to green.
      stroke(128,255,0,100);      
    } else {            
      
      // otherwise set center dot color to black.. 
      fill(0,0,0,255);
      
      // and set line color to turquoise.
      stroke(64,128,128,255);      
    }
    
    // Loop through all circles
    for(int k=0;k< count;k++){
      
      // If the circles are close...
      if( dist(e[j][0],e[j][1],e[k][0],e[k][1]) < radi){
        
        // Stroke a line from current circle to adjacent circle
        line(e[j][0],e[j][1],e[k][0],e[k][1]);
      }
    }
    
    // Turn off stroke/border
    noStroke();      
    
    // Draw dot in center of circle
    rect(e[j][0]-ds,e[j][1]-ds,ds*2,ds*2);
  }
}
</script>
<canvas id="navCanvas" width="900px" height="74px" style="position: absolute"><img class="navCanvas" src="interactive-circles.gif" /></canvas>
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