<!DOCTYPE html>
<html lang="en" dir="ltr">

<head>
<title>Homenet Spaces OS | HTML5 Canvas!</title>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1, windows-1252" />
<meta name="author" content="Homenet Spaces Andrew Gerst" />
<meta name="copyright" content="© HnS Desktop" />
<meta name="keywords" content="Homenet, Spaces, HnS, OS, Web, Desktop, The, Place, To, Be, Creative, Andrew, Gerst, Free, Profile, Profiles, Apps, Applications" />
<meta name="description" content="Welcome to Homenet Spaces OS | This is the place to be creative! Feel free to add yourself to our wonderful community by registering! HnS Desktop" />

<style type="text/css"><!-- @import "all2.css"; --></style>
<script type="text/javascript" src="logic_min2.js"></script>
<base target="_top" />
</head>

<body scroll="no">
<!-- Begin page content -->
<div id="main">
<div id="content"> 
<div id="loading">
You must enable JavaScript to play this game.
</div>
<script type="text/javascript">
if (!window.CanvasRenderingContext2D) {
	document.getElementById('loading').innerHTML = 'Your browser doesn\'t support canvas, if you are using IE then the <a href="v1/">older version</a> will likely work.<br>I suggest you download a standards compliant browser such as <a href="http://www.opera.com/">Opera</a>.';
} else {
	document.getElementById('loading').innerHTML = '<img src="loading.gif" alt="" style="width:16px;height:16px;vertical-align:top;"> Loading game...';
}
</script>
<div id="container">
<div id="menu">
<div id="menu_area">
<div id="screen0">
<div id="helpBox">
<P><b>How To Play</b><br>
Use the arrow keys to guide the falling blocks, spacebar and the up-arrow will rotate the block in opposite directions.
Alternatively the keys A,W,S and D can be used as arrow keys for the same purpose.<br><br>
When a complete horizontal ring of 15 blocks is occupied by pieces the row will collapse and earn 100 points.
If multiple rows are cleared at a time you will earn bonus points.
</P>
</div>
<div id="copy">&copy;2010 <a href="http://www.benjoffe.com/">Benjamin Joffe</a></div>
<div id="but_main0" class="but"></div>
</div>
<div id="screen1">
<div id="go1" class="but"></div>
<div id="go2" class="nonstick"></div>
<div id="go3" class="nonstick"></div>
<div id="but_main1" class="but"></div>
</div>
<div id="screen2">
<div id="quote"></div>
<div id="but_play" class="but"></div>
<div id="but_settings" class="but"></div>
<div id="but_high" class="but"></div>
<div id="but_help" class="but"></div>
</div>
<div id="screen3">
<select id="bestType">
<option>Traditional</option>
<option>Time Attack</option>
<option>Garbage</option>
</select>
<div id="best1"></div>
<div id="best2"></div>
<div id="best3"></div>
<div id="but_main2" class="but"></div>
</div>
<div id="screen4">
<div id="div_base">
<label>Skin:
<select id="set_base">
<option>Glass</option>
<option>Storybook</option>
</select>
</label>
</div>
<div id="div_ghost">
<label><input id="set_ghost" type="checkbox"> Show ghost</label>
</div>
<div id="but_main3" class="but"></div>
</div>
</div>	
</div>
<div id="out"></div>
<div id="playing">
<canvas id="canvas" width="200" height="400"></canvas>
<div id="paused">
<div id="but_resume" class="but"></div>
<div id="but_restart" class="but"></div>
<div id="but_quit" class="but"></div>
</div>
<div id="panel">
<div id="title1"></div>
<div id="title2"></div>
<div id="title3"></div>
<div id="score"></div>
<div id="time"></div>
<div id="next"></div>
<div id="but_pause" class="but"></div>
</div>
<div id="gameover">
<div id="winner">
<form id="high_form">
You have achieved a high score, please enter your name:<br>
<input type="text" id="high_name" maxlength="20"> <input type="submit" value="OK">
</form>
</div>
<div id="newgame">
<div id="sorryText"></div>
<div id="skull"></div>
<div id="but_restart2" class="but"></div>
<div id="but_main4" class="but"></div>
</div>
</div>
</div>
<div id="close" class="nonstick"></div>
</div>
</div>
</div>
<!-- End page content -->
</body>

</html>