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
<meta name="viewport" content="width=device-width; initial-scale=1.0; maximum-scale=1.0; user-scalable=0;" /> 
<script type="text/javascript" src="/hnsdesktop/jquery.js"></script>
<script type="text/javascript" src="/hnsdesktop/jquery.corners.js"></script>
<base target="_top" />
<style type="text/css">
body {
margin: 0px;
-webkit-user-select: none;
}
</style> 
<script type="text/javascript">
// ---
// Copyright (c) 2010 Francesco Cottone, http://www.kesiev.com/
// ---

// ----------------------------------------------------------------------------
// gbox
// ----------------------------------------------------------------------------

var dynalist={
	create:function() {
		return {
			first:null,
			last:null,
			data:[],
			dl:0,
			gar:[],
			disconnect:function(obd) {
				if (this.data[obd].__first!=null)  this.data[this.data[obd].__first].__next=this.data[obd].__next; else this.first=this.data[obd].__next;
				if (this.data[obd].__next!=null)  this.data[this.data[obd].__next].__first=this.data[obd].__first; else this.last=this.data[obd].__first;
			},
			addObject:function(obj,prio) {
				var nid=this.gar.pop();
				if (nid==null) {
					nid=this.dl;
					this.dl++;
				}
				if (this.first==null) { // First element
					obj.__next=null;
					obj.__first=null;
					this.first=nid;
					this.last=nid;
				} else { // Chain next
					var i=this.first;
					while (i!=null)
						if (this.data[i].__prio>prio) break; else i=this.data[i].__next;
					if (i==null) { // if last, chain in queue
						obj.__next=null;
						obj.__first=this.last;								
						this.data[this.last].__next=nid;
						this.last=nid;								
					} else { // else reconnect objects
						obj.__first=this.data[i].__first;
						obj.__next=i;
						this.data[i].__first=nid;
						if (obj.__first!=null) this.data[obj.__first].__next=nid; else this.first=nid;
					}
					
				}
				obj.__prio=prio;
				obj.__id=nid;
				this.data[nid]=obj;
				return nid;
			},
			setPrio:function(obd,prio) {
				if (this.data[obd].__prio==prio) return;
				if (this.first!=this.last)
				if (this.data[obd].__prio<prio) {
					if (this.data[obd].__id!=this.last) {
						var i=this.data[obd].__next;
						while (i!=null)
							if (this.data[i].__prio>=prio) break; else i=this.data[i].__next;
						if ((i==null)||(this.data[i].__first!=this.data[obd].__id)) {
							// disconnect
							this.disconnect(obd);
							// Reconnect
							if (i==null) {
								this.data[this.last].__next=this.data[obd].__id;
								this.data[obd].__first=this.last;
								this.data[obd].__next=null;
								this.last=this.data[obd].__id;
							} else {
								this.data[obd].__first=this.data[i].__first;
								this.data[obd].__next=i;
								this.data[i].__first=this.data[obd].__id;
								if (this.data[obd].__first!=null) this.data[this.data[obd].__first].__next=this.data[obd].__id; else this.first=this.data[obd].__id;
							}
						}
					}
				} else {
					if (this.data[obd].__id!=this.first) {
						var i=this.data[obd].__first;
						while (i!=null)
							if (this.data[i].__prio<=prio) break; else i=this.data[i].__first;
						if ((i==null)||(this.data[i].__next!=this.data[obd].__id)) {
							// disconnect
							this.disconnect(obd);
							if (i==null) {
								this.data[this.first].__first=this.data[obd].__id;
								this.data[obd].__first=null;
								this.data[obd].__next=this.first;
								this.first=this.data[obd].__id;
							} else {
								this.data[obd].__first=i;
								this.data[obd].__next=this.data[i].__next;
								this.data[i].__next=this.data[obd].__id;
								if (this.data[obd].__next!=null) this.data[this.data[obd].__next].__first=this.data[obd].__id; else this.last=this.data[obd].__id;
							}
						}
					}
				}
				this.data[obd].__prio=prio;
			},
			remove:function(obd) {
				this.disconnect(obd);
				this.gar.push(this.data[obd].__id);
				delete this.data[this.data[obd].__id];
			}
		}
	}
}

// A special circular queue with some features useful for the resource loader
var cyclelist={
	create:function(size) {
		return {
			_head:0,
			_tail:0,
			_data:[],
			_size:(size?size:10),
			_total:0,
			_done:0,
			_current:null,
			getTotal:function(){return this._total}, // Number of elements to be "poped"
			getDone:function(){return this._done}, // Number of popped elements since the last empty
			getSize:function(){return this._size}, // The maximum number of elements in the queue
			isProcessing:function(){return this._current!=null}, // The last pop was not a null (i.e. the queue returned a valid object)
			isEnded:function(){return (this._head==this._tail)}, // There are other elements in the queue
			isBusy:function(){return this.isProcessing()||!this.isEnded()}, // There are elements in the queue/the last one pop returned an object that is being processed
			getCurrent:function(){return this._current}, // Return the last popped element
			push:function(d) {
				this._data[this._head]=d;
				this._head=(this._head+1)%this._size;
				this._total++;
			},
			pop:function() {
				if (this.isEnded()) {
					this._total=0;
					this._done=0;
					this._current=null;
				} else {
					this._current=this._data[this._tail];
					this._tail=(this._tail+1)%this._size;
					this._done++;
				}
				return this._current;
			},
			dump:function() {
				var r="";
				for (var i=0;i<this._size;i++) {
					r+=i+") "+this._data[i]+" | "+(i==this._head?"HEAD ":"")+(i==this._tail?"TAIL ":"")+"\n";
				}
				r+="\n\n"+this._done+"/"+this._total;
				return r;
			}
		}
	}
}

// A simple circular cache handler
var cachelist={
	create:function(size) {
		return {
			_cache:{},
			_queue:[],
			_head:0,
			_size:(size?size:10),
			add:function(k,v) {
				if (!this._cache[k]) {
					if (this._queue[this._head])
						delete this._cache[this._queue[this._head]];
					this._queue[this._head]=k;
					this._cache[k]={pos:this._head,value:v};
					this._head=(this._head+1)%this._size;
				} else this._cache[k].value=v;
			},
			read:function(k) {
				return (this._cache[k]?this._cache[k].value:null);
			},
			clear:function() {
				this._cache={};
				this._queue=[];
				this._head=0;
			}
		}
	}
}

/*
Gamebox module allows multiple grouped objects to move simultaneously, it helps with collisions, 
rendering and moving objects. It also provides monospaced pixel-font rendering, keyboard handling,  
audio, double buffering and eventually FSEs. Gamebox can also store and load data from cookies! 
*/

