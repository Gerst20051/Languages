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
<style type="text/css">
html, body div, span, applet, object, iframe, h1, h2, h3, h4, h5, h6, p, blockquote, pre, a abbr, acronym, address, big, cite, code, del, dfn, em, font, img, ins, kdb, q, s, samp, small, strike, strong, sub, sup, tt, var, b, u, i, center, dl, dt, dd, ol, ul, li fieldset, form, label, legend, table, caption, tbody, tfoot, thead, tr, th, td {
margin: 0;
padding: 0;
font-size: 100%;
vertical-align: baseline;
border: 0;
outline: 0;
background: transparent;
}

ol, ul {
list-style: none;
}

blockquote, q {
quotes: none;
}

a, a:hover {
text-decoration: none;
}

table {
border-collapse: collapse;
border-spacing: none;
}

body {
background: #111;
color: white;
font-family: helvetica;
}

body canvas {
border: solid 1px red;
}

body #bar {
width: 400px;
}

body #logo {
font-size: 1.4em;
}

body #score_container {
display: inline;
float: right;
}

body #pause_menu, body #restart_menu {
display: none;
}
</style>
</head>

<body scroll="no" onload="checkSupported();">
<!-- Begin page content -->
<div id="main">
<script type="text/javascript">
function checkSupported() {
  canvas = document.getElementById('canvas');
  if (canvas.getContext){
    ctx = canvas.getContext('2d');
    this.gridSize = 10;
    start();
  } else {
    alert("We're sorry, but your browser does not support the canvas tag. Please use any web browser other than Internet Explorer.");
  }
}

function start(){
  ctx.clearRect(0,0, canvas.width, canvas.height);
  this.currentPosition = {'x':50, 'y':50};
  snakeBody = [];
  snakeLength = 3;
  updateScore();
  makeFoodItem();
  drawSnake();
  direction = 'right';
  play();  
}

function restart(){
  pause();
  start();
}

function pause(){
  clearInterval(interval);
  allowPressKeys = false;
}

function play(){
  interval = setInterval(moveSnake,100);
  allowPressKeys = true;
}

function drawSnake() {
  if (snakeBody.some(hasEatenItself)) {
    gameOver();
    return false;
  }
  snakeBody.push([currentPosition['x'], currentPosition['y']]);
  ctx.fillStyle = "rgb(200,0,0)";
  ctx.fillRect(currentPosition['x'], currentPosition['y'], gridSize, gridSize);
  if (snakeBody.length > snakeLength) {
    var itemToRemove = snakeBody.shift();
    ctx.clearRect(itemToRemove[0], itemToRemove[1], gridSize, gridSize);
  }  
  if (currentPosition['x'] == suggestedPoint[0] && currentPosition['y'] == suggestedPoint[1]) {
    makeFoodItem();
    snakeLength += 1;
    updateScore();
  }
}

function leftPosition(){
 return currentPosition['x'] - gridSize;  
}

function rightPosition(){
  return currentPosition['x'] + gridSize;
}

function upPosition(){
  return currentPosition['y'] - gridSize;  
}

function downPosition(){
  return currentPosition['y'] + gridSize;
}

function whichWayToGo(axisType){  
  if (axisType=='x') {
    a = (currentPosition['x'] > canvas.width / 2) ? moveLeft() : moveRight();
  } else {
    a = (currentPosition['y'] > canvas.height / 2) ? moveUp() : moveDown();
  }
}

function moveUp(){
  if (upPosition() >= 0) {
    executeMove('up', 'y', upPosition());
  } else {
    whichWayToGo('x');
  }
}

function moveDown(){
  if (downPosition() < canvas.height) {
    executeMove('down', 'y', downPosition());    
  } else {
    whichWayToGo('x');
  }
}

function moveLeft(){
  if (leftPosition() >= 0) {
    executeMove('left', 'x', leftPosition());
  } else {
    whichWayToGo('y');
  }
}

function moveRight(){
  if (rightPosition() < canvas.width) {
    executeMove('right', 'x', rightPosition());
  } else {
    whichWayToGo('y');
  }
}

function executeMove(dirValue, axisType, axisValue) {
  direction = dirValue;
  currentPosition[axisType] = axisValue;
  drawSnake();
}

function makeFoodItem(){
  suggestedPoint = [Math.floor(Math.random()*(canvas.width/gridSize))*gridSize, Math.floor(Math.random()*(canvas.height/gridSize))*gridSize];
  if (snakeBody.some(hasPoint)) {
    makeFoodItem();
  } else {
    ctx.fillStyle = "rgb(10,100,0)";
    ctx.fillRect(suggestedPoint[0], suggestedPoint[1], gridSize, gridSize);
  };
}

function hasPoint(element, index, array) {
  return (element[0] == suggestedPoint[0] && element[1] == suggestedPoint[1]);
}

function hasEatenItself(element, index, array) {
  return (element[0] == currentPosition['x'] && element[1] == currentPosition['y']);  
}

function gameOver(){
  var score = (snakeLength - 3)*10;
  pause();
  alert("Game Over. Your score was "+ score);
  ctx.clearRect(0,0, canvas.width, canvas.height);
  document.getElementById('play_menu').style.display='none';
  document.getElementById('restart_menu').style.display='block';
}

function updateScore(){
  var score = (snakeLength - 3)*10
  document.getElementById('score').innerText = score;
}

document.onkeydown = function(event) {
  if (!allowPressKeys){
    return null;
  }
  var keyCode; 
  if(event == null)
  {
    keyCode = window.event.keyCode; 
  }
  else 
  {
    keyCode = event.keyCode; 
  }
 
  switch(keyCode)
  {
    case 37:
      if (direction != "right"){
        moveLeft();
      }
      break;
     
    case 38:
      if (direction != "down"){
        moveUp();
      }
      break; 
      
    case 39:
      if (direction != "left"){
        moveRight();
      }
      break; 
    
    case 40:
      if (direction != "up"){
        moveDown();
      }
      break; 
    
    default: 
      break; 
  } 
}

function moveSnake(){
  switch(direction){
    case 'up':
      moveUp();
      break;

    case 'down':
      moveDown();
      break;
      
    case 'left':
      moveLeft();
      break;

    case 'right':
      moveRight();
      break;
  }
}
</script>
<div id="bar"> 
<span id="logo">SNAKE</span> 
<div id="score_container"> 
Score:
<span id="score">0</span> 
</div> 
<div id="play_menu"> 
<button onclick="pause(); document.getElementById('play_menu').style.display='none'; document.getElementById('pause_menu').style.display='block';">Pause</button> 
</div> 
<div id="pause_menu"> 
<button onclick="restart(); document.getElementById('play_menu').style.display='block'; document.getElementById('pause_menu').style.display='none';">Restart</button> 
<button onclick="play(); document.getElementById('play_menu').style.display='block'; document.getElementById('pause_menu').style.display='none';">Resume</button> 
</div> 
<div id="restart_menu"> 
<button onclick="restart(); document.getElementById('play_menu').style.display='block'; document.getElementById('restart_menu').style.display='none';">Restart</button> 
</div> 
</div> 
<canvas id="canvas" height="300" width="400"></canvas>
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