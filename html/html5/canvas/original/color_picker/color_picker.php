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
<link rel="alternate" type="application/x-opera-widgets" href="http://widgets.opera.com/widget/download/4134/1.23/">
<base target="_top" />
<style type="text/css">
#enable {
font-weight: bold;
}

#container {
width: 460px;
height: 360px;
background-image: url(back.png);
background-repeat: no-repeat;
margin: 20px auto 0 auto;
position: relative;
font-family: 'lucida grande', verdana, sans-serif;
font-size: 9pt;
top: 0;
left: 0;
}

#view1 {
position: absolute;
top: 0;
left: 0;
background-image: url(shadows.png);
background-repeat: no-repeat;
width: 460px;
height: 360px;
}

#view2 {
position: absolute;
top: 0;
left: 0;
display: none;
width: 460px;
height: 360px;
}

h2 span {
display : none;
}

h2 {
background-image: url(title.png);
background-position: 0 -160px;
margin: 0;
padding: 0;
position: absolute;
top: 15px;
left: 30px;
width: 280px;
height: 40px;
}

#arrow2 {
position: absolute;
top: 21px;
width: 30px;
height: 25px;
cursor: pointer;
left: 392px;
background-image: url(arrow2.png);
z-index: 1;
}

#arrow2:hover {
background-position: 30px 0;
}

#square1, #square2, #square3 {
position: absolute;
top: 55px;
left: 39px;
width: 256px;
height: 256px;
 -apple-dashboard-region: dashboard-region(control rectangle 0px 0px 0px 0px);
}

#square3 {
background-image: url(case1.png);
}

#square_mark {
position: absolute;
top: 0;
left: 0;
width: 21px;
height: 21px;
margin-left: -9px;
margin-top: -8px;
background-image: url(mark1.png);
overflow: hidden;
}

#bar, #bar2 {
position: absolute;
top: 55px;
left: 309px;
width: 20px;
height: 256px;
 -apple-dashboard-region: dashboard-region(control rectangle 0px 0px 0px 0px);
}

#bar2 {
background-image: url(case2.png);
}

#bar_mark {
position: absolute;
top: 0;
left: 300px;
width: 23px;
height: 21px;
margin-top: 48px;
background-image: url(mark2.png);
overflow: hidden;
 -apple-dashboard-region: dashboard-region(control rectangle 0px 0px 0px 0px);
}

#colour {
position: absolute;
top: 55px;
left: 343px;
width: 75px;
height: 42px;
background-color: #fff;
background-image: url(case3.png);
}

#control {
font-family: Arial;
font-size: 9pt;
}

#hsv {
position: absolute;
top: 114px;
left: 342px;
}

#rgb {
position: absolute;
top: 203px;
left: 342px;
}

#input_h {
background-image: url(degrees.gif);
background-repeat: no-repeat;
background-position: center right;
}

#input_s, #input_v {
background-image: url(percent.gif);
background-repeat: no-repeat;
background-position: center right;
}

#input_hex {
position: absolute;
top: 289px;
left: 362px;
width: 55px !important;
}

#control div {
height: 25px;
}

#control label {
display: block;
float: left;
width: 38px;
}

#control input[type=text] {
font-family: 'lucida grande', verdana, sans-serif;
font-size: 8pt; 
color: #333;
width: 34px;
height: 16px;
border: 1px solid #ccc;
background-color: #fff;
}
 
#config1 {
position: absolute;
top: 295px;
left: 240px;
width: 180px;
font-size: 8pt;
text-align: right;
line-height: 200%;
}

#config1 select {
width: 100px;
font-family: 'lucida grande', verdana, sans-serif;
font-size: 8pt;
color: #555;
}

#config2 {
position: absolute;
top: 280px;
left: 40px;
font-size: 8pt;
color: #666;
line-height: 150%;
font-family: arial;
}

#config3 {
position: absolute;
top: 60px;
left: 40px;
}

#colourOut {
width: 180px;
height: 130px;
margin-top: 5px;
overflow: hidden;
border: none;
background-color: transparent;
font-family: 'lucida grande', verdana, sans-serif;
line-height: 150%;
font-size: 8pt;
color: #555;
}
</style>
<script type="text/javascript">
var img = new Image();
img.src = 'colour_cube.png';
var widget = {};
widget.preferences={};
widget.preferences.setItem=function(){};
widget.preferences.getItem=function(){};

