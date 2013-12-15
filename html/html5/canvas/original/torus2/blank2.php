<!DOCTYPE html>
<html lang="en-AU">
<head>
<title>www.benjoffe.com | Torus</title>
<meta name="author" content="Benjamin Joffe" />
<meta name="Content-Script-Type" content="text/javascript" />
<meta name="Content-Style-Type" content="text/css" />
<style><!-- @import "main.css"; --></style>
<script type="text/javascript" src="main.js"></script>
<meta name="viewport" content="width=device-width, minimum-scale=1.0, maximum-scale=1.0" />
<link rel="alternate" type="application/x-opera-widgets" href="http://widgets.opera.com/widget/download/4196/1.2/">
<style type="text/css"><!-- @import "all.css"; --></style>
<script type="text/javascript" src="logic_min.js"></script>
</head>
<body>
<div id="benjoffe">
  <h1>Ben Joffe</h1>
  <ul id="nav">
    <li><a href="/">Home</a> 
    <li><a href="/code/">Code</a> 
    <li><a href="/contact">Contact</a> 
  </ul>
  <div id="content"> 
<div id="loading">
    You must enable JavaScript to play this game.
</div>
<script type="text/javascript"><!--
if (!window.CanvasRenderingContext2D)
{
   document.getElementById('loading').innerHTML = 'Your browser doesn\'t support canvas, if you are using IE then the <a href="v1/">older version</a> will likely work.<br>I suggest you download a standards compliant browser such as <a href="http://www.opera.com/">Opera</a>.';
}
else
{
    document.getElementById('loading').innerHTML = '<img src="img/loading.gif" alt="" style="width:16px;height:16px;vertical-align:top;"> Loading game...';
}
--></script>
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
      <a href="http://widgets.opera.com/author/Benjamin%20Joffe">More widgets by this author</a>
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
<div id="foot"><p>&copy; 2010 Benjamin Joffe</p></div></div><script type="text/javascript">var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));</script><script type="text/javascript">try {var pageTracker = _gat._getTracker("UA-7116912-1");pageTracker._trackPageview();} catch(err) {}</script>
</body>
</html>