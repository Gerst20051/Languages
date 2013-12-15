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
<meta name="content-script-type" content="text/javascript" />
<meta name="content-style-type" content="text/css" />
<meta name="author" content="Homenet Spaces Andrew Gerst" />
<meta name="copyright" content="© HnS Desktop" />
<meta name="keywords" content="Homenet, Spaces, HnS, OS, Web, Desktop, The, Place, To, Be, Creative, Andrew, Gerst, Free, Profile, Profiles, Apps, Applications" />
<meta name="description" content="Welcome to Homenet Spaces OS | This is the place to be creative! Feel free to add yourself to our wonderful community by registering! HnS Desktop" />
<script type="text/javascript" src="/hnsdesktop/jquery.js"></script>
<script type="text/javascript" src="/hnsdesktop/jquery.corners.js"></script>
<base target="_top" />
<style type="text/css">
html {
background-image: url(/img/bg.png);
background-repeat: repeat-x;
background-color: #fcfcfc;
}

body {
font-family: arial;
font-size: 14px;
margin: 0;
padding: 0;
line-height: 18px;
text-align: center;
background-image: url(/img/splash.jpg);
background-repeat: no-repeat;
background-position: top right;
}

#benjoffe {
width: 800px;
margin: 0 auto;
text-align: left;
}

a:hover {
color: red;
}

h1 {
padding: 18px 0 0 9px;
margin: 0;
height: 36px;
border-bottom: 1px solid #b8b8b8;
color: #666;
text-shadow: #fff 0 1px 1px;
font-size: 24px;
}

h2 {
padding: 0;
margin: 0;
color: #444;
text-shadow: #ccc 0 1px 1px;
font-size: 20px;
}

#nav {
list-style-type: none;
padding: 12px 0 0 10px;
margin :0;
border-bottom: 1px solid #6e6e6e;
height: 32px;
line-height: 32px;
line-height: 1.5em;
text-shadow: #fff 0 1px 0;
font-weight: bold;
color: #666;
}

#nav a {
color: #333;
font-weight: normal;
}

#nav li {
float: left;
padding-right: 20px;
}

#content {
clear: left;
position: relative;
padding: 18px 10px 10px 10px;
min-height: 200px;
}

#foot {
clear: both;
border-top: 1px solid #999;
padding: 10px;
font-size: 0.9em;
text-align: right;
}

#foot a {
color: #666;
}

#foot a:hover {
color: #000;
}

p {
margin: 18px 0;
}

ul {
list-style: square;
margin: 0 0 18px 0;
padding: 0 0 0 18px;
}

p + ul {
margin-top: -9px;
}

ol {
margin: 18px 0;
padding: 0 0 0 18px;
}

p:first-child,
ul:first-child,
h2:first-child
ol:first-child {
margin-top: 0;
}

p:last-child,
ul:last-child,
ol:last-child,
h2:last-child {
margin-bottom: 0;
}
</style>
<style type="text/css">
#app {
margin-top: 15px;
}

#holder {
position: relative;
width: 400px;
height:300px;
left: 100px;
border: 2px solid #333;
}

#sky {
position: absolute;
left: 0;
top: 0;
height: 150px;
width: 400px;
background-color: #CCD;
background-image: url(sky.jpg);
}

#floor {
position: absolute;
left: 0;
top: 150px;
height: 150px;
width: 400px;
background-color: #565;
background-image: url(floor.png);
}

#canvas {
position: absolute;
top: 0;
left: 0;
}

#overlay {
position: absolute;
top: 0;
left: 0;
width: 400px;
height: 300px;
background-image: url(overlay.gif);
}

#map, #underMap {
position: absolute;
}

#code {
position: absolute;
top: 140px;
}
</style>
<script type="text/javascript">
// make sure website is not running in a frame:
try {
if (top != window) {
top.location.replace(location.href)
}
} catch(ignore){}

var map;
var canvas;
var overlay;
// variables initiated at the bottom of the code...

var pi = Math.PI;
var total = 0;

Number.prototype.range = function() {
	return ((this + 2 * pi) % (2 * pi));
}

Number.prototype.roundC = function() {
	return (Math.round(this * 100) / 100);
}

var total = 0;
var samples = 200;
var arena = [];
arena[0]=[1,1,1,1,1,1,1,1,1,1]
arena[1]=[1,0,0,0,0,0,0,0,0,1]
arena[2]=[1,0,0,1,0,1,1,1,0,1]
arena[3]=[1,0,1,0,0,0,0,1,0,1]
arena[4]=[1,0,0,0,0,1,0,1,0,1]
arena[5]=[1,0,1,1,0,0,0,0,0,1]
arena[6]=[1,0,0,1,0,1,1,1,0,1]
arena[7]=[1,1,0,1,0,0,0,1,0,1]
arena[8]=[1,0,0,1,0,1,0,0,0,1]
arena[9]=[1,1,1,1,1,1,1,1,1,1]