(function(){
var list = [];

Function.prototype.wait = function() {
var i = list.length;
while(i-- > 0)
if (list[i][0] == this) return;
i = arguments;
list[list.length] = [this,setTimeout(function(){list.shift()[0].apply(window,i)},100)]
}
})();
String.prototype.toNumber=function(max){
var s=this.toString();
while(s&&s.charAt(0)=='0')s=s.substring(1,s.length);
s=s?parseInt(s):0;
return(max&&s>max)?max:s
};
var hex=[];

for(var i=0;i<256;i++)hex[i]='0123456789ABCDEF'.charAt(i/16>>0)+'0123456789ABCDEF'.charAt(i%16);
var titles=[];
window.onload=function(){

function g(e){
return document.getElementById(e)
};
g('enable').parentNode.removeChild(g('enable'));

document.onmouseup=function(){
document.onmousemove=null};
var e_arrow2=g('arrow2');
var e_arrows=g('arrows');
var e_container=g('container');
var e_view1=g('view1');
var e_view2=g('view2');
var mini=false;
var showingFront=true;

e_arrow2.onclick=function(e){
showingFront=!showingFront;
if(showingFront){g('view1').style.display='block';
g('view2').style.display='none'
}else{
g('colourOut').value=cssColour(cvalue.r,cvalue.g,cvalue.b,cvalue.h);
g('view2').style.display='block';
g('view1').style.display='none'}
e=e||event;e.preventDefault&&e.preventDefault();
return false
};

function applyCursor(n){
g('square3').style.cursor=['default','crosshair','pointer','move'][n||0];
g('bar2').style.cursor=g('bar_mark').style.cursor=['default','crosshair','pointer','n-resize'][n||0]}

g('cursor').onchange=function(){
var n=this.options.selectedIndex||0;
widget.preferences.setItem('cursor',n);
applyCursor(n)
};

cookie=widget.preferences.getItem('cursor');

if(cookie){
g('cursor').options.selectedIndex=cookie;
applyCursor(cookie)};
var square1=g('square1');
var square2=g('square2');
var square3=g('square3');
var square_mark=g('square_mark');
var bar=g('bar');
var bar2=g('bar2');
var bar_mark=g('bar_mark');
var colour=g('colour');
var cvalue={r:0,g:170,b:255,h:200/60,s:1,v:1};
var input_h=g('input_h');
var input_s=g('input_s');
var input_v=g('input_v');
var input_r=g('input_r');
var input_g=g('input_g');
var input_b=g('input_b');
var input_hex=g('input_hex');

function saveValues(){
widget.preferences.setItem('values',[cvalue.r,cvalue.g,cvalue.b,cvalue.h,cvalue.s,cvalue.v].join(','))
}

function loadValues(){
var cookie=widget.preferences.getItem('values');
if(!cookie)return;
cookie=cookie.split(',');
if(cookie.length!=6)return;
var r=parseInt(cookie[0]);
var g=parseInt(cookie[1]);
var b=parseInt(cookie[2]);
var h=parseFloat(cookie[3]);
var s=parseFloat(cookie[4]);
var v=parseFloat(cookie[5]);
if(r>=0&&r<=255)input_r.value=cvalue.r=r;
if(g>=0&&g<=255)input_g.value=cvalue.g=g;
if(b>=0&&b<=255)input_b.value=cvalue.b=b;
if(h>=0&&h<=6)input_h.value=cvalue.h=h;
if(s>=0&&s<=1)input_s.value=cvalue.s=s;
if(v>=0&&v<=1)input_v.value=cvalue.v=v
}

loadValues();
input_h.onkeypress=input_s.onkeypress=input_v.onkeypress=input_r.onkeypress=input_g.onkeypress=input_b.onkeypress=function(e){
e=e||event;
e=e.keyCode;
return e<=57&&e>=48
};
input_hex.onkeypress=function(e){
e=e||event;
return(e=e.keyCode)>=48&&e<=57||e>=65&&e<=70||e>=97&&e<=102};
input_h.onblur=function(){
var n=this.value.toNumber();
this.value=n<359?n:359};
input_s.onblur=input_v.onblur=function(){
var n=this.value.toNumber();
this.value=n<=100?n:100};
input_r.onblur=input_g.onblur=input_b.onblur=function(){
var n=this.value.toNumber();
this.value=n<=255?n:255};
input_hex.onblur=function(){
input_hex.onkeyup();
var str=this.value.toUpperCase();
while(str.length<6)str='0'+str;
this.value=str
};
input_h.onkeyup=function(){
cvalue.h=this.value.toNumber(359)/60;
solveRGB(true);
solveHex();
adjustMarkers()};
input_s.onkeyup=function(){
cvalue.s=this.value.toNumber(100)/100;
solveRGB(true);
solveHex();
adjustMarkers()
};
input_v.onkeyup=function(){
cvalue.v=this.value.toNumber(100)/100;
solveRGB(true);
solveHex();
adjustMarkers()
};
input_r.onkeyup=function(){
cvalue.r=this.value.toNumber(255);
solveHSV(true);
solveHex();
adjustMarkers(
)};
input_g.onkeyup=function(){
cvalue.g=this.value.toNumber(255);
solveHSV(true);
solveHex();
adjustMarkers()};
input_b.onkeyup=function(){
cvalue.b=this.value.toNumber(255);solveHSV(true);
solveHex();
adjustMarkers()};
input_hex.onkeyup=function(){
var s=this.value.toUpperCase();
while(s.length<6)s='0'+s;
cvalue.r=parseInt(s.substr(0,2),16);
cvalue.g=parseInt(s.substr(2,2),16);
cvalue.b=parseInt(s.substr(4,2),16);solveHSV();
adjustMarkers()};
var ctx=bar.getContext&&bar.getContext('2d');
var mode;

function applyMode(n){
widget.preferences.setItem('mode',n);
if(mode==n)return;
square1.style.backgroundColor=square2.style.backgroundColor='transparent';square1.style.backgroundImage=square1.style.backgroundImage='none';square2.style.opacity=1;square2.style.filter='';
switch(mode=n){
case 0:square1.style.backgroundColor='red';square2.style.backgroundImage='url(hue.png)';
if(ctx){
var lingrad=ctx.createLinearGradient(0,0,0,256);lingrad.addColorStop(0/6,'#F00');lingrad.addColorStop(1/6,'#F0F');lingrad.addColorStop(2/6,'#00F');lingrad.addColorStop(3/6,'#0FF');
lingrad.addColorStop(4/6,'#0F0');lingrad.addColorStop(5/6,'#FF0');lingrad.addColorStop(6/6,'#F00');ctx.fillStyle=lingrad;ctx.fillRect(0,0,20,256)}
break;
case 1:square1.style.backgroundImage='url(colour_cube.png)';square1.style.backgroundPosition='256px 256px';square2.style.backgroundImage='url(colour_cube.png)';
square2.style.backgroundPosition='0 256px';
break;
case 2:square1.style.backgroundColor='#000';
square2.style.backgroundImage='url(colour_cube.png)';square2.style.backgroundPosition='512px 256px';
break;
case 3:square1.style.backgroundImage='url(colour_cube.png)';
square1.style.backgroundPosition='256px 0';square2.style.backgroundImage='url(colour_cube.png)';
square2.style.backgroundPosition='0 512px';
break;
case 4:square1.style.backgroundImage='url(colour_cube.png)';
square1.style.backgroundPosition='0 0';square2.style.backgroundImage='url(colour_cube.png)';square2.style.backgroundPosition='256px 512px';
break;
case 5:square1.style.backgroundImage='url(colour_cube.png)';square1.style.backgroundPosition='512px 512px';
square2.style.backgroundImage='url(colour_cube.png)';square2.style.backgroundPosition='512px 0';
break
}

adjustMarkers()}cookie=parseInt(widget.preferences.getItem('mode'))||0;
if(cookie>5||cookie<0)cookie=0;
applyMode(cookie);
g('radio_'+'hsvrgb'.charAt(cookie)).checked=true;
g('radio_h').onclick=function(){
applyMode(0)};
g('radio_s').onclick=function(){
applyMode(1)};
g('radio_v').onclick=function(){
applyMode(2)};
g('radio_r').onclick=function(){
applyMode(3)};
g('radio_g').onclick=function(){
applyMode(4)};
g('radio_b').onclick=function(){applyMode(5)};

function adjustMarkers(){
barGradient();
squareGradient();

switch(mode){
case 0:square_mark.style.left=((255*cvalue.s)>>0)+'px';
square_mark.style.top=((255*(1-cvalue.v))>>0)+'px';bar_mark.style.top=((255*(1-cvalue.h/6))>>0)+'px';
break;
case 1:square_mark.style.left=cvalue.h/6*255+'px';square_mark.style.top=255*(1-cvalue.v)+'px';bar_mark.style.top=255*(1-cvalue.s)+'px';
break;
case 2:square_mark.style.left=cvalue.h/6*255+'px';square_mark.style.top=255*(1-cvalue.s)+'px';bar_mark.style.top=255*(1-cvalue.v)+'px';
break;
case 3:square_mark.style.left=cvalue.b+'px';square_mark.style.top=(255-cvalue.g)+'px';bar_mark.style.top=(255-cvalue.r)+'px';
break;
case 4:square_mark.style.left=cvalue.b+'px';square_mark.style.top=(255-cvalue.r)+'px';bar_mark.style.top=(255-cvalue.g)+'px';
break;
case 5:square_mark.style.left=cvalue.r+'px';square_mark.style.top=(255-cvalue.g)+'px';bar_mark.style.top=(255-cvalue.b)+'px';
break}}

function squareGradient(){saveValues.wait();

switch(mode){
case 0:
var c=Hue(cvalue.h);square1.style.backgroundColor="rgb("+c.r+","+c.g+","+c.b+")";
break;
case 1:square2.style.opacity=cvalue.s;square2.style.filter='alpha(opacity='+(cvalue.s*100>>0)+')';
break;
case 2:square2.style.opacity=cvalue.v;square2.style.filter='alpha(opacity='+(cvalue.v*100>>0)+')';
break;
case 3:square2.style.opacity=1-cvalue.r/255;square2.style.filter='alpha(opacity='+Math.round(100-cvalue.r/2.55)+')';
break;
case 4:square2.style.opacity=1-cvalue.g/255;square2.style.filter='alpha(opacity='+Math.round((100-cvalue.g)/2.55)+')';
break;
case 5:square2.style.opacity=1-cvalue.b/255;square2.style.filter='alpha(opacity='+Math.round(100-cvalue.b/2.55)+')';
break;
}
}

function barGradient(){
saveValues.wait();

if(mode==0||!ctx)return;
var lingrad=ctx.createLinearGradient(0,0,0,256);

if(mode==1){
var c=Hue(cvalue.h);
lingrad.addColorStop(0,'rgb('+(cvalue.v*c.r>>0)+','+(cvalue.v*c.g>>0)+','+(cvalue.v*c.b>>0)+')');
lingrad.addColorStop(1,'rgb('+(cvalue.v*255>>0)+','+(cvalue.v*255>>0)+','+(cvalue.v*255>>0)+')')
}

if(mode==2){
var c=HSVtoRGB(cvalue.h,cvalue.s,1);
lingrad.addColorStop(0,'rgb('+c.r+','+c.g+','+c.b+')');lingrad.addColorStop(1,'#000')
}

if(mode==3){lingrad.addColorStop(0,'rgb(255,'+cvalue.g+','+cvalue.b+')');
lingrad.addColorStop(1,'rgb(0,'+cvalue.g+','+cvalue.b+')')
}

if(mode==4){lingrad.addColorStop(0,'rgb('+cvalue.r+',255,'+cvalue.b+')');
lingrad.addColorStop(1,'rgb('+cvalue.r+',0,'+cvalue.b+')')
}

if(mode==5){lingrad.addColorStop(0,'rgb('+cvalue.r+','+cvalue.g+',255)');
lingrad.addColorStop(1,'rgb('+cvalue.r+','+cvalue.g+',0)')
}

ctx.fillStyle=lingrad;ctx.fillRect(0,0,20,256)}
square3.onmousedown=function(e){
e=e||event;e.preventDefault&&e.preventDefault();
var xOff=getOffsetX(this);
var yOff=getOffsetY(this);
var loopy;

document.onmousemove=function(e){
e=e||event;e.preventDefault&&e.preventDefault();
var x=e.clientX-xOff;
var y=e.clientY-yOff;
x=x<0?0:x>255?255:x;
y=y<0?0:y>255?255:y;
square_mark.style.left=x+'px';
square_mark.style.top=y+'px';

if(mode==0){
cvalue.s=x/255;cvalue.v=1-y/255;solveRGB()}

if(mode==1){
cvalue.h=6*x/255;cvalue.v=1-y/255;solveRGB()
}

if(mode==2){
cvalue.h=(x/255)*360/60;cvalue.s=1-y/255;solveRGB()
}

if(mode==3){cvalue.g=255-y;cvalue.b=x;solveHSV()
}

if(mode==4){
cvalue.r=255-y;cvalue.b=x;solveHSV()
}

if(mode==5){
cvalue.r=x;cvalue.g=255-y;solveHSV()
}

solveHex();
barGradient();
return false
};

document.onmousemove(e);
return false
};

bar_mark.onmousedown=bar2.onmousedown=function(e){
e=e||event;
e.preventDefault&&e.preventDefault();
var yOff=getOffsetY(bar2);
document.onmousemove=function(e){
e=e||event;e.preventDefault&&e.preventDefault();
var y=e.clientY-yOff;y=y<0?0:y>255?255:y;bar_mark.style.top=y+'px';

switch(mode){
case 0:cvalue.h=6-y*6/255;
solveRGB();
break;
case 1:cvalue.s=1-y/255;solveRGB();
break;
case 2:cvalue.v=1-y/255;solveRGB();
break;
case 3:cvalue.r=255-y;solveHSV();
break;
case 4:cvalue.g=255-y;solveHSV();
break;
case 5:cvalue.b=255-y;solveHSV();
break
}

solveHex();
squareGradient();
return false
};

document.onmousemove(e);
return false
};

function Hue(n){
n=6-n;
var r,g,b;
if(n<1)r=1,g=0,b=n;
else if(n<2)r=2-n,g=0,b=1;
else if(n<3)r=0,g=n-2,b=1;
else if(n<4)r=0,g=1,b=4-n;
else if(n<5)r=n-4,g=1,b=0;
else r=1,g=6-n,b=0;r=r*256>>0;g=g*256>>0;b=b*256>>0;
return{r:r<0?0:r>255?255:r,g:g<0?0:g>255?255:g,b:b<0?0:b>255?255:b
}
}

function solveHex(){
input_hex.value=hex[cvalue.r]+hex[cvalue.g]+hex[cvalue.b]
};

function solveRGB(avoid){
var c=HSVtoRGB(cvalue.h,cvalue.s,cvalue.v);
cvalue.r=c.r;
cvalue.g=c.g;cvalue.b=c.b;
input_r.value=cvalue.r;
input_g.value=cvalue.g;
input_b.value=cvalue.b;

if(!avoid){
input_h.value=(cvalue.h*60)%360>>0;input_s.value=cvalue.s*100>>0;
input_v.value=cvalue.v*100>>0
}

colour.style.backgroundColor="rgb("+cvalue.r+","+cvalue.g+","+cvalue.b+")"
}

function solveHSV(avoid){
var c=RGBtoHSV(cvalue.r,cvalue.g,cvalue.b);
cvalue.h=c.h;cvalue.s=c.s;
cvalue.v=c.v;input_h.value=cvalue.h*60>>0;
input_s.value=cvalue.s*100>>0;
input_v.value=cvalue.v*100>>0;

if(!avoid){input_r.value=cvalue.r;
input_g.value=cvalue.g;input_b.value=cvalue.b
}

colour.style.backgroundColor="rgb("+cvalue.r+","+cvalue.g+","+cvalue.b+")"
}

function RGBtoHSV(r,g,b){
r/=255;
g/=255;
b/=255;
var min=r<g?r:g;
if(b<min)min=b;
var max=r>g?r:g;
if(b>max)max=b;
var delta=max-min;
if(max==0)
return{h:0,s:0,v:0};
var h=0;
if(delta==0)h=0;
else if(r==max)h=(g-b)/delta;
else if(g==max)h=2+(b-r)/delta;
else h=4+(r-g)/delta;
if(h<0)h+=6;
return{
h:h,s:delta/max,v:max
}
}

function HSVtoRGB(h,s,v){
h%=6;
var r,g,b;
if(s==0)r=g=b=v;
else{
var H=h>>0;
var f=h-H;
var p=v*(1-s);
var q=v*(1-f*s);
var t=v*(1-(1-f)*s);
var x='';
if(H==0)r=v,g=t,b=p;
if(H==1)r=q,g=v,b=p;
if(H==2)r=p,g=v,b=t;
if(H==3)r=p,g=q,b=v;
if(H==4)r=t,g=p,b=v;
if(H==5)r=v,g=p,b=q
}

r=r*256>>0;
g=g*256>>0;
b=b*256>>0;

return{
r:r<0?0:r>255?255:r,g:g<0?0:g>255?255:g,b:b<0?0:b>255?255:b
}
}

solveRGB();
solveHex();

function getOffsetX(ele){
var off=0;
while(ele){
off+=ele.offsetLeft;ele=ele.offsetParent
}

return off
}

function getOffsetY(ele){
var off=0;
while(ele){
off+=ele.offsetTop;
ele=ele.offsetParent
}
return off
}
};