var gbox={
	// CONSTANTS
	ALIGN_CENTER:0,
	ALIGN_MIDDLE:0,
	ALIGN_RIGHT:1,
	ALIGN_BOTTOM:1,
	COLOR_BLACK:'rgb(0,0,0)',
	COLOR_WHITE:'rgb(255,255,255)',
	ZINDEX_LAYER:-1,
	
	// VARS
	_autoid:0,
	_keyboard:[],
	_keymap:{
		up:38,
		down:40,
		right:39,
		left:37,
		a:90,
		b:88,
		c:67
	},
	_flags:{
		experimental:false,
		noaudio:false
	},
	_fonts:{},
	_tiles:{},
	_images:{},
	_camera:{},
	_screen:0,
	_screenposition:0,
	_keyboardpicker:0,
	_screenh:0,
	_screenw:0,
	_screenhh:0,
	_screenhw:0,
	_zoom:1,
	_canvas:{},
	_objects:{},
	_groups:[],
	_renderorder:[],
	_groupplay:{},
	_actionqueue:["first","then","blit","after"], // initialize is executed once
	_mspf:0,
	_fps:0,
	_gametimer:0,
	_frameskip:0,
	_autoskip:{min:0,max:5,lowidle:0,hiidle:5}, // minimum frameskip, maximum frameskip, minimum idle time allowed for increasing frameskip, maximum idle time allowed for decreasing frameskip
	_fskid:0,
	_statbar:0,
	_border:0,
	_garbage:[],
	_zindexch:[],
	_framestart:0,
	_zindex:dynalist.create(),
	_db:false,
	_systemcookie:"__gboxsettings",
	_sessioncache:"",
	_breakcacheurl:function(a) {return a+(a.indexOf("?")==-1?"?":"&")+"_brc="+gbox._sessioncache; },
	_forcedidle:0,
	_gamewaiting:0,
	_splash:{
		gaugeLittleColor:"rgb(255,240,0)",
		gaugeLittleBackColor:"rgb(255,255,255)",
		gaugeBorderColor:"rgb(0,0,0)",
		gaugeBackColor:"rgb(100,100,100)",
		gaugeColor:"rgb(255,240,0)",
		gaugeHeight:10,
		background:null,
		minimalTime:0,
		footnotes:null,
		footnotesSpacing:1
	},
	_minimalexpired:0, // 0: not triggered, 1: triggered, 2: done
	_safedrawimage:function(tox,img,sx,sy,sw,sh,dx,dy,dw,dh) {
		if (!img||!tox) return;
		if (sx<0) { dx-=(dw/sw)*sx;sw+=sx; sx=0; }
		if (sy<0) { dy-=(dh/sh)*sy;sh+=sy; sy=0; }
		if (sx+sw>img.width) { dw=(dw/sw)*(img.width-sx);sw=img.width-sx;}
		if (sy+sh>img.height) { dh=(dh/sh)*(img.height-sy);sh=img.height-sy;}
		try { if ((sh>0)&&(sw>0)&&(sx<img.width)&&(sy<img.height)) tox.drawImage(img, sx,sy,sw,sh,dx,dy,dw,dh); } catch(e){}
	},
	_keydown:function(e){
		var key=(e.fake||window.event?e.keyCode:e.which);
		if (!gbox._keyboard[key]) gbox._keyboard[key]=1;
	},
	_keyup:function(e){
		var key=(e.fake||window.event?e.keyCode:e.which);
		gbox._keyboard[key]=-1;
	},
	_resetkeys:function() {
		for (var key in gbox._keymap)
			gbox._keyup({fake:1,keyCode:gbox._keymap[key]});
	},
	_showkeyboardpicker:function(){
		gbox._keyboardpicker.value="Click/Tap here to enable the keyboard";
		gbox._keyboardpicker.style.left=(gbox._screenposition.x+5)+"px";
		gbox._keyboardpicker.style.top=(gbox._screenposition.y+5)+"px";
		gbox._keyboardpicker.style.width=(gbox._screenposition.w-10)+"px";
		gbox._keyboardpicker.style.height="30px";
		this._keyboardpicker.style.border="1px dashed white";
		this._keyboardpicker.readOnly=null;
	},
	_hidekeyboardpicker:function(){
		this._keyboardpicker.style.zIndex=100;
		this._keyboardpicker.readOnly="yes";
		this._keyboardpicker.style.position="absolute";
		this._keyboardpicker.style.textAlign="center";
		this._keyboardpicker.style.backgroundColor="#000000";
		this._keyboardpicker.style.color="#fefefe";
		this._keyboardpicker.style.cursor="pointer";			
		this._keyboardpicker.value="";
		this._keyboardpicker.style.left="0px";
		this._keyboardpicker.style.top="0px";
		this._keyboardpicker.style.height="0px";
		this._keyboardpicker.style.width="0px";
		this._keyboardpicker.style.border="0px";
		this._keyboardpicker.style.padding="0px";
		this._keyboardpicker.style.margin="0px";
	},
	_domgetabsposition:function(oElement) {
		var sizes={x:0,y:0,h:0,w:0};
		sizes.h=oElement.offsetHeight;
		sizes.w=oElement.offsetWidth;
		while( oElement != null) {
			sizes.y += oElement.offsetTop;
			sizes.x += oElement.offsetLeft;
			oElement = oElement.offsetParent;
		}
		return sizes;
	},
	setForcedIdle:function(f) { this._forcedidle=f},
	getFlag:function(f) { return this._flags[f] },
	setStatusBar:function(a) { this._statbar=a },
	setScreenBorder:function(a) { this._border=a},
	initScreen:function(w,h) {
		document.body.style.textAlign="center";
		var container=document.createElement("div");
		container.style.width="100%";
		container.style.height="100%";
		container.style.display="table";
		this._box=document.createElement("div");
		this._box.style.display="table-cell";
		this._box.style.width="100%";
		this._box.style.textAlign="center";
		this._box.style.verticalAlign="middle";
		
		this._screen=document.createElement("canvas");
		if (this._border) this._screen.style.border="1px solid black";
		this._screen.setAttribute('height',h);
		this._screen.setAttribute('width',w);
		this._screen.style.width=(w*this._zoom)+"px";
		this._screen.style.height=(h*this._zoom)+"px";
		this._screenh=h;
		this._screenw=w;
		this._screenhh=Math.floor(h/2);
		this._screenhw=Math.floor(w/2);
		this._camera.x=0;
		this._camera.y=0;
		this._camera.h=h;
		this._camera.w=w;
		this._box.appendChild(this._screen);
		container.appendChild(this._box);
		document.body.appendChild(container);

		this.createCanvas("_buffer");
		window.addEventListener('keydown', this._keydown,false);
		window.addEventListener('keyup', this._keyup,false);
		if (this._statbar) {
			this._statbar=document.createElement("div");
			if (this._border) this._statbar.style.border="1px solid black";
			this._statbar.style.margin="auto";
			this._statbar.style.backgroundColor="#ffffff";
			this._statbar.style.fontSize="10px";
			this._statbar.style.fontFamily="sans-serif";
			this._statbar.style.width=(w*this._zoom)+"px";
			this._box.appendChild(this._statbar);
		}
		// Keyboard support on devices that needs focus (like iPad) - actually is not working for a bug on WebKit's "focus" command.
		this._keyboardpicker=document.createElement("input");
		this._keyboardpicker.onclick=function(evt) { gbox._hidekeyboardpicker();evt.preventDefault();evt.stopPropagation();};
		this._hidekeyboardpicker(this._keyboardpicker);
		
		gbox._box.appendChild(this._keyboardpicker);
		gbox._screen.ontouchstart=function(evt) { gbox._screenposition=gbox._domgetabsposition(gbox._screen);if (evt.touches[0].pageY-gbox._screenposition.y<30) gbox._showkeyboardpicker();else gbox._hidekeyboardpicker();evt.preventDefault();evt.stopPropagation();};
		gbox._screen.ontouchend=function(evt) {evt.preventDefault();evt.stopPropagation();};
		gbox._screen.ontouchmove=function(evt) { evt.preventDefault();evt.stopPropagation();};
		gbox._screen.onmousedown=function(evt) {gbox._screenposition=gbox._domgetabsposition(gbox._screen);if (evt.pageY-gbox._screenposition.y<30)  gbox._showkeyboardpicker(); else gbox._hidekeyboardpicker();evt.preventDefault();evt.stopPropagation();};
		
		var d=new Date();
		gbox._sessioncache=d.getDate()+"-"+d.getMonth()+"-"+d.getFullYear()+"-"+d.getHours()+"-"+d.getMinutes()+"-"+d.getSeconds();
		
		gbox._loadsettings(); // Load default configuration
		gbox.setCanAudio(true); // Tries to enable audio by default
	},
	setDoubleBuffering:function(db){this._db=db},
	setStatBar:function(txt){ if (gbox._statbar) this._statbar.innerHTML=(txt?txt:"&nbsp")},
	setFps:function(f){
		this._fps=f;
		this._mspf=Math.floor(1000/f)
	},
	getFps:function() { return this._fps },
	setAutoskip:function(f){this._autoskip=f},
	setFrameskip:function(f){this._frameskip=f},
	getScreenH:function(){return this._screenh},
	getScreenW:function(){return this._screenw},
	getScreenHH:function(){return this._screenhh},
	getScreenHW:function(){return this._screenhw},
	setZoom:function(z) { this._zoom=z},
	setCallback:function(cb) { this._cb=cb; },
	_playobject:function(g,obj,a) {
		if (gbox._objects[g][obj].initialize) {
			gbox._objects[g][obj].initialize(obj);
			delete gbox._objects[g][obj].initialize;
		}
		if (gbox._objects[g][obj][a]) gbox._objects[g][obj][a](obj,a);
	},
	_nextframe:function(){
		gbox._framestart=gbox._mspf-(new Date().getTime()-gbox._framestart);
		if (gbox._autoskip)
			if ((gbox._framestart<gbox._autoskip.lowidle)&&(gbox._frameskip<gbox._autoskip.max)) gbox.setFrameskip(gbox._frameskip+1); else
			if ((gbox._framestart>gbox._autoskip.hiidle)&&(gbox._frameskip>gbox._autoskip.min)) gbox.setFrameskip(gbox._frameskip-1);
		if (gbox._statbar) gbox.debugGetstats();
		this._gametimer=setTimeout(gbox.go,(gbox._framestart<=0?1:gbox._framestart));		
	},
	go:function() {
		if (gbox._loaderqueue.isBusy()) {
			if (gbox._gamewaiting==1) {
				gbox.blitFade(gbox._screen.getContext("2d"),{alpha:0.5});
				gbox.blitText(gbox._screen.getContext("2d"),{font:"_dbf",dx:2,dy:2,text:"LOADING..."});
				gbox._gamewaiting=true;
			}
			if (gbox._gamewaiting<=1) {
				var bw=Math.floor(((gbox.getScreenW()-4)*gbox._loaderqueue.getDone())/gbox._loaderqueue.getSize());
				gbox._screen.getContext("2d").globalAlpha=1;
				gbox._screen.getContext("2d").fillStyle = gbox._splash.gaugeLittleBackColor;
				gbox._screen.getContext("2d").fillRect(0,4+gbox.getFont("_dbf").tileh,gbox.getScreenW(),1);
				gbox._screen.getContext("2d").fillStyle = gbox._splash.gaugeLittleColor;
				gbox._screen.getContext("2d").fillRect(0,4+gbox.getFont("_dbf").tileh,(bw>0?bw:0),1);
				gbox._screen.getContext("2d").restore();
				gbox.setStatBar("Loading... ("+gbox._loaderqueue.getDone()+"/"+gbox._loaderqueue.getTotal()+")");
			}
			if (gbox._gamewaiting) gbox._gamewaiting--;
			setTimeout(gbox.go,1000);
		} else {
			gbox._gamewaiting=3;
			gbox._framestart=new Date().getTime();
			var gr="";
			for (var g=0;g<gbox._renderorder.length;g++)
				if (gbox._groupplay[gbox._renderorder[g]])
					if (gbox._renderorder[g]==gbox.ZINDEX_LAYER) {
						var id;
						for (var i=0;i<gbox._actionqueue.length;i++) {
							id=gbox._zindex.first;
							while (id!=null) {
								if (gbox._groupplay[gbox._zindex.data[id].g])
									gbox._playobject(gbox._zindex.data[id].g,gbox._zindex.data[id].o,gbox._actionqueue[i]);
								id=gbox._zindex.data[id].__next;
							}
						}
					} else
						for (var i=0;i<gbox._actionqueue.length;i++)
							for (var obj in gbox._objects[gbox._renderorder[g]])
								gbox._playobject(gbox._renderorder[g],obj,gbox._actionqueue[i]);
			if (gbox._fskid>=gbox._frameskip) {
				if (gbox._db) gbox.blitImageToScreen(gbox.getBuffer());
				gbox._fskid=0;
			} else gbox._fskid++;
			
			gbox.purgeGarbage();
	
			if (gbox._zindexch.length) {
				
				for (var i=0;i<gbox._zindexch.length;i++) {
					if (gbox._objects[gbox._zindexch[i].o.g][gbox._zindexch[i].o.o])
						if (gbox._objects[gbox._zindexch[i].o.g][gbox._zindexch[i].o.o].__zt==null)
							gbox._objects[gbox._zindexch[i].o.g][gbox._zindexch[i].o.o].__zt=gbox._zindex.addObject(gbox._zindexch[i].o,gbox._zindexch[i].z);
						else
							gbox._zindex.setPrio(gbox._objects[gbox._zindexch[i].o.g][gbox._zindexch[i].o.o].__zt,gbox._zindexch[i].z);
				}
				gbox._zindexch=[];
			}
			
			
			// Handle holding
			for (var key in gbox._keymap)
				if (gbox._keyboard[gbox._keymap[key]]==-1) gbox._keyboard[gbox._keymap[key]]=0; else
				if (gbox._keyboard[gbox._keymap[key]]&&(gbox._keyboard[gbox._keymap[key]]<100))	gbox._keyboard[gbox._keymap[key]]++;
			if (gbox._forcedidle)
				this._gametimer=setTimeout(gbox._nextframe,gbox._forcedidle); // Wait for the browser
			else
				gbox._nextframe();
		}
	},
	
	debugGetstats:function() {
		var statline="Idle: "+gbox._framestart+"/"+gbox._mspf+(gbox._frameskip>0?" ("+gbox._frameskip+"skip)":"")+" | ";
		var cnt=0;
		for (var g=0;g<gbox._groups.length;g++)
			if (gbox._groupplay[gbox._groups[g]]) {
				cnt=0;
				for (var obj in gbox._objects[gbox._groups[g]]) cnt++;
				if (cnt) statline+=gbox._groups[g]+"["+cnt+"] ";
			}
		var cnt=0;
		var ply=0;
		for (var g in gbox._audio.aud) 
			for (var x=0;x<gbox._audio.aud[g].length;x++) {
				cnt++;
				if (!gbox._audio.aud[g][x].paused&&!gbox._audio.aud[g][x].ended) ply++;
			}
		statline+="| audio: "+ply+"/"+cnt+":"+this._audioteam;
			/*
			statline+="<br><br>";
		var id=gbox._zindex.first;
			while (id!=null) {
				if (gbox._groupplay[gbox._zindex.data[id].g]) statline+=gbox._zindex.data[id].g+" | "+gbox._zindex.data[id].o+" ("+gbox._zindex.data[id].__prio+")<br>";
				id=gbox._zindex.data[id].__next;
			}
			*/
		gbox.setStatBar(statline);
	},
	setZindex:function(th,z) {
		if ((th.__zt==null)||(th.zindex!=z)) {
			th.zindex=z;
			this._zindexch.push({o:{g:th.group,o:th.id},z:z});
		}
	},
	keyIsHit:function(id) { return this._keyboard[this._keymap[id]]==1},
	keyIsPressed:function(id) { return this._keyboard[this._keymap[id]]>0},
	keyIsHold:function(id) { return this._keyboard[this._keymap[id]]>1},
	keyIsReleased:function(id) { return this._keyboard[this._keymap[id]]==-1},
	_savesettings:function() {
		var saved="";
		for (var k in this._keymap) saved+="keymap-"+k+":"+this._keymap[k]+"~";
		for (var f in this._flags) saved+="flag-"+f+":"+(this._flags[f]?1:0)+"~";
		this.dataSave("sys",saved);
	},
	_loadsettings:function() {
		var cfg=this.dataLoad("sys");
		if (cfg!==null) {
			cfg=cfg.split("~");
			var kv;
			var mk;
			for (var i=0;i<cfg.length;i++) {
				kv=cfg[i].split(":");
				mk=kv[0].split("-");
				switch (mk[0]) {
					case "keymap": { this._keymap[mk[1]]=kv[1]*1; break }
					case "flag": { this._flags[mk[1]]=kv[1]*1; break }
				}
			}
		}
	},
	dataSave:function(k,v,d) { 
		var date = new Date();
		date.setTime(date.getTime()+((d?d:356*10)*24*60*60*1000));
		document.cookie =this._systemcookie+"~"+k+"="+v+"; expires="+date.toGMTString()+"; path=/";
	},
	dataLoad:function(k,a) {
		var nameeq=this._systemcookie+"~"+k+"=";
		var ca = document.cookie.split(";");
		var rt;
		for (var i=0;i<ca.length;i++) {
			var c=ca[i];
			while (c.charAt(0)==' ') c=c.substring(1,c.length);
			if (c.indexOf(nameeq)==0) {
				rt=c.substring(nameeq.length,c.length);
				if (a&&a.number) return rt*1; else return rt;
			}
		}
		return null;
	},
	dataClear:function(k) { this.dataSave(k,"",-1) },
	getCamera:function() { return this._camera; },
	setCameraY:function(y,viewdata) {
		this._camera.y=y;
		if (this._camera.y+this._camera.h>viewdata.h) this._camera.y=viewdata.h-this._screenh;
		if (this._camera.y<0) this._camera.y=0;
	},
	setCameraX:function(x,viewdata) {
		this._camera.x=x;
		if (this._camera.x+this._camera.w>viewdata.w) this._camera.x=viewdata.w-this._screenw;
		if (this._camera.x<0) this._camera.x=0;
	},
	centerCamera:function(data,viewdata) {
		this.setCameraX(data.x-this._screenhw,viewdata);
		this.setCameraY(data.y-this._screenhh,viewdata);
	},
	
	

	getGroups:function() { return this._groups; },
	setGroups:function(g){
		this._groups=g;
		this._groupplay[gbox.ZINDEX_LAYER]=true;
		for (var i=0;i<g.length;i++)
			if (!this._objects[g[i]]) {
				this._objects[g[i]]={};
				this._groupplay[g[i]]=true;
				this._renderorder[i]=g[i];
			}
	},
	setRenderOrder:function(g) { this._renderorder=g; },
	playGroup:function(gid){this._groupplay[gid]=true;},
	stopGroup:function(gid){this._groupplay[gid]=false;},
	toggleGroup:function(gid){this._groupplay[gid]=!this._groupplay[gid];},
	soloGroup:function(gid) {
		for (var i=0;i<this._groups.length;i++)
			if (this._groups[i]==gid) this.playGroup(this._groups[i]); else this.stopGroup(this._groups[i]);
	},
	playAllGroups:function() { for (var i=0;i<this._groups.length;i++) this.playGroup(this._groups[i]); },
	clearGroup:function(group) {
		for (var obj in this._objects[group]) {
			if (this._objects[group][obj].__zt!=null) this._zindex.remove(this._objects[group][obj].__zt);
			delete this._objects[group][obj];
		}
	},
	playGroups:function(gid){for (var i=0;i<gid.length;i++)this.playGroup(gid[i])},
	stopGroups:function(gid){for (var i=0;i<gid.length;i++)this.stopGroup(gid[i])},
	toggleGroups:function(gid){for (var i=0;i<gid.length;i++)this.toggleGroup(gid[i])},
	getObject:function(group,id) {return this._objects[group][id]},
	addFont:function(data) {
		data.tilehh=Math.floor(data.tileh/2);
		data.tilehw=Math.floor(data.tilew/2);
		this._fonts[data.id]=data;
		this._fonts[data.id].firstascii=data.firstletter.charCodeAt(0);
	},
	getFont:function(id) {
		return this._fonts[id];
	},
	trashObject:function(obj) {
		if (!this._garbage[obj.group]) this._garbage[obj.group]={};
		this._garbage[obj.group][obj.id]=1;
		obj.__trashing=true;
	},
	purgeGarbage:function() {
		for (var group in this._garbage)
			for (var id in this._garbage[group]) {
				if (this._objects[group][id].__zt!=null)
					this._zindex.remove(this._objects[group][id].__zt)
				delete this._objects[group][id];
			}
		gbox._garbage={};
	},
	trashGroup:function(group) {
		if (!this._garbage[group]) this._garbage[group]={};
		for (var obj in this._objects[group])
			this._garbage[group][obj]=1;
	},
	objectIsTrash:function(o) { return o.__trashing },
	addObject:function(data) {
		// Extras
		if (!data.id) {
			data.id="obj-"+this._autoid;
			this._autoid=(this._autoid+1)%1000;
		}
		if (data.tileset) {
			if (data.h==null) data.h=this._tiles[data.tileset].tileh;
			if (data.w==null) data.w=this._tiles[data.tileset].tilew;
			if (data.hw==null) data.hw=this._tiles[data.tileset].tilehw;
			if (data.hh==null) data.hh=this._tiles[data.tileset].tilehh;
		}
		this._objects[data.group][data.id]=data;
		if (data.zindex!=null)
			this.setZindex(this._objects[data.group][data.id],data.zindex);
		return this._objects[data.group][data.id];
	},
	groupIsEmpty:function(gid) { for (var i in this._objects[gid]) return false; return true; },
	createCanvas:function(id,data) {
		this.deleteCanvas(id);
		this._canvas[id]=document.createElement("canvas");
		this._canvas[id].setAttribute('height',(data&&data.h?data.h:this._screenh));
		this._canvas[id].setAttribute('width',(data&&data.w?data.w:this._screenw));
	},
	deleteCanvas:function(id) {
		if (this._canvas[id]) delete this._canvas[id];	
	},
	imageIsLoaded:function(id){ return this._images[id]&&(this._images[id].getAttribute("wasloaded"))&&this._images[id].width },
	getImage:function(id){return this._images[id]},
	getBuffer:function(id){return this.getCanvas("_buffer")},
	getBufferContext:function(id){ return (gbox._fskid>=gbox._frameskip?(this._db?this.getCanvasContext("_buffer"):this._screen.getContext("2d")):null) },
	getCanvas:function(id){return this._canvas[id]},
	getCanvasContext:function(id){return this.getCanvas(id).getContext("2d");},
	addImage:function(id,filename) {
		if (this._images[id])
			if (this._images[id].getAttribute("src_org")==filename)
				return;
			else
				delete this._images[id];
		this._addtoloader({type:"image",id:id,filename:filename});
	},
	deleteImage:function(id) {
		delete this._images[id];
	},
	addTiles:function(t) { 
		t.tilehh=Math.floor(t.tileh/2);
		t.tilehw=Math.floor(t.tilew/2);
		this._tiles[t.id]=t;
	},
	getTiles:function(t) { return this._tiles[t] },
	getFont:function(t){ return this._fonts[t] },
	loadAll:function() {
		// Default stuff
		this.addImage("_dbf","debugfont.png");
		if (this._splash.background) this.addImage("_splash",this._splash.background);
		gbox.addFont({id:"_dbf",image:"_dbf",firstletter:" ",tileh:5,tilew:4,tilerow:16,gapx:0,gapy:0});
		if (!gbox._splash.minimalTime)
			gbox._minimalexpired=2;
		this._waitforloaded();
	},
	_implicitsargs:function(data) {
		if (data.camera) {
			data.dx-=this._camera.x;
			data.dy-=this._camera.y;	
		}
		if (data.sourcecamera) {
			data.x=this._camera.x*(data.parallaxx?data.parallaxx:1);
			data.y=this._camera.y*(data.parallaxy?data.parallaxy:1);	
		}
	},
	blitTile:function(tox,data) {
		if (tox==null) return;
		var ts=this._tiles[data.tileset];
		var img=this.getImage(ts.image);
		this._implicitsargs(data);
		tox.save();
		tox.globalAlpha=(data.alpha?data.alpha:1);
		tox.translate((data.fliph?ts.tilew:0), (data.flipv?ts.tileh:0)); tox.scale((data.fliph?-1:1), (data.flipv?-1:1));
		this._safedrawimage(tox,img, ts.gapx+(ts.tilew*(data.tile%ts.tilerow)),ts.gapy+(ts.tileh*Math.floor(data.tile/ts.tilerow)),(data.w==null?ts.tilew:data.w),(data.h==null?ts.tileh:data.h),data.dx*(data.fliph?-1:1),data.dy*(data.flipv?-1:1),(data.w?data.w:ts.tilew),(data.h?data.h:ts.tileh));
		tox.restore();
	},
	blitAll:function(tox,image,data) {
		if (tox==null) return;
		this._implicitsargs(data);
		tox.save();
		tox.globalAlpha=(data.alpha?data.alpha:1);
		tox.translate((data.fliph?image.width:0), (data.flipv?image.height:0)); tox.scale((data.fliph?-1:1), (data.flipv?-1:1));
		this._safedrawimage(tox,image, 0,0, image.width,image.height,data.dx*(data.fliph?-1:1),data.dy*(data.flipv?-1:1),image.width,image.height);
		tox.restore();
	},
	blit:function(tox,image,data) {
		if (tox==null) return;
		this._implicitsargs(data);
		tox.save();
		tox.globalAlpha=(data.alpha?data.alpha:1);
		tox.translate((data.fliph?data.dw:0), (data.flipv?data.dh:0)); tox.scale((data.fliph?-1:1), (data.flipv?-1:1));
		this._safedrawimage(tox,image,(data.x?data.x:0), (data.y?data.y:0),(data.w?data.w:data.dw),(data.h?data.h:data.dh),data.dx*(data.fliph?-1:1),data.dy*(data.flipv?-1:1),data.dw,data.dh);
		tox.restore();
	},
	blitTilemap:function(tox,data) {
		if (tox==null) return;
		var ts=this._tiles[data.tileset];
		for (var y=0;y<data.map.length;y++)
			for (var x=0;x<data.map[y].length;x++)
				if (data.map[y][x]!=null) this.blitTile(tox,{tileset:data.tileset,tile:data.map[y][x],dx:x*ts.tilew,dy:y*ts.tilew});
	},
	blitText:function(tox,data) {
		if (tox==null) return;
		data.text+=""; // Convert to string.
		var fn=this._fonts[data.font];
		var tile=0;
		this._implicitsargs(data);
		var dx=data.dx;
		var dy=data.dy;			
		if (data.valign==gbox.ALIGN_BOTTOM) dy = dy+data.dh-fn.tileh;
		else if (data.valign==gbox.ALIGN_MIDDLE) dy = dy+Math.floor(data.dh/2)-fn.tileh;
		if (data.halign==gbox.ALIGN_RIGHT) dx = dx+data.dw-(data.text.length*fn.tilew);
		else if (data.halign==gbox.ALIGN_CENTER) dx = dx+Math.floor((data.dw-(data.text.length*fn.tilew))/2);
		tox.save();
		tox.globalAlpha=(data.alpha?data.alpha:1);
		for (var y=0;y<data.text.length;y++) {
			tile=data.text.charCodeAt(y)-fn.firstascii;
			if (tile>=0) {
				if (data.clear) tox.clearRect(dx+(y*fn.tilew),dy,(data.w?data.w:fn.tilew),(data.h?data.h:fn.tileh));
				this._safedrawimage(tox,this.getImage(fn.image), fn.gapx+(fn.tilew*(tile%fn.tilerow)),
				fn.gapy+(fn.tileh*Math.floor(tile/fn.tilerow)),fn.tilew,fn.tileh,dx+(y*fn.tilew),dy,(data.w?data.w:fn.tilew),(data.h?data.h:fn.tileh));
			}
		}
		tox.restore();
	},
	blitClear:function(image,data) {
		if (image==null) return;
		if (data==null) data={x:0,y:0};
		this._implicitsargs(data);
		image.clearRect(data.x,data.y,(data.w==null?image.canvas.width:data.w),(data.h==null?image.canvas.height:data.h));
	},
	blitImageToScreen:function(image) {
		this._screen.getContext("2d").drawImage(image,0,0);
	},
	blitFade:function(tox,data) { 
		if (tox) this.blitRect(tox,{x:0,y:0,w:tox.canvas.width,h:tox.canvas.height,alpha:data.alpha,color:data.color});
	},
	blitRect:function(tox,data) {
		if (tox==null) return;
		tox.save();
		tox.globalAlpha=(data.alpha?data.alpha:1);
		tox.fillStyle = (data.color?data.color:gbox.COLOR_BLACK);
		tox.fillRect(data.x,data.y,data.w,data.h);
		tox.restore();
	},
	collides:function(o1,o2,t) {
		if (!t) t=0;
		return !((o1.y+o1.h-1-t<o2.y+t) || (o1.y+t> o2.y+o2.h-1-t) || (o1.x+o1.w-1-t<o2.x+t) || (o1.x+t>o2.x+o2.w-1-t));
	},
	pixelcollides:function(o1,o2,t) {
		if (!t) t=0;
		return !((o1.y<o2.y+t) || (o1.y> o2.y+o2.h-1-t) || (o1.x<o2.x+t) || (o1.x>o2.x+o2.w-1-t));
	},
	objectIsVisible:function(obj) { return this.collides(obj,this._camera,0); },
	
	// --- 
	// --- 
	// ---  AUDIO ENGINE
	// --- 
	// --- 
	
	_audiochannels:{},
	_audiomastervolume:1.0,
	_canaudio:false,
	_audiodequeuetime:0,
	_audioprefetch:0.5,
	_audiocompatmode:0, // 0: pause/play, 1: google chrome compatibility, 2: ipad compatibility (single channel)
	_createmode:0, // 0: clone, 1: rehinstance
	_fakecheckprogressspeed:100, // Frequency of fake audio monitoring
	_fakestoptime:1, // Fake audio stop for compatibility mode
	_audioteam:2,
	_loweraudioteam:1,
	_audio:{lding:null,qtimer:false,aud:{},ast:{}},
	_audioactions:[],
	_showplayers:false,
	_singlechannelname:"bgmusic",
	_positiondelay:0,
	_playerforcer:0,
	_audiomutevolume:0.0001, // Zero is still not accepted by everyone :(
	_rawstopaudio:function(su) {
		if (gbox._audiocompatmode==1) {
			if (su.duration-su.currentTime>gbox._fakestoptime)
				su.currentTime=su.duration-gbox._fakestoptime;
			su.muted=true;
		} else
			su.pause();

	},
	_rawplayaudio:function(su) {
		if (gbox._audiocompatmode==1) {
			try { su.currentTime=0; } catch (e) {}
			su.muted=false;
			su.play();
		} else if (gbox._audiocompatmode==2) {
			su.load();
			gbox._playerforcer=setInterval(function(e){try{su.play();clearInterval(gbox._playerforcer)}catch(e){}},1000);
		} else {
			try { su.currentTime=0; } catch (e) {}
			su.play();
		}
	},
	_finalizeaudio:function(ob,who,donext){
	
		var cur=(who?who:this);
		cur.removeEventListener('ended', gbox._finalizeaudio,false);
		cur.removeEventListener('timeupdate', gbox._checkprogress,false);
		
		cur.addEventListener('ended', gbox._playbackended,false);
		if (donext) gbox._loaderloaded();
	},
	_audiodoload:function() {
		if (gbox._audiocompatmode==1) gbox._audio.lding.muted=true;
		else if (gbox._audiocompatmode==2)
			gbox._finalizeaudio(null,gbox._audio.lding,true);
		else {
			gbox._audio.lding.load();
			gbox._audio.lding.play();
		}
	},
	_timedfinalize:function() {
		gbox._rawstopaudio(gbox._audio.lding);
		gbox._finalizeaudio(null,gbox._audio.lding,true);	
	},
	_checkprogress:function() {
		if (gbox._audio.lding.currentTime>gbox._audioprefetch) gbox._timedfinalize();
	},
	_fakecheckprogress:function() {
		if (gbox._audio.lding.currentTime>gbox._audioprefetch) gbox._timedfinalize(); else setTimeout(gbox._fakecheckprogress,gbox._fakecheckprogressspeed);	
	},
	_audiofiletomime:function(f) {
		var fsp=f.split(".");
		switch (fsp.pop().toLowerCase()) {
			case "ogg": { return "audio/ogg"; break }
			case "mp3": { return "audio/mpeg"; break }
			default: {
				return "audio/mpeg";
			}
		}
	},
	_pushaudio:function(){try {this.currentTime=1.0} catch(e){} },
	_createnextaudio:function(cau) {
		if (cau.def) {
			gbox.deleteAudio(cau.id);
			this._audio.aud[cau.id]=[];
			this._audio.ast[cau.id]={cy:-1,volume:1,channel:null,play:false,mute:false,filename:cau.filename[0]};
			if (cau.def) for (var a in cau.def) this._audio.ast[cau.id][a]=cau.def[a];
		}
		if ((gbox._createmode==0)&&(cau.team>0)) {
			var ael =this._audio.aud[cau.id][0].cloneNode(true);
			gbox._finalizeaudio(null,ael,false);
		} else {
			var ael=document.createElement('audio');
			ael.volume=gbox._audiomutevolume;
		}
		if (!gbox._showplayers) {
			ael.style.display="none";
			ael.style.visibility="hidden";
			ael.style.width="1px";
			ael.style.height="1px";
			ael.style.position="absolute";
			ael.style.left="0px";
			ael.style.top="-1000px";
		}
		ael.setAttribute('controls',gbox._showplayers);
		ael.setAttribute('aki_id',cau.id);
		ael.setAttribute('aki_cnt',cau.team);
		ael.addEventListener('loadedmetadata', gbox._pushaudio,false); // Push locked audio in safari
		if (((gbox._createmode==0)&&(cau.team==0))||(gbox._createmode==1)) {
			if (ael.canPlayType) {
				var cmime;
				for (var i=0;i<cau.filename.length;i++) {
					cmime=gbox._audiofiletomime(cau.filename[i]);
					if (("no" != ael.canPlayType(cmime)) && ("" != ael.canPlayType(cmime))) {
						ael.src=gbox._breakcacheurl(cau.filename[i]);
						break;
					}
				}
			} else {
				for (var i=0;i<cau.filename.length;i++) {
					var src=document.createElement('source');
					src.setAttribute('src', gbox._breakcacheurl(cau.filename[i]));
					ael.appendChild(src);
				}
			}
			ael.addEventListener('ended',this._finalizeaudio,false);
			if (gbox._audiocompatmode==1)
				setTimeout(gbox._fakecheckprogress,gbox._fakecheckprogressspeed);
			else
				ael.addEventListener('timeupdate',this._checkprogress,false);
			ael.setAttribute('buffering',"auto");
			ael.volume=0;
			this._audio.aud[cau.id].push(ael);
			document.body.appendChild(ael);
			gbox._audio.lding=ael;
			setTimeout(gbox._audiodoload,1);
		} else {
			this._audio.aud[cau.id].push(ael);
			document.body.appendChild(ael);
			gbox._loaderloaded();
		}
	},
	_playbackended:function(e) {
		if (gbox._audio.ast[this.getAttribute('aki_id')].cy==this.getAttribute('aki_cnt')) {
			if (gbox._audio.ast[this.getAttribute('aki_id')].play&&gbox._audio.ast[this.getAttribute('aki_id')].loop)
				if (gbox._audiocompatmode==2)
					gbox._rawplayaudio(this);
				else
					this.currentTime=0;
			else
				gbox._audio.ast[this.getAttribute('aki_id')].play=false; 
		} else if (gbox._audiocompatmode==1) {
			this.pause();
			this.muted=false;
		}
	},
	_updateaudio:function(a) {
		if (this._audio.ast[a].play) {
			this._audio.aud[a][this._audio.ast[a].cy].volume=(this._audio.ast[a].mute?this._audiomutevolume: 
				this._audiomastervolume*
				(this._audio.ast[a].volume!=null?this._audio.ast[a].volume:1)*
				((this._audio.ast[a].channel!=null)&&(this._audiochannels[this._audio.ast[a].channel]!=null)&&(this._audiochannels[this._audio.ast[a].channel].volume!=null)?this._audiochannels[this._audio.ast[a].channel].volume:1)
			)
		}
	},
	_minimaltimeexpired:function() { gbox._minimalexpired=2; },
	_splashscreeniscompleted:function() { return (gbox._splash.background?gbox.imageIsLoaded("_splash"):true) && (gbox._splash.minilogo?gbox.imageIsLoaded("logo"):true) && (gbox._splash.footnotes?gbox.imageIsLoaded("_dbf"):true) },
	_addqueue:function(a) {
		if (!gbox._audiodequeuetime)
			gbox._dequeueaudio(null,a);
		else {
			gbox._audioactions.push(a);
			if (!gbox._audio.qtimer) {
				gbox._audio.qtimer=true;
				setTimeout(gbox._dequeueaudio,gbox._audiodequeuetime);
			}
		}
	},
	_dequeueaudio:function(k,rt) {
			var ac=(rt?rt:gbox._audioactions.pop());
			switch (ac.t) {
				case 0: {
					gbox._updateaudio(ac.a.getAttribute("aki_id"));
					gbox._rawplayaudio(ac.a);
					break
				}
				case 1: {
					gbox._rawstopaudio(ac.a);
					break;
				}
				case 2: {
					gbox._updateaudio(ac.a.getAttribute("aki_id"));
					break;
				}
			}
			if (!rt&&gbox._audioactions.length) {
				gbox._audio.qtimer=true;
				setTimeout(gbox._dequeueaudio,gbox._audiodequeuetime);
			} else gbox._audio.qtimer=false;
	
	},
	getAudioIsSingleChannel:function() { return this._audiocompatmode==2; },
	setAudioPositionDelay:function(m) { gbox._positiondelay=m },
	setAudioDequeueTime:function(m) { gbox._audiodequeuetime=m },
	setShowPlayers:function(m) { gbox._showplayers=m},
	setAudioCompatMode:function(m) { gbox._audiocompatmode=m },
	setAudioCreateMode:function(m) { gbox._createmode=m },
	addAudio:function(id,filename,def) {
		if (gbox._canaudio) {
			if (gbox._audio.aud[id])
				if (gbox._audio.ast[id].filename==filename[0])
					return;
				else
					gbox.deleteAudio(id);
			if ((gbox._audiocompatmode!=2)||(def.channel==gbox._singlechannelname)) {
				var grsize=(def.channel==gbox._singlechannelname?gbox._loweraudioteam:(def.background?gbox._loweraudioteam:gbox._audioteam));
				for (var i=0;i<grsize;i++)
					gbox._addtoloader({type:"audio",data:{id:id,filename:filename,def:(i==0?def:null),team:i}});
			}
		}
	},
	deleteAudio:function(id) {
		if (gbox._audio.aud[id]) {
			for (var i=0;i<gbox._audio.aud[id].length;i++) {
				try {document.body.removeChild(gbox._audio.aud[id][i]);}catch(e){}
				delete gbox._audio.aud[id][i];
			}
			delete gbox._audio.aud[id];
			if (gbox._audio.ast[id]) delete gbox._audio.ast[id];
		}
	},
	playAudio:function(a,data) {
		if (this._canaudio&&this._audio.ast[a])
			if (!this._audio.ast[a].play) this.hitAudio(a,data);
	},
	hitAudio:function(a,data) {
		if (this._canaudio&&this._audio.ast[a]) {
			var ael;
			if (this._audio.ast[a].cy!=-1)
				this.stopAudio(a,true);
			this._audio.ast[a].cy=(this._audio.ast[a].cy+1)%this._audio.aud[a].length;
			ael=this._audio.aud[a][this._audio.ast[a].cy];
			if (data) 
				for (var n in data) this._audio.ast[a][n]=data[n];
			this._audio.ast[a].play=true;
			this._addqueue({t:0,a:ael});
		}
	},
	stopAudio:function(a,permissive) {
		if (this._canaudio) {
			var ael;
			if (this._canaudio&&this._audio.ast[a]&&this._audio.ast[a].play) {
				this._audio.ast[a].play=false;
				ael=this._audio.aud[a][this._audio.ast[a].cy];
				if (ael.duration-1.5>0)
					this._addqueue({t:1,a:ael});
			}
		}
	},
	setSplashSettings:function(a) { for (var n in a) this._splash[n]=a[n]; },
	resetChannel:function(ch) {
		if (this._canaudio&&this._audiochannels[ch])
			if (ch=="master")
				for (var ch in this._audiochannels)
					this.setChannelVolume(ch,this._audiochannels[ch]._def.volume);
			else if (this._audiochannels[ch])
				this.setChannelVolume(ch,this._audiochannels[ch]._def.volume);
	},
	getChannelDefaultVolume:function(ch) {
		if (this._canaudio&&this._audiochannels[ch]) return this._audiochannels[ch]._def.volume; else return null;
	},
	setChannelVolume:function(ch,a) {
		if (this._canaudio&&this._audiochannels[ch]) {
			if (ch=="master") this._audiomastervolume=a; else this._audiochannels[ch].volume=a
			for (var j in gbox._audio.aud)
				if (this._audio.ast[j].cy>-1) this._updateaudio(j);
		}
	},
	getChannelVolume:function(ch) { if (ch=="master") return this._audiomastervolume; else if (this._audiochannels[ch]) return this._audiochannels[ch].volume; else return 0 },
	changeChannelVolume:function(ch,a) {
		if (this._canaudio&&this._audiochannels[ch]) {
			var vol=this.getChannelVolume(ch)+a;
			if (vol>1) vol=1; else if (vol<0) vol=0;
			this.setChannelVolume(ch,vol);
		}
	},
	stopChannel:function(ch) {
		if (this._canaudio)
			for (var j in gbox._audio.aud)
				if (this._audio.ast[j].cy>-1&&gbox._audio.ast[j].play&&((ch=="master")||(this._audio.ast[j].channel==ch)))
					this.stopAudio(j);
	},
	
	setAudioUnmute:function(a) { if (this._canaudio&&this._audio.ast[a]) { this._audio.ast[a].mute=false; this._updateaudio(a); } },
	setAudioMute:function(a) { if (this._canaudio&&this._audio.ast[a]) { this._audio.ast[a].mute=true; this._updateaudio(a); } },
	getAudioMute:function(a) { if (this._canaudio&&this._audio.ast[a]) return this._audio.ast[a].mute; else return null},
	
	setAudioVolume:function(a,vol) { if (this._canaudio&&this._audio.ast[a]) { this._audio.ast[a].volume=vol; this._updateaudio(a); } },
	getAudioVolume:function(a,vol) { if (this._canaudio&&this._audio.ast[a]) return this._audio.ast[a].volume; else return null},
	
	setAudioPosition:function(a,p) {  if (this._canaudio&&this._audio.ast[a]&&this._audio.aud[a][this._audio.ast[a].cy]) this._audio.aud[a][this._audio.ast[a].cy].currentTime=p;},
	getAudioPosition:function(a) {if (this._canaudio&&this._audio.ast[a]&&this._audio.aud[a][this._audio.ast[a].cy]) if (this._audio.aud[a][this._audio.ast[a].cy].currentTime>this._positiondelay) return this._audio.aud[a][this._audio.ast[a].cy].currentTime-this._positiondelay; else return 0; else return 0},

	getAudioDuration:function(a) {if (this._canaudio&&this._audio.ast[a]&&this._audio.aud[a][this._audio.ast[a].cy]) return this._audio.aud[a][this._audio.ast[a].cy].duration; else return 0},

	changeAudioVolume:function(a,vol) { if (this._canaudio&&this._audio.ast[a]) { if (this._audio.ast[a].volume+vol>1) this._audio.ast[a].volume=1; else  if (this._audio.ast[a].volume+vol<0) this._audio.ast[a].volume=0; else this._audio.ast[a].volume+=vol; this._updateaudio(a); } },
	setCanAudio:function(a) { this._canaudio=!this._flags.noaudio&&a;},
	setAudioChannels:function(a){
		this._audiochannels=a;
		for (var ch in a) {
			this._audiochannels[ch]._def={};
			for (var attr in this._audiochannels[ch])
				if (attr!="_def") this._audiochannels[ch]._def[attr]=this._audiochannels[ch][attr];
		}
	},
	setAudioTeam:function(a){ this._audioteam=a; },
	setLowerAudioTeam:function(a){ this._loweraudioteam=a; },
	
	// ---
	// ---
	// ---  DYNAMIC SCRIPT INCLUSION
	// ---
	// ---
	
	addScript:function(call) {
		gbox._addtoloader({type:"script",call:call});
	},
	
	// ---
	// ---
	// ---  BUNDLES
	// ---
	// ---
	
	addBundle:function(call){
		gbox._addtoloader({type:"bundle",call:call});
	},
	
	readBundleData:function(pack,call) {
		// Local resources first
		if (pack.setObject) for (var i=0;i<pack.setObject.length;i++) eval("("+pack.setObject[i].object+")")[pack.setObject[i].property]=pack.setObject[i].value;
		if (pack.addFont) for (var i=0;i<pack.addFont.length;i++) gbox.addFont(pack.addFont[i]);
		if (pack.addTiles) for (var i=0;i<pack.addTiles.length;i++) gbox.addTiles(pack.addTiles[i]);
		// Remote resources for last
		if (pack.addImage) for (var i=0;i<pack.addImage.length;i++) gbox.addImage(pack.addImage[i][0],pack.addImage[i][1]);
		if (pack.addAudio) for (var i=0;i<pack.addAudio.length;i++) gbox.addAudio(pack.addAudio[i][0],pack.addAudio[i][1],pack.addAudio[i][2]);
		if (pack.addBundle) for (var i=0;i<pack.addBundle.length;i++) gbox.addBundle(pack.addBundle[i]);
		if (pack.addScript) for (var i=0;i<pack.addScript.length;i++) gbox.addScript(pack.addScript[i]);
		// Trigger the onLoad events in resource and loader
		if (pack.onLoad) gbox._addtoloader({type:"exec-onl",func:pack.onLoad,call:call,pack:pack});				
		if (call.onLoad) gbox._addtoloader({type:"exec-onl",func:call.onLoad,call:call,pack:pack});	
	},
	
	// --- 
	// --- 
	// ---  DATA LOADER
	// --- 
	// --- 

	_xmlhttp:null,
	_loaderqueue:cyclelist.create(200),
	_loadercache:cachelist.create(30),
	
	// Callback for loaded image
	_loaderimageloaded:function() {
		this.setAttribute('wasloaded',true);
		this.hheight=Math.floor(this.height/2);
		this.hwidth=Math.floor(this.width/2);
		gbox._loaderloaded();	
	},
	// Callback for loaded bundle
	_loaderhmlhttploading:function(){
		if(this.readyState == 4 && (this.status == 0||this.status == 200)) {
			if (this.responseText) {
				if (!gbox._loaderqueue.getCurrent().call.skipCacheSave)
					gbox._loadercache.add(gbox._loaderqueue.getCurrent().call.file,this.responseText);
				var pack=eval("("+this.responseText+")");
				gbox.readBundleData(pack,gbox._loaderqueue.getCurrent().call);
				// Keep loading the other resources.
				gbox._loaderloaded();
			}	
		}
	},
	
	// Loader code
	_addtoloader:function(d) { // type:xx, data:yy
		gbox._loaderqueue.push(d);
		if (!gbox._loaderqueue.isProcessing())
			gbox._loadnext();
	},
	_loaderloaded:function() {
		setTimeout(gbox._loadnext,10);
	},
	_loaderscript:function() {
		if (gbox._loaderqueue.getCurrent().call.onLoad) gbox._addtoloader({type:"exec-onl",func:gbox._loaderqueue.getCurrent().call.onLoad,call:gbox._loaderqueue.getCurrent().call});
		gbox._loadnext();
	},
	_loadnext:function() {
		var current=gbox._loaderqueue.pop();
		if (gbox._loaderqueue.isProcessing()) {
			switch (gbox._loaderqueue.getCurrent().type) {
				case "image":{
					gbox._images[current.id]=new Image();
					gbox._images[current.id].addEventListener('load', gbox._loaderimageloaded,false);
					gbox._images[current.id].src=gbox._breakcacheurl(current.filename);
					gbox._images[current.id].setAttribute('src_org',current.filename);
					gbox._images[current.id].setAttribute('id',current.id);
					gbox._images[current.id].setAttribute('wasloaded',false);
					break;
				}
				case "bundle":{
					var done=false;
					if (!current.call.skipCacheLoad) {
						var data=gbox._loadercache.read(current.call.file);
						if (data) {
							var pack=eval("("+data+")");
							gbox.readBundleData(pack,current.call);
							// Keep loading the other resources.
							gbox._loaderloaded();
							done=true;
						}
					}
					if (!done) {
						gbox._xmlhttp=new XMLHttpRequest();
						gbox._xmlhttp.open((current.call.data?"POST":"GET"), gbox._breakcacheurl(current.call.file),true);
						gbox._xmlhttp.onreadystatechange = gbox._loaderhmlhttploading;
						if (current.call.data) {
							gbox._xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
							gbox._xmlhttp.send(current.call.data);
						} else gbox._xmlhttp.send();
					}
					break;
				}
				case "audio":{
					gbox._createnextaudio(current.data);
					break;
				}
				case "exec-onl":{
					current.func(current.call,current.pack);
					gbox._loaderloaded();
					break;
				}
				case "script":{
					var script= document.createElement('script');
					script.type="text/javascript";
					script.onload=gbox._loaderscript;
					script.src=current.call.file;
					document.getElementsByTagName('body')[0].appendChild(script);
					break;
				}
			}
		}
	
	},
	_waitforloaded:function() {
		var aul;
		if (gbox._loaderqueue.isBusy()||(gbox._minimalexpired!=2)) {
			var tox=gbox._screen.getContext("2d");
			tox.save();
			gbox.blitFade(tox,{alpha:1});
			if (!gbox._minimalexpired&&gbox._splashscreeniscompleted()) {
				gbox._minimalexpired=1;
				setTimeout(gbox._minimaltimeexpired,gbox._splash.minimalTime);
			}
			if (gbox._splash.loading) gbox._splash.loading(tox,gbox._loaderqueue.getDone(),gbox._loaderqueue.getTotal());
			if (gbox._splash.background&&gbox.imageIsLoaded("_splash"))
				gbox.blit(tox,gbox.getImage("_splash"),{w:gbox.getImage("_splash").width,h:gbox.getImage("_splash").height,dx:0,dy:0,dw:gbox.getScreenW(),dh:gbox.getScreenH()});
			if (gbox._splash.minilogo&&gbox.imageIsLoaded("logo")) {
				var dw=gbox.getScreenW()/4;
				var dh=(gbox.getImage("logo").height*dw)/gbox.getImage("logo").width
				gbox.blit(tox,gbox.getImage(gbox._splash.minilogo),{w:gbox.getImage("logo").width,h:gbox.getImage("logo").height,dx:gbox.getScreenW()-dw-5,dy:gbox.getScreenH()-dh-5,dw:dw,dh:dh});
			}
			if (gbox._splash.footnotes&&gbox.imageIsLoaded("_dbf")) {
				if (!gbox.getCanvas("_footnotes")) {
					var fd=gbox.getFont("_dbf");
					gbox.createCanvas("_footnotes",{w:gbox.getScreenW()-5,h:(gbox._splash.footnotes.length)*(fd.tileh+gbox._splash.footnotesSpacing)});
					for (var i=0;i<gbox._splash.footnotes.length;i++)
						gbox.blitText(gbox.getCanvasContext("_footnotes"),{
										font:"_dbf",
										dx:0,
										dy:i*(fd.tileh+gbox._splash.footnotesSpacing),
										text:gbox._splash.footnotes[i]
									});
				}
				gbox.blitAll(tox,gbox.getCanvas("_footnotes"),{dx:5,dy:gbox.getScreenH()-gbox.getCanvas("_footnotes").height-5});
			}
			if (gbox._loaderqueue.isBusy()) {
				var bw=Math.floor(((gbox.getScreenW()-4)*gbox._loaderqueue.getDone())/gbox._loaderqueue.getTotal());
				tox.globalAlpha=1;
				tox.fillStyle = gbox._splash.gaugeBorderColor;
				tox.fillRect(0,Math.floor((gbox.getScreenH()-gbox._splash.gaugeHeight)/2),gbox.getScreenW(),gbox._splash.gaugeHeight);
				tox.fillStyle = gbox._splash.gaugeBackColor;
				tox.fillRect(1,Math.floor(((gbox.getScreenH()-gbox._splash.gaugeHeight)/2)+1),gbox.getScreenW()-4,gbox._splash.gaugeHeight-2);
				tox.fillStyle = gbox._splash.gaugeColor;
				tox.fillRect(1,Math.floor(((gbox.getScreenH()-gbox._splash.gaugeHeight)/2)+1),(bw>0?bw:0),gbox._splash.gaugeHeight-2);
			}
			tox.restore();		
			gbox.setStatBar("Loading... ("+gbox._loaderqueue.getDone()+"/"+gbox._loaderqueue.getTotal()+")");
			setTimeout(gbox._waitforloaded,50);
		} else {
			gbox.deleteImage("_splash");
			gbox.setStatBar();
			gbox._cb();
		}
	},
	clearCache:function() { this._loadercache.clear(); }

};

