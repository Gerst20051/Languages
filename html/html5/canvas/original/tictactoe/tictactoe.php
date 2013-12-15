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
<style type="text/css" media="print, handheld">
html {
background: none;
}

body {
margin: 0 !important;
}

header h1 {
margin: 0;
padding: 0;
font-size: 12pt;
}
</style>
<style type="text/css">
#grid {
position:absolute;top:0;left:0;z-index:1;text-shadow:white 1px 1px 3px;-webkit-box-shadow: 0px 2px 5px black;-moz-box-shadow: 0px 2px 5px black;
-o-box-shadow: 0px 2px 5px black;box-shadow: 0px 2px 5px black;border:none;background:#222;width:501px;height:501px;
color:black;text-align:center;
font:128px sans-serif;
-webkit-transition: -webkit-transform 1s ease-in-out;-moz-transition: -moz-transform 1s ease-in-out;
-o-transition: -o-transform 1s ease-in-out;transition: transform 1s ease-in-out
}

#grid tr {
height:167px;
}

#grid td {
width:165px;height:165px;vertical-align:top;border:1px solid #ccc
}

#grid div {
width:167px;height:167px;background:#eee;position:relative
}

#grid a {
-webkit-transition: opacity 0.5s ease-in-out;-moz-transition: opacity 0.5s ease-in-out;
-o-transition: opacity 0.5s ease-in-out;transition: opacity 0.5s ease-in-out;opacity:0;position:absolute;top:0;left:0;z-index:8;
color:black;text-decoration:none;display:block;width:165px;height:155px;padding-top:10px
}

#grid div div {
position:absolute;background:#fefefe;height:30%;border:none;z-index:3;
}
</style>
</head>

