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
<meta name="copyright" content="� HnS Desktop" />
<meta name="keywords" content="Homenet, Spaces, HnS, OS, Web, Desktop, The, Place, To, Be, Creative, Andrew, Gerst, Free, Profile, Profiles, Apps, Applications" />
<meta name="description" content="Javascript + Canvas + SoundManager 2: 360-degree circular control / jog wheel example for playing MP3 links, with EQ and spectrum visualization options" /> 
<script type="text/javascript" src="/hnsdesktop/jquery.js"></script>
<script type="text/javascript" src="/hnsdesktop/jquery.corners.js"></script>
<base target="_top" />
</head>

<body scroll="no">
<!-- Begin page content -->
<div id="main">
<!-- demo, make the fonts nicer etc. --> 
<link rel="stylesheet" type="text/css" href="index.css" /> 
 
<!-- soundManager.useFlashBlock: related CSS --> 
<link rel="stylesheet" type="text/css" href="flashblock.css" /> 
 
<!-- required --> 
<link rel="stylesheet" type="text/css" href="360player.css" /> 
<link rel="stylesheet" type="text/css" href="360player-visualization.css" />

<!-- special IE-only canvas fix --> 
<!--[if IE]><script type="text/javascript" src="excanvas.js"></script><![endif]--> 

<!-- Apache-licensed animation library --> 
<script type="text/javascript" src="berniecode-animator.js"></script> 
 
<!-- the core stuff --> 
<script type="text/javascript" src="soundmanager2.js"></script> 
<script type="text/javascript" src="360player.js"></script> 
<script type="text/javascript">
soundManager.url = ''; // path to directory containing SM2 SWF
 
soundManager.useFastPolling = true; // increased JS callback frequency, combined with useHighPerformance = true
 
threeSixtyPlayer.config.scaleFont = (navigator.userAgent.match(/msie/i)?false:true);
threeSixtyPlayer.config.showHMSTime = true;
 
// enable some spectrum stuffs
 
threeSixtyPlayer.config.useWaveformData = true;
threeSixtyPlayer.config.useEQData = true;
 
// enable this in SM2 as well, as needed
 
if (threeSixtyPlayer.config.useWaveformData) {
  soundManager.flash9Options.useWaveformData = true;
}
if (threeSixtyPlayer.config.useEQData) {
  soundManager.flash9Options.useEQData = true;
}
if (threeSixtyPlayer.config.usePeakData) {
  soundManager.flash9Options.usePeakData = true;
}
 
// favicon is expensive CPU-wise, but can be used.
threeSixtyPlayer.config.useFavIcon = false;
</script>

<!-- DEMO only, customization UI: Yahoo! YUI colorpicker stuff --> 
<link rel="stylesheet" type="text/css" href="slider.css" /> 
<link rel="stylesheet" type="text/css" href="colorpicker.css" /> 
<script type="text/javascript" src="yahoo-dom-event.js"></script> 
<script type="text/javascript" src="animation-min.js"></script> 
<script type="text/javascript" src="dragdrop-min.js"></script> 
<script type="text/javascript" src="slider-min.js"></script> 
<script type="text/javascript" src="element-min.js"></script> 
<script type="text/javascript" src="colorpicker-min.js"></script> 
<link rel="stylesheet" type="text/css" href="demo.css" /> 
<style type="text/css">
#left h2 {
 padding-top:0px;
 margin-bottom:0.25em;
 color:#666;
}
 
pre.block {
 margin-top:0.5em;
}
 
/* special case */
 
#left {
 width:auto;
 max-width:100%;
}
</style> 

<!-- demo configuration stuff, you don't need this --> 
<link rel="stylesheet" type="text/css" href="demo-slider-controls.css" /> 
<script type="text/javascript" src="demo-slider-controls.js"></script> 
</head> 