// ----------------------------------------------------------------------------
// iphofretboard
// ----------------------------------------------------------------------------

/*
Fretboard module provides a touchpad for touch-based device (for now, Android and iDevices).
*/
var iphofretboard={
	_swap:false,
	_frets:["a","b","c","left","right"],
	_move:["up","down"],
	_buttons:{up:false,down:false,left:false,right:false,a:false,b:false,c:false},
	_fretw:0,
	_moveh:0,
	_gapx:0,
	_gapy:0,
	_listen:function(e) {
		var nc={up:false,down:false,left:false,right:false,a:false,b:false,c:false};
		var fret;
		for (var i=0;i<e.touches.length;i++) {
			fret=Math.floor((e.touches[i].pageX-iphofretboard._gapx)/iphofretboard._fretw);
			if (fret<iphofretboard._frets.length)			
				nc[iphofretboard._frets[fret]]=true;
			else
				nc[iphofretboard._move[Math.floor((e.touches[i].pageY-iphofretboard._gapy)/iphofretboard._moveh)]]=true;
		}
		this._swap=!this._swap;
		for (var i in nc) {
			if (nc[i]!=iphofretboard._buttons[i])
				if (nc[i]) gbox._keydown({fake:true,keyCode:gbox._keymap[i]});
				else gbox._keyup({fake:true,keyCode:gbox._keymap[i]});
		}
		
		iphofretboard._buttons=nc;
	},
	initialize:function(data) {
		var oElement=document.createElement("img");
		oElement.src=data.bg;		
		oElement.style.margin="auto";
		oElement.style.padding="0px";
		oElement.style.height=data.h+"px";
		oElement.style.width="100%";
		
		gbox._box.appendChild(oElement);

		oElement.ontouchstart=function(evt) { evt.preventDefault();evt.stopPropagation(); iphofretboard._listen(evt) };
		oElement.ontouchend=function(evt) { evt.preventDefault();evt.stopPropagation();iphofretboard._listen(evt) };
		oElement.ontouchmove=function(evt) { evt.preventDefault();evt.stopPropagation();iphofretboard._listen(evt) };

		var sizes=gbox._domgetabsposition(oElement);
		this._gapx=sizes.x;
		this._gapy=sizes.y;
		this._fretw=sizes.w/(this._frets.length+1);
		this._moveh=sizes.h/(this._move.length);
	}
	
}

// ----------------------------------------------------------------------------
// trigo
// ----------------------------------------------------------------------------

/*
Trigo module provides some math stuff for moving objects in a 
direction or following a round path.
*/

var trigo={
	ANGLE_RIGHT:0,
	ANGLE_DOWN:Math.PI*0.5,
	ANGLE_LEFT:Math.PI,
	ANGLE_UP:Math.PI*1.5555555,
	addAngle:function(a,add) {
		a=(a+add)%(Math.PI*2);
		if (a<0) return (Math.PI*2)+a; else return a;
	},
	getDistance:function(p1,p2) {
		return Math.sqrt(Math.pow(p2.x-p1.x,2)+Math.pow(p2.y-p1.y,2))
	},
	getAngle:function(p1,p2,transl) {
		return this.addAngle(Math.atan2(p2.y-p1.y,p2.x-p1.x),(transl?transl:0));
	},
	translate:function(p1,a,d) {
		p1.x=p1.x+Math.cos(a)*d;
		p1.y=p1.y+Math.sin(a)*d;
	},
	translateX:function(x1,a,d) {
		return x1+Math.cos(a)*d
	},
	translateY:function(y1,a,d) {
		return y1+Math.sin(a)*d;
	}
}

// ----------------------------------------------------------------------------
// toys
// ----------------------------------------------------------------------------

/*
Toys module provides lots of common routines during the game developing: 
from effects for screen titles to HUD handling to platform/SHMUP/RPG oriented routines, 
like jumping characters, Z-Indexed objects, bullets, sparks, staff rolls, bonus screens, dialogues etc.
*/