var playerPos=[4,4]; // x,y (from top left)
var playerDir=0.4; // theta, facing right=0=2pi
var playerPosZ=1;
var key=[0,0,0,0,0]; // left, right, up, down
var playerVelY=0;
var face=[];

function wallDistance(theta) {
var data = [];
face = [];

var x = playerPos[0], y = playerPos[1];
var deltaX, deltaY;
var distX, distY;
var stepX, stepY;
var mapX, mapY
var atX=Math.floor(x), atY=Math.floor(y);
var thisRow=-1;
var thisSide=-1;
var lastHeight=0;

for (var i=0; i<samples; i++) {
theta+=pi/(3*samples)+2*pi;
theta%=2*pi;

mapX = atX, mapY = atY;

deltaX=1/Math.cos(theta);
deltaY=1/Math.sin(theta);

if (deltaX>0) {
stepX = 1;
distX = (mapX + 1 - x) * deltaX;
}
else {
stepX = -1;
distX = (x - mapX) * (deltaX*=-1);
}
if (deltaY>0) {
stepY = 1;
distY = (mapY + 1 - y) * deltaY;
}
else {
stepY = -1;
distY = (y - mapY) * (deltaY*=-1);
}


for (var j=0; j<20; j++) {
if (distX < distY) {
mapX += stepX;
if (arena[mapX][mapY]) {
if (thisRow!=mapX || thisSide!=0) {
if (i>0) {
data.push(i);
data.push(lastHeight);
}
data.push(i);
data.push(distX);
thisSide=0;
thisRow=mapX;
face.push(1+stepX);
}
lastHeight=distX;
break;
}
distX += deltaX;
}
else {
mapY += stepY;
if (arena[mapX][mapY]) {
if (thisRow!=mapY || thisSide!=1) {
if (i>0) {
data.push(i);
data.push(lastHeight);
}
data.push(i);
data.push(distY);
thisSide=1;
thisRow=mapY;
face.push(2+stepY)
}
lastHeight=distY;
break;
}
distY += deltaY;
}
}
}
data.push(i);
data.push(lastHeight);

return data;
}


function drawCanvas() {
canvas.clearRect(0,0,400, 300);

var theta = playerDir-pi/6;

var wall=wallDistance(theta);

map.beginPath();
map.clearRect(0,0,80,80);
map.fillStyle="#3366cc";
map.arc(playerPos[0]*8, playerPos[1]*8, 3, 0, 2*pi, true);
map.fill();
map.beginPath();
map.moveTo(8*playerPos[0], 8*playerPos[1]);


var linGrad;

var tl,tr,bl,br;

var theta1,theta2,fix1,fix2;

for (var i=0; i<wall.length; i+=4) {

theta1=playerDir-pi/6 + pi*wall[i]/(3*samples);
theta2=playerDir-pi/6 + pi*wall[i+2]/(3*samples);

fix1 = Math.cos(theta1-playerDir);
fix2 = Math.cos(theta2-playerDir);

var h=2-playerPosZ;

var wallH1=100/(wall[i+1]*fix1);
var wallH2=100/(wall[i+3]*fix2);

tl=[wall[i]*2, 150-wallH1*h];
tr=[wall[i+2]*2, 150-wallH2*h]
br=[wall[i+2]*2, tr[1]+wallH2*2];
bl=[wall[i]*2, tl[1]+wallH1*2]

var shade1=Math.floor(wallH1*2+20); if (shade1>255) shade1=255;
var shade2=Math.floor(wallH2*2+20); if (shade2>255) shade2=255;

linGrad = canvas.createLinearGradient(tl[0],0,tr[0],0);
linGrad.addColorStop(0, 'rgba('+(face[i/4]%2==0 ? shade1 : 0)+','+(face[i/4]==1 ? shade1 : 0)+','+(face[i/4]==2 ? 0 : shade1)+',1.0)');
linGrad.addColorStop(1, 'rgba('+(face[i/4]%2==0 ? shade2 : 0)+','+(face[i/4]==1 ? shade2 : 0)+','+(face[i/4]==2 ? 0 : shade2)+',1.0)');

canvas.beginPath();
canvas.moveTo(tl[0], tl[1]);
canvas.lineTo(tr[0], tr[1]);
canvas.lineTo(br[0], br[1]);
canvas.lineTo(bl[0], bl[1]);
canvas.fillStyle = linGrad;
canvas.fill();


map.lineTo(playerPos[0]*8+Math.cos(theta1)*(wall[i+1])*8, playerPos[1]*8+Math.sin(theta1)*(wall[i+1])*8);
map.lineTo(playerPos[0]*8+Math.cos(theta2)*(wall[i+3])*8, playerPos[1]*8+Math.sin(theta2)*(wall[i+3])*8);


}
map.fillStyle="#ff0000"
map.fill();

}

