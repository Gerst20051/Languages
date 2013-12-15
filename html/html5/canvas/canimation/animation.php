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
</head>

<body id="main" scroll="no">
<canvas id="animation_canvas"></canvas>
<script type="text/javascript">
var animation_canvas, animation_ctx, animation_timer;
var currentX = 0, currentY = 0;

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

animation_canvas = document.getElementById('animation_canvas');
animation_ctx = animation_canvas.getContext('2d');
animation_canvas.width = width;
animation_canvas.height = height;

var lastX = (width * Math.random());
var lastY = (height * Math.random());

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
 
animation_timer = setInterval(animation, 50);

//http://ie.microsoft.com/testdrive/Graphics/CanvasPad/Default.html
</script>
</body>

</html>