var toys={

	// CONSTANTS
	NOOP:function(){},
	PUSH_NONE:0,
	PUSH_LEFT:1,
	PUSH_RIGHT:2,
	PUSH_UP:3,
	PUSH_DOWN:4,
	
	FACES:["up","right","down","left"],
	FACES_ANGLE:[trigo.ANGLE_UP,trigo.ANGLE_RIGHT,trigo.ANGLE_DOWN,trigo.ANGLE_LEFT],
	FACE_UP:0,
	FACE_RIGHT:1,
	FACE_DOWN:2,
	FACE_LEFT:3,
	
	// Top-view (Zelda-alike) specifics
	
	topview:{
		collides:function(fr,to,t) { // Special collision. Counts also the Z
			if (Math.abs(fr.z,to.z)<5) return gbox.collides({x:fr.x+fr.colx,y:fr.y+fr.coly,h:fr.colh,w:fr.colw},{x:to.x+to.colx,y:to.y+to.coly,h:to.colh,w:to.colw},t); else return false;
		},
		pixelcollides:function(fr,to,t) { // Special collision. Counts also the Z
			return gbox.pixelcollides(fr,{x:to.x+to.colx,y:to.y+to.coly,h:to.colh,w:to.colw},t);
		},
		initialize:function(th,data) {
			help.mergeWithModel(
				th,
				help.mergeWithModel(
					data,
					{
						x:0, y:0,
						z:0,
						accx:0, accy:0, accz:0,
						frames:{},
						shadow:null,
						maxacc:4, controlmaxacc:4,
						responsive:0, // Responsiveness
						weapon:0, // Weapon
						camera:true,
						flipv:false, fliph:false,
						facing:toys.FACE_DOWN,
						flipside:true,
						haspushing:false,
						frame:0,
						colh:gbox.getTiles(th.tileset).tilehh,
						colw:gbox.getTiles(th.tileset).tilew,
						colx:0,
						staticspeed:0,
						nodiagonals:false,
						noreset:false
					}
				)
			);
			if (th.coly==null) th.coly=gbox.getTiles(th.tileset).tileh-th.colh;
			th.colhh=Math.floor(th.colh/2);
			th.colhw=Math.floor(th.colw/2);
			
			toys.topview.spawn(th);
		},
		spawn:function(th,data) {
			th.xpushing=toys.PUSH_NONE; // user is moving side
			th.vpushing=toys.PUSH_NONE; // user is moving side
			th.zpushing=toys.PUSH_NONE; // user is moving side
			th.counter=0; // self counter
			th.hittimer=0;
			th.killed=false;				
			help.copyModel(th,data);
			gbox.setZindex(th,th.y+th.h); // these object follows the z-index and uses ZINDEX_LAYER
		},
		controlKeys:function(th,keys) {
			var cancelx=false;
			var cancely=false;
			var idlex=false;
			var idley=false;
			
			if (gbox.keyIsPressed(keys.left)||keys.pressleft) {
				th.xpushing=toys.PUSH_LEFT;
				th.facing=toys.FACE_LEFT;
				if (th.accx>th.responsive) th.accx=th.responsive;
				if (th.staticspeed) th.accx=-th.staticspeed; else th.accx=help.limit(th.accx-1,-th.controlmaxacc,th.controlmaxacc);
				if (th.nodiagonals) { cancely=true; idley=true }
			} else if (gbox.keyIsPressed(keys.right)||keys.pressright) {
				th.xpushing=toys.PUSH_RIGHT;
				th.facing=toys.FACE_RIGHT;
				if (th.accx<-th.responsive) th.accx=-th.responsive;
				if (th.staticspeed) th.accx=th.staticspeed; else th.accx=help.limit(th.accx+1,-th.controlmaxacc,th.controlmaxacc);
				if (th.nodiagonals) { cancely=true; idley=true }
			} else idlex=true;
			
			if (!cancely&&(gbox.keyIsPressed(keys.up)||keys.pressup)) {
				th.ypushing=toys.PUSH_UP;
				th.facing=toys.FACE_UP;
				if (th.accy>th.responsive) th.accy=th.responsive;
				if (th.staticspeed) th.accy=-th.staticspeed; else th.accy=help.limit(th.accy-1,-th.controlmaxacc,th.controlmaxacc);
				if (th.nodiagonals) { cancelx=true; idlex=true; }
			} else if (!cancely&&(gbox.keyIsPressed(keys.down)||keys.pressdown)) {
				th.ypushing=toys.PUSH_DOWN;
				th.facing=toys.FACE_DOWN;
				if (th.accy<-th.responsive) th.accy=-th.responsive;
				if (th.staticspeed) th.accy=th.staticspeed; else th.accy=help.limit(th.accy+1,-th.controlmaxacc,th.controlmaxacc);
				if (th.nodiagonals) { cancelx=true; idlex=true; }
			} else idley=true;
			
			
			
			 if (idlex) {
				if (cancelx) th.accx=0;
				if (cancelx||!th.noreset) th.xpushing=toys.PUSH_NONE;
			}
			if (idley) {
				if (cancely) th.accy=0;				
				if (cancely||!th.noreset) th.ypushing=toys.PUSH_NONE;
			}
		},
		getNextX:function(th) { return th.x+help.limit(th.accx,-th.maxacc,th.maxacc); },
		getNextY:function(th) { return th.y+help.limit(th.accy,-th.maxacc,th.maxacc); },
		getNextZ:function(th) { return th.z+help.limit(th.accz,-th.maxacc,th.maxacc); },
		applyForces:function(th) {
			th.x=toys.topview.getNextX(th);
			th.y=toys.topview.getNextY(th);
		},
		applyGravity:function(th) {
			th.z=toys.topview.getNextZ(th);
		},
		handleAccellerations:function(th) {
			if (!th.xpushing) th.accx=help.goToZero(th.accx);
			if (!th.ypushing) th.accy=help.goToZero(th.accy);
			
		},
		handleGravity:function(th) {
			th.accz++;
		},
		setFrame:function(th) {
			var pref="stand";
			if (th.xpushing||th.ypushing) 
				if (th.haspushing&&(th.toucheddown||th.touchedup||th.touchedleft||th.touchedright)) pref="pushing"; else pref="moving";
			if (th.flipside)
				th.fliph=(th.facing==toys.FACE_RIGHT);
			th.frame=help.decideFrame(th.counter,th.frames[pref+toys.FACES[th.facing]]);
		},
		tileCollision:function(th,map,tilemap,defaulttile,data) {
			
			th.touchedup=false;
			th.toucheddown=false;
			th.touchedleft=false;
			th.touchedright=false;
			
			var tollerance=(data&&(data.tollerance!=null)?data.tollerance:6);
			var approximation=(data&&(data.approximation!=null)?data.approximation:10);
			var t=tollerance-approximation;
			do {
				t+=approximation;
				if (t>th.colw-tollerance-1) t=th.colw-tollerance-1;
				var bottom=help.getTileInMap(th.x+th.colx+t,th.y+th.coly+th.colh-1,map,defaulttile,tilemap);
				var top=help.getTileInMap(th.x+th.colx+t,th.y+th.coly,map,defaulttile,tilemap);
				if (map.tileIsSolid(th,top)) th.touchedup=true;
				if (map.tileIsSolid(th,bottom)) th.toucheddown=true;	
			} while (t!=th.colw-tollerance-1);
			
			t=tollerance-approximation;
			do {
				t+=approximation;
				if (t>th.colh-tollerance-1) t=th.colh-tollerance-1;
				var left=help.getTileInMap(th.x+th.colx,th.y+th.coly+t,map,defaulttile,tilemap);
				var right=help.getTileInMap(th.x+th.colx+th.colw-1,th.y+th.coly+t,map,defaulttile,tilemap);
				if (map.tileIsSolid(th,left)) th.touchedleft=true;
				if (map.tileIsSolid(th,right)) th.touchedright=true;
			} while (t!=th.colh-tollerance-1);
			
			if (th.touchedup) {
				th.accy=0;
				th.y=help.yPixelToTile(map,th.y+th.coly,1)-th.coly;				
			}
			if (th.toucheddown) {
				th.accy=0;
				th.y=help.yPixelToTile(map,th.y+th.coly+th.colh-1)-th.coly-th.colh;				
			}
			if (th.touchedleft) {
				th.accx=0;
				th.x=help.xPixelToTile(map,th.x+th.colx,1)-th.colx;				
			}
			if (th.touchedright) {
				th.accx=0;
				th.x=help.xPixelToTile(map,th.x+th.colx+th.colw-1)-th.colx-th.colw;				
			}
			
		},
		spritewallCollision:function(th,data) {
			var wl;
			for (var i in gbox._objects[data.group])
				if ((!gbox._objects[data.group][i].initialize)&&toys.topview.collides(th,gbox._objects[data.group][i])) {
					wl=gbox._objects[data.group][i];
					if (toys.topview.pixelcollides({x:th.x+th.colx,y:th.y+th.coly+th.colhh},wl)) {
						th.touchedleft=true;
						th.accx=0;
						th.x=wl.x+wl.colx+wl.colw-th.colx;
					} else if (toys.topview.pixelcollides({x:th.x+th.colx+th.colw,y:th.y+th.coly+th.colhh},wl)) {
						th.touchedright=true;
						th.accx=0;
						th.x=wl.x+wl.colx-th.colw-th.colx;
					}
					if (toys.topview.pixelcollides({x:th.x+th.colx+th.colhw,y:th.y+th.coly+th.colh},wl)) {
						th.toucheddown=true;
						th.accy=0;
						th.y=wl.y+wl.coly-th.colh-th.coly;
					} else if (toys.topview.pixelcollides({x:th.x+th.colx+th.colhw,y:th.y+th.coly},wl)) {
						th.touchedup=true;
						th.accy=0;
						th.y=wl.y+wl.coly+wl.colh-th.coly;
					}
				}
						
		},
		floorCollision:function(th,data) {
			th.touchedfloor=false;
			if (th.z>0) {
				th.accz=(data==null?0:-Math.floor(th.accz/data.bounce));
				if (data&&data.audiobounce&&th.accz) gbox.hitAudio(data.audiobounce);
				th.z=0;
				th.touchedfloor=true;
			}			
		},
		adjustZindex:function(th) {
			gbox.setZindex(th,th.y+th.h);
		},
		// Helper: returns the ahead pixel (i.e. destination use action)
		getAheadPixel:function(th,data) {
			switch (th.facing) {
				case toys.FACE_RIGHT:{
					return {x:th.x+th.colx+th.colw+data.distance,y:th.y+th.coly+th.colhh};
					break;
				}
				case toys.FACE_LEFT:{
					return {x:th.x+th.colx-data.distance,y:th.y+th.coly+th.colhh};
					break;
				}
				case toys.FACE_UP:{
					return {x:th.x+th.colx+th.colhw,y:th.y+th.coly-data.distance};
					break;
				}
				case toys.FACE_DOWN:{
					return {x:th.x+th.colx+th.colhw,y:th.y+th.coly+th.colh+data.distance};
					break;
				}
			}
		},
		// Helper: trigger a method in colliding objects (i.e. "use action")
		callInColliding:function(th,data) {
			for (var i in gbox._objects[data.group])
				if ((!gbox._objects[data.group][i].initialize)&&toys.topview.pixelcollides(data,gbox._objects[data.group][i]))
					if (gbox._objects[data.group][i][data.call]) {
						gbox._objects[data.group][i][data.call](th);
						return i;
					}
			return false;
		},
		// Enemy methods
		wander:function(th,map,tilemap,defaulttile,data) {
			if ((!th.wandercounter)||(th.toucheddown||th.touchedup||th.touchedleft||th.touchedright)) {
				th.wandercounter=help.random(data.minstep,data.steprange);
				th.wanderdirection=help.random(0,4);
			} else th.wandercounter--;
			switch (th.wanderdirection) {
				case toys.FACE_LEFT: {
					th.xpushing=toys.PUSH_LEFT;
					th.ypushing=toys.PUSH_NONE;
					th.facing=toys.FACE_LEFT;
					th.accx=-data.speed;
					th.accy=0;
					break;
				}
				case toys.FACE_RIGHT: {
					th.xpushing=toys.PUSH_RIGHT;
					th.ypushing=toys.PUSH_NONE;
					th.facing=toys.FACE_RIGHT;
					th.accx=data.speed;
					th.accy=0;
					break;
				}
				case toys.FACE_UP: {
					th.ypushing=toys.PUSH_UP;
					th.xpushing=toys.PUSH_NONE;
					th.facing=toys.FACE_UP;
					th.accy=-data.speed;
					th.accx=0;
					break;
				}
				case toys.FACE_DOWN: {
					th.ypushing=toys.PUSH_DOWN;
					th.xpushing=toys.PUSH_NONE;
					th.facing=toys.FACE_DOWN;
					th.accy=data.speed;
					th.accx=0;
					break;
				}					
			}
		},
		// generators (firebullet specific for topdown - more complex than SHMUP one)
		fireBullet:function(gr,id,data) {

			var ts=gbox.getTiles(data.tileset);
			
						
			var obj=gbox.addObject(
				help.mergeWithModel(
					data,{
						_bullet:true,
						zindex:0,
						fliph:false, flipv:false,
						id:id,
						group:gr,
						cnt:0,
						tileset:"",
						frames:{},
						acc:0,
						angle:0,
						camera:data.from.camera,
						accx:(data.accx==null?Math.floor(trigo.translateX(0,data.angle,data.acc)):0),
						accy:(data.accy==null?Math.floor(trigo.translateY(0,data.angle,data.acc)):0),
						accz:0,
						x:(data.sidex==toys.FACE_LEFT?data.from.x-ts.tilehw:(data.sidex==toys.FACE_RIGHT?data.from.x+data.from.w-ts.tilehw:data.from.x+data.from.hw-ts.tilehw))+(data.gapx?data.gapx:0),
						y:(data.sidey==toys.FACE_UP?data.from.y-ts.tilehh:(data.sidey==toys.FACE_DOWN?data.from.y+data.from.h-ts.tilehh:data.from.y+data.from.hh-ts.tilehh))+(data.gapy?data.gapy:0),
						z:(data.from.z==null?0:data.from.z),
						collidegroup:"",
						spark:toys.NOOP,
						power:1,
						lifetime:null,
						tilemap:null,
						defaulttile:0,
						applyzgravity:false,
						map:null,
						defaulttile:0,
						mapindex:"",
						spritewalls:null,
						colx:(data.fullhit?0:null),
						coly:(data.fullhit?0:null),
						colh:(data.fullhit?ts.tileh:null),
						colw:(data.fullhit?ts.tilew:null),
						duration:null,
						onWallHit:function() {
							this.spark(this);
							gbox.trashObject(this);
						},
						bulletIsAlive:function() {
							return gbox.objectIsVisible(this);
						}
					}
				)
			);
			
			obj.initialize=function() {
				toys.topview.initialize(this);
			};
			
			obj[(data.logicon==null?"first":data.logicon)]=function() {
				this.cnt=(this.cnt+1)%10;
				
				if (this.applyzgravity) toys.topview.handleGravity(this); // z-gravity					
				toys.topview.applyForces(this); // Apply forces
				if (this.applyzgravity) toys.topview.applyGravity(this); // z-gravity
				if (this.map!=null) toys.topview.tileCollision(this,this.map,this.mapindex,this.defaulttile); // tile collisions
				if (this.spritewalls!=null) toys.topview.spritewallCollision(this,{group:this.spritewalls}); // walls collisions
				if (this.applyzgravity) toys.topview.floorCollision(this); // Collision with the floor (for z-gravity)
				toys.topview.adjustZindex(this);
				if (this.duration!=null) {
					this.duration--;
					if (this.duration==0) gbox.trashObject(this);
				}
				if (!this.bulletIsAlive()) gbox.trashObject(this);
				else if (this.toucheddown||this.touchedup||this.touchedleft||this.touchedright) this.onWallHit();
				else if (this.collidegroup!=null)
					for (var i in gbox._objects[this.collidegroup])
						if ((!gbox._objects[this.collidegroup][i].initialize)&&toys.topview.collides(this,gbox._objects[this.collidegroup][i],gbox._objects[this.collidegroup][i].tollerance)) {
							if (gbox._objects[this.collidegroup][i]["hitByBullet"]!=null)
								if (!gbox._objects[this.collidegroup][i].hitByBullet(this)) {
									this.spark(this);
									gbox.trashObject(this);
								}
						}
			}
			
			obj[(data.bliton==null?"blit":data.bliton)]=function() {
				if (!gbox.objectIsTrash(this))
					gbox.blitTile(gbox.getBufferContext(),{tileset:this.tileset,tile:help.decideFrame(this.cnt,this.frames),dx:this.x,dy:this.y+this.z,camera:this.camera,fliph:this.fliph,flipv:this.flipv});
			}
			
			gbox.setZindex(obj,obj.y+obj.h);
			
			return obj;
		
		},
		makedoor:function(gr,id,map,data) {

			var mts=gbox.getTiles(map.tileset);
			var ts=gbox.getTiles(data.tileset);
						
			var obj=gbox.addObject(
				help.mergeWithModel(
					data,{
						zindex:0,
						fliph:false, flipv:false,
						id:id,
						group:gr,
						cnt:0,
						tileset:"",
						frames:{},
						camera:true,
						x:data.tilex*mts.tilew,
						y:data.tiley*mts.tileh,
						z:0,
						tilemap:null,
						defaulttile:0,
						map:map,
						colx:(data.fullhit?0:null),
						coly:(data.fullhit?0:null),
						colh:(data.fullhit?ts.tileh:null),
						colw:(data.fullhit?ts.tilew:null),
						opening:false,
						doorheight:ts.tileh,
						opencounter:0,
						opening:false,
						closing:false,
						audiobefore:null,
						audioafter:null,
						doOpen:function() {
							this.opening=true;
						},
						whenClosed:toys.NOOP,
						whenOpened:toys.NOOP,
						whileMoving:toys.NOOP,
						hitByBullet:function(by) {
						
						}
					}
				)
			);
			
			// Closing animation
			if (obj.closing) obj.opencounter=obj.doorheight;
			
			obj.initialize=function() {
				this.ismoving=false;
				toys.topview.initialize(this);
			};
			
			obj[(data.logicon==null?"first":data.logicon)]=function() {
				if (this.closing) {
					if (!this.ismoving) {
						if (this.audiobefore) gbox.hitAudio(this.audiobefore);
						this.ismoving=true;
					}
					this.whileMoving();
					this.opencounter--;
					if (this.opencounter<0) {
						if (this.audioafter) gbox.hitAudio(this.audioafter);
						this.ismoving=false;
						this.opencounter=0;
						this.closing=false;
						this.whenClosed();
					}
				}
				if (this.opening) {
					if (!this.ismoving) {
						if (this.audiobefore) gbox.hitAudio(this.audiobefore);
						this.ismoving=true;
					}
					this.whileMoving();
					this.opencounter++;
					if (this.opencounter>=this.doorheight) {
						if (this.audioafter) gbox.hitAudio(this.audioafter);
						this.ismoving=false;
						if (!this.whenOpened()) gbox.trashObject(this);
					}
				}
			}
			
			obj[(data.bliton==null?"blit":data.bliton)]=function() {
				if (!gbox.objectIsTrash(this))
					gbox.blitTile(gbox.getBufferContext(),{tileset:this.tileset,tile:help.decideFrame(this.cnt,this.frames),dx:this.x,dy:this.y+this.z+this.opencounter,h:this.h-this.opencounter,camera:this.camera,fliph:this.fliph,flipv:this.flipv});
			}
			
			gbox.setZindex(obj,obj.y+obj.h);
			
			return obj;
		},
		// Set the object speed making sure that the X and Y coords are multiple of the speed. Useful on maze-based games.
		setStaticSpeed:function(th,speed) {
			th.staticspeed=speed;
			th.x=Math.round(th.x/speed)*speed;
			th.y=Math.round(th.y/speed)*speed;
		}
	},
	
	
	// Shoot'em up specifics
	shmup:{
		initialize:function(th,data) {
			help.mergeWithModel(
				th,
				help.mergeWithModel(
					data,
					{
						x:0, y:0,
						accx:0, accy:0,
						frames:{},
						maxacc:5, controlmaxacc:5,
						responsive:0, // Responsiveness
						bounds:{x:0,y:0,w:gbox.getScreenW(),h:gbox.getScreenH()}, // Bounds box (ship cannot exit from there)
						weapon:0, // Weapon
						hittime:5,
						camera:false,
						flipv:false, fliph:false,
						health:1,
						tollerance:4
					}
				)
			);
			toys.shmup.spawn(th);
		},
		spawn:function(th,data) {
			th.xpushing=toys.PUSH_NONE; // user is moving side
			th.vpushing=toys.PUSH_NONE; // user is moving side
			th.counter=0; // self counter
			th.hittimer=0;
			th.killed=false;
			help.copyModel(th,data);
		},
		getNextX:function(th) { return th.x+help.limit(th.accx,-th.maxacc,th.maxacc); },
		getNextY:function(th) { return th.y+help.limit(th.accy,-th.maxacc,th.maxacc); },
		controlKeys:function(th,keys) {
			
			if (gbox.keyIsPressed(keys.left)) {
				th.xpushing=toys.PUSH_LEFT;
				if (th.accx>th.responsive) th.accx=th.responsive;
				th.accx=help.limit(th.accx-1,-th.controlmaxacc,th.controlmaxacc);
			} else if (gbox.keyIsPressed(keys.right)) {
				th.xpushing=toys.PUSH_RIGHT;
				if (th.accx<-th.responsive) th.accx=-th.responsive;
				th.accx=help.limit(th.accx+1,-th.controlmaxacc,th.controlmaxacc);
			} else th.xpushing=toys.PUSH_NONE;
			if (gbox.keyIsPressed(keys.up)) {
				th.ypushing=toys.PUSH_UP;
				if (th.accy>th.responsive) th.accy=th.responsive;
				th.accy=help.limit(th.accy-1,-th.controlmaxacc,th.controlmaxacc);
			} else if (gbox.keyIsPressed(keys.down)) {
				th.ypushing=toys.PUSH_DOWN;
				if (th.accy<-th.responsive) th.accy=-th.responsive;
				th.accy=help.limit(th.accy+1,-th.controlmaxacc,th.controlmaxacc);
			} else th.ypushing=toys.PUSH_NONE;
		},
		applyForces:function(th) {
			th.x=toys.shmup.getNextX(th);
			th.y=toys.shmup.getNextY(th);
		},
		handleAccellerations:function(th) {
			if (!th.xpushing) th.accx=help.goToZero(th.accx);
			if (!th.ypushing) th.accy=help.goToZero(th.accy);
		},
		keepInBounds:function(th) {
			if (th.x<th.bounds.x) {
				th.x=th.bounds.x;
				th.accx=0;
			} else if (th.x+th.w>th.bounds.x+th.bounds.w) {
				th.x=th.bounds.x+th.bounds.w-th.w;
				th.accx=0;				
			}
			if (th.y<th.bounds.y) {
				th.y=th.bounds.y;
				th.accy=0;
			} else if (th.y+th.h>th.bounds.y+th.bounds.h) {
				th.y=th.bounds.y+th.bounds.h-th.h;
				th.accy=0;				
			}
		},
		setFrame:function(th) {
			if (th.hittimer) th.hittimer--;
			th.frame=help.decideFrame(th.counter,(th.hittimer?th.frames.hit:th.frames.still));
		},
		
		fireBullet:function(gr,id,data) {
		
			var ts=gbox.getTiles(data.tileset);
			
			var obj=gbox.addObject(
				help.mergeWithModel(
					data,{
						_bullet:true,
						fliph:false, flipv:false,
						id:id,
						group:gr,
						cnt:0,
						tileset:"",
						frames:{},
						acc:0,
						angle:0,
						camera:false,
						accx:(data.accx==null?Math.floor(trigo.translateX(0,data.angle,data.acc)):0),
						accy:(data.accy==null?Math.floor(trigo.translateY(0,data.angle,data.acc)):0),
						x:data.from.x+data.from.hw-ts.tilehw+(data.gapx?data.gapx:0),
						y:(data.upper?data.from.y-ts.tilehh+(data.gapy?data.gapy:0):data.from.y+data.from.h-ts.tilehh-(data.gapy?data.gapy:0)),
						collidegroup:"",
						spark:toys.NOOP,
						power:1
					}
				)
			);
			
			obj[(data.logicon==null?"first":data.logicon)]=function() {
				this.x+=this.accx;
				this.y+=this.accy;
				this.cnt=(this.cnt+1)%10;
				if (!gbox.objectIsVisible(this)) gbox.trashObject(this);
				else if (this.collidegroup!=null)
					for (var i in gbox._objects[this.collidegroup])
						if ((!gbox._objects[this.collidegroup][i].initialize)&&gbox.collides(this,gbox._objects[this.collidegroup][i],gbox._objects[this.collidegroup][i].tollerance)) {
							if (gbox._objects[this.collidegroup][i]["hitByBullet"]!=null)
								if (!gbox._objects[this.collidegroup][i].hitByBullet(this)) {
									this.spark(this);
									gbox.trashObject(this);
								}
						}
			}
			
			obj[(data.bliton==null?"blit":data.bliton)]=function() {
				gbox.blitTile(gbox.getBufferContext(),{tileset:this.tileset,tile:help.decideFrame(this.cnt,this.frames),dx:this.x,dy:this.y,camera:this.camera,fliph:this.side,flipv:this.flipv});
			}
			
			return obj;
		
		},
		
		hitByBullet:function(th,by) {
			if (by.power) {
				th.health-=by.power;
				if (th.health<=0) th.kill(by); else  th.hittimer=th.hittime;
			}
		},
		
		generateEnemy:function(gr,id,data,model) {
			help.mergeWithModel(data,model);
			var obj=gbox.addObject(
				help.mergeWithModel(
					data,{
						id:id,
						group:gr,
						cnt:0,
						tileset:"",
						frames:{},
						acc:0,
						angle:0,
						camera:false,
						fliph:false,
						flipv:false,
						accx:(data.accx==null?Math.floor(trigo.translateX(0,data.angle,data.acc)):0),
						accy:(data.accy==null?Math.floor(trigo.translateY(0,data.angle,data.acc)):0),
						x:data.x,
						y:data.y,
						// -- spec
						animationset:"still",
						defaultanimationset:"still",
						hitanimationset:"still",
						hittime:5,
						script:toys.NOOP,
						handler:toys.NOOP,
						scriptline:(data.scriptline==null?-1:data.scriptline-1),
						newline:true,
						waitframes:0,
						doframes:0,
						mode:0,
						line:{},
						dohandler:null,
						ended:false,
						health:1,
						hittimer:0,
						kill:toys.NOOP,
						tollerance:0,
						initialize:null,
						invulnerable:false,
						hitAnimation:function(time) {
							this.hittimer=(time==null?this.hittime:time);
							this.animationset=this.hitanimationset;
						},
						goTo:function(nl) { // Jump to a line
							this.waitframes=0;
							this.doframes=0;
							this.line={};
							this.scriptline=nl-1;
						},
						hitByBullet:function(by) {
							if (!this.invulnerable&&by.power) {
								this.health-=by.power;
								if (this.health<=0) this.kill(this,by); else this.hitAnimation();
							}
						},
					}
				)
			);
			
			
			obj[(data.logicon==null?"first":data.logicon)]=function() {
				if (this.initialize!=null)  {
					this.initialize(this);
					this.initialize=null;
				}
				if (!this.ended) {
					if (!this.waitframes&&!this.doframes&&((this.line.waitfor==null)||this.line.waitfor(this))) {
						this.scriptline++;
						this.everycnt=-1;
						if (this.script[this.scriptline]==null)
							this.ended=true;
						else {
							if (this.script[this.scriptline].goto!=null) this.scriptline=this.script[this.scriptline].goto;
							this.line=this.script[this.scriptline];
							if (this.line.afterframes!=null)
								this.waitframes=this.line.afterframes;
							if (this.line.forframes!=null)
								this.doframes=this.line.forframes;
							else
								this.doframes=1;
							if (this.line.cleardo)
								this.dohandler=null;
							else if (this.line.doit!=null) {
								this.dohandler={
									actiontimes:0,
									timer:(this.line.doit.every=="keep"?this.dohandler.every:this.line.doit.every),
									every:(this.line.doit.every=="keep"?this.dohandler.every:this.line.doit.every),
									once:(this.line.doit.once=="keep"?this.dohandler.once:this.line.doit.once),
									action:(this.line.doit.action=="keep"?this.dohandler.action:this.line.doit.action),
									render:(this.line.doit.render=="keep"?this.dohandler.render:this.line.doit.render)
								}
							}
								
						}
					}
					if (!this.waitframes&&this.doframes&&!this.ended) {
						this.doframes--;
						if (this.line.setinvulnerable!=null) this.invulnerable=this.line.setinvulnerable;
						if (this.line.setx!=null) this.x=this.line.setx;
						if (this.line.sety!=null) this.y=this.line.sety;
						if (this.line.addx!=null) this.x+=this.line.addx;
						if (this.line.addy!=null) this.y+=this.line.addy;
						if (this.line.setaccx!=null) this.accx=this.line.setaccx;
						if (this.line.setaccy!=null) this.accy=this.line.setaccy;
						if (this.line.setacc!=null) {
							this.acc=this.line.setacc;
							this.accx=Math.floor(trigo.translateX(0,this.angle,this.acc));
							this.accy=Math.floor(trigo.translateY(0,this.angle,this.acc));
						}
						if (this.line.addaccx!=null) this.accx+=this.line.addaccx;
						if (this.line.addaccy!=null) this.accy+=this.line.addaccy;
						if (this.line.addacc!=null) {
							this.acc+=this.line.addacc;
							this.accx=Math.floor(trigo.translateX(0,this.angle,this.acc));
							this.accy=Math.floor(trigo.translateY(0,this.angle,this.acc));
						}
						
						if (this.line.setangle!=null) {
							this.angle=this.line.setangle;
							this.accx=Math.floor(trigo.translateX(0,this.angle,this.acc));
							this.accy=Math.floor(trigo.translateY(0,this.angle,this.acc));
						}
						if (this.line.addangle!=null) {
							this.angle+=this.line.addangle;
							this.accx=Math.floor(trigo.translateX(0,this.angle,this.acc));
							this.accy=Math.floor(trigo.translateY(0,this.angle,this.acc));
						}
						if (this.line.everyframe) this.waitframes=this.line.everyframe;
							
					}
					if (this.waitframes>0) this.waitframes--;
				}
				if (this.dohandler&&(this.dohandler.action!=null)) {
					if (this.dohandler.timer==this.dohandler.every) {
						this.dohandler.action(this,this.dohandler.actiontimes);
						this.dohandler.timer=0;
						this.dohandler.actiontimes++;
					} else if (!this.dohandler.once) this.dohandler.timer++;
				}
				if (this.handler!=null) this.handler(this);
				
				if (this.hittimer) {
					this.hittimer--;
					if (!this.hittimer) this.animationset=this.defaultanimationset;
				}
				
				this.x+=this.accx;
				this.y+=this.accy;
				this.cnt=(this.cnt+1)%10;
				
			}

			obj[(data.bliton==null?"blit":data.bliton)]=function() {
				gbox.blitTile(gbox.getBufferContext(),{tileset:this.tileset,tile:help.decideFrame(this.cnt,this.frames[this.animationset]),dx:this.x,dy:this.y,camera:this.camera,fliph:this.side,flipv:this.flipv});
				if (this.dohandler&&(this.dohandler.render!=null)) this.dohandler.render(this);
			}
			
			return obj;

		},
		generateScroller:function(gr,id,data) {
			var obj=gbox.addObject(
				help.mergeWithModel(
					help.cloneObject(data),{
						id:id, group:gr,
						y:0, x:0,
						stage:{},
						speed:0,
						stop:null, // Remember to set the last stop ever! or the last loop!
						block:-1,
						bly:0,
						lblock:-1,
						lbly:0,
						lget:0,
						tbly:0,
						trb:0,
						maxwidth:0,
						loopstart:null, loopend:null, looprounds:0,
						panspeed:1, panstimer:0, destspeed:0,
						
						setLoop:function(st,en) {
							this.loopstart=st;
							this.loopend=en;
							this.lget=1;
							this.looprounds=1;
						},
						
						quitLoop:function() {
							this.setLoop(null,null);
							this.looprounds=0;
						},
					
						setSpeed:function(s) {
							this.speed=s;
							this.destspeed=s;
						},
						
						panToSpeed:function(s,pans) {
							this.destspeed=s;
							this.panspeed=pans;
						},
						
						quitStop:function() {
							this.stop=null;
						},
						
						setStop:function(s) {
							this.stop=s;
						},
						
						setX:function(x) {
							if (x<0) this.x=0; else
							if (x+gbox.getScreenW()>this.maxwidth) this.x=this.maxwidth-gbox.getScreenW();
							else this.x=x;
						}
						
					}
				)
			);
			
			obj[(data.logicon==null?"first":data.logicon)]=function() {
				if ((this.stop==null)||(this.y<this.stop)) {
					if (this.speed!=this.destspeed) {
						if (this.panstimer) {
							this.panstimer--;
						} else {
							if (this.speed<this.destspeed) this.speed++; else
							if (this.speed>this.destspeed) this.speed--;
							this.panstimer=this.panspeed;
						}
					}
					this.y+=this.speed;
					if ((this.stop!=null)&&(this.y>=this.stop)) this.y=this.stop;
					if ((this.loopend!=null)&&(this.y>this.loopend)) {
						this.y=this.loopstart+(this.y-this.loopend);
						this.looprounds++;
						if (this.lget==1) {
							this.block=0;
							this.bly=0;
							this.lget=2;
						} else {
							this.block=this.lblock;
							this.bly=this.lbly;
						}
						
					}
				}
						
				// Cerca il blocco da mostrare
				this.trb=this.block;
				this.tbly=this.bly;
				do {
					this.trb++;
					this.tbly+=gbox.getImage(this.stage[this.trb].image).height;	
				} while (this.tbly<this.y);
				
				this.block=this.trb-1;
				this.bly=this.tbly-gbox.getImage(this.stage[this.trb].image).height;
				
						
				if (this.lget==2) {
					this.lblock=this.block;
					this.lbly=this.bly;
					this.lget=3;
				}
				
			}
				
			obj[(data.bliton==null?"blit":data.bliton)]=function() {
				var dy=this.tbly-this.y;
				var done=false;
				do {
					if (dy>gbox.getScreenH()) done=true;
					gbox.blitAll(gbox.getBufferContext(),gbox.getImage(this.stage[this.trb].image),{dx:-this.x,dy:gbox.getScreenH()-dy});
					this.trb++;
					dy+=gbox.getImage(this.stage[this.trb].image).height;
				} while (!done);
			}
			
			return obj;
		}
	},
	platformer:{
		initialize:function(th,data) {
			help.mergeWithModel(
				th,
				help.mergeWithModel(
					data,
					{
						maxaccx:5, maxaccy:10,
						jumpsize:6, jumpaccy:6,
						accx:0, accy:0,
						x:0, y:0,
						frames:{},
						camera:true,
						flipv:false,
						side:false
					}
				)
			);
			toys.platformer.spawn(th);
		},
		spawn:function(th,data) {
			th.curjsize=0; // current jump size
			th.counter=0; // self counter
			th.touchedfloor=false; // touched floor
			th.touchedceil=false;
			th.touchedleftwall=false;
			th.touchedrightwall=false;
			th.pushing=toys.PUSH_NONE; // user is moving side
			th.killed=false;
			help.copyModel(th,data);
		},
		getNextX:function(th) { return th.x+th.accx; },
		getNextY:function(th) { return th.y+help.limit(th.accy,-th.maxaccy,th.maxaccy); },
		applyGravity:function(th) {
			th.x=toys.platformer.getNextX(th);
			th.y=toys.platformer.getNextY(th);
		},	
		horizontalKeys:function(th,keys) {
			if (gbox.keyIsPressed(keys.left)) {
				th.pushing=toys.PUSH_LEFT;
				th.accx=help.limit(th.accx-1,-th.maxaccx,th.maxaccx);
			} else if (gbox.keyIsPressed(keys.right)) {
				th.pushing=toys.PUSH_RIGHT;
				th.accx=help.limit(th.accx+1,-th.maxaccx,th.maxaccx);
			} else th.pushing=toys.PUSH_NONE;
		},
		verticalTileCollision:function(th,map,tilemap) {
			var bottom=help.getTileInMap(th.x+(th.w/2),th.y+th.h,map,0,tilemap);
			var top=help.getTileInMap(th.x+(th.w/2),th.y,map,0,tilemap);
			th.touchedfloor=false;
			th.touchedceil=false;
	
			if (map.tileIsSolidCeil(th,top)) {
				th.accy=0;
				th.y=help.yPixelToTile(map,th.y,1);
				th.touchedceil=true;
			}
			if (map.tileIsSolidFloor(th,bottom)) {
				th.accy=0;
				th.y=help.yPixelToTile(map,th.y+th.h)-th.h;
				th.touchedfloor=true;
			}
		},
		horizontalTileCollision:function(th,map,tilemap) {
			var left=0;
			var right=0;
			var t=0;
			
			th.touchedleftwall=false;
			th.touchedrightwall=false;
			
			while (t<th.h) {
				left=help.getTileInMap(th.x,th.y+t,map,0,tilemap);
				right=help.getTileInMap(th.x+th.w-1,th.y+t,map,0,tilemap);
					
				if ((th.accx<0)&&map.tileIsSolidFloor(th,left)) {
					th.accx=0;
					th.x=help.xPixelToTile(map,th.x-1,1);
					th.touchedleftwall=true;
				} 
				if ((th.accx>0)&&map.tileIsSolidFloor(th,right)) {
					th.accx=0;
					th.x=help.xPixelToTile(map,th.x,1)-th.w;
					th.touchedrightwall=true;
				}
				t+=gbox.getTiles(map.tileset).tileh;
			}
		},
		canJump:function(th) {
			return th.touchedfloor;
		},
		jumpKeys:function(th,key) {
			if (toys.platformer.canJump(th)&&gbox.keyIsHit(key.jump)&&(th.curjsize==0)) {
				if (key.audiojump) gbox.hitAudio(key.audiojump);
				th.accy=-th.jumpaccy;
				th.curjsize=th.jumpsize;
			} else if (th.curjsize&&gbox.keyIsHold(key.jump)) { // Jump modulation
				th.accy--;
				th.curjsize--;
			} else
				th.curjsize=0;
		},	
		bounce:function(th,data) {
			th.curjsize=0;
			th.accy=-data.jumpsize;
		},
		handleAccellerations:function(th) {
			// Gravity
			if (!th.touchedfloor) th.accy++;
			// Attrito
			if (th.pushing==toys.PUSH_NONE) if (th.accx) th.accx=help.goToZero(th.accx);
		},
		setSide:function(th) {
			if (th.accx) th.side=th.accx>0;
		},
		setFrame:function(th) {
			if (th.touchedfloor)
				if (th.pushing!=toys.PUSH_NONE)
					th.frame=help.decideFrame(th.counter,th.frames.walking);
				else
					th.frame=help.decideFrame(th.counter,th.frames.still);
			else if (th.accy>0)
				th.frame=help.decideFrame(th.counter,th.frames.falling);
			else
				th.frame=help.decideFrame(th.counter,th.frames.jumping);
		},
		// ---
		auto:{
			// Moves on a platform. It tries to do not fall down, if specified.
			// Args: (object,{moveWhileFalling:<moves while not touching the floor>,speed:<movement speed>})
			// Outs: the frame
			goomba:function(th,data) {
				if (data.moveWhileFalling||th.touchedfloor) {
					if (th.side) {
						th.pushing=toys.PUSH_LEFT;
						th.accx=-data.speed;
					} else {
						th.pushing=toys.PUSH_RIGHT;
						th.accx=data.speed;
					}
				} else th.pushing=toys.PUSH_NONE;
			},
			dontFall:function(th,map,tilemap) {
				if (th.accx) {
					var til;
					if (th.accx>0) til=help.getTileInMap(toys.platformer.getNextX(th)+th.w-1+th.accx,th.y+th.h,map,0,tilemap);				
					else til=help.getTileInMap(toys.platformer.getNextX(th),th.y+th.h,map,0,tilemap);
					if (!map.tileIsSolidFloor(th,til)) {
						th.side=!th.side;
						th.accx=0;
					}
				}
			},
			horizontalBounce:function(th) {
				if (th.touchedleftwall||th.touchedrightwall) th.side=!th.side;
			},
		}
	},
	
	// State-based toys
	// CONSTANTS
	TOY_BUSY:0,
	TOY_DONE:1,
	TOY_IDLE:2,
	
	// PRIVATE

	// Generical toys method
	resetToy:function(th,id) { if (th.toys) delete th.toys[id] },
	getToyValue:function(th,id,v,def) { return ((th.toys==null)||(th.toys[id]==null)?def:th.toys[id][v]) },
	getToyStatus:function(th,id) { return ((th.toys==null)||(th.toys[id]==null)?toys.TOY_BUSY:th.toys[id].__status) },
	
	_toydone:function(th,id) {
		if (th.toys[id].__status<toys.TOY_IDLE) th.toys[id].__status++;
		return th.toys[id].__status;
	},
	_toybusy:function(th,id) {
		th.toys[id].__status=toys.TOY_BUSY;
		return th.toys[id].__status;
	},
	_toyfrombool:function(th,id,b) { return (b?toys._toydone(th,id):toys._toybusy(th,id)) },
	_maketoy:function(th,id){
		if (!th.toys) th.toys={};
		if (!th.toys[id]) {
			th.toys[id]={__status:toys.TOY_BUSY};
			return true;
		} else return false;
	},
	// Pure timers
	timer:{
		randomly:function(th,id,data) {
			if (toys._maketoy(th,id)) {
				th.toys[id].time=help.random(data.base,data.range);
			}
			if (th.toys[id].time) {
				th.toys[id].time--;
				return toys._toybusy(th,id);
			} else {
				th.toys[id].time=help.random(data.base,data.range);
				return toys._toydone(th,id);
			}	
		},
		real:function(th,id,data) {
			if (toys._maketoy(th,id)) {
				th.toys[id].subtimer=gbox.getFps();
				th.toys[id].done=false;
				if (data.countdown)
					th.toys[id].time=data.countdown;
				else
					th.toys[id].time=0;
			}
			th.toys[id].subtimer--;
			if (!th.toys[id].subtimer) {
				th.toys[id].subtimer=gbox.getFps();
				if (data.countdown) {
					if (th.toys[id].time) {
						th.toys[id].time--; 
						if (data.audiocritical&&(th.toys[id].time<=data.critical))
							gbox.hitAudio(data.audiocritical);
					} else th.toys[id].done=true;
				} else
					th.toys[id].time++;
			}
			return toys._toyfrombool(th,id,th.toys[id].done);

		},
		every:function(th,id,frames){
			if (toys._maketoy(th,id)) th.toys[id].timer=0;
			th.toys[id].timer++;
			if (th.toys[id].timer==frames) {
				th.toys[id].timer=0;
				return toys._toydone(th,id);
			} else return toys._toybusy(th,id)
		},
		after:function(th,id,frames) {
			if (toys._maketoy(th,id)) th.toys[id].timer=0;
			if (th.toys[id].timer==frames) return toys._toydone(th,id); else {
				th.toys[id].timer++;
				return toys._toybusy(th,id);
			}
		}
	},
	// Logical helpers
	logic: {
		once:function(th,id,cond){
			if (toys._maketoy(th,id)) th.toys[id].done=false;
			if (th.toys[id].done) return false; else if (cond) th.toys[id].done=true;
			return cond;
		}
	},
	// UI
	ui:{
		menu:function(th,id,opt) {
			if (toys._maketoy(th,id)||opt.resetmenu) {
				var fd=gbox.getFont(opt.font);
				th.toys[id].selected=(opt.selected?opt.selected:0);
				th.toys[id].ok=0;
				var w=0;
				for (var i=0;i<opt.items.length;i++)
					if (opt.items[i].length>w) w=opt.items[i].length;
				gbox.createCanvas("menu-"+id,{w:w*fd.tilew,h:opt.items.length*fd.tileh});
				for (var i=0;i<opt.items.length;i++)
					gbox.blitText(gbox.getCanvasContext("menu-"+id),{font:opt.font,text:opt.items[i],dx:0,dy:fd.tileh*i});
				th.toys[id].fh=fd.tileh;
				th.toys[id].fw=fd.tilew;
			}
			if (!th.toys[id].ok) {
				if (gbox.keyIsHit(opt.keys.up)&&(th.toys[id].selected>0)) {
					if (opt.audiooption) gbox.hitAudio(opt.audiooption);
					th.toys[id].selected--;
				} else
				if (gbox.keyIsHit(opt.keys.down)&&(th.toys[id].selected<opt.items.length-1)) {
					if (opt.audiooption) gbox.hitAudio(opt.audiooption);
					th.toys[id].selected++;
				} else
				if (gbox.keyIsHit(opt.keys.ok)) {
					if (opt.audioconfirm) gbox.hitAudio(opt.audioconfirm);
					th.toys[id].ok=1;
				} else
				if (gbox.keyIsHit(opt.keys.cancel)) th.toys[id].ok=-1;
			}
			gbox.blitAll(gbox.getBufferContext(),gbox.getCanvas("menu-"+id),{dx:opt.x+th.toys[id].fw,dy:opt.y,camera:opt.camera});
			if (!(th.toys[id].ok%2)) gbox.blitText(gbox.getBufferContext(),{font:opt.font,text:opt.selector,dx:opt.x,dy:opt.y+th.toys[id].selected*th.toys[id].fh,camera:opt.camera});
			if (th.toys[id].ok) {
				if (th.toys[id].ok>0)
					if (th.toys[id].ok<10) {
						th.toys[id].ok++;
						toys._toybusy(th,id);
					} else return toys._toydone(th,id); // selected > 0
				else return toys._toydone(th,id); // selected == -1
			} else return toys._toybusy(th,id);
		},
		
		// Returns a full customizable object for optimized huds
		hud:function(id) {
			gbox.createCanvas(id);
			return {
				w:{},
				surfaceid:id,
				
				updateWidget:function(i){
					if (!this.w[i].__hidden) {
						if (this.w[i].widget=="label") {
							if (this.w[i].prepad!=null) this.w[i].text=help.prepad(this.w[i].value,this.w[i].prepad,this.w[i].padwith); else
							if (this.w[i].postpad!=null) this.w[i].text=help.postpad(this.w[i].value,this.w[i].postpad,this.w[i].padwith); else
							this.w[i].text=this.w[i].value+"";
							gbox.blitText(gbox.getCanvasContext(this.surfaceid),this.w[i]);
						}
						if (this.w[i].widget=="symbols") {
							var ts=gbox.getTiles(this.w[i].tileset);
							gbox.blitClear(gbox.getCanvasContext(this.surfaceid),{x:this.w[i].dx,y:this.w[i].dy,w:((this.w[i].maxshown-1)*this.w[i].gapx)+ts.tilew,h:((this.w[i].maxshown-1)*this.w[i].gapy)+ts.tileh});
							var cnt=this.w[i].value;
							for (var x=0;x<this.w[i].maxshown;x++) {
								if (cnt>0) {
									gbox.blitTile(gbox.getCanvasContext(this.surfaceid),{tileset:this.w[i].tileset,tile:this.w[i].tiles[(cnt>this.w[i].tiles.length?this.w[i].tiles.length-1:cnt-1)],dx:this.w[i].dx+(x*this.w[i].gapx),dy:this.w[i].dy+(x*this.w[i].gapy),fliph:this.w[i].fliph,flipv:this.w[i].flipv});
								} else
									if (this.w[i].emptytile!=null)
										gbox.blitTile(gbox.getCanvasContext(this.surfaceid),{tileset:this.w[i].tileset,tile:this.w[i].emptytile,dx:this.w[i].dx+(x*this.w[i].gapx),dy:this.w[i].dy+(x*this.w[i].gapy),fliph:this.w[i].fliph,flipv:this.w[i].flipv});	
								cnt-=this.w[i].tiles.length;
							}

						}
						if (this.w[i].widget=="stack") {
							var ts=gbox.getTiles(this.w[i].tileset);
							var bw=((this.w[i].maxshown-1)*this.w[i].gapx)+ts.tilew;
							gbox.blitClear(gbox.getCanvasContext(this.surfaceid),{x:this.w[i].dx-(this.w[i].rightalign?bw:0),y:this.w[i].dy,w:bw,h:((this.w[i].maxshown-1)*this.w[i].gapy)+ts.tileh});
							for (var x=0;x<this.w[i].maxshown;x++)
								if (x<this.w[i].value.length)
									gbox.blitTile(gbox.getCanvasContext(this.surfaceid),{tileset:this.w[i].tileset,tile:this.w[i].value[x],dx:(this.w[i].rightalign?this.w[i].dx-ts.tileh-(this.w[i].gapx*x):this.w[i].dx+(x*this.w[i].gapx)),dy:this.w[i].dy+(x*this.w[i].gapy),fliph:this.w[i].fliph,flipv:this.w[i].flipv});
						}
						if (this.w[i].widget=="radio") {
							var ts=gbox.getTiles(this.w[i].tileset);
							gbox.blitClear(gbox.getCanvasContext(this.surfaceid),{x:this.w[i].dx,y:this.w[i].dy,w:ts.tilew,h:ts.tileh});
							gbox.blitTile(gbox.getCanvasContext(this.surfaceid),{tileset:this.w[i].tileset,tile:this.w[i].frames[this.w[i].value],dx:this.w[i].dx,dy:this.w[i].dy,fliph:this.w[i].fliph,flipv:this.w[i].flipv});

						}
						if (this.w[i].widget=="blit") {
							var ts=gbox.getTiles(this.w[i].tileset);
							gbox.blitClear(gbox.getCanvasContext(this.surfaceid),{x:this.w[i].dx,y:this.w[i].dy,w:ts.tilew,h:ts.tileh});
							gbox.blitTile(gbox.getCanvasContext(this.surfaceid),{tileset:this.w[i].tileset,tile:this.w[i].value,dx:this.w[i].dx,dy:this.w[i].dy,fliph:this.w[i].fliph,flipv:this.w[i].flipv});

						}
						if (this.w[i].widget=="bool") {
							var ts=gbox.getTiles(this.w[i].tileset);
							gbox.blitClear(gbox.getCanvasContext(this.surfaceid),{x:this.w[i].dx,y:this.w[i].dy,w:ts.tilew,h:ts.tileh});
							if (this.w[i].value)
								gbox.blitTile(gbox.getCanvasContext(this.surfaceid),{tileset:this.w[i].tileset,tile:this.w[i].frame,dx:this.w[i].dx,dy:this.w[i].dy,fliph:this.w[i].fliph,flipv:this.w[i].flipv});
						}
						if (this.w[i].widget=="gauge") {
							var ts=gbox.getTiles(this.w[i].tileset);
							gbox.blitTile(gbox.getCanvasContext(this.surfaceid),{tileset:this.w[i].tileset,tile:0,dx:this.w[i].dx,dy:this.w[i].dy});						
							gbox.blitTile(gbox.getCanvasContext(this.surfaceid),{tileset:this.w[i].tileset,tile:1,dx:this.w[i].dx,dy:this.w[i].dy,w:(this.w[i].value*ts.tilew)/this.w[i].maxvalue});
						}
					}
				},
				
				hideWidgets:function(l) {
					for (var i=0;i<l.length;i++) this.w[l[i]].__hidden=true;
					this.redraw();
				},
				
				showWidgets:function(l) {
					for (var i=0;i<l.length;i++) this.w[l[i]].__hidden=false;
					this.redraw();
				},
				
				getValue:function(w,k) {
					return this.w[w][k];
				},
				

				getNumberValue:function(w,k) {
					return this.w[w][k]*1;
				},

				setValue:function(w,k,v) {
					if (this.w[w][k]!=v) {
						if (k=="value") {
							if ((this.w[w].maxvalue!=null)&&(v>this.w[w].maxvalue)) v=this.w[w].maxvalue;
							if ((this.w[w].minvalue!=null)&&(v<this.w[w].minvalue)) v=this.w[w].minvalue;	
							if (this.w[w].widget=="radio") v=(v%this.w[w].frames.length);
						}
						this.w[w][k]=v;							
						this.updateWidget(w);
					}
				},
				
				pushValue:function(w,k,v) {
					this.w[w][k].push(v);
					this.updateWidget(w);
				},
				
				addValue:function(w,k,v) {
					if (v) this.setValue(w,k,this.w[w][k]+v);
				},
				
				setWidget:function(id,w) {
					this.w[id]=w;
					this.updateWidget(id);
				},
				
				redraw:function() {
					gbox.blitClear(gbox.getCanvasContext(this.surfaceid));
					for (var i in this.w) this.updateWidget(i);
				},
				
				blit:function() {
					gbox.blitAll(gbox.getBufferContext(),gbox.getCanvas(this.surfaceid),{dx:0,dy:0});
				}
			
			}
		}
	},
	fullscreen:{
		fadeout:function(th,id,tox,data) {
			if (toys._maketoy(th,id)||data.resetfade) {
				th.toys[id].fade=0;
				if (data.audiofade) th.toys[id].stv=gbox.getAudioVolume(data.audiofade);
				if (data.audiochannelfade) th.toys[id].chv=gbox.getChannelVolume(data.audiochannelfade);
			}
			th.toys[id].fade+=data.fadespeed;
			if (th.toys[id].fade>1) th.toys[id].fade=1;
			data.alpha=th.toys[id].fade;
			gbox.blitFade(tox,data);
			if (data.audiofade) gbox.setAudioVolume(data.audiofade,th.toys[id].stv*(1-data.alpha));
			if (data.audiochannelfade) 
				if (data.alpha==1)
					gbox.stopChannel(data.audiochannelfade);
				else
					gbox.setChannelVolume(data.audiochannelfade,th.toys[id].chv*(1-data.alpha));
			return toys._toyfrombool(th,id,th.toys[id].fade==1)
		},
		fadein:function(th,id,tox,data) {
			if (toys._maketoy(th,id)||data.resetfade) {
				th.toys[id].fade=1;
				if (data.audiofade) th.toys[id].stv=gbox.getAudioVolume(data.audiofade);
				if (data.audiochannelfade) th.toys[id].chv=gbox.getChannelDefaultVolume(data.audiochannelfade);
			}
			th.toys[id].fade-=data.fadespeed;
			if (th.toys[id].fade<0) th.toys[id].fade=0;
			if (th.toys[id].fade) {
				data.alpha=th.toys[id].fade;
				if (data.audiofade) gbox.setAudioVolume(data.audiofade,th.toys[id].stv*(1-data.alpha));
				if (data.audiochannelfade) gbox.setChannelVolume(data.audiochannelfade,th.toys[id].chv*(1-data.alpha));
				gbox.blitFade(tox,data);
			}
			return toys._toyfrombool(th,id,th.toys[id].fade==0);
		}
	},
	text:{
		blink:function(th,id,tox,data) {
			if (toys._maketoy(th,id)) {
				th.toys[id].texttimer=0;
				th.toys[id].visible=false;
				th.toys[id].times=0;
			}
			if (th.toys[id].texttimer>=data.blinkspeed) {
				th.toys[id].texttimer=0;
				th.toys[id].visible=!th.toys[id].visible;		
				if (data.times) th.toys[id].times++;
			} else th.toys[id].texttimer++;
			if (th.toys[id].visible)
				gbox.blitText(tox,data);
			return toys._toyfrombool(th,id,(data.times?data.times<th.toys[id].times:false));
		},
		fixed:function(th,id,tox,data) {
			if (toys._maketoy(th,id))
				th.toys[id].texttimer=0;
			else
				th.toys[id].texttimer++;
			gbox.blitText(tox,data);
			return toys._toyfrombool(th,id,data.time<th.toys[id].texttimer);
		}
	},
	logos:{
		linear:function(th,id,data) {
			if (toys._maketoy(th,id)) {
				th.toys[id].x=data.sx;
				th.toys[id].y=data.sy;
				th.toys[id].every=data.every;
				th.toys[id].played=false;
			}
			if (!th.toys[id].every) {
				if ((data.x!=th.toys[id].x)||(data.y!=th.toys[id].y))
					if ((Math.abs(data.x-th.toys[id].x)<=data.speed)&&(Math.abs(data.y-th.toys[id].y)<=data.speed)) {
						th.toys[id].x=data.x;
						th.toys[id].y=data.y;
					} else
						trigo.translate(th.toys[id],trigo.getAngle(th.toys[id],data),data.speed);
				else
					if (!th.toys[id].played) {
						if (data.audioreach) gbox.hitAudio(data.audioreach);
						th.toys[id].played=true;
					}
				th.toys[id].every=data.every;
			} else th.toys[id].every--;
			if (data.text)
				gbox.blitText(gbox.getBufferContext(),{
					font:data.font,
					dx:th.toys[id].x,dy:th.toys[id].y,
					text:data.text
				});
			else if (data.tileset)
				gbox.blitTile(gbox.getBufferContext(),{tileset:data.tileset,tile:data.tile,dx:th.toys[id].x,dy:th.toys[id].y,camera:data.camera,fliph:data.fliph,flipv:data.flipv,alpha:data.alpha});			
			else
				gbox.blitAll(gbox.getBufferContext(),gbox.getImage(data.image),{dx:th.toys[id].x,dy:th.toys[id].y,alpha:data.alpha});
			return toys._toyfrombool(th,id,(data.x==th.toys[id].x)&&(data.y==th.toys[id].y));
		},
		crossed:function(th,id,data) {
			if (toys._maketoy(th,id)) {
				th.toys[id].gapx=data.gapx;
				th.toys[id].lw=gbox.getImage(data.image).height;
				th.toys[id].done=false;
			}
			if (th.toys[id].gapx) {
				th.toys[id].gapx-=data.speed;
				if (th.toys[id].gapx<0) th.toys[id].gapx=0;
				gbox.blitAll(gbox.getBufferContext(),gbox.getImage(data.image),{dx:data.x-th.toys[id].gapx,dy:data.y,alpha:data.alpha});
				gbox.blitAll(gbox.getBufferContext(),gbox.getImage(data.image),{dx:data.x+th.toys[id].gapx,dy:data.y,alpha:data.alpha});
				return toys._toybusy(th,id);
			} else {
				if (!th.toys[id].done) {
					th.toys[id].done=true;
					if (data.audioreach) gbox.hitAudio(data.audioreach);					
				}
				gbox.blitAll(gbox.getBufferContext(),gbox.getImage(data.image),{dx:data.x,dy:data.y});
				return toys._toydone(th,id);
			}
		},
		zoomout:function(th,id,data) {
			if (toys._maketoy(th,id)) {
				th.toys[id].zoom=data.zoom;
				th.toys[id].done=false;
				th.toys[id].img=gbox.getImage(data.image);
			}
			if (th.toys[id].zoom!=1) {
				th.toys[id].zoom-=data.speed;
				if (th.toys[id].zoom<=1) {
					th.toys[id].zoom=1;
					if (data.audioreach) gbox.hitAudio(data.audioreach);
				}
				gbox.blit(gbox.getBufferContext(),gbox.getImage(data.image),{h:th.toys[id].img.height,w:th.toys[id].img.width,dx:data.x-Math.floor(th.toys[id].img.width*(th.toys[id].zoom-1)/2),dy:data.y-Math.floor(th.toys[id].img.height*(th.toys[id].zoom-1)/2),dh:Math.floor(th.toys[id].img.height*th.toys[id].zoom),dw:Math.floor(th.toys[id].img.width*th.toys[id].zoom),alpha:1/th.toys[id].zoom});
				return toys._toybusy(th,id);
			} else {
				gbox.blitAll(gbox.getBufferContext(),gbox.getImage(data.image),{dx:data.x,dy:data.y});
				return toys._toydone(th,id);
			}
		},
		rising:function(th,id,data) {
			if (toys._maketoy(th,id)) {
				th.toys[id].cnt=0;
				th.toys[id].lh=gbox.getImage(data.image).height;
				th.toys[id].lw=gbox.getImage(data.image).width;
				th.toys[id].done=false;
			}
			if (th.toys[id].cnt<th.toys[id].lh) {
				th.toys[id].cnt+=data.speed;
				if (th.toys[id].cnt>th.toys[id].lh) th.toys[id].gapx=th.toys[id].lh;
				gbox.blit(gbox.getBufferContext(),gbox.getImage(data.image),{dh:th.toys[id].cnt,dw:th.toys[id].lw,dx:data.x,dy:data.y+th.toys[id].lh-th.toys[id].cnt,alpha:data.alpha});
				if (data.reflex) gbox.blit(gbox.getBufferContext(),gbox.getImage(data.image),{dh:th.toys[id].cnt,dw:th.toys[id].lw,dx:data.x,dy:data.y+th.toys[id].lh,alpha:data.reflex,flipv:true});
				if (th.toys[id].cnt>=th.toys[id].lh)
					if (data.audioreach) gbox.hitAudio(data.audioreach);					
				return toys._toybusy(th,id);
			} else {
				gbox.blitAll(gbox.getBufferContext(),gbox.getImage(data.image),{dx:data.x,dy:data.y});
				if (data.reflex) gbox.blitAll(gbox.getBufferContext(),gbox.getImage(data.image),{dx:data.x,dy:data.y+th.toys[id].lh,alpha:data.reflex,flipv:true});
				
				return toys._toydone(th,id);
			}
		},
		bounce:function(th,id,data) {
			if (toys._maketoy(th,id)) {
				th.toys[id].accy=data.accy;
				th.toys[id].y=data.y;
				th.toys[id].h=gbox.getImage(data.image).height;
				th.toys[id].done=false;
			}
			if (!th.toys[id].done) {
				if (th.toys[id].y+th.toys[id].h>=data.floory) {
					if (data.audiobounce) gbox.hitAudio(data.audiobounce);
					th.toys[id].y=data.floory-th.toys[id].h;
					th.toys[id].accy=-Math.ceil(th.toys[id].accy/(data.heavy?data.heavy:2));
					th.toys[id].done=(th.toys[id].accy==0);
				} else th.toys[id].accy--;
				th.toys[id].y-=th.toys[id].accy;
			}
			gbox.blitAll(gbox.getBufferContext(),gbox.getImage(data.image),{dx:data.x,dy:th.toys[id].y});
					
			return toys._toyfrombool(th,id,th.toys[id].done);
		}
	},
	dialogue: {
		render:function(th,id,data){
			if (toys._maketoy(th,id)) {
				th.toys[id].newscene=true;
				th.toys[id].sceneid=-1;
				th.toys[id].ended=false;
				th.toys[id].timer=0;
				th.toys[id].counter=0;
				th.toys[id].anim=0;
				gbox.createCanvas("dialogue-"+id);
			}
			if (!data.hideonend||(data.hideonend&&!th.toys[id].ended)) {
				if (th.toys[id].newscene&&!th.toys[id].ended) {
					th.toys[id].anim=0;
					th.toys[id].timer=0;
					th.toys[id].newscene=false;
					th.toys[id].sceneid++;
					th.toys[id].ended=(th.toys[id].sceneid==data.scenes.length);
					if (!th.toys[id].ended) {
						th.toys[id].letter=0;
						th.toys[id].wait=false;
						th.toys[id].scene=data.scenes[th.toys[id].sceneid];
						th.toys[id].fd=gbox.getFont((th.toys[id].scene.font?th.toys[id].scene.font:data.font));
						th.toys[id].sceneH=(th.toys[id].scene.dh?th.toys[id].scene.dh:gbox.getScreenH());
						th.toys[id].sceneW=(th.toys[id].scene.dw?th.toys[id].scene.dw:gbox.getScreenW());
						th.toys[id].sceneX=(th.toys[id].scene.dx?th.toys[id].scene.dx:0);
						th.toys[id].sceneY=(th.toys[id].scene.dy?th.toys[id].scene.dy:0);
						gbox.blitClear(gbox.getCanvasContext("dialogue-"+id));
						if (th.toys[id].scene.slide) {							
							gbox.blitAll(gbox.getCanvasContext("dialogue-"+id),gbox.getImage(th.toys[id].scene.slide.image),{dx:th.toys[id].scene.slide.x,dy:th.toys[id].scene.slide.y});
						}
						if (th.toys[id].scene.scroller) {
							gbox.createCanvas("scroller-"+id,{w:th.toys[id].sceneW,h:(th.toys[id].scene.scroller.length)*(th.toys[id].fd.tileh+th.toys[id].scene.spacing)});
							for (var i=0;i<th.toys[id].scene.scroller.length;i++)
								gbox.blitText(gbox.getCanvasContext("scroller-"+id),{
										font:th.toys[id].fd.id,
										dx:0,
										dy:i*(th.toys[id].fd.tileh+th.toys[id].scene.spacing),
										dw:th.toys[id].sceneW,
										halign:gbox.ALIGN_CENTER,
										text:th.toys[id].scene.scroller[i]
									});
						}
						if (th.toys[id].scene.bonus) {
							gbox.createCanvas("bonus-"+id,{w:th.toys[id].sceneW,h:(th.toys[id].scene.bonus.length)*(th.toys[id].fd.tileh+th.toys[id].scene.spacing)});
						}
						if (th.toys[id].scene.audiomusic) gbox.hitAudio(th.toys[id].scene.audiomusic);
					}
				}
				if (!th.toys[id].ended) {
					if (th.toys[id].wait) {
						if (gbox.keyIsHit(data.esckey)) th.toys[id].ended=true; else
						if (gbox.keyIsHit(data.skipkey)) th.toys[id].newscene=true;
					} else {
									
						// SKIP KEYS
						
						if (gbox.keyIsHit(data.esckey)) th.toys[id].ended=true; else
						if (gbox.keyIsHold(data.skipkey)) th.toys[id].counter=th.toys[id].scene.speed;
						else th.toys[id].counter++;
						
						// MOVING
						
						if (th.toys[id].scene.talk) { // DIALOGUES
						
						
							if (th.toys[id].counter==th.toys[id].scene.speed) {
								th.toys[id].letter++;
								th.toys[id].counter=0;
								if (th.toys[id].scene.audio&&!(th.toys[id].letter%3)) gbox.hitAudio(th.toys[id].scene.audio);
								var tmp=th.toys[id].letter;
								var row=0;
								while (tmp>th.toys[id].scene.talk[row].length) {
									tmp-=th.toys[id].scene.talk[row].length;
									row++;
									if (row==th.toys[id].scene.talk.length)  {
										row=-1;
										break;
									}
								}
								if (row>=0) {
									gbox.blitText(gbox.getCanvasContext("dialogue-"+id),{
										font:data.font,
										dx:data.who[th.toys[id].scene.who].x,
										dy:(data.who[th.toys[id].scene.who].y)+(row*th.toys[id].fd.tileh),
										text:th.toys[id].scene.talk[row].substr(0,tmp)
									});
								} else
									th.toys[id].wait=true;
							}
						
						} else if (th.toys[id].scene.scroller) { // SCROLLER (i.e. credits)
						
							if (th.toys[id].counter==th.toys[id].scene.speed) {
								th.toys[id].letter++;
								th.toys[id].counter=0;
								if (th.toys[id].letter==(gbox.getCanvas("scroller-"+id).height+th.toys[id].scene.push))
									th.toys[id].wait=true;
							}
							
						} else if (th.toys[id].scene.bonus) { // BONUS (classic bonus award screen)
							for (var row=0;row<=th.toys[id].letter;row++) {
								if (th.toys[id].scene.bonus[row].text)
									gbox.blitText(gbox.getCanvasContext("bonus-"+id),{
										font:data.font,
										dx:0,
										dy:(row*(th.toys[id].fd.tileh+th.toys[id].scene.spacing)),
										text:th.toys[id].scene.bonus[row].text
									});
								else if (th.toys[id].scene.bonus[row].mul) {
									// Mask is %VAL%e%MUL%=%TOT%
									th.toys[id].scene.bonus[row].tmptext=th.toys[id].scene.bonus[row].mask.replace(/%VAL%/,th.toys[id].timer).replace(/%MUL%/,th.toys[id].scene.bonus[row].mul).replace(/%TOT%/,(th.toys[id].timer*th.toys[id].scene.bonus[row].mul));
									gbox.blitText(gbox.getCanvasContext("bonus-"+id),{
										clear:true,
										font:data.font,
										dx:0,
										dy:(row*(th.toys[id].fd.tileh+th.toys[id].scene.spacing)),
										text:th.toys[id].scene.bonus[row].tmptext
									});
								}
							}
							
							if (!th.toys[id].wait) {
								var next=false;
								if (th.toys[id].scene.bonus[th.toys[id].letter].mul&&!th.toys[id].scene.bonus[th.toys[id].letter].text) {
									if (th.toys[id].counter>=th.toys[id].scene.bonus[th.toys[id].letter].speed) {
										th.toys[id].counter=0;
										th.toys[id].timer++;
										if (th.toys[id].timer>th.toys[id].scene.bonus[th.toys[id].letter].mulvalue) {
											th.toys[id].scene.bonus[th.toys[id].letter].text=th.toys[id].scene.bonus[th.toys[id].letter].tmptext;
											next=true;
										} else {
											if (th.toys[id].scene.bonus[th.toys[id].letter].callback)
												th.toys[id].scene.bonus[th.toys[id].letter].callback(th.toys[id].scene.bonus[th.toys[id].letter],th.toys[id].scene.bonus[th.toys[id].letter].arg);
										}
									}
									
								} else if (th.toys[id].counter>=th.toys[id].scene.speed) next=true;
								if (next) {
									if (th.toys[id].letter==th.toys[id].scene.bonus.length-1)
										th.toys[id].wait=true;
									else {
										th.toys[id].letter++;
										if (th.toys[id].scene.bonus[th.toys[id].letter].mul) {
											th.toys[id].scene.bonus[th.toys[id].letter].text=null;
											th.toys[id].scene.bonus[th.toys[id].letter].tmptext=null;
											th.toys[id].timer=0;
										}
										th.toys[id].counter=0;
									}
								}
							}
						}
							
					}
				
				}
				
				// RENDERING
				
				
				if (th.toys[id].scene.talk) { // DIALOGUES
					if (data.who[th.toys[id].scene.who].box)
							gbox.blitRect(gbox.getBufferContext(),data.who[th.toys[id].scene.who].box);
					if (data.who[th.toys[id].scene.who].tileset) {
						th.toys[id].anim=(th.toys[id].anim+1)%20;
						gbox.blitTile(gbox.getBufferContext(),{tileset:data.who[th.toys[id].scene.who].tileset,tile:help.decideFrame(th.toys[id].anim,data.who[th.toys[id].scene.who].frames),dx:data.who[th.toys[id].scene.who].portraitx,dy:data.who[th.toys[id].scene.who].portraity,camera:false,fliph:data.who[th.toys[id].scene.who].fliph,flipv:data.who[th.toys[id].scene.who].flipv});
					}
					gbox.blitAll(gbox.getBufferContext(),gbox.getCanvas("dialogue-"+id),{dx:0,dy:0});
				} else if (th.toys[id].scene.scroller) // SCROLLER (i.e. credits)
					gbox.blit(gbox.getBufferContext(),gbox.getCanvas("scroller-"+id),{dx:th.toys[id].sceneX,dy:th.toys[id].sceneY+(th.toys[id].letter<th.toys[id].sceneH?th.toys[id].sceneH-th.toys[id].letter:0),dw:th.toys[id].sceneW,y:(th.toys[id].letter<th.toys[id].sceneH?0:th.toys[id].letter-th.toys[id].sceneH),dh:(th.toys[id].letter<th.toys[id].sceneH?th.toys[id].letter:th.toys[id].sceneH)});
				else if (th.toys[id].scene.bonus) // BONUS (i.e. credits)
					gbox.blitAll(gbox.getBufferContext(),gbox.getCanvas("bonus-"+id),{dx:th.toys[id].sceneX,dy:th.toys[id].sceneY});
			}		
			return toys._toyfrombool(th,id,th.toys[id].ended);
		}
	},
	
	// GENERATORS
	
	generate: {
		sparks:{
			simple:function(th,group,id,data) {
				var ts=gbox.getTiles(data.tileset);
				if (data.frames==null) {
					data.frames={ speed:(data.animspeed==null?1:data.animspeed), frames:[]};
					for (var i=0;i<ts.tilerow;i++) data.frames.frames[i]=i;
				}

				var obj=gbox.addObject(
					help.mergeWithModel(
						data,{
							id:id,
							group:group,
							x:th.x+th.hw-ts.tilehw+(data.gapx==null?0:data.gapx),
							y:(data.valign=="top"?th.y:th.y+th.hh-ts.tilehh+(data.gapy==null?0:data.gapy)),
							tileset:data.tileset,
							alpha:null,
							accx:0, accy:0,
							frame:0,
							timer:(data.delay?-data.delay:-1),
							frames:data.frames,
							toptimer:((data.frames.frames.length)*data.frames.speed)-1,
							camera:th.camera,
							gravity:false,
							trashoffscreen:true,
							fliph:(data.fliph==null?th.fliph:data.fliph), flipv:(data.flipv==null?th.flipv:data.flipv)
						}
					)
				);

				obj[(data.logicon==null?"first":data.logicon)]=function() {
					this.timer++;
					if (this.timer>=0) {
						this.x+=this.accx;
						this.y+=this.accy;
						if (this.gravity) this.accy++;
						if ((this.timer==this.toptimer)||(this.trashoffscreen&&(!gbox.objectIsVisible(this)))) gbox.trashObject(this);
					}
				}
				
				obj[(data.bliton==null?"blit":data.bliton)]=function() {
					if (this.timer>=0)
						gbox.blitTile(gbox.getBufferContext(),{tileset:this.tileset,tile:help.decideFrame(this.timer,this.frames),dx:this.x,dy:this.y,camera:this.camera,fliph:this.fliph,flipv:this.flipv,alpha:this.alpha});					
				}
				
				return obj;
			},
			popupText:function(th,group,id,data) {
				data.text+="";
				var fd=gbox.getFont(data.font);
				
				var obj=gbox.addObject(
					help.mergeWithModel(
						data,{
							id:id,
							group:group,
							x:Math.floor(th.x+th.hw-(fd.tilehw*data.text.length)),
							y:th.y-fd.tilehh,
							vaccy:-data.jump,
							font:"small",
							keep:0,
							text:data.text,
							cnt:0,
							camera:th.camera
						}
					)
				);
				
				obj[(data.logicon==null?"first":data.logicon)]=function() {
					if (gbox.objectIsVisible(this)) {
						if (this.vaccy)
							this.vaccy++;
						else
							this.cnt++;
						this.y+=this.vaccy;
						if (this.cnt>=this.keep) gbox.trashObject(this);
					} else gbox.trashObject(this);
				}
				
				obj[(data.bliton==null?"blit":data.bliton)]=function() {
					gbox.blitText(gbox.getBufferContext(),{font:this.font,text:this.text,dx:this.x,dy:this.y,camera:this.camera});
				}
				
				return obj;
			},
			bounceDie:function(th,group,id,data){
				var obj=gbox.addObject(
					help.mergeWithModel(
						data,{
							id:id,
							group:group,
							tileset:th.tileset,
							frame:th.frame,
							side:th.side,
							frames:th.frames.die,
							x:th.x,
							y:th.y,
							vaccy:-data.jump,
							flipv:data.flipv,
							cnt:0,
							camera:th.camera
						}
					)
				);
				
				obj[(data.logicon==null?"first":data.logicon)]=function() {
					if (gbox.objectIsVisible(this)) {
						this.vaccy++;
						this.y+=this.vaccy;
						this.cnt=(this.cnt+1)%10;
					} else gbox.trashObject(this);	
				}
				
				obj[(data.bliton==null?"blit":data.bliton)]=function() {
					gbox.blitTile(gbox.getBufferContext(),{tileset:this.tileset,tile:help.decideFrame(this.cnt,this.frames),dx:this.x,dy:this.y,camera:this.camera,fliph:this.side,flipv:this.flipv});					
				}
				
				return obj;
			}
		},
		audio:{
			fadeOut:function(th,group,id,data){
				var obj=gbox.addObject(
					help.mergeWithModel(
						data,{
							id:id,
							group:group,
							fadespeed:-0.02*(data.fadein?-1:1),
							stoponmute:true,
							audio:null,
							channel:null,
							destination:null
						}
					)
				);

				obj[(data.logicon==null?"first":data.logicon)]=function() {
					if (this.destination==null)
						if (this.audio)
							if (this.fadespeed>0) this.destination=1; else this.destination=0;
						else
							if (this.fadespeed>0) this.destination=gbox.getChannelDefaultVolume(this.channel); else this.destination=0;
					if (this.fadespeed>0) gbox.playAudio(this.audio);
				}
				
				obj[(data.bliton==null?"blit":data.bliton)]=function() {
					if (this.audio) gbox.changeAudioVolume(this.audio,this.fadespeed);
					if (this.channel) gbox.changeChannelVolume(this.channel,this.fadespeed);
					if (
						(this.audio&&(
							((this.fadespeed<0)&&(gbox.getAudioVolume(this.audio)<=this.destination))||
							((this.fadespeed>0)&&(gbox.getAudioVolume(this.audio)>=this.destination))
					   ))||
						(this.channel&&(
							((this.fadespeed<0)&&(gbox.getChannelVolume(this.channel)<=this.destination))||
							((this.fadespeed>0)&&(gbox.getChannelVolume(this.channel)>=this.destination))
					   ))
					) {
						if (this.channel&&this.stoponmute&&(this.fadespeed<0)) gbox.stopChannel(this.channel);
						if (this.audio&&this.stoponmute&&(this.fadespeed<0)) gbox.stopAudio(this.audio);
						gbox.trashObject(this);
					}
				}
			}
		
		}
		
	}
	

}

