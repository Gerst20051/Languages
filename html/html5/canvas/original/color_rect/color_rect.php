<html>

<head>
<script type="application/x-javascript">
function displayCanvas() {
	var canvas = document.getElementById("myCanvas");
	
	if (canvas.getContext) {
        var ctx = canvas.getContext("2d");

        ctx.fillStyle = "rgb(200,0,0)";
        ctx.fillRect (0,0,150,75);

        ctx.fillStyle = "rgba(0, 0, 200, 0.5)";
        ctx.fillRect (40,30,125,75);

        ctx.fillStyle = "rgb(0,0,150)";
        ctx.strokeRect (20,20,50,100);
	}
}
</script>
</head>

<body onload="displayCanvas();">
<canvas id="myCanvas" width="300" height="200">
<p>Your browser does not support the canvas tag. At the time of writing, Firefox, Opera, and Chrome support this tag.</p> 
<p>Here's an <a href="/pix/html_5/tags/html_canvas_tag.gif">image of what it's supposed to look like</a>.</p>
</canvas>
</body>

</html>