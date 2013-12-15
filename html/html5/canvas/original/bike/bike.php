<?php
session_start();
chdir("/xampp/HomenetSpaces/hnsdesktop/");

include ("db.member.inc.php");
include ("login.inc.php");

if (isset($_SESSION['logged']) && ($_SESSION['logged'] ==  1)) { // user is logged in
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
<meta http-equiv="content-type" content ="text/html; charset=iso-8859-1, windows-1252" />
<meta name="author" content ="Homenet Spaces Andrew Gerst" />
<meta name="copyright" content="© HnS Desktop" />
<meta name="keywords" content="Homenet, Spaces, HnS, OS, Web, Desktop, The, Place, To, Be, Creative, Andrew, Gerst, Free, Profile, Profiles, Apps, Applications" />
<meta name="description" content="Welcome to Homenet Spaces OS | This is the place to be creative! Feel free to add yourself to our wonderful community by registering! HnS Desktop" />
<script type="text/javascript" src="/hnsdesktop/jquery.js"></script>
<script type="text/javascript" src="/hnsdesktop/jquery.corners.js"></script>
<base target="_top" />
</head>

<body scroll="no" onkeyup="kitd(event)" onkeydown="kit(event)">
<!-- Begin page content -->
<div id="main">
<script type="text/javascript">
document.write("<canvas id =\"c\" onmousedown=\"click = true\" style=\"background: #000; left: 0; position: absolute; top: 0\" height=\"" + window.innerHeight + "\" width=\"" + window.innerWidth + "\"><a>Your browser not compatible with this page.<br /><br />Compatible browsers include: Safari, Chrome, Firefox and Opera.</a></canvas>");
var c = document.getElementById("c"); //the canvas object
document.body.style.margin = "0px";
document.body.style.padding = "0px"; // right to the edges
document.body.style.overflow = "hidden"; //no scrollbars

var width;
var height;
var draw;
var mx = 400; //mouse x and mouse y
var my = 300;
var ctx;

window.onresize = function() { // when the window resizes, the canvas resizes
    c.width = window.innerWidth;
    c.height = window.innerHeight;
    width = c.width;
    height = c.height;
    wchanged = true;
    
    if (draw) {
	draw();
    }
}

window.onresize(); // make sure it begins at the right size

if (c.getContext) {
	ctx = c.getContext("2d");
} else {
	if (G_vmlCanvasManager) {
		G_vmlCanvasManager.initElement(c);

		if (c.getContext) {
			ctx = c.getContext("2d");
		}
	}
}
</script>
<div style="background: rgba(255,255,255,0.3); font-family: sans-serif; font-size: small; left: 0; min-height: 80px; padding: 8px; position: fixed; top: 0; width: 256px">
<table>
<tr>
<td style="width: 200px">My High Score:</td>
<td><span id="ss">0</span></td>
</tr>
<tr>
<td>Time:</td>
<td><span id="tt">00:00</span></td>
</tr>
<tr>
<td>Current Score:</td>
<td><span id="gg">0</span></td>
</tr>
</table>
Share score: <a href="javascript:void(window.location='http://twitter.com/?status='+encodeURIComponent('My score in jsCanvasBike: '+Math.round(document.getElementById('ss').innerHTML)+'. Play it here http://url2.tk/N'))"><img src="http://twitter-badges.s3.amazonaws.com/t_mini-a.png" alt="twitter" /></a>
<a href ="javascript:void(window.location='http://www.facebook.com/sharer.php?u=http%3A%2F%2Furl2.tk%2FN&t='+encodeURIComponent('My score in jsCanvasBike: '+Math.round(document.getElementById('ss').innerHTML)+'. (HTML5 Game)'))"><img src="http://static.ak.fbcdn.net/images/connect_favicon.png" alt="facebook" /></a>
<a href ="javascript:void(newgame())">New Game</a>
<input type ="checkbox" name="cc" id="cc" checked="checked" onchange="drawtexture=this.checked?true:false" />
<label for="cc" style="font-size:xx-small">tex</label>
<a id="how"><br />Do Flips to Score Points!<br />Controls:<br />Speed UP/DOWN<br />Lean: LEFT/RIGHT</a>
<script type="text/javascript">
setTimeout('document.getElementById("how").innerHTML = "";', 30000);
</script>
<div style="float: right; font-size: 8pt">
<a href="http://url3.tk/?p=bike" target="_blank">Info</a> | <a href="http://url3.tk/?p=code" target="_blank">More JS</a>
</div> 
</div>
<script>
function map(x) {
	return 0.04*x-130*Math.sin(0.003*x)+180+Math.sin(0.001*x)*120+Math.cos(0.0005*x)*300+Math.sin(x*0.003)*80+Math.sin(x*0.0001)*400+Math.sin(x*0.008)*30-Math.sin(x*0.0082)*20;
}

var camx = 0;
var camy = 0;
var opx = 110;

function opy() {
	if (false) {
		return (2*height)-map(px)-85;
	}

	return height-map(px)-85;
}

var px =  opx;
var py =  opy();
var vx =  0;
var drawtexture =  true;
var vy =  0;
var wheelradius =  40;
var rwx =  49;
var lwx =- 57;
var whel =  10;
var opr =  0.01;
var pr =  opr;
var zoom = 1;
var tq = 0;
var w = 0;
var bike = new Image();
var cloud1 = new Image();
var text = new Image();
var imgloaded = 0;
var ready = false;

function imageload() {
imgloaded++;

if (imgloaded >=  2) {
    if (startup) {
        startup();
    } else {
        ready = true;
    }
}
}

text.onload = imageload;
bike.onload = imageload;
text.src = "text.png";
var touchingl = 0;
var touchingr = 0;
var msperframe = 33;

function applyforce(rx,ry,fx,fy,dt) {
vx +=  fx*dt;
vy +=  fy*dt;
w +=  dt*((rx*fy)-(ry*fx))/3060.0;
}

var clouds = [];
var i;

for(i = 0; i < 8; i++) {
var clox = (Math.random()*(1200+300))-300;
clouds.push([clox,Math.random()*height]);
}

bike.src = "bike.png";

//cloud1.src ="cloud1.png";
var speed = 0;
var keys = new Array(4096);
var keycode = {"up":38,"down":40,"left":37,"right":39,"shift":16};

function kit(e) {
	keys[e.which] = 1;
}

function kitd(e) {
	keys[e.which] = 0;
}

var showtime = 0;
var stxt = "Score!";
var lastt = new Date();

function score(n) {
	
	showtime = 100.0;
	stxt = "";
	docuhtmlpage += n;
	if(n == 2)
	{
		stxt += "Double ";
	}
	else if(n == 3)
	{
	
		stxt += "Triple ";
	}
	else if(n>= 4)
	{
	
		stxt += "Super ";
	}
	
	stxt += "Flip";
}

var eeee = 0;
var lastflip = pr;
var locked = false;
var debug;
var scale;
var docuhtmlpage = 0;
var tt;
var gg;
var ss;
var maxscore = 0;
var timeleft;
var awidth,aheight;
var rtt = 0;
function td(x)
{
if(x<10)
{
return "0" + x;

}
return x;
}
function updatebox()
{
	gg.innerHTML = docuhtmlpage;
	ss.innerHTML = maxscore;
	tt.innerHTML = td(Math.floor(timeleft/60000)) + ":" + td(Math.floor((timeleft/1000)%60));
if(docuhtmlpage>maxscore)
{
	maxscore = docuhtmlpage;
	document.cookie = "bikescore =" + maxscore;
}

}
function newgame()
{
speed = 0;
if(docuhtmlpage>maxscore)
{
	maxscore = docuhtmlpage;
	document.cookie = "bikescore =" + maxscore;
}
		px = opx;
		py = opy();
		pr = opr;
		
lastflip = pr;
		vx = 0;
		vy = 0;
		w = 0;
		
docuhtmlpage = 0;
timeleft = 120000;
died = false;
updatebox();

}

	var died = false;
function nextframe()
{
	
	timeleft -= msperframe;
	if(timeleft <= 0)
	{
		died = true;
		stxt = "Out of time!";
		newgame();	
	}
	var dt = ((new Date())-lastt)/35;
	lastt = new Date();
	debug = dt;
	dt = 1;
	
	scale = 1.0;
	if(false)
	{
		scale = 0.5;
	}

	awidth = width/scale;
	aheight = height/scale;
	camx += (px-camx-(awidth/3))/3.0;
	camy += (py-camy-(aheight/2))/4.0;
	//ctx.save();
	
	

	var lwy = whel;
	var wheelx = Math.cos(pr)*lwx-Math.sin(pr)*lwy;
	var wheely = Math.sin(pr)*lwx+Math.cos(pr)*lwy;
	
	var ax = wheelx+px;
	var ay = wheely+py+20;
	
	var lx = px;
	var ly = py;
	touchingl--;
	touchingr--;
	if(touchingl < 0)
	{
		touchingl = 0;
	}
	
	if(touchingr < 0)
	{
		touchingr = 0;
	}
	var fl = aheight-map(ax);
	var letmove = true;
	var cor = 0.01;
	if(ay >= (fl))
	{
		touchingl = 5;
		
		var nvy = 0-Math.abs(vy*cor);
		var nvx = vx*0.99;
		
		var Ix = 0.7*(nvx-vx);
		var Iy = 0.7*(nvy-vy);
		applyforce(wheelx,wheely,Ix/dt,Iy/dt,dt);
		vy = nvy;
		vx = nvx;
		vy += ((fl-wheely-20)-(py))/10.0;
		py = fl-wheely-19;
		w *= 0.9;
	}
	
	
	
	wheelx = Math.cos(pr)*-16-Math.sin(pr)*-35;
	wheely = Math.sin(pr)*-16+Math.cos(pr)*-35;
	
	ax = wheelx+px;
	ay = wheely+py;
	
	var fl = aheight-map(ax);
	if(ay >= (fl))
	{
		
		died = true;
		newgame();
		stxt = "Game Over!";
		
	}
	
	if(!died)
	{
	
	
	
	wheelx = Math.cos(pr)*rwx-Math.sin(pr)*lwy;
	wheely = Math.sin(pr)*rwx+Math.cos(pr)*lwy;
	
	ax = wheelx+px;
	ay = wheely+py+20;
	
	var fl = aheight-map(ax);
	if(ay >= (fl))
	{
		touchingr = 5;
		
		var nvy = 0-Math.abs(vy*cor);
		var nvx = vx*0.99;
		
		var Ix = 0.7*(nvx-vx);
		var Iy = 0.7*(nvy-vy);
		applyforce(wheelx,wheely,Ix/dt,Iy/dt,dt);
		vy = nvy;
		vx = nvx;
		vy += ((fl-wheely-20)-(py))/100.0;
		//vy*= 0.8;
		//py = fl-wheely-20;
		py = fl-wheely-19;
		w *= 0.9;
		
	}
	
	
	
	
	
	
	
	if((touchingl+touchingr)>0)
	{
	if(Math.abs(pr-lastflip)>12.12477)
	{
		score(4);
	}
	else if(Math.abs(pr-lastflip)>8.98318)
	{
		score(3);
	}
	else if(Math.abs(pr-lastflip)>5.841)
	{
		score(2);
	}
	
	else if(Math.abs(pr-lastflip)>2.7)
	{
		score(1);
	}
	lastflip = pr;
	if(keys[keycode.down])
	{
        if(touchingl+touchingr>0)
        {
            speed *= 0.2;
        }
        vx *= 0.9;
		w += 0.0003*vx*dt;
	}
	
	}
	
	if(touchingl>0)
	{
		if(keys[keycode.up])
	{
		speed += 0.01;
		w -= 0.005*dt;
		
	}
	speed *= Math.pow(0.99,dt);
	var fy = (map(ax+0.1)-map(ax-0.1))/(0.2);
	fx = 1;
	var fmag = Math.sqrt((fy*fy)+(fx*fx));
	fy /= fmag;
	fx /= fmag;
	fy *= speed;
	fx *= speed;
	
	applyforce(wheelx,wheely,fx,fy,dt);
	
	
	}
	
	
	if(keys[keycode.left])
	{
		if(w>0)
		{w *= Math.pow(0.7,dt);}
		w -= 0.008*dt;
	
	}
	else if(keys[keycode.right])
	{
		if(w<0)
		{w *= Math.pow(0.7,dt);}
		w += 0.008*dt;
	
	}
	w *= Math.pow(0.999,dt);
	var mass = 150;
	var g = 9.8000;
	//F =  m.g
	
	
	
	
	
	
	vy+= 0.7*dt;
	
	var friction = 0.00001*((vx*vx)+(vy*vy));
	vx+=-(friction*vx);
	vy+=-(friction*vy);
	px+= vx*dt;
	py+= vy*dt;
	

	pr+= w*dt;
	
	}
    if(!locked)
    {
        locked = true;
        draw();
        locked = false;
    }

}
function draw()
{
    
    showtime--;
	
	ctx.clearRect(0,0,width,height);
	
	ctx.scale(scale,scale);
	ctx.translate(-camx,-camy);
    ctx.fillStyle = "#9f6b3d";
	ctx.fillRect(camx,camy,awidth,aheight);
	var sx = camx-(camx%256)-256;
	var sy = camy-(camy%256)-256;
	
	var xi;
	var yi;
    if(drawtexture)
    {
	for(xi = 0;xi-2<= Math.floor(awidth/256);xi++)
	{
		for(yi = 0;yi-1<= Math.floor(aheight/256);yi++)
		{
			ctx.drawImage(text,sx+256*xi,sy+256*yi);
		}
	}
	}
    
	var i,b;
    
	ctx.lineWidth = 9;
	ctx.strokeStyle ="#773300";
    ctx.fillStyle ="#b8deea";
	ctx.beginPath();
	ctx.moveTo(camx-20,camy-30);
	for(i = camx-20;i<= (camx+awidth)+20;i+= 15)
	{
		ctx.lineTo(i,aheight-map(i));
	}
	ctx.lineTo((camx+awidth)+20,camy-30);
	ctx.closePath();
	ctx.fill();
	ctx.stroke();
   	ctx.translate(px, py);
	ctx.rotate(pr);
	ctx.drawImage(bike,-80,-50);
	ctx.rotate(-pr);
    ctx.translate(-px, -py);
	ctx.translate(camx,camy);
    ctx.scale(scale,1.0/scale);
	if(died)
	{
		showtime = 100;
	}
	if(showtime>0 || 1)
	{
        
        if(ctx.fillText)
        {
		ctx.fillStyle = "rgba(255,255,255,"+(showtime/100.0)+")";
		//ctx.save();
		rtt+= (showtime-rtt)/10.0;
		ctx.rotate((100.0-rtt)/300.0);
		if(scale>0.6)
		{
		
    		ctx.font = "72pt sans-serif";
		}
		else
		{
			ctx.font = "24pt sans-serif";
    	}
        ctx.fillText(stxt,width/2,height/2);
        
        ctx.rotate(-(100.0-rtt)/300.0);
        }
	}
	else
	{
		showtime = 0;
	}
    
    updatebox();
    
}

	function startup()
	{
	tt = document.getElementById("tt");
	gg = document.getElementById("gg");
	ss = document.getElementById("ss");
	var mff = /bikescore = \d+/.exec(document.cookie);
	if(mff)
	{
		maxscore = mff[0].substring(10);
	}
	newgame();
	setInterval(nextframe,msperframe);
	}

if (ready) {
	startup();
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