<body>
<div id="left"> 
 
 <h1><a href="http://www.schillmania.com/projects/soundmanager2/" title="Javascript MP3 player project home">SoundManager 2</a> / <a href="http://schillmania.com/projects/soundmanager2/demo/360-player/">360&deg; Player Demo</a>: JS + Canvas Visualization</h1> 
 
 <p class="note" style="color:#666;margin-bottom:0.5em">Canvas-based UI with visualization options. <b>Note: Beta-ish code. Spectrum/EQ visualizations disabled for IE due to low performance. Data is not currently available under HTML5.</b></p> 
 <p class="note" style="color:#666;margin-bottom:0.5em">You can also <a href="#debug=1" onclick="window.location.href = this.href;window.location.reload()" title="Enable debug mode, show frames per second">show FPS</a><span id="config-link"> or <a href="#customize" onclick="window.location.href=this.href;window.location.reload()">customize the UI</a></span><span id="hifi">, or see the <a href="#hifi=1" onclick="window.location.href=this.href;window.location.reload()">hi-fi version</a></span>. Check the <a href="canvas-visualization-basic.html">basic template</a> for a minimal code example; also see the <a href="../360-player/" title="360&deg; player UI">default 360&deg; UI</a>.</p> 
 
	<!-- customization crap --> 
 
	<div id="config-ui" style="clear:both;position:relative;max-width:1110px;margin-top:1em;display:none"> 
 
		<div style="position:relative"> 
 
			<form action="#" method="get"> 
 
				<div id="controls"> 
 
			  <div class="checkbox"> 
			   <div> 
			    <input id="use-waveform" type="checkbox" checked="checked" title="Enable waveform feature." onclick="controller.updateExample();controller.updateExampleCode()" value="Apply" /> Waveform
			    <input id="disabled-1" type="checkbox" title="Enable EQ (spectrum) feature." onclick="controller.updateExample();controller.updateExampleCode()" value="Apply" style="margin-left:1em" checked="checked" /> EQ
			    <input type="checkbox" name="use-amplifier" id="use-amplifier" checked="checked" onclick="controller.updateExample();controller.updateExampleCode()" style="margin-left:1em"> Amplifier
			   </div> 
			  </div> 
 
			  <div style="clear:left"></div> 
 
			  <dl class="col"> 
 
			   <dt>Circle Diameter</dt> 
			   <dd class="title">Size</dd> 
			   <dd>1-256</dd> 
			   <dd class="control"> 
			    <div class="bar" title="Click to move here"></div> 
			    <div class="slider" title="Size of circle"></div> 
			   </dd> 
 
			   <dt>Waveform Thickness</dt> 
			   <dd class="title">thickness</dd> 
			   <dd>1-100</dd> 
			   <dd class="control"> 
			    <div class="bar" title="Click to move here"></div> 
			    <div class="slider" title="Thickness of line"></div> 
			   </dd> 
 
			   <dt>Wave Downsample</dt> 
			   <dd class="title">(Scale)</dd> 
			   <dd>1-16</dd> 
			   <dd class="control"> 
			    <div class="bar" title="Click to move here"></div> 
			    <div class="slider" title="Primary (inner circle) colour"></div> 
			   </dd> 
 
			   <dt>EQ Thickness</dt> 
			   <dd class="title">thickness</dd> 
			   <dd>1-50</dd> 
			   <dd class="control"> 
			    <div class="bar" title="Click to move here"></div> 
			    <div class="slider" title="Thickness of line"></div> 
			   </dd> 
 
			   <dt>EQ Downsample</dt> 
			   <dd class="title">(Scale)</dd> 
			   <dd>1-16</dd> 
			   <dd class="control"> 
			    <div class="bar" title="Click to move here"></div> 
			    <div class="slider" title="Primary (inner circle) colour"></div> 
			   </dd> 
 
			  </dl> 
 
			  <div id="options" class="col"> 
 
			   <div> 
				Waveform position:
			    <input type="radio" name="waveform-inside" id="waveform-inside" value="true" checked="checked" onclick="controller.updateExample();controller.updateExampleCode()"> Inside | <input type="radio" name="waveform-inside" id="waveform-inside" value="false" onclick="controller.updateExample();controller.updateExampleCode()"> Outside
			   </div> 
 
			   <div> 
				EQ position:
			    <input type="radio" name="eq-inside" id="eq-inside" value="true" onclick="controller.updateExample();controller.updateExampleCode()"> Inside | <input type="radio" name="eq-inside" id="eq-inside" value="false" checked="checked" onclick="controller.updateExample();controller.updateExampleCode()"> Outside
			   </div> 
 
			   <div> 
				Waveform color:
				<input type="text" name="waveform-color" id="waveform-color" value="#000000" onclick="createCP(this,setWaveformColor)" /> 
			   </div> 
 
			   <div> 
				EQ color:
				<input type="text" name="eq-color" id="eq-color" value="#000000" onclick="createCP(this,setEQColor)" /> 
			   </div> 
 
			   <div> 
				Loaded ring color:
				<input type="text" name="loaded-ring-color" id="loaded-ring-color" value="#000000" onclick="createCP(this,setLoadedRingColor)" /> 
			   </div> 
 
			   <div> 
				Progress ring color:
				<input type="text" name="progress-ring-color" id="progress-ring-color" value="#000000" onclick="createCP(this,setProgressRingColor)" /> 
			   </div> 
 
			   <div> 
				Background ring color:
				<input type="text" name="bg-ring-color" id="bg-ring-color" value="#000000" onclick="createCP(this,setBackgroundRingColor)" /> 
			   </div> 
		
			   <p class="compact"> 
			    <input type="button" onclick="controller.randomize()" value="Randomize controls" title="Assign random control values" style="font-size:x-small" /> 
			   </p> 
 
			</form> 
	
		  </div> 
 
		  <div id="cp-container"> 
		  <!-- color picker stuff goes here --> 
		  </div> 
	
		  <div id="config-code-block" style="float:right;display:inline;margin-left:1em;margin-top:-0.7em"> 
			<!--
			<pre id="config-link" class="block"><code style="cursor:pointer" onclick="document.getElementById('config-link').style.display='none';document.getElementById('config-pre-block').style.display='block';return false"> [click to show code]                 </code></pre>
		--> 
	<pre id="config-pre-block" class="block"><code id="config-code">Code goes here</code></pre> 
		  </div> 
 
		 </div> 
 
		</div> 
		
		<p style="clear:left">Get a sound playing, then adjust the values to see real-time updates.</p> 
 
	</div> 
 
		<!-- here are the MP3 links, which are decorated with the 360 canvas element/UI etc. --> 
		
		<div id="sm2-container"> 
			<!-- sm2 flash goes here --> 
		</div> 
 
	 	<h2 style="margin-top:1em">Inline list</h2> 
 
		<div class="sm2-inline-list"> <!-- remove this class to have one item per line --> 
			
		<div class="ui360"><a href="http://freshly-ground.com/data/audio/mpc/20060826%20-%20Armstrong.mp3">20060826 - Armstrong Groove</a></div> 
		<div class="ui360"><a href="http://freshly-ground.com/data/audio/mpc/20090119%20-%20Untitled%20Groove.mp3">20090119 - Untitled Groove</a></div> 
		<div class="ui360"><a href="../../demo/_mp3/sine,%20square,%20sawtooth,%20rando.mp3">Sine, Square, Sawtooth, Wave (Warning: <span style="color:#ff3333">LOUD</span>)</a></div> 
	     	<div class="ui360"><a href="http://freshly-ground.com/data/audio/mpc/20090207%20-%20Loverman.mp3">20090207 - Loverman</a></div> 
 
	    	</div> 
 
	 	<h2 style="clear:left;padding-top:1em">Block list</h2> 
 
		<div> 
			<div class="ui360"><a href="http://www.freshly-ground.com/data/audio/binaural/Mak.mp3">Angry cow sound?</a></div> 
		 	<div class="ui360"><a href="../../demo/_mp3/1hz-10khz-sweep.mp3">1-10 Khz Sweep (Warning: <span style="color:#ff3333">LOUD</span>)</a></div> 
		</div> 
		
 
		<div style="clear:left"></div> 
		
		<h2 style="clear:left;padding-top:1em">Variant: Annotations/meta-data</h2> 
 
 
		 <ul class="ui360playlist"> 
 
		  <li> 
 
		   <div class="ui360"> 
		    <a href="http://freshly-ground.com/data/audio/binaural/A%20Virtual%20Haircut%20in%20San%20Francisco%20%283%20Scenes%29.mp3">A Virtual Haircut (3 scenes)</a> 
		    <div class="metadata"> 
		     <div class="duration">4:43</div> <!-- total track time (for positioning while loading, until determined --> 
		     <ul> 
		      <li><p>Electric razor</p><span>0:00</span></li> <!-- first scene --> 
		      <li><p>Water, scissors</p><span>2:41</span></li> <!-- start time of second scene --> 
		      <li><p>More razor work</p><span>4:00</span></li> 
		     </ul> 
		    </div> 
		   </div> 
 
		  </li> 
 
		  <li> 
 
		   <div class="ui360"> 
		    <a href="http://freshly-ground.com/data/audio/binaural/Rubber%20Chicken%20Launch%20%28office%29.mp3">Rubber Chicken Launch (Office)</a> 
 
			   <div class="metadata"> 
			    <div class="duration">0:47</div> 
			    <ul> 
			     <li><p>First attempt</p><span>0:00</span></li> 
			     <li><p>Fire!</p><span>0:02</span></li> 
			     <li><p>"Too much angle"</p><span>0:05</span></li> 
			     <li><p>Random chicken noise</p><span>0:18</span></li> 
			     <li><p>"Wait a second"</p><span>0:31</span></li> 
			     <li><p>Derrr..</p><span>0:34</span></li> 
			     <li><p>Launch attempt #2</p><span>0:36</span></li> 
			     <li><p>"Wrong angle"</p><span>0:39</span></li> 
			     <li><p>"Fail"</p><span>0:42</span></li> 
			    </ul> 
			   </div> 
 
 
		   </div> 
 
		  </li> 
 
		 </ul> 
 
	<div style="clear:both"></div>	
	<script type="text/javascript"> 
	  if (window.location.toString().match(/#customize/i)) {
		document.getElementById('config-link').style.display = 'none';
	    document.getElementById('config-ui').style.display = 'block';	
	  }
	</script> 
</div> 
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