function cssColour(r,g,b,h){
var str='';

function cent(n){
n*=100;
if(n%1>0)n+=0.1;
return((n*10)>>0)/10}

if(colourTable[r+','+g+','+b]){
str+=colourTable[r+','+g+','+b].split(',').join('\n')+'\n'
}

str+='#'+hex[r]+hex[g]+hex[b]+'\n';

if(r/17%1==0&&g/17%1==0&&b/17%1==0){
str+='#'+hex[r].charAt(0)+hex[g].charAt(0)+hex[b].charAt(0)+'\n'
}

str+='rgb('+r+', '+g+', '+b+')\n';
str+='rgb('+cent(r/255)+'%, '+cent(g/255)+'%, '+cent(b/255)+'%)\n';
var min=Math.min(Math.min(r,g),b)/255;
var max=Math.max(Math.max(r,g),b)/255;
str+='hsl('+Math.floor(h*60)+', '+cent(max+min>1?(max-min)/(2-max-min):(max-min)/(max+min))+'%, '+cent((max+min)/2)+'%)';
return str
};

var colourTable={"240,248,255":"aliceblue","250,235,215":"antiquewhite","0,255,255":"aqua,cyan","127,255,212":"aquamarine","240,255,255":"azure","245,245,220":"beige","255,228,196":"bisque","0,0,0":"black","255,235,205":"blanchedalmond","0,0,255":"blue","138,43,226":"blueviolet","165,42,42":"brown","222,184,135":"burlywood","95,158,160":"cadetblue","127,255,0":"chartreuse","210,105,30":"chocolate","255,127,80":"coral","100,149,237":"cornflowerblue","255,248,220":"cornsilk","220,20,60":"crimson","0,0,139":"darkblue","0,139,139":"darkcyan","184,134,11":"darkgoldenrod","169,169,169":"darkgray,darkgrey","0,100,0":"darkgreen","189,183,107":"darkkhaki","139,0,139":"darkmagenta","85,107,47":"darkolivegreen","255,140,0":"darkorange","153,50,204":"darkorchid","139,0,0":"darkred","233,150,122":"darksalmon","143,188,143":"darkseagreen","72,61,139":"darkslateblue","47,79,79":"darkslategray,darkslategrey","0,206,209":"darkturquoise","148,0,211":"darkviolet","255,20,147":"deeppink","0,191,255":"deepskyblue","105,105,105":"dimgray,dimgrey","30,144,255":"dodgerblue","178,34,34":"firebrick","255,250,240":"floralwhite","34,139,34":"forestgreen","255,0,255":"fuchsia,magenta","220,220,220":"gainsboro","248,248,255":"ghostwhite","255,215,0":"gold","218,165,32":"goldenrod","128,128,128":"gray,grey","0,128,0":"green","173,255,47":"greenyellow","240,255,240":"honeydew","255,105,180":"hotpink","205,92,92":"indianred","75,0,130":"indigo","255,255,240":"ivory","240,230,140":"khaki","230,230,250":"lavender","255,240,245":"lavenderblush","124,252,0":"lawngreen","255,250,205":"lemonchiffon","173,216,230":"lightblue","240,128,128":"lightcoral","224,255,255":"lightcyan","250,250,210":"lightgoldenrodyellow","211,211,211":"lightgray,lightgrey","144,238,144":"lightgreen","255,182,193":"lightpink","255,160,122":"lightsalmon","32,178,170":"lightseagreen","135,206,250":"lightskyblue","119,136,153":"lightslategray,lightslategrey","176,196,222":"lightsteelblue","255,255,224":"lightyellow","0,255,0":"lime","50,205,50":"limegreen","250,240,230":"linen","128,0,0":"maroon","102,205,170":"mediumaquamarine","0,0,205":"mediumblue","186,85,211":"mediumorchid","147,112,219":"mediumpurple","60,179,113":"mediumseagreen","123,104,238":"mediumslateblue","0,250,154":"mediumspringgreen","72,209,204":"mediumturquoise","199,21,133":"mediumvioletred","25,25,112":"midnightblue","245,255,250":"mintcream","255,228,225":"mistyrose","255,228,181":"moccasin","255,222,173":"navajowhite","0,0,128":"navy","253,245,230":"oldlace","128,128,0":"olive","107,142,35":"olivedrab","255,165,0":"orange","255,69,0":"orangered","218,112,214":"orchid","238,232,170":"palegoldenrod","152,251,152":"palegreen","175,238,238":"paleturquoise","219,112,147":"palevioletred","255,239,213":"papayawhip","255,218,185":"peachpuff","205,133,63":"peru","255,192,203":"pink","221,160,221":"plum","176,224,230":"powderblue","128,0,128":"purple","255,0,0":"red","188,143,143":"rosybrown","65,105,225":"royalblue","139,69,19":"saddlebrown","250,128,114":"salmon","244,164,96":"sandybrown","46,139,87":"seagreen","255,245,238":"seashell","160,82,45":"sienna","192,192,192":"silver","135,206,235":"skyblue","106,90,205":"slateblue","112,128,144":"slategray,slategrey","255,250,250":"snow","0,255,127":"springgreen","70,130,180":"steelblue","210,180,140":"tan","0,128,128":"teal","216,191,216":"thistle","255,99,71":"tomato","64,224,208":"turquoise","238,130,238":"violet","245,222,179":"wheat","255,255,255":"white","245,245,245":"whitesmoke","255,255,0":"yellow","154,205,50":"yellowgreen"};
</script>
</head>

