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
<meta http-equiv="X-UA-Compatible" content="chrome=1" />
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<meta name="viewport" content="width=device-width; height=device-height; initial-scale=1.0; maximum-scale=1.0; user-scalable=false;" />
<meta name="apple-mobile-web-app-capable" content="yes" />
<meta name="apple-mobile-web-app-status-bar-style" content="black-translucent" />
<meta name="author" content="Homenet Spaces Andrew Gerst" />
<meta name="copyright" content="© HnS Desktop" />
<meta name="keywords" content="Homenet, Spaces, HnS, OS, Web, Desktop, The, Place, To, Be, Creative, Andrew, Gerst, Free, Profile, Profiles, Apps, Applications" />
<meta name="description" content="A Gravitational Attraction Particle Engine n-body simulator in Javascript. Uses HTML 5 canvas for 2D rendering. The particles are attracted to the mouse." />
<link rel="icon" type="image/vnd.microsoft.icon" href="favicon.ico" />
<script type="text/javascript" src="/hnsdesktop/jquery.js"></script>
<script type="text/javascript" src="/hnsdesktop/jquery.corners.js"></script>
<base target="_top" />
<style type="text/css">
*{-webkit-tap-higlight-color:transparent;padding:0px;margin:0px; font-family:sans-serif; color:#dddddd; font-size:x-small;overflow:hidden;}
input{font-size:small;color:black;}
input.z{border: 1px solid #cccccc;-moz-border-radius:2px;-webkit-border-radius:2px;}
#space{background:black;}
#btm:hover{opacity:1.0}
#btm{-webkit-transition: opacity 0.1s linear;opacity:0.1;position:fixed;left:0;right:0;bottom:0;padding-top:2px;padding-bottom:2px;color:white;}
</style>
<script type="text/javascript">
var height,height,onstop;
var mstr=1;var cspeed=1;var mousetouch=1;
var mouseone=1;var locktrail=1;
function g(p){return document.getElementById(p);}function again(){cspeed=(g("sm").value);mstr=(g("ms").value);setup(g("ss").value);nF();}
function stopall(){stop=1;onstop=null}function sfa(){stop=0; again()}function chgsett(){if(running){stop=1;onstop=sfa;}else{sfa();}}
function resaa(){height=window.innerHeight;width=window.innerWidth;var x=g("space");x.width=width;x.height=height;}var j,vx,vy,px,py,ci,ly,lx,max;var i=0;var running=0;var nbody_enable=1;var sch=0;
function next_scheme(){if((++sch)>=6){sch=0;}}function mr(){
return Math.random();}
var elements,mmx,mmy,beginn;var box=0;var vmax=0;var spf=0.01;
var cq=[["#ff0000","#ff6000","#ff9c00","#ffde00","#fffae3","#ffc257"],["#33cccc","#9bf2ff","#6ec0e6","#3075b6","#99c2eb","#bfe9ff"],["#efffbf","#e6ffbf","#71bf25","#48ff00","#78d47c","#81dcd4"],["#fff","#eee","#ddd","#ccc","#bbb","#aaa"],["#bbffab","#ffffdc","#ffe7dc","#dcf7ff","#dabbf7","#f5bbf7"],["#8b37a6","#7d021f","#3a99bc","#d4ffb2","#ff5a00","#ffffff"]];
function nar(m){
return new Array(m)}
function setup(mm){
max=mm;vx=new Array(max);vy=nar(max);px=nar(max);py=nar(max);lx=nar(max);ly=nar(max);ci=nar(max);mmx=width/2;mmy=height/2;vmax=0;
for(i=0;i<max;i++){vx[i]=(mr()*vmax)-(vmax/2);vy[i]=(mr()*vmax)-(0.5*vmax);px[i]=mr()*width;py[i]=mr()*height;ci[i]=~~(mr()*6);}
beginn=new Date().getMilliseconds();
if(canvas.getContext){
var n=canvas.getContext("2d");n.fillStyle="rgba(0,5,10,255)";n.fillRect(0,0,width,height);
}else{
alert("Your browser does not support the canvas tag");}}
function m(e){mmx=e.clientX;mmy=e.clientY;}var f=0;var checkfps=30;var maxv=5;var mdown=0;var stop=0;var sent=0;var spfset=0;var times=0;var mup=0;var debug=1;
function nF(){running=1;f++;if(f==checkfps){var elap=new Date().getTime()-beginn;if(elap<3000){times++;
var na=Math.round(checkfps*1000/(elap));if(max==100 && times>=3 && !sent){senddata(na);sent=1;}
if(!spfset){spf=1/(checkfps*1000/(elap));spfset=1;}}f=0;beginn=new Date().getTime();}var speed=160*cspeed*spf/max;if(!canvas.getContext){return 0;}var ctx=canvas.getContext("2d");var t=2.0/255.0;if(mdown||locktrail){ctx.fillStyle="rgba(0,0,0,"+t+")";ctx.fillRect(0,0,width,height);}else{ctx.fillStyle="rgba(0,0,0,"+0.1+")";ctx.fillRect(0,0,width,height);}for(i=0;i<max;i++){var b;if(nbody_enable){for(b=i+1;b<max;b++){var str=speed;var rx=px[b]-px[i];var ry=py[b]-py[i];var r2=(rx*rx)+(ry*ry);var rtpow=Math.pow(r2+0.1,-1.5);vx[i]+=rx*str*rtpow;vy[i]+=ry*str*rtpow;vx[b]-=rx*str*rtpow;vy[b]-=ry*str*rtpow;}}if(mouseone&&mousetouch){rx=mmx-px[i];ry=mmy-py[i];str=speed*max*mstr;var magrr=(rx*rx)+(ry*ry);if(!magrr){magrr=1e23;}var magr=Math.sqrt(magrr);var resX=rx/magr;var resY=ry/magr;vx[i]+=resX*str/magrr;vy[i]+=resY*str/magrr;}if(box){if(vx[i]>maxv){vx[i]=maxv;}if(vx[i]<-maxv){vx[i]=-maxv;}if(vy[i]>maxv){vy[i]=maxv;}if(vy[i]<-maxv){vy[i]=-maxv;}}else{var sp=(vx[i]*vx[i])+(vy[i]*vy[i]);if(sp>maxv*maxv){var len=Math.sqrt(sp);vx[i]*=maxv/len;vy[i]*=maxv/len;}}vx[i]*=0.999;vy[i]*=0.999;ly[i]=py[i];lx[i]=px[i];px[i]+=vx[i];py[i]+=vy[i];var jmp=0;if(px[i]<0){px[i]+=width;jmp=1;}if(px[i]>width){px[i]-=width;jmp=1;}if(py[i]<0){py[i]+=height;jmp=1;}if(py[i]>height){py[i]-=height;jmp=1;}if(!jmp){ctx.strokeStyle=cq[sch][ci[i]];ctx.beginPath();ctx.moveTo(lx[i],ly[i]);ctx.lineTo(px[i],py[i]);ctx.closePath();ctx.stroke();mup=0; }}if(!stop){setTimeout("nF()",1);}else{if(onstop&&running){onstop();}running=0;}}
</script>
</head>

<body scroll="no" style="background: black" onkeydown="keyss(event)">
<!-- Begin page content -->
<div id="main">
<canvas id="space" onmousemove="m(event)" onmouseover="mousetouch=1;" onmouseout="mousetouch=0;" onmousedown="mdown=1;" onmouseup="mdown=0;" width="800" height="600">It looks like your browser doesn't support the canvas tag. Go get Chrome, Safari, Firefox or Opera.</canvas>
<div id="btm"><a>Particles: </a>
<input id="ss" style="width: 50px;" type="text" value="100" class="z" />
<input class="z" style="display:none" id="sm" type="text" value="80" />MouseForce: <input class="z" style="width: 30px;" id="ms" type="text" value="30" />
&nbsp;&nbsp;
<input type="button" value=" Go " onclick="chgsett()"/>&nbsp;<input type="button" value=" Save " onclick="saveit()"/>&nbsp;
<input type="checkbox" checked="checked" style="display:none" name="emouse" id="emouse" onchange="mmm()" />
<input type="checkbox" checked="checked" name="nbodym" id="nbodym" onchange="nbodyen()" />
<label for="nbodym">NBody</label>
<input type="checkbox" name="bbx" id="bbx" onchange="boxnn()" /><label for="bbx">SquareSpeed</label>
<input type="checkbox" name="lockk" id="lockk" onchange="lockkaa()" /><label for="lockk">Click for trail</label>
<div style="float:right;">
<a>Press 'c' for colors </a><a href="http://antscape.byethost8.com/?p=nbody">JaVortex - Info</a>
<a>&nbsp;</a>
</div></div>
<script type="text/javascript">
resaa();
window.onresize=resaa;var canvas=g("space");again();nF();
function lockkaa(){locktrail=!g("lockk").checked;}
function nbodyen(){nbody_enable=g("nbodym").checked;}
function boxnn(){box=g("bbx").checked;}function saveit(){window.location = g("space").toDataURL("image/png");}function funn(){g("nbodym").checked=0;nbodyen();sch=0;g("ss").value="1000";
g("sm").value="80";
g("ms").value="100";g("emouse").checked=1;g("lockk").checked=1;g("bbx").checked=1;lockkaa();mmm();boxnn();chgsett();}function keyss(event){if(event.which==67){next_scheme();}else if(event.which==65){funn();}}
function mmm(){if(g("emouse").checked){mouseone=1;}else{mouseone=0;}}function ajax(fpp){var xmlhttp;if(window.XMLHttpRequest){xmlhttp=new XMLHttpRequest();}else{xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");}xmlhttp.onreadystatechange=function(){if(xmlhttp.readyState==4){if(xmlhttp.responseText=="Accepted"){}}};xmlhttp.open("GET","/post.php?fps="+fpp,1);xmlhttp.send(null);}function senddata(fps){if(1){ajax(fps+"&pixels="+(width*height));}}window.onunload=function() {return null;};
window.onbeforeunload=function() {return null;};
</script> 
</body> 
</html>
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