// ----------------------------------------------------------------------------
// help
// ----------------------------------------------------------------------------

/*
Help module provides some Javascript-specific functions, such object copying, randomizing functions, 
string/array handlers and the akihabaraInit function, that automatically sets a comfortable 
preset of configurations: 25fps, double-sized display for non-mobile devices, dynamic frameskip.
*/

var help={

	/**
	    *  generates numbers from st to ed, skipping skip
	    */
	seq:function(st,ed,skip) {
		var ret=[];
		for (var i=st;i<ed;i+=(skip==null?1:skip)) ret.push(i);
		return ret;
	},

	// Handle a multiplier like counter. that means, 0=1 / 1=1 / 2=2*mul etc...
	multiplier:function(v,mul) {
		return (!v||(v<2)?1:v*(!mul?1:mul));
	},
	
	// pad str with heading pad until is "len" long
	prepad:function(str,len,pad) {
		str+="";
		while (str.length<len) str=pad+str;
		return str;
	},
	
	// pad str with tail pad until is "len" long
	postpad:function(str,len,pad) {
		str+="";
		while (str.length<len) str+=pad;
		return str;
	},

	// true if "th" is squished by "by"
	isSquished:function(th,by) {
		return ((by.accy>0)&&gbox.collides(th,by)&&(Math.abs(th.y-(by.y+by.h))<(th.h/2)))
	},
	// Random number
	random:function(min,range) {
		return min+Math.floor(Math.random()*range);
	},
	// Decides an animation frame.
	// Args: (number,{speed:<animation speed>,frames:<animation sequence>})
	// Outs: the frame
	decideFrame:function(cnt,anim) {
		return anim.frames[Math.floor(cnt/anim.speed)%anim.frames.length];
	},
	// Numbers going up and down once reached the half
	// Args: (counter)
	// Outs: the value
	upAndDown:function(counter,max) {
		if ((counter%max)>(max/2)) return max-(counter%max); else return (counter%max);
	},
	
	// Get the tile value in a map, using pixels as coords
	// Args: (x,y,map,<output if any tile is found>,<index of the map array in the passed map>)
	// Outs: the tile or ifout
	getTileInMap:function(x,y,map,ifout,mapid) {
		if (!mapid) mapid="map";
		var ts=gbox._tiles[map.tileset];
		var tx=Math.floor(x/ts.tilew);
		var ty=Math.floor(y/ts.tileh);
		if ((ty<0)||(ty>=map[mapid].length)) return ifout; else
		if ((tx<0)||(tx>=map[mapid][ty].length)) return ifout; else
		return map[mapid][ty][tx];
	},
	// Convert an ascii art array of string to a map. The first argument is the map, the second one is the translation map.
	// i.e. (["---","- -","---"],[[null," "],[1,"-"]]);
	asciiArtToMap:function(map,tra) {
		var sz=tra[0][1].length;
		var ret=[];
		var xpos;
		var pie;
		for (var y=0;y<map.length;y++) {
			var row=[];
			xpos=0;
			while (xpos<map[y].length) {
				pie=map[y].substr(xpos,sz);
				for (var t=0;t<tra.length;t++)
					if (pie==tra[t][1]) {
						if (t==0) row.push(null); else row.push(tra[t][0]);
						break;
					}
				xpos+=sz;
			}
			ret.push(row);
		}
		return ret;
	},
	
	
	// Finalize a map definition, setting height and width in pixels etc.
	// Args: (map)
	// Outs: finalized map
	finalizeTilemap:function(map) {
		var ts=gbox._tiles[map.tileset];
		map.h=map.map.length*ts.tileh;
		map.w=map.map[0].length*ts.tilew;
		map.hw=Math.floor(map.w/2);
		map.hh=Math.floor(map.h/2);
		return map;
	},
	// Converts a x-coord pixel in a x-coord tile
	xPixelToTileX:function(map,x,gap) {
		var ts=gbox._tiles[map.tileset];
		return Math.floor(x/ts.tilew);
	},
	// Converts a x-coord pixel in a x-coord tile
	yPixelToTileY:function(map,y,gap) {
		var ts=gbox._tiles[map.tileset];
		return Math.floor(y/ts.tileh);
	},
	// Converts a x-coord in pixel to the x-coord of the corresponding tile column in pixels
	// Args: (map,x,<gap of the position in tiles>)
	// Outs: the x coord in pixel of the tile column
	xPixelToTile:function(map,x,gap) {
		var ts=gbox._tiles[map.tileset];
		return (Math.floor(x/ts.tilew)+(gap?gap:0))*ts.tilew;
	},
	// Converts a y-coord in pixel to the x-coord of the corresponding tile row in pixels
	// Args: (map,y,<gap of the position in tiles>)
	// Outs: the y coord in pixel of the tile row
	yPixelToTile:function(map,y,gap) {
		var ts=gbox._tiles[map.tileset];
		return (Math.floor(y/ts.tileh)+(gap?gap:0))*ts.tileh;
	},
	// Limit a value between 2 numbers
	// Args: (value,<minimal limit>,<maximal limit>)
	// Outs: the value limited
	limit:function(v,min,max) {
		if (v<min) return min; else if (v>max) return max; else return v;
	},
	// Subtracts or adds 1 to a value to reach 0. i.e passing -3, outs -2. Passing 3, outs 2
	// Args: (value)
	// Outs: new value
	goToZero:function(v) { return (v?v-(v/Math.abs(v)):0); },
	
	// Copy a model on a data set if a value is not defined
	mergeWithModel:function(data,model) {
		if (data==null) data={};
		if (model!=null)
			for (var i in model)
				if (data[i]==null) data[i]=model[i];
		return data;
	},
	
	// Copy a model into data set
	copyModel:function(data,model) {
		if (data==null) data={};
		if (model!=null)
			for (var i in model) data[i]=model[i];
		return data;
	},
	
	// Create a model from an object (copies attributes)
	createModel:function(obj,attrs) {
		var ret={};
		for (var i=0;i<attrs.length;i++) ret[attrs[i]]=obj[attrs[i]];
		return ret;
	},
	
	// Clones an objecy
	cloneObject:function(model) {
		if (!model) return model;
		var data={};
		for (var i in model) data[i]=model[i];
		return data;
	},
	
	// Blit a tile in a map and changes a surface accordingly
	setTileInMap:function(ctx,tilemap,x,y,tile,map) {
		var ts=gbox.getTiles(tilemap.tileset);
		tilemap[(map==null?"map":map)][y][x]=tile;
		if (tile==null)
			gbox.blitClear(ctx,{x:x*ts.tilew,y:y*ts.tilew,h:ts.tileh,w:ts.tilew});
		else
			gbox.blitTile(ctx,{tileset:tilemap.tileset,tile:tile,dx:x*ts.tilew,dy:y*ts.tilew});
	},
	
	// Get the item of an array, if available. Else returns the last one
	getArrayCapped:function(a,id) {
		if (id>=a.length) return a[a.length-1]; else return a[id];
	},
	
	// Get an item of an array of object, using a field as index. is returned the first entry if the field is not valued.
	getArrayIndexed:function(a,value,field) {
		if (a[0][field]==null) return a[0];
		var i=0;
		while ((value>a[i][field])&&(i!=a.length-1)) i++;
		return a[i];
	},
	
			
	// Convert frames to minutes, seconds and csecs
	framestotime:function(frames) {
		var csec=Math.ceil(frames/gbox.getFps()*100);
		return this.prepad((Math.floor(csec/6000)%60),2,"0")+":"+this.prepad((Math.floor(csec/100)%60),2,"0")+":"+this.prepad(csec%100,2,"0");
		
	},
	
	// get an url parameter
	geturlparameter:function( name ) {
	  name = name.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
	  var regexS = "[\\?&]"+name+"=([^&#]*)";
	  var regex = new RegExp( regexS );
	  var results = regex.exec( window.location.href );
	  if( results == null )
		return "";
	  else
		return results[1];
	},
	
	// transform simple object in string, for debug
	objToStr:function(o) {
		var ret="";
		for (var n in o) ret+=n+":["+o[n]+"] ";
		return ret;
	},
	
	// Check if a variable is defined or not
	isDefined:function(v) {
		return ((typeof(v) !== 'undefined') || (v===null));
	},
	
	// Get device configuration
	getDeviceConfig:function() {

		var cap;
		if (navigator.userAgent.match(/iPhone/i)||navigator.userAgent.match(/iPod/i) || navigator.userAgent.match(/Android/i))
			cap={touch:true,width:320};			
		else if (navigator.userAgent.match(/iPad/i))
			cap={touch:true,width:768,forcedidle:10}; // Forced idle time is needed for correct framerate calculation.
		else
			cap={zoom:2};
		
		cap.canaudio=!!(document.createElement('audio').canPlayType);

		if (cap.canaudio) {
			if (navigator.userAgent.match(/iPad/i)||navigator.userAgent.match(/iPhone/i)||navigator.userAgent.match(/iPod/i)) {
				cap.audiocompatmode=2; // Single audio per time, so compatibility mode is needed. Plays only the "bgmusic" channel.
				cap.audioteam=1; // Only a member is required in the audioteam.
				cap.audioisexperimental=true; // Audio is experimental, since limited.
			} else if (navigator.userAgent.match(/Chrome/i)) {
				cap.audioteam=3; // Quite low performance on playback responsiveness.
			} else if (navigator.userAgent.match(/Firefox/i)) {
				cap.audioteam=1; // Testing smaller audioteam
				cap.audiopositiondelay=0.3; // Ogg playback is slower 0.3 between MP3 playback. Don't know why :)
				cap.audiocreatemode=1; // Firefox is stalling while downloading lot of things
			} else if (navigator.userAgent.match(/Minefield/i)) {
				cap.audioteam=1; // Testing smaller audioteam
				cap.audiocreatemode=1; // Firefox is stalling while downloading lot of things
				// Minefield has fixed the 0.3 delay!
			} else if (navigator.userAgent.match(/Safari/i)) {
				cap.audioteam=1; // Testing smaller audioteam						
			} else if (navigator.userAgent.match(/Opera/i)) {
				cap.audioteam=1; // Testing smaller audioteam			
				cap.audiocreatemode=1; // Do not like audio object cloning very much
			} else
				cap.audioisexperimental=true; // Audio is just experimental on all other devices.
				
		}

		return cap;
	},

	// The default Akihabara initialization. Low-res, low-framerate, zoomed view
	akihabaraInit:function(data) {
		if ((typeof data).toLowerCase() == "string") data={title:data};
		var device=this.getDeviceConfig();
		var footnotes=["MADE WITH AKIHABARA (C)2010 - GPL2/MIT","Project: www.kesiev.com/akihabara","Sources: github.com/kesiev/akihabara"];
		document.title=(data.title?data.title:"Akihabara");
		if (data.splash) {
			if (data.splash.footnotes) 
				for (var i=0;i<footnotes.length;i++) data.splash.footnotes.push(footnotes[i]);
			gbox.setSplashSettings(data.splash);
		}
		if (!data.splash||(data.splash.minilogo==null)) gbox.setSplashSettings({minilogo:"logo"});
		if (!data.splash||(data.splash.background==null)) gbox.setSplashSettings({background:"splash.png"});
		if (!data.splash||(data.splash.minimalTime==null)) gbox.setSplashSettings({minimalTime:3000});
		if (!data.splash||(data.splash.footnotes==null)) gbox.setSplashSettings({footnotes:footnotes});
		document.body.style.backgroundColor="#000000";
		gbox.setScreenBorder(false);
		if (help.geturlparameter("statusbar")) gbox.setStatusBar(1);
		if (help.geturlparameter("db")) gbox.setDoubleBuffering(true);
		if (help.geturlparameter("noautoskip")) gbox.setAutoskip(null);
		if (help.geturlparameter("zoom")) gbox.setZoom(help.geturlparameter("zoom")); else
			if (help.isDefined(device.zoom)) gbox.setZoom(device.zoom); else
			if (help.isDefined(device.width)) gbox.setZoom(device.width/320);
		if (help.geturlparameter("fps")) gbox.setFps(help.geturlparameter("fps")*1);
			else gbox.setFps((data.fps?data.fps:25));
		if (help.geturlparameter("fskip")) gbox.setFrameskip(help.geturlparameter("fskip"));
		if (help.geturlparameter("forcedidle")) gbox.setForcedIdle(help.geturlparameter("forcedidle")*1);
			else if (help.isDefined(device.forcedidle)) gbox.setForcedIdle(device.forcedidle);

		gbox.initScreen((data.width?data.width:(data.portrait?240:320)),(data.height?data.height:(data.portrait?320:240)));

		if (help.geturlparameter("showplayers")) gbox.setShowPlayers(help.geturlparameter("showplayers")=="yes");
		if (help.geturlparameter("canaudio")) gbox.setCanAudio(help.geturlparameter("canaudio")=="yes"); else
			gbox.setCanAudio(device.canaudio&&(!device.audioisexperimental||gbox.getFlag("experimental")));
		if (help.geturlparameter("audiocompatmode")) gbox.setAudioCompatMode(help.geturlparameter("audiocompatmode")*1); else
			if (help.isDefined(device.audiocompatmode)) gbox.setAudioCompatMode(device.audiocompatmode);
		if (help.geturlparameter("audioteam")) gbox.setAudioTeam(help.geturlparameter("audioteam")*1); else
			if (help.isDefined(device.audioteam)) gbox.setAudioTeam(device.audioteam);
		if (help.geturlparameter("loweraudioteam")) gbox.setLowerAudioTeam(help.geturlparameter("loweraudioteam")*1); else
			if (help.isDefined(device.loweraudioteam)) gbox.setLowerAudioTeam(device.loweraudioteam);			
		if (help.geturlparameter("audiocreatemode")) gbox.setAudioCreateMode(help.geturlparameter("audiocreatemode")*1); else
			if (help.isDefined(device.audiocreatemode)) gbox.setAudioCreateMode(device.audiocreatemode);
		if (help.geturlparameter("audiodequeuetime")) gbox.setAudioDequeueTime(help.geturlparameter("audiodequeuetime")*1); else
			if (help.isDefined(device.audiodequeuetime)) gbox.setAudioDequeueTime(device.audiodequeuetime);
		if (help.geturlparameter("audiopositiondelay")) gbox.setAudioPositionDelay(help.geturlparameter("audiopositiondelay")*1); else
			if (help.isDefined(device.audiopositiondelay)) gbox.setAudioPositionDelay(device.audiopositiondelay);
			
			
			
		if (help.geturlparameter("touch")=="no");
			else if ((help.geturlparameter("touch")=="yes")||device.touch)
				switch (data.padmode) {
					case "fretboard": {
						iphofretboard.initialize({h:100,bg:"fretboard.png"});		
						break;
					}
					case "none": {
						break;
					}
					default: {
						iphopad.initialize({h:100,dpad:"dpad.png",buttons:"buttons.png",bg:"padbg.png"});		
						break;
					}
				}
	}
}