<body scroll="no">
<!-- Begin page content -->
<div id="main">
<article>
<h1>Tic Tac Toe</h1>
<div style="position:relative;-webkit-perspective: 800;-moz-perspective: 800;-o-perspective: 800;perspective: 800;width:501px;height:501px;margin:auto"> 
<table id="grid" cellpadding="0" cellspacing="0"> 
<tr><td><div><div></div><a href="javascript:void(0)" onclick="click(0,0)" id="g1">1</a></div></td><td><div><div></div><a href="javascript:void(0)" onclick="click(1,0)" id="g2">2</a></div></td><td><div><div></div><a href="javascript:void(0)" onclick="click(2,0)" id="g3">3</a></div></td></tr> 
<tr><td><div><div></div><a href="javascript:void(0)" onclick="click(0,1)" id="g4">4</a></div></td><td><div><div></div><a href="javascript:void(0)" onclick="click(1,1)" id="g5">5</a></div></td><td><div><div></div><a href="javascript:void(0)" onclick="click(2,1)" id="g6">6</a></div></td></tr> 
<tr><td><div><div></div><a href="javascript:void(0)" onclick="click(0,2)" id="g7">7</a></div></td><td><div><div></div><a href="javascript:void(0)" onclick="click(1,2)" id="g8">8</a></div></td><td><div><div></div><a href="javascript:void(0)" onclick="click(2,2)" id="g9">9</a></div></td></tr> 
</table> 
<canvas onmousedown="if(event.layerX){absclick(event.layerX,event.layerY)}else{absclick(event.x,event.y)};" width="501" height="501" id="canvas" style="position:absolute;top:0;left:0;z-index:999">ERROR - LAME WEB BROWSER</canvas> 
</div> 
<script type="text/javascript">
var ctx=document.getElementById("canvas").getContext("2d");
var grid=[];
var debug="none";
var spintimes=0;
var canclick=false;
function spin() {
	document.getElementById("grid").style.webkitTransform="rotateY(-"+((++spintimes)*360)+"deg)";
	document.getElementById("grid").style.mozTransform="rotateY(-"+(spintimes*360)+"deg)";
	document.getElementById("grid").style.oTransform="rotateY(-"+(spintimes*360)+"deg)";
	document.getElementById("grid").style.transform="rotateY(-"+(spintimes*360)+"deg)";
}
var filled=0;
var ai=true;
var gameover=false;
var lastgame="";
function ai_fill(n)
{
var counter=0;
	for(x=0;x<3;x++)
	{
		counter=0;
		counter+=1*(grid[0][x].innerHTML=="X");
		counter+=1*(grid[1][x].innerHTML=="X");
		counter+=1*(grid[2][x].innerHTML=="X");
		if(counter==n)
		{
			if(click(x,0)){return true}
			if(click(x,1)){return true}
			if(click(x,2)){return true}
		}
	}
	for(y=0;y<3;y++)
	{
		counter=0;
		counter+=1*(grid[y][0].innerHTML=="X");
		counter+=1*(grid[y][1].innerHTML=="X");
		counter+=1*(grid[y][2].innerHTML=="X");
		if(counter==n)
		{
			if(click(0,y)){return true}
			if(click(1,y)){return true}
			if(click(2,y)){return true}
		}
	}
	counter=0;
	counter+=1*(grid[0][0].innerHTML=="X");
	counter+=1*(grid[1][1].innerHTML=="X");
	counter+=1*(grid[2][2].innerHTML=="X");
	if(counter==n)
	{
		if(click(0,0)){return true}
		if(click(1,1)){return true}
		if(click(2,2)){return true}
	}
	counter=0;
	counter+=1*(grid[0][2].innerHTML=="X");
	counter+=1*(grid[1][1].innerHTML=="X");
	counter+=1*(grid[2][0].innerHTML=="X");
	if(counter==n)
	{
		if(click(2,0)){return true}
		if(click(1,1)){return true}
		if(click(0,2)){return true}
	}
	
	
	//check for potential losing positions
	for(x=0;x<3;x++)
	{
		counter=0;
		counter+=1*(grid[0][x].innerHTML=="O");
		counter+=1*(grid[1][x].innerHTML=="O");
		counter+=1*(grid[2][x].innerHTML=="O");
		if(counter==n)
		{
			if(click(x,0)){return true}
			if(click(x,1)){return true}
			if(click(x,2)){return true}
		}
	}
	
	for(y=0;y<3;y++)
	{
		counter=0;
		counter+=1*(grid[y][0].innerHTML=="O");
		counter+=1*(grid[y][1].innerHTML=="O");
		counter+=1*(grid[y][2].innerHTML=="O");
		if(counter==n)
		{
			if(click(0,y)){return true}
			if(click(1,y)){return true}
			if(click(2,y)){return true}
		}
	}
	
	counter=0;
	counter+=1*(grid[0][0].innerHTML=="O");
	counter+=1*(grid[1][1].innerHTML=="O");
	counter+=1*(grid[2][2].innerHTML=="O");
	if(counter==n)
	{
		if(click(0,0)){return true}
		if(click(1,1)){return true}
		if(click(2,2)){return true}
	}
	counter=0;
	counter+=1*(grid[0][2].innerHTML=="O");
	counter+=1*(grid[1][1].innerHTML=="O");
	counter+=1*(grid[2][0].innerHTML=="O");
	if(counter==n)
	{
		if(click(2,0)){return true}
		if(click(1,1)){return true}
		if(click(0,2)){return true}
	}
	
	return false;
	
}
function ai_fillm(n)
{
	var counter=0;
	for(x=0;x<3;x++)
	{
		counter=1;
		counter*=1*(grid[0][x].innerHTML!="O");
		counter*=1*(grid[1][x].innerHTML!="O");
		counter*=1*(grid[2][x].innerHTML!="O");
		
		if(counter)
		{
			if(click(x,0)){return true}
			if(click(x,1)){return true}
			if(click(x,2)){return true}
		}
	}
	for(y=0;y<3;y++)
	{
		counter=1;
		counter*=1*(grid[y][0].innerHTML!="O");
		counter*=1*(grid[y][1].innerHTML!="O");
		counter*=1*(grid[y][2].innerHTML!="O");
		if(counter)
		{
			if(click(0,y)){return true}
			if(click(1,y)){return true}
			if(click(2,y)){return true}
		}
	}
	counter=1;
	counter*=1*(grid[0][0].innerHTML!="O");
	counter*=1*(grid[1][1].innerHTML!="O");
	counter*=1*(grid[2][2].innerHTML!="O");
	if(counter)
	{
		if(click(0,0)){return true}
		if(click(1,1)){return true}
		if(click(2,2)){return true}
	}
	counter=1;
	counter*=1*(grid[0][2].innerHTML!="O");
	counter*=1*(grid[1][1].innerHTML!="O");
	counter*=1*(grid[2][0].innerHTML!="O");
	if(counter)
	{
		if(click(2,0)){return true}
		if(click(1,1)){return true}
		if(click(0,2)){return true}
	}
	
	
	return false;
	
}
var currentgame="";
function ai_go()
{
	
	//check for any winning positions
	think(1);
	if(click(1,1)){return}
	think(1);
	if(ai_fill(2)){return true}
	think(5);
	if(ai_fillm(1)){return true}
	//need to fork
	think(9);
	
	if(grid[0][0].innerHTML=="O"){if(click(2,2)){return true}}
	think(1);
	if(grid[0][2].innerHTML=="O"){if(click(0,2)){return true}}
	think(1);
	if(grid[2][0].innerHTML=="O"){if(click(2,0)){return true}}
	think(1);
	if(grid[2][2].innerHTML=="O"){if(click(0,0)){return true}}
	think(1);

	if(click(0,0)){return true}
	if(click(2,0)){return true}
	if(click(0,2)){return true}
	if(click(2,2)){return true}
	think(4);
	if(click(1,0)){return true}
	if(click(2,1)){return true}
	if(click(1,2)){return true}
	think(9);
	if(click(0,1)){return true}
}
function win(cha,type,index)
{
	if(cha!="")
	{
		ctx.lineWidth=20;
		ctx.strokeStyle="red";
		ctx.beginPath();
		if(type==0)
		{
			ctx.moveTo(80+167*index,20);
			ctx.lineTo(80+167*index,481);
		}
		else if(type==1)
		{
			ctx.moveTo(20,80+167*index);
			ctx.lineTo(481,80+167*index);
		}
		else if(type==2)
		{
			if(index==0)
			{
				ctx.moveTo(20,20);
				ctx.lineTo(481,481);
			}
			else
			{
				ctx.moveTo(20,481);
				ctx.lineTo(481,20);
			
			}
		}
		if (cha == "O") {
			if (currentgame == lastgame) {
				alert("It's not my fault I can't learn - This game doesn't count.");
			} else {
/*
var xmlhttp;

if (window.XMLHttpRequest) {
	xmlhttp = new XMLHttpRequest();
} else {
	xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
}

xmlhttp.onreadystatechange = function() {
	if (xmlhttp.readyState == 4) {
		alert(xmlhttp.responseText);
	}
};

xmlhttp.open("POST","ttt.php?rand="+Math.random(),true);
xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
xmlhttp.send("mejfjka=efhhsj&fehf=42389");
*/

winc(1);
lastgame = currentgame;
			}
		} else if (cha == "X") {
			losec(1);
		}
		
		ctx.stroke();
               
		gameover = true;
		return true;
	}
	return false;
}