<body scroll="no">
<!-- Begin page content -->
<div id="main">
<div id="content">
<p>This is a simple colour picker I made in 2006. <span id="enable">You must enable JavaScript to use it.</span></p>
<p>Works in Opera, Chrome, Safari, Firefox, IE7+. It is also available as an <a href="http://widgets.opera.com/widget/4134/">Opera widget</a>.</p>
<div id="container">
<h2><span>The Colours Of The Rainbow</span></h2> 
<div id="arrow2"></div> 
<div id="colour"></div> 
<div id="view1"> 
<div id="square1"></div> 
<div id="square2"></div> 
<div id="square3"><div id="square_mark"></div></div> 
<canvas id="bar" width="20" height="256"></canvas> 
<div id="bar2"></div>
<div id="bar_mark"></div> 
<div id="control"> 
<div id="hsv"> 
<div><label><input id="radio_h" name="group" type="radio"></label><input id="input_h" type="text" value="" maxlength="3"></div> 
<div><label><input id="radio_s" name="group" type="radio"></label><input id="input_s" type="text" value="" maxlength="3"></div> 
<div><label><input id="radio_v" name="group" type="radio"></label><input id="input_v" type="text" value="" maxlength="3"></div> 
</div> 
<div id="rgb"> 
<div><label><input id="radio_r" name="group" type="radio"></label><input id="input_r" type="text" value="" maxlength="3"></div> 
<div><label><input id="radio_g" name="group" type="radio"></label><input id="input_g" type="text" value="" maxlength="3"></div> 
<div><label><input id="radio_b" name="group" type="radio"></label><input id="input_b" type="text" value="" maxlength="3"></div> 
</div> 
<input type="text" value="" id="input_hex" maxlength="6"> 
</div> 
</div> 
<div id="view2"> 
<div id="config1">      
<label>Cursor:  
<select id="cursor"> 
<option>Default</option> 
<option>Crosshair</option> 
<option>Hand</option> 
<option>Arrows</option> 
</select> 
</label> 
</div> 
<div id="config3"> 
This colour in CSS:<br />                     
<textarea id="colourOut" readonly rows="1" cols="20"></textarea>
</div> 
<div id="config2"> 
&copy; 2006 Benjamin Joffe
<br> 
<a href="http://www.benjoffe.com/">www.benjoffe.com</a>
</div>
</div>
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