// ----------------------------------------------------------------------------
// tool
// ----------------------------------------------------------------------------

/*
Tool module provides simple developing tools. Currently, this file only has a cel-composer: 
it can compose an image stacking a set of frames for animating objects, applying a 
number of filters to each frame.
*/

var tool={
	_images:[],
	_loadedflag:[],
	_data:{},
	_count:0,
	_countloaded:0,
	_loaded:function(id) {
		this._loadedflag[id]=true;
		tool._countloaded++;
		document.title=tool._countloaded+"/"+tool._count;
		for (var i=0;i<this._images.length;i++)
			if (!this._loadedflag[i]) document.title+=this._images[i].src+", ";
	},
	_loadall:function() {
		if (tool._count!=tool._countloaded)
			setTimeout(tool._loadall,1000);
		else
			tool._allloaded();
	},
	makecels:function(data) {
		this._data=data;
		var id=0;
		for (var r=0;r<data.rows.length;r++) {
			for (var i=0;i<data.rows[r].length;i++) {
				this._images[id]=new Image();
				this._images[id].addEventListener('load', function(){tool._loaded(this.id)},false);
				this._images[id].setAttribute("id",id);
				this._images[id].src=data.rows[r][i].img;
				this._count++;
				id++;
			}
		}
		this._loadall();
	},
	_allloaded:function() {
		var data=this._data;
		var wid=0;
		var hei=0;
		var curwid=0;
		var id=0;
		for (var r=0;r<data.rows.length;r++) {
			hei+=this._images[id].height*1;
			curwid=0;
			for (var i=0;i<data.rows[r].length;i++) { curwid+=this._images[id].width*1; id++}
			if (wid<curwid) wid=curwid;
		}
		
		var cels=document.createElement("canvas");
		cels.style.border="1px solid red";
		cels.setAttribute('height',hei);
		cels.setAttribute('width',wid);
		document.body.appendChild(cels);
		var ctx=cels.getContext("2d");
	
		var curx=0;
		var cury=0;
		id=0;
		for (var r=0;r<data.rows.length;r++) {
			curx=0;
			for (var i=0;i<data.rows[r].length;i++) {
				ctx.drawImage(this._images[id],curx,cury);
				if (data.rows[r][i].filter) {
					if (data.rows[r][i].filter) {
						var imgd = ctx.getImageData(curx, cury, this._images[id].width, this._images[id].height);
						var pix = imgd.data;

						// Loop over each pixel and invert the color.
						for (var z = 0, n = pix.length; z < n; z += 4) {
							if (data.rows[r][i].filter.replace) {
								for (var w=0;w<data.rows[r][i].filter.replace.length;w++) {
									
									repl=data.rows[r][i].filter.replace[w].from;
									to=data.rows[r][i].filter.replace[w].to;
									if ((pix[z]==repl.r)&&(pix[z+1]==repl.g)&&(pix[z+2]==repl.b)&&(pix[z+3]==repl.a)) {
										pix[z  ] = to.r;
										pix[z+1] = to.g;
										pix[z+2] =to.b;
										pix[z+3] =to.a;
									}
								}
							}
							if (data.rows[r][i].filter.color&&(pix[z+3]!=0)) {
								pix[z  ] = data.rows[r][i].filter.color.r;
								pix[z+1] = data.rows[r][i].filter.color.g;
								pix[z+2] =data.rows[r][i].filter.color.b;
								pix[z+3] =data.rows[r][i].filter.color.a;
							}
							
							// i+3 is alpha (the fourth element)
						}
						ctx.putImageData(imgd, curx, cury);
					
					}
				}
				curx+=this._images[id].width*1;
				id++;
			}
			cury+=this._images[id-1].height*1;
		}
		
	}

}