function checkwin() {
	var x,y;
	for(x=0;x<3;x++) {
		if((grid[0][x].innerHTML==grid[1][x].innerHTML)&&(grid[1][x].innerHTML==grid[2][x].innerHTML))
		{
			if(win(grid[0][x].innerHTML,0,x)){return true}
		}
	}
	for(y=0;y<3;y++)
	{
		if((grid[y][0].innerHTML==grid[y][1].innerHTML)&&(grid[y][1].innerHTML==grid[y][2].innerHTML))
		{
			if(win(grid[y][0].innerHTML,1,y)){return true}
		}
	}
	if((grid[0][0].innerHTML==grid[1][1].innerHTML)&&(grid[1][1].innerHTML==grid[2][2].innerHTML))
	{
		if(win(grid[0][0].innerHTML,2,0)){return true}
	}
	if((grid[0][2].innerHTML==grid[1][1].innerHTML)&&(grid[1][1].innerHTML==grid[2][0].innerHTML))
	{
		if(win(grid[0][2].innerHTML,2,1)){return true}
	}
	return false;
}
function absclick(x,y)
{
	click(Math.floor(x/167),Math.floor(y/167));
}
function ob(t)
{
	if(t=="")
	{
		return "-";
	}
	return t;
}
function click(x,y)
{
	if(!canclick)
	{
		return false;
	}
	if(gameover)
	{
		newgame();
		return true;
	}
	if(grid[y][x].innerHTML=="")
	{
		grid[y][x].innerHTML=turn?"X":"O";
		currentgame+="\n\n"+ob(grid[0][0].innerHTML)+ob(grid[0][1].innerHTML)+ob(grid[0][2].innerHTML);
		currentgame+="\n"+ob(grid[1][0].innerHTML)+ob(grid[1][1].innerHTML)+ob(grid[1][2].innerHTML);
		currentgame+="\n"+ob(grid[2][0].innerHTML)+ob(grid[2][1].innerHTML)+ob(grid[2][2].innerHTML);
	
		grid[y][x].style.opacity="1";
		filled++;
		checkwin();
		if(gameover){return true}
		turn=!turn;
		
		if(filled==9)
		{
			gameover=true;
			return true;
			
		}
		
		if(turn&&ai)
		{
			ai_go();
		}
		
		return true;
	}
	return false;
}
function lsinc(keyn) {
	if (window.localStorage) {
		if (localStorage.getItem(keyn)) {
			localStorage.setItem(keyn,parseInt(localStorage.getItem(keyn)) + 1);
		} else {
			localStorage.setItem(keyn,1);
		}
	}
}

