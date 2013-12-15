<!DOCTYPE html> 
<html lang="en" dir="ltr" xmlns="http://www.w3.org/1999/xhtml" xmlns:fb="http://www.facebook.com/2008/fbml">

<head>
<title>Homenet Spaces OS | Welcome to HnS Desktop!</title>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<meta name="author" content="Homenet Spaces Andrew Gerst" />
<meta name="copyright" content="© HnS Desktop" />
<meta name="keywords" content="Homenet, Spaces, HnS, Desktop, OS, Web, WebOS, Webtop, Online, Operating, System, Applications, Application, Apps, App, Services, Internet, The, Place, To, Be, Creative, Andrew, Gerst, Free, Profile, Profiles" />
<meta name="description" content="Welcome to Homenet Spaces OS | This is the place to be creative! Feel free to add yourself to our wonderful community by registering! HnS Desktop" />
<meta property="og:title" content="HnS Desktop" />
<meta property="og:type" content="website" />
<meta property="og:url" content="http://hnsdesktop.tk" />
<meta property="og:site_name" content="HnS Desktop" />
<meta property="fb:admins" content="637458869" />
<meta property="og:description" content="Welcome to Homenet Spaces OS | This is the place to be creative! Feel free to add yourself to our wonderful community by registering! HnS Desktop" />
<script type="text/javascript" src="jquery.js"></script>
<!--[if lt IE 9]>
<script type="text/javascript" src="html5.js"></script>
<![endif]-->
<base href="" target="_top" />
<style type="text/css">
canvas#animation_canvas {
background-color: #ffffff;
}
</style>
</head>

<body id="main" scroll="no">
<canvas id="animation_canvas"></canvas>
<script type="text/javascript">
var mouseX = 0, mouseY = 0, time_inactive = 0;

function getthedate() {
	var dayarray = new Array("Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday");
	var montharray = new Array("January","February","March","April","May","June","July","August","September","October","November","December");
	var mydate = new Date();
	var year = mydate.getYear();
	var day = mydate.getDay();
	var month = mydate.getMonth();
	var daym = mydate.getDate();
	var hours = mydate.getHours();
	var minutes = mydate.getMinutes();
	var seconds = mydate.getSeconds();
	var dn = "AM"; 

	if (year < 1000) year += 1900;
	if (daym < 10) daym = "0" + daym;
	if (hours >= 12) dn = "PM";
	if (hours > 12) hours = hours - 12;
	
	{
	d = new Date();
	Time24H = new Date();
	Time24H.setTime(d.getTime() + (d.getTimezoneOffset() * 60000) + 3600000);
	InternetTime = Math.round((Time24H.getHours() * 60 + Time24H.getMinutes()) / 1.44);
	if (InternetTime < 10) InternetTime = '00' + InternetTime;
	else if (InternetTime < 100) InternetTime = '0' + InternetTime;
	}

	if (hours == 0) hours = 12;
	if (minutes <= 9) minutes = "0" + minutes;
	if (seconds <= 9) seconds = "0" + seconds;
	var cdate = dayarray[day] + ", " + montharray[month] + " " + daym + " " + year + " | " + hours + ":" + minutes + ":" + seconds + " " + dn + " | @" + InternetTime + "";
	var mytime = hours + ":" + minutes + ":" + seconds + " " + dn;
	var mytime2 = hours + ":" + minutes + " " + dn;
	var mydate = (month + 1) + "/" + daym + "/" + year;
	var myfulldate = dayarray[day] + ", " + montharray[month] + " " + daym + ", " + year;

	time_inactive++;
	if (time_inactive == 10) {
		$("canvas#animation_canvas").css('visibility','visible');
		animation_timer = setInterval(animation, 50);
	}
}

function getdate() {
	if (document.all || document.getElementById) {
		setInterval("getthedate()", 1000);
	} else {
		getthedate();
	}
}

getdate();

if (typeof(window.innerWidth) == 'number') { // Non-IE
	var height = window.innerHeight;
	var width = window.innerWidth;
} else if (document.documentElement && (document.documentElement.clientWidth || document.documentElement.clientHeight)) { // IE 6+ in 'standards compliant mode'
	var height = document.documentElement.clientHeight;
	var width = document.documentElement.clientWidth;
} else if (document.body && (document.body.clientWidth || document.body.clientHeight)) { // IE 4 compatible
	var height = document.body.clientHeight;
	var width = document.body.clientWidth;
}

$(document.body).mousemove(function(e) {
if ((e.pageX != mouseX) || (e.pageY != mouseY)) { 
mouseX = e.pageX;
mouseY = e.pageY;
time_inactive = 0;
$("canvas#animation_canvas").css('visibility','hidden');
clearInterval(animation);
animation_ctx.fillStyle = "rgb(255,255,255)";
animation_ctx.fillRect(0, 0, animation_canvas.width, animation_canvas.height);
}
});

var animation_canvas, animation_ctx, animation_timer;
var currentX = 0, currentY = 0, lastX = 0, lastY = 0;

animation_canvas = document.getElementById('animation_canvas');

if (animation_canvas && animation_canvas.getContext) {
animation_ctx = animation_canvas.getContext('2d');

if (animation_ctx) {
animation_canvas.height = height;
animation_canvas.width = width;

lastX = (width * Math.random());
lastY = (height * Math.random());

function animation() {
animation_ctx.save();
animation_ctx.translate((width / 2), (height / 2));
animation_ctx.scale(0.9, 0.9);
animation_ctx.translate((-width / 2), (-height / 2));
animation_ctx.beginPath();
animation_ctx.lineWidth = (5 + (Math.random() * 10));
animation_ctx.moveTo(lastX, lastY);

lastX = (width * Math.random());
lastY = (height * Math.random());

animation_ctx.bezierCurveTo(
width * Math.random(),
height * Math.random(),
width * Math.random(),
height * Math.random(),
lastX, lastY);

var r = (Math.floor(Math.random() * 255) + 70);
var g = (Math.floor(Math.random() * 255) + 70);
var b = (Math.floor(Math.random() * 255) + 70);
var s = 'rgba(' + r + ',' + g + ',' + b +', 1.0)';

animation_ctx.shadowColor = 'white';
animation_ctx.shadowBlur = 10;
animation_ctx.strokeStyle = s;
animation_ctx.stroke();
animation_ctx.restore();
}
}
}

//http://ie.microsoft.com/testdrive/Graphics/CanvasPad/Default.html
</script>
</body>

</html>