function nearWall(x,y){
var xx,yy;
if (isNaN(x)) x=playerPos[0];
if (isNaN(y)) y=playerPos[1];
for (var i=-0.1; i<=0.1; i+=0.2) {
xx=Math.floor(x+i)
for (var j=-0.1; j<=0.1; j+=0.2) {
yy=Math.floor(y+j);
if (arena[xx][yy]) return true;
}
}
return false;
}

var xOff = 0;
var yOff = 0;
function wobbleGun(){
var mag=playerVelY;
    xOff = (10+Math.cos(total/6.23)*mag*90)
    yOff = (10+Math.cos(total/5)*mag*90)
overlay.style.backgroundPosition = xOff + "px " + yOff + "px";
}


var jumpCycle=0;

var audio = window.Audio && new Audio("shoot.wav");

function shoot()
{
audio && audio.play();
canvas.save();
canvas.strokeStyle = "#ffff00";
canvas.beginPath();
    
canvas.moveTo(190+xOff, 140+yOff);
canvas.lineTo(250+xOff, 200+yOff);
canvas.closePath();
canvas.stroke();
canvas.restore();
setTimeout('drawCanvas()',100);
}


function update(){

total++;

var change=false;

if (jumpCycle) {
jumpCycle--;
change=true;
playerPosZ = 1 + jumpCycle*(20-jumpCycle)/110;
}
else if (key[4]) jumpCycle=20;

if (key[0]) {
if (!key[1]) {
playerDir-=0.07; //left
change=true;
}
}
else if (key[1]) {
playerDir+=0.07; //right
change=true;
}

if (change) {
playerDir+=2*pi;
playerDir%=2*pi;
document.getElementById("sky").style.backgroundPosition=Math.floor(1-playerDir/(2*pi)*2400)+"px 0";
}

if (key[2] && !key[3]) {
if (playerVelY<0.1) playerVelY += 0.02;
}
else if (key[3] && !key[2]) {
if (playerVelY>-0.1) playerVelY -= 0.02;
}
else {
if (playerVelY<-0.02) playerVelY += 0.015;
else if (playerVelY>0.02) playerVelY -= 0.015;
else playerVelY=0;
}


if (playerVelY!=0) {

var oldX=playerPos[0];;
var oldY=playerPos[1];
var newX=oldX+Math.cos(playerDir)*playerVelY;
var newY=oldY+Math.sin(playerDir)*playerVelY;

if (!nearWall(newX, oldY)) {
playerPos[0]=newX;
oldX=newX;
change=true;
}
if (!nearWall(oldX, newY)) {
playerPos[1]=newY;
change=true;
}

}

if (playerVelY) wobbleGun();
if (change) drawCanvas();

}


function changeKey(which, to){
switch (which){
case 65:case 37: key[0]=to; break; // left
case 87: case 38: key[2]=to; break; // up
case 68: case 39: key[1]=to; break; // right
case 83: case 40: key[3]=to; break;// down
case 32: key[4]=to; break; // space bar;
case 17: key[5]=to; break; // ctrl
case 66: if (to) { shoot() } break; // b
}
}
document.onkeydown=function(e){changeKey((e||window.event).keyCode, 1);}
document.onkeyup=function(e){changeKey((e||window.event).keyCode, 0);}


function initUnderMap(){
var underMap=document.getElementById("underMap").getContext("2d");
underMap.fillStyle="#FFF";
underMap.fillRect(0,0, 200, 200);
underMap.fillStyle="#444";
for (var i=0; i<arena.length; i++) {
for (var j=0; j<arena[i].length; j++) {
if (arena[i][j]) underMap.fillRect(i*8, j*8, 8, 8);
}
}
}


window.onload = function() {
var ele = document.getElementById("map");
if (!ele.getContext) {
alert('An error occured creating a Canvas 2D context. This may be because you are using an old browser, if not please contact me and I\'ll see if I can fix the error.');
return;
}

map=ele.getContext("2d");
canvas=document.getElementById("canvas").getContext("2d");
overlay=document.getElementById("overlay");
document.getElementById("sky").style.backgroundPosition=Math.floor(-playerDir/(2*pi)*2400)+"px 0";
drawCanvas();
initUnderMap();
setInterval(update, 35);
}
</script>
</head>

<body scroll="no">
<!-- Begin page content -->
<div id="main">
<h2>Canvascape - "3D Walker"</h2>
<div id="app">
<canvas id="underMap" width="80" height="80"></canvas>
<canvas id="map" width="80" height="80"></canvas>
<div id="holder" style="clear:both;">
<div id="sky"></div>
<div id="floor"></div>
<canvas id="canvas" width="400" height="300"></canvas>
<div id="overlay"></div>
</div> 
<p>Use the arrow keys to walk around the map. Space bar = jump. B = Shoot.</p>
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