function think(x) {
	document.getElementById("think").innerHTML=parseInt(document.getElementById("think").innerHTML)+x;
}

function winc(x) {
	document.getElementById("win").innerHTML=parseInt(document.getElementById("win").innerHTML)+x;
	lsinc("win");
}

function losec(x) {
	document.getElementById("lose").innerHTML=parseInt(document.getElementById("lose").innerHTML)+x;
	lsinc("lost");
}

function g(x) {
return document.getElementById(x);
}

document.body.onselectstart = function () { return false; }
document.body.onmousedown = function () { return false; }  
var turn = 0;
grid.push([g("g1"),g("g2"),g("g3")]);
grid.push([g("g4"),g("g5"),g("g6")]);
grid.push([g("g7"),g("g8"),g("g9")]);

function cleargrid() {
var x,y;
for(x=0;x<3;x++){
for(y=0;y<3;y++){
grid[x][y].innerHTML="";
}
}
canclick=true;
}

function newgame() {
currentgame = "";
gameover = false;
ctx.clearRect(0,0,501,501);
filled = turn = 0;
var x,y;
canclick = false;
for (x=0;x<3;x++){for(y=0;y<3;y++){
grid[y][x].style.opacity="0";
}
}
setTimeout(cleargrid,900);
spin();
}

newgame();
</script> 
<br /><br /> 
</article>
<article>
<table style="font-size: large">
<tr><td>Games Won:</td><td><span id="win">0</span></td></tr> 
<tr><td>Games Lost:</td><td><span id="lose">0</span></td></tr> 
<tr><td>AI Thinkingness:</td><td><span id="think">0</span></td></tr> 
</table>
</article>
<script type="text/javascript"> 
if (window.localStorage) {
	var wincc = localStorage.getItem("win");
	var losecc = localStorage.getItem("lost");
	if (wincc) {
		document.getElementById("win").innerHTML = wincc;
	}

	if (losecc) {
		document.getElementById("lose").innerHTML = losecc;
	}
}
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