// ----------------------------------------------------------------------------
// gamecycle
// ----------------------------------------------------------------------------

/*
Gamecycle contains your basic game loop: intro, menus, 
crossfading between stages/lifes, gameover and ending. 
*/

var gamecycle={

	/**
	 * Gamecycle constructor - initializes a new game object
	 * 
	 * @param	id		unique id of object
	 * @param	group		name of group to store the object in
	 */
	createMaingame:function(id,group) {
	return gbox.addObject({
		id:id,
		group:group,
		counter:0,
		difficulty:0,
		
		// state transition
		state:50,  
		stateFirstIteration:true, 
		
		hud:{},
		
		/**
		 * This method is called whenever you load a new map. It's meant to be
		 * overridden when you create your game.
		 */
		changeLevel:function() { },
		
		/**
		 * This method is called every time a player is "reborn". This method is 
		 * meant to be overridden since you have to do garbage collection. 
		 */
		newLife:function() { },

		// game disclaimer animation (if needed)
		gameDisclaimerAnimation:function(reset) {
			return true;
		 },
		
		// game intro animation
		gameIntroAnimation:function(reset) {
			if (reset) {
				gbox.stopChannel("bgmusic");
				toys.resetToy(this,"default-blinker");
			} else {
				gbox.blitFade(gbox.getBufferContext(),{alpha:1});
				return toys.text.blink(this,"default-blinker",gbox.getBufferContext(),{font:"small",text:"LETS BEGIN!",valign:gbox.ALIGN_MIDDLE,halign:gbox.ALIGN_CENTER,dx:0,dy:0,dw:gbox.getScreenW(),dh:gbox.getScreenH(),blinkspeed:5,times:6});
			}
		 },
		 
		// level intro animation
		levelIntroAnimation:function(reset) {
			if (reset) {
				gbox.stopChannel("bgmusic");
				toys.resetToy(this,"default-blinker");
			} else {
				gbox.blitFade(gbox.getBufferContext(),{alpha:1});
				return toys.text.blink(this,"default-blinker",gbox.getBufferContext(),{font:"small",text:"GET READY!",valign:gbox.ALIGN_MIDDLE,halign:gbox.ALIGN_CENTER,dx:0,dy:0,dw:gbox.getScreenW(),dh:gbox.getScreenH(),blinkspeed:5,times:6});
			}
		 },
	  
		 // Life intro animation
		 newlifeIntroAnimation:function(reset) {
			 if (reset) {
				gbox.stopChannel("bgmusic");
				toys.resetToy(this,"default-blinker");
			} else {
				gbox.blitFade(gbox.getBufferContext(),{alpha:1});
				return toys.text.fixed(this,"default-blinker",gbox.getBufferContext(),{font:"small",text:"GET READY!",valign:gbox.ALIGN_MIDDLE,halign:gbox.ALIGN_CENTER,dx:0,dy:0,dw:gbox.getScreenW(),dh:gbox.getScreenH(),time:30});
			}
		},
		
		// gameover animation
		gameoverIntroAnimation:function(reset) {
			 if (reset) {
				gbox.stopChannel("bgmusic");
				toys.resetToy(this,"default-blinker");
			} else {
				gbox.blitFade(gbox.getBufferContext(),{alpha:1});
				return toys.text.fixed(this,"default-blinker",gbox.getBufferContext(),{font:"small",text:"GAME OVER",valign:gbox.ALIGN_MIDDLE,halign:gbox.ALIGN_CENTER,dx:0,dy:0,dw:gbox.getScreenW(),dh:gbox.getScreenH(),time:50});
			}
		},
		
		// game title animation
		gameTitleIntroAnimation:function(reset) {
			if (reset)
				gbox.stopChannel("bgmusic");
			else {
				gbox.blitFade(gbox.getBufferContext(),{alpha:1});
				gbox.blitText(gbox.getBufferContext(),{font:"small",text:"GAME TITLE",valign:gbox.ALIGN_MIDDLE,halign:gbox.ALIGN_CENTER,dx:0,dy:0,dw:gbox.getScreenW(),dh:gbox.getScreenH()-100});
			}
		},
		
		// End level animation
		endlevelIntroAnimation:function(reset) {
			 if (reset) {
				 toys.resetToy(this,"default-blinker");
			} else {
				return toys.text.blink(this,"default-blinker",gbox.getBufferContext(),{font:"small",text:"WELL DONE!",valign:gbox.ALIGN_MIDDLE,halign:gbox.ALIGN_CENTER,dx:0,dy:0,dw:gbox.getScreenW(),dh:gbox.getScreenH(),blinkspeed:5,times:10});
			}
		},
		
		// Game ending
		gameEndingIntroAnimation:function(reset) {
			if (reset) {
				toys.resetToy(this,"default-blinker");
			} else {
				gbox.blitFade(gbox.getBufferContext(),{alpha:1});
				return toys.text.blink(this,"default-blinker",gbox.getBufferContext(),{font:"small",text:"CONGRATULATIONS!",valign:gbox.ALIGN_MIDDLE,halign:gbox.ALIGN_CENTER,dx:0,dy:0,dw:gbox.getScreenW(),dh:gbox.getScreenH(),blinkspeed:5,times:10});
			}		  	
		},
		
		// PRESS START
		pressStartIntroAnimation:function(reset) {
			if (reset) {
				toys.resetToy(this,"default-blinker");
			} else {
				toys.text.blink(this,"default-blinker",gbox.getBufferContext(),{font:"small",text:"PRESS A TO START",valign:gbox.ALIGN_MIDDLE,halign:gbox.ALIGN_CENTER,dx:0,dy:Math.floor(gbox.getScreenH()/3),dw:gbox.getScreenW(),dh:Math.floor(gbox.getScreenH()/3)*2,blinkspeed:10});
				return gbox.keyIsHit("a");
			}
		},
		
		/**
		 * This method is called when the player dies.
		 */
		gameIsOver:function() { return true; },
		
		/** 
		 * Actions done during the game (i.e. stage is clear or other ending conditions)
		 */
		gameEvents:function() {
		
		},
		
		gameMenu:function(reset) {
			if (reset) {
				toys.resetToy(this,"difficulty");
			} else {
				gbox.blitFade(gbox.getBufferContext(),{alpha:0.5});
				if (toys.ui.menu(this,"difficulty",{audiooption:"default-menu-option",audioconfirm:"default-menu-confirm",font:"small",keys:{up:"up",down:"down",ok:"a",cancel:"b"},selector:">",items:["EASY","NORMAL","HARD"],x:10,y:10})) {
					if (toys.getToyValue(this,"difficulty","ok") == -1) return -1;
					else {
						this.difficulty=toys.getToyValue(this,"difficulty","selected");
						return true;
					}
				}
				return false;
			}
		},
	  
		// CHECK
		
		gameIsHold:function() { // Use this clause to check collision and kill player: if true the level is changing
			return (this.state==400)||(this.state==401);
		},
		
		isCompleted:function() {
			return (this.state==800);
		},
		
		// GAME CYCLE
		
		getNextLevel:function() {
			return this._nextlevel;
		},
		
		gotoLevel:function(level) {
			this._nextlevel=level;
			this.setState(400);
		},
		
		playerDied:function(data) {
			this._loselife=data;
			this.setState(500);
		},
		
		gameIsCompleted:function() {
			this.setState(800);
		},
		
		// private methods
		
		/**
		 * Changes the current game state
		 * 
		 * @param	st	state number
		 */
		setState:function(st) {
			this.state=st;
			this.stateFirstIteration=true;
		},
	
		/*
		 * Removes all objects in each group except the game 
		 * cycle group. Used for garbage collection when resetting the game.
		 */	
		_resetGroups:function() {
			var g=gbox.getGroups();
			for (var i=0;i<g.length;i++)
				if (g[i]!=this.group) gbox.clearGroup(g[i]);
			gbox.soloGroup(this.group);
		},

		stateIsReady:function() { this.stateFirstIteration=false; },

		blit:function() {
			switch (this.state) {

				// Disclaimer
				case 50: {
					if (this.stateFirstIteration) {
						this._resetGroups();
						this.gameDisclaimerAnimation(true);
						this.stateIsReady();
					}
					if (this.gameDisclaimerAnimation(false)) 
						this.setState(100);
					break;
				}
				// main menu
				case 100: 
				case 101:
				case 102: { // Press Start / Menu
					if (this.stateFirstIteration && (this.state == 100)) {
						this._resetGroups();
						this.gameTitleIntroAnimation(true);
					}
					this.gameTitleIntroAnimation(false);
					switch (this.state) {
						case 100: { // Press to start
							if (this.stateFirstIteration) {
								gbox.resetChannel("bgmusic");
								this.pressStartIntroAnimation(true);
								this.stateIsReady();
							}
							if (this.pressStartIntroAnimation(false)) this.setState(101);
							break;
						}
						case 101: { // Game menu
							if (this.stateFirstIteration) {
								gbox.setChannelVolume("bgmusic",0.5);
								this.gameMenu(true);
								this.stateIsReady();
							}
							
							var menu=this.gameMenu(false);
							if (menu)
								if (menu == -1) this.setState(100); else this.setState(102);
							break;
						}
						case 102: { // Fader
							if (this.stateFirstIteration) {
								this._resetGroups();
								toys.resetToy(this,"fadeout");
								this.stateIsReady();
							}
							if (toys.fullscreen.fadeout(this,"fadeout",gbox.getBufferContext(),{fadespeed:0.05,audiochannelfade:"bgmusic"}))
								this.setState(200);
							break;
						}
						break;
					}
					break;
				}
				case 200:// Game intro animation
				
				case 300:// Start game
				case 301:// Game is going
				
				case 400:// Fade out to change level
				case 401:// Level animation (levelIntroAnimation)
				case 402:// Fade in to next level
				
				case 500:// Wait after dead
				case 501:// Dead fadeout
				
				case 600:// Next life intro
				case 601:// New life fadein
				
				case 700:// Gameover animation

				case 800:// Fade out game ending
				case 801:// Game ending

				{ // Game playing
					if (this.stateFirstIteration) {
						switch (this.state) {
							case 200: { // Game intro
								toys.resetToy(this,"fadein");
								this.level=null;
								this._nextlevel=null;
								this.hud=toys.ui.hud("maingamehud");
								
								this.initializeGame();
								this.gameIntroAnimation(true);
								
								break;
							}
							case 300: {
								// Game start
								this.level=this._nextlevel;
								gbox.playAllGroups();
								this.changeLevel(this._nextlevel);
							}
							case 800:
							case 400:{
								this.endlevelIntroAnimation(true);
								toys.resetToy(this,"fadeout");
								break;
							}
							case 501: {
								toys.resetToy(this,"fadeout");
								break;
							}
							case 401: {
								gbox.soloGroup(this.group);
								this.levelIntroAnimation(true);
								break;
							}
							case 402: {
								toys.resetToy(this,"fadein");
								this.level=this._nextlevel;
								gbox.playAllGroups();
								this.changeLevel(this._nextlevel);
								break;
							}
							case 600: {
								gbox.soloGroup(this.group);
								this.newlifeIntroAnimation(true);
								break;
							}
							case 500: {
								this._loselife.counter=0;
								break;
							}
							case 601: {
								toys.resetToy(this,"fadein");
								this.newLife();
								gbox.playAllGroups();
								break;
							}
							case 700: {
								gbox.resetChannel("bgmusic");
								gbox.soloGroup(this.group);
								this.gameoverIntroAnimation(true);
								break;
							}
							case 801: {
								gbox.resetChannel("bgmusic");
								gbox.soloGroup(this.group);
								this.gameEndingIntroAnimation(true);
								break;
							}
						}
						this.stateIsReady();
					}
					
					switch (this.state) {
						case 200: { // Game intro
							if (this.gameIntroAnimation(false)) this.setState(300);
							break;
						}
						case 601: // Fade in with new life
						case 402: // Fade in after level change
						case 300: { // Fade in at the beginning of the game
							if (toys.fullscreen.fadein(this,"fadein",gbox.getBufferContext(),{fadespeed:0.05,audiochannelfade:"bgmusic"})) this.setState(301);
							break;
						}
						case 301: { // Ingame stuff
							this.gameEvents();
							break;
						}
						case 400: { // Fade out before changing the level
							if (this.endlevelIntroAnimation(false))
								if (toys.fullscreen.fadeout(this,"fadeout",gbox.getBufferContext(),{fadespeed:0.05,audiochannelfade:"bgmusic"})) this.setState(401);
							break;
						}
						case 800: { // Fade out before game ending
							if (this.endlevelIntroAnimation(false))
								if (toys.fullscreen.fadeout(this,"fadeout",gbox.getBufferContext(),{fadespeed:0.05,audiochannelfade:"bgmusic"})) this.setState(801);
							break;
						}
						case 501: { // Fade out after dead
							if (toys.fullscreen.fadeout(this,"fadeout",gbox.getBufferContext(),{fadespeed:0.05,audiochannelfade:"bgmusic"})) 
								if (this.gameIsOver())
									this.setState(700); // GAME OVER
								else
									this.setState(600);
							break;
						}
						case 401:{ // Level intro animation
							if (this.levelIntroAnimation(false)) this.setState(402);
							break;
						}
						case 500: { // Wait after dead
							this._loselife.counter++;
							if (this._loselife.counter==this._loselife.wait) this.setState(501);
							break;
						}
						case 600:{ // New life intro
							if (this.newlifeIntroAnimation(false)) this.setState(601);
							break;
						}
						case 700:{ // gameover
							if (this.gameoverIntroAnimation(false)) this.setState(100); // Restart game
							break;
						}
						case 801:{ // Game ending
							if (this.gameEndingIntroAnimation(false)) {
								this._loselife={ending:true};
								this.setState(700); // Game over
							}
							break;
						}
					}
					this.hud.blit();
					break;
				}
			}
		}
	});
	}
}

</script>
</head>

<body scroll="no">
</body>
<script type="text/javascript">
	var ending;
	var maingame;
	var song={};
	var ending={};
	var audioserver;
 
	// Add seconds lines to the song and stats. You have to call this once the song data is loaded into the resource file.
	function addMarkerToSong(sng) {
		var i=1;
		var p=0;
		var worked=[];
		sng.notes=0;
		while (i<sng.duration) {
			while ((p<sng.track.length)&&(sng.track[p].time<i)) { worked.push(sng.track[p]); sng.notes++; p++ }
			worked.push({ time:i, d:[0,0,0,0,0,1],nofret:true});
			i+=2;
		}
		worked.push({ time:sng.duration, d:[0,0,0,0,0,1],nofret:true,ended:true});
		worked.push({ time:sng.duration+10, d:[0,0,0,0,0,1],nofret:true});
		sng.track=worked;
	}
 
	function go() {
 
		gbox.setGroups(["background","field","sparks","overlay","gamecycle"]);
		gbox.setAudioChannels({bgmusic:{volume:0.8},audio:{volume:0.9},guitar:{volume:1.0},sfx:{volume:1.0}});
		maingame=gamecycle.createMaingame("gamecycle","gamecycle");	
 
		maingame.gameIntroAnimation=function() { return true; }
		maingame.gameIsOver=function() { return true; } // No lives!
		maingame.gameTitleIntroAnimation=function(reset) { 
			if (reset) {
				toys.resetToy(this,"rising"); 
				toys.resetToy(this,"flash"); 
				this.udcnt=0;
				gbox.playAudio("intro");
			} else {
				if (toys.getToyStatus(this,"rising")) {
			  		gbox.blitFade(gbox.getBufferContext(),{alpha:1,color:"rgb("+(help.upAndDown(this.udcnt,100)*2)+",0,0)"});
			  		gbox.blitAll(gbox.getBufferContext(),gbox.getImage("wp"),{dx:0,dy:0});
		 	 		toys.fullscreen.fadein(this,"flash",gbox.getBufferContext(),{fadespeed:0.05,color:gbox.COLOR_WHITE});
		 	 		this.udcnt=(this.udcnt+1)%100;
			  	} else
			  		gbox.blitFade(gbox.getBufferContext(),{alpha:1});
				toys.logos.linear(this,"rising",{image:"logo",x:gbox.getScreenHW()-gbox.getImage("logo").hwidth,y:20,sx:gbox.getScreenHW()-gbox.getImage("logo").hwidth,sy:gbox.getScreenH(),audioreach:"explosion",speed:10});
			}
		};
		// gameover animation
		maingame.gameoverIntroAnimation=function(reset) {
			 if (reset) {
				gbox.stopChannel("bgmusic");
				gbox.stopChannel("audio");
				gbox.stopChannel("guitar");
				toys.resetToy(this,"default-blinker");
			} else {
				gbox.blitFade(gbox.getBufferContext(),{alpha:1});
				return toys.text.fixed(this,"default-blinker",gbox.getBufferContext(),{font:"small",text:"GAME OVER",valign:gbox.ALIGN_MIDDLE,halign:gbox.ALIGN_CENTER,dx:0,dy:0,dw:gbox.getScreenW(),dh:gbox.getScreenH(),time:50});
			}
		};		
		maingame.gameDisclaimerAnimation=function(reset) {
			if (reset) {
				toys.resetToy(this,"warn-timer"); 
				toys.resetToy(this,"fadein"); 
				toys.resetToy(this,"fadeout"); 
			} else {
				gbox.blitAll(gbox.getBufferContext(),gbox.getImage("warn"),{dx:0,dy:0});
				// A nice way to enqueue effects :)
				if (
					toys.fullscreen.fadein(this,"fadein",gbox.getBufferContext(),{fadespeed:0.05}) &&
					toys.timer.after(this,"warn-timer",300) &&
					toys.fullscreen.fadeout(this,"fadeout",gbox.getBufferContext(),{fadespeed:0.01})
				) return true;
				return false;
			}
		};
		// Song selector
		maingame.gameMenu=function(reset) {
			if (reset) {
				this._songs=[];
				this._songsid=[];
				for (var i in song) {
					this._songsid.push(i);
					this._songs.push("'"+song[i].title+"' by "+song[i].artist);
				}
				toys.resetToy(this,"songselect");
			} else {
				gbox.blitFade(gbox.getBufferContext(),{alpha:0.5});
				gbox.blitText(gbox.getBufferContext(),{font:"big", dx:10, dy:10, text:"TRACKLIST"});
				gbox.blitText(gbox.getBufferContext(),{font:"small2", dx:10, dy:gbox.getScreenH()-(gbox.getFont("small2").tileh*2)-10, text:"Left/right/blue/orange frets to move."});
				gbox.blitText(gbox.getBufferContext(),{font:"small2", dx:10, dy:gbox.getScreenH()-gbox.getFont("small2").tileh-10, text:"A button or green button to start."});
				
				if (toys.ui.menu(this,"songselect",{audiooption:"default-menu-option",audioconfirm:"default-menu-confirm",font:"small",keys:{up:"left",down:"right",ok:"a",cancel:"b"},selector:">",items:this._songs,x:10,y:40})) {
					if (toys.getToyValue(this,"difficulty","ok") == -1) return -1;
					else {
						this._selectedsong=this._songsid[toys.getToyValue(this,"songselect","selected")];
						return true;
					}
				}
				return false;
			}
		},
		
		// Game ending
		  maingame.gameEndingIntroAnimation=function(reset){
		  	if (reset) {
		  		toys.resetToy(this,"intro-animation");
		  		
				var endingtheme="song-mix";
				if (maingame.hud.getValue("stars","value")>8) ; // Extra song for winners
		  		gbox.hitAudio(endingtheme);
 
		  	} else {
		  		gbox.blitFade(gbox.getBufferContext(),{alpha:1});
		  		return toys.dialogue.render(this,"intro-animation",ending.credits);
		  	}
		  }
		  
		 // YOU ROCK!
		maingame.endlevelIntroAnimation=function(reset) {
			 if (reset) {
				 toys.resetToy(this,"default-blinker");
				 this._scoremessage="NICE JOB!";
				switch (maingame.hud.getValue("stars","value")) {
					case 9: { this._scoremessage="YOU ROCK!"; break }
					case 10: { this._scoremessage="OMG FIVE STARS !1!!"; break }
				}
			} else {
				return toys.text.blink(this,"default-blinker",gbox.getBufferContext(),{font:"big",text:this._scoremessage,valign:gbox.ALIGN_MIDDLE,halign:gbox.ALIGN_CENTER,dx:0,dy:0,dw:gbox.getScreenW(),dh:gbox.getScreenH(),blinkspeed:5,times:10});
			}
		},
 
 
	
		maingame.initializeGame=function() {
		 	// Build hud
		 	maingame.hud.setWidget("label",{widget:"label",font:"small",value:"SCORE",dx:10,dy:10,clear:true});
		 	maingame.hud.setWidget("score",{widget:"label",font:"small",value:0,dx:58,dy:10,postpad:10,padwith:" ",clear:true});
		 	maingame.hud.setWidget("label",{widget:"label",font:"big",value:"COMBO X",dx:10,dy:20,clear:true});
		 	maingame.hud.setWidget("combo",{widget:"label",font:"big",value:0,dx:122,dy:20,postpad:4,padwith:" ",clear:true});
		 	maingame.hud.setWidget("energy",{widget:"gauge",tileset:"energy",value:0,maxvalue:100,minvalue:0,value:60,dx:10,dy:40});
			maingame.hud.setWidget("stars",{widget:"symbols",tiles:[1,0],minvalue:0,maxvalue:10,value:0,maxshown:5,tileset:"stars",emptytile:2,dx:10,dy:60,gapx:12,gapy:0});
 
		 	// The song title screen.
		 	gbox.addObject({
				group:"overlay",
				
		 		stilltime:100,
		 		initialize:function() {
					toys.resetToy(this,"rising1"); 
					toys.resetToy(this,"rising2"); 
					toys.resetToy(this,"rising3"); 
					toys.resetToy(this,"rising4"); 
				},
		 		blit:function() {
		 			var a=toys.logos.linear(this,"rising1",{tileset:"songbar",tile:0,sx:-gbox.getTiles("songbar").tilew,sy:gbox.getScreenH()-47,x:0,y:gbox.getScreenH()-47,alpha:0.8,speed:15});
		 			a=(toys.logos.linear(this,"rising2",{font:"small",text:song[gbox.getObject("field","player").track].artist,sx:gbox.getScreenW(),sy:gbox.getScreenH()-44,x:10,y:gbox.getScreenH()-44,speed:10})&&a);
		 			a=(toys.logos.linear(this,"rising3",{font:"big",text:song[gbox.getObject("field","player").track].title,sx:10,sy:-16,x:10,y:gbox.getScreenH()-35,speed:10,audioreach:"coin"})&&a)
		 			a=(toys.logos.linear(this,"rising4",{font:"small2",text:song[gbox.getObject("field","player").track].contact,sx:10,sy:gbox.getScreenH(),x:10,y:gbox.getScreenH()-16,speed:1})&&a)
		 			
		 			if (a) {
		 				this.stilltime--;
		 				if (this.stilltime==0) gbox.trashObject(this);
		 			}
		 		}
		 	});
		 	
		 	// The fretboard. A basic GH clone is all this.
			gbox.addObject({
				id:"player",
				group:"field",
	
				x:gbox.getScreenHW(), // Center of the cone 
				basey:gbox.getScreenH(), // Y position of the lower part of the cone
				
				depthtime:1.5, // Depth of the cone in time
				depthpixel:130, // Depth of the cone in pixel
				conepixel:100, // With of the pixel cone
				largerconepixel:135, // Larger cone, for time lines
	
				linetime:0.2, // The line is <secs> before the bottom
				frettime:0.1, // Delay tollerance for hitting the notes
				track:maingame._selectedsong, // Playing track...
				
				block:0, // current block
				shaketime:0, // Time to shake the table
				
				fretkey:["a","b","c","left","right"],
				hitkey:["up","down"],
				
				errors:0, // Error counter
				combo:0, // Combo counter
				introy:0, // For sliding fretboard
				ready:80, // Prestart
				killed:false, // Is gameover?
				notes:0, // Notes hits correctly
				
				drawpattern:function(yp,row,frames,discs,fire) {
					var w;
					var h;
					var ts;
					var sx;
					var x;
					
					w=gbox.getTiles(frames).tilew-((yp*gbox.getTiles(frames).tilew)/this.depthpixel);
					h=gbox.getTiles(frames).tileh-((yp*gbox.getTiles(frames).tileh)/this.depthpixel);
					for (var d=0;d<5;d++) 
						if (row[d]) {
							ts=(row[d]==2?"up":"")+frames;
							sx=this.conepixel-((yp*this.conepixel)/this.depthpixel);
							x=d*sx/2;
							if (fire&&fire[d])
								toys.generate.sparks.simple(this,"sparks",null,{x:this.x-(w/2)-sx+x,y:this.basey-yp-gbox.getTiles("discfire").tileh,animspeed:1,accy:-5,tileset:"discfire"});
							discs.push({img:ts,data:{x:d*gbox.getTiles(ts).tilew,y:gbox.getTiles(ts).gapy,h:gbox.getTiles(ts).tileh,w:gbox.getTiles(ts).tilew,dw:w,dh:h,dx:this.x-(w/2)-sx+x,dy:this.basey-yp-h+this.introy,alpha:1}});
						}
					if (row[5])
						discs.push({rect:{x:this.x-this.largerconepixel+((yp*this.largerconepixel)/this.depthpixel),y:this.basey-yp,w:(this.largerconepixel-((yp*this.largerconepixel)/this.depthpixel))*2,h:1,color:"rgb(100,100,100)"}});
				},
				
				initialize:function() {
					
					for (var i=0;i<song[this.track].track.length;i++) // Cleanup
						song[this.track].track[i].done=0; // 0: doable, 1: failed, 2:done
					this.introy=this.depthpixel;
					this.pxline=this.depthpixel-(((this.depthtime-this.linetime)*this.depthpixel)/this.depthtime);
					// Load the wallpaper image
					gbox.addImage("ingame",song[this.track].wallpaper);
					// Load audio from track's data.
					if (song[this.track].audio) gbox.addAudio("song-audio",song[this.track].audio,{channel:"audio",background:true});
					if (song[this.track].guitar) gbox.addAudio("song-guitar",song[this.track].guitar,{channel:"guitar",background:true});
					if (song[this.track].mix) gbox.addAudio("song-mix",song[this.track].mix,{channel:"bgmusic"});
				},
				
				blit:function() {
					gbox.blitAll(gbox.getBufferContext(),gbox.getImage("ingame"),{dx:0,dy:this.introy*2});
					if (this.introy>0) this.introy=Math.floor(this.introy/1.1); else // Sliding fretboard on start
					if (this.ready) this.ready--;
 
					var fretb=[1,1,1,1,1];
					for (var i=0;i<this.fretkey.length;i++) 
						if (gbox.keyIsPressed(this.fretkey[i])) fretb[i]=2;
					var hit=false;
					for (var i=0;i<this.hitkey.length;i++)
						if (gbox.keyIsHit(this.hitkey[i])) { hit=true; break; }
					if (this.shaketime) this.shaketime--;
					gbox.blitTile(gbox.getBufferContext(),{tileset:"table",tile:0,dx:this.x-gbox.getTiles("table").tilehw-(this.shaketime%2),dy:this.basey-gbox.getTiles("table").tileh+this.introy,alpha:1});
					
					var discs=[];
					var songtime;
 
					if (!this.ready&&!this.killed) {
						if (gbox.getAudioIsSingleChannel())
							songtime=gbox.getAudioPosition("song-mix");
						else if (song[this.track].guitar)
							songtime=((gbox.getAudioPosition("song-audio")+gbox.getAudioPosition("song-guitar"))/2);
						else
							songtime=gbox.getAudioPosition("song-audio");
						
						var lowtime=songtime-this.linetime;
		
		
						while (song[this.track].track[this.block].time<lowtime) this.block++;
		
						var b=this.block;
						var s=song[this.track].track[b].time;
						var first=true;
						var yp;
						var error=0; // 0: no error, 1:note-ok, 2: error, 3:error-missed
						var fire;
						
						while (s<lowtime+this.depthtime) {
							if (song[this.track].track[b].done!=2) {
								if (first&&!song[this.track].track[b].nofret) {
									if (hit) { // If fretting...
										if (song[this.track].track[b].done==0) { // ...and the note is playable
											if (Math.abs(s-songtime)<=this.frettime) { //...and the time is right
												var error=1;
												for (var i=0;i<fretb.length;i++)
													if (((fretb[i]==2)&&(!song[this.track].track[b].d[i]))||((fretb[i]!=2)&&(song[this.track].track[b].d[i]))) {
														error=2;
														break;
													}
												if (error!=1)
													song[this.track].track[b].done=1; // Note failed (wrong keys)
											} else
												error=2; // Hitting a note too much in the future!
		
										} else
											error=2; // Wrong timing. Hitting an invalidated note
									} else if (((songtime-s)>this.frettime)&&(song[this.track].track[b].done==0)) { // If not fretting and the note goes away...
										song[this.track].track[b].done=1;
										error=3;
									}
									if (error==1) {
										this.notes++;
										maingame.hud.addValue("combo","value",1);
										maingame.hud.addValue("score","value",maingame.hud.getValue("combo","value")*10);
										maingame.hud.addValue("energy","value",Math.ceil(maingame.hud.getValue("combo","value")/2));
										maingame.hud.addValue("energy","value",Math.ceil(maingame.hud.getValue("combo","value")/2));
										maingame.hud.setValue("stars","value",Math.floor(10*this.notes/song[this.track].notes));
										
										
										fire=song[this.track].track[b].d;
										song[this.track].track[b].done=2;
										if (!gbox.getAudioIsSingleChannel()&&song[this.track].guitar&&gbox.getAudioMute("song-guitar"))
											gbox.setAudioUnmute("song-guitar");
									}
									first=false;
								}
								if (song[this.track].track[b].done!=2) {
									yp=Math.floor((((s-lowtime)*this.depthpixel)/this.depthtime));
									this.drawpattern(yp,song[this.track].track[b].d,"discs",discs);
								}
							}
							if (song[this.track].track[b].ended) {
								song[this.track].track[b].done=2;
								maingame.gameIsCompleted();
							}
							b++;
							if (b>=song[this.track].track.length)
								break;
							else
								s=song[this.track].track[b].time;
						}
						if (hit&&first) // Hitting when nothing is on the screen!
							error=2;
						if (error>1) {
							if (!gbox.getAudioIsSingleChannel()) {
								if (song[this.track].guitar)  gbox.setAudioMute("song-guitar");
								if (error==2)
									gbox.hitAudio(song[this.track].errors[help.random(0,song[this.track].errors.length)]);
								else
									gbox.hitAudio(song[this.track].errorleave[help.random(0,song[this.track].errorleave.length)]);
							}
							maingame.hud.setValue("combo","value",0);
							maingame.hud.addValue("energy","value",-5);
							if (maingame.hud.getValue("energy","value")==0) {
								this.killed=true;
								maingame.playerDied({wait:1});
							}
							this.shaketime=10;
							this.errors++;
						}
					}
					this.drawpattern(this.pxline,fretb,"rings",discs,((this.introy==0)&&this.ready?[this.ready==80,this.ready==75,this.ready==70,this.ready==65,this.ready==60]:fire));
					if (!gbox.getAudioIsSingleChannel()&&(this.introy==0)&&(this.ready>=60)&&(this.ready%5==0)) gbox.hitAudio("explosion");
					for (var x=discs.length-1;x>=0;x--)
						if (discs[x].rect)
							gbox.blitRect(gbox.getBufferContext(),discs[x].rect);
						else
							gbox.blit(gbox.getBufferContext(),gbox.getImage(gbox.getTiles(discs[x].img).image),discs[x].data);
					if ((this.introy==0)&&(this.ready)) {
						if ((this.ready==80)&&!gbox.getAudioIsSingleChannel()) {
							if (song[this.track].guitar) gbox.hitAudio("song-guitar",{mute:true});
							gbox.hitAudio("song-audio",{mute:true});
						}
						if (this.ready==1) {
							if (gbox.getAudioIsSingleChannel())
								gbox.hitAudio("song-mix");
							else {
								if (song[this.track].guitar) {
									gbox.setAudioPosition("song-guitar",0);
									gbox.setAudioUnmute("song-guitar");
								}
								gbox.setAudioPosition("song-audio",0);
								gbox.setAudioUnmute("song-audio");
							}
						}
					}
				}
			});
			// Once here, buy a plasic guitar!
		}
		
		gbox.go();
	
	};
	
	// BOOTSTRAP
	window.addEventListener('load', function () {
 
		// Put additional licenses here!
		help.akihabaraInit({title:"Akiba Hero",padmode:"fretboard",splash:{footnotes:["Music usage permitted by their owner.","Credits on song select screen and ending."]}});
		gbox.setCallback(go);
		
		audioserver="audio/"
		
		gbox.addBundle({file:"bundle.js"}); // Audio, sprites, fonts etc. are loaded here now. Cleaner code! Btw you can still load resources from the code, like in Capman.
		// Complex bundles here. Interesting stuff. Have a look to this bundle.js
		
		gbox.loadAll();
	}, false);
 
</script>
</html>
<?php
mysql_close($db);
?>