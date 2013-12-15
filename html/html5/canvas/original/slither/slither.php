<!--
----------------------------------------------------------------------------------------
Slither: Taking a Bite Out of Canvas (1.0)
----------------------------------------------------------------------------------------
A modern replica of the 2D, retro "Snake" or "Nibbles" game, wherein the player navigates
a snake around the screen eating objects that are scattered about the playing area. This
action in turn grows the length of the snake and increases the player's score. When all
food items have been eaten, the next level starts: each with a touch more of difficulty
(poison items, walls, and so on).
----------------------------------------------------------------------------------------
 
http://www.brokenresolve.com/2010/05/slither-game/ 
Released 05-10-2010.
 
Copyright 2010 Erik Johnson.
Licensed under the LGPL Version 3 License.
 
Includes modified "KSnake" graphic, part of the "Crystal Project Icons" icon set.
Copyright 2006-2007 Everaldo Coelho. Released under the LGPL license.
 
Includes generated spinner ajax-loader graphic.
Copyright 2007-2010 LoadInfo.net. Free for personal use.
 
Includes modified "Snake (Snakes) Icon", part of the "Black Ink Grunge Stamps
Textures Icons" icon set and "Checkered Racing Flags Icon", "Clear Skull (Skulls) Icon",
part of the "Glossy Black Icons" icon set. Copyright 2009 MySiteMyWay ETC Network.
Free for personal use.
 
Includes modified "Pause Icon" graphic, part of the "Vista Style Play/Stop/Pause Icon Set".
Copyright 2006-2010 Icons-Land. Free for personal use.
 
Includes modified "Christmas, Star icon", part of the "Xmas pack" icon set.
Copyright 2006 Blackblurr. Free for non-commercial use.
 
----------------------------------------------------------------------------------------
This file is part of Slither.
 
Slither is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
 
Slither is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with Slither.  If not, see <http://www.gnu.org/licenses/>.
// -->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<link rel="shortcut icon" href="favicon.png" />
<title>Slither</title>
<script type="text/javascript" src="/hnsdesktop/jquery.js"></script>
<!-- jQuery 1.4.2 -->
<!-- <script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.4.2/jquery.min.js"></script> -->
<!-- Slither Core + Game Driver Code -->
<script type="text/javascript">
(function(window,document) {
var Slither = {
AUTOLOAD_BASE_PATH:false,AUTOLOAD_SCRIPTS:false,FRAME_RATE:40,PLAYER_CLASS:"Slither.Objects.Snake",SpeedLevels:{Min:1,Max:5},Headings:{Left:1,Up:2,Right:3,Down:4},UpgradeItemTypes:{ExtraLife:1,TimeWarp:2,WallBreaker:3,RatParalyzer:4,ShieldsUp:5},PoisonItemTypes:{Deadly:1,Toxic:2,Obnoxious:3},WallTypes:{FourCorner:1,CrossHair:2,CrossDoubleBar:3,CornerSofa:4,BrokenSquare:5,TripleTee:6,GoalSplit:7,ClownInsanity:8},toString:function(){return"Slither"},loadScripts:function(base_path,scripts){scripts=Slither.makeArray(scripts);if(scripts.length==0){Slither.ready(true);return}var head=document.getElementsByTagName("head")[0];var s=document.createElement("script");s.type="text/javascript";s.src=(base_path||"")+scripts[0];s.onload=function(){Slither.loadScripts(base_path,scripts.slice(1))};head.appendChild(s)},ready:function(callback){if(!Slither.ready.list){Slither.ready.list=[]}if(typeof(callback)=="function"){Slither.ready.list.push(callback)}else{Slither.ready.loaded=(callback===true)}if(Slither.ready.loaded){Slither.Objects.PlayerClass=eval(Slither.PLAYER_CLASS);if(typeof(Slither.Objects.PlayerClass)!="function"){throw new Error("Failed to load object referenced by Slither.PLAYER_CLASS")}var len=Slither.ready.list.length;for(var i=0;i<len;i++){Slither.ready.list[i].call(document)}Slither.ready.list=[]}},extend:function(base,sub){var proxy=function(){};proxy.prototype=base.prototype;sub.prototype=new proxy();sub.prototype.init=sub;sub.base=base.prototype;if(base.prototype.init==Object.prototype.init){base.prototype.init=base}},setDefaults:function(obj,options,deep){var newObj=new Object();for(var option in obj){var type=typeof(obj[option]);if(deep&&(type=="object"||type=="function")){newObj[option]=Slither.setDefaults(obj[option],{},deep)}else{newObj[option]=obj[option]}}if(options){for(var option in options){newObj[option]=options[option]}}return newObj},genNumber:function(low,high){if(low==high){return low}return Math.floor(Math.random()*(high-low+1))+low},makeArray:function(obj){if(!obj&&typeof(obj)!="number"){return[]}if(Object.prototype.toString.call(obj)==="[object Array]"){return Array.prototype.slice.call(obj,0)}return[obj]},Core:{},Objects:{PlayerClass:function(){throw new Error("Slither.Objects.PlayerClass failed to load")}}};var Base=function(boundaries,context,options){options=Slither.setDefaults(Base.DEFAULT_OPTIONS,options);this.boundaries=boundaries;this.ctx=context;this.body_blocks=[];this.block_size=Base.DEFAULT_OPTIONS.block_size;this.fill_style=options.fill_style;this.animated=options.animated;this.setSpeed(options.speed);if(!Slither.Core.EventManager.register("collide",this.onCollide,this)){throw new Error("Failed to register 'collide' event for new "+this+" object")}if(!Slither.Core.EventManager.register("alloc-recv",this.onAllocRecv,this)){throw new Error("Failed to register 'alloc-recv' event for new "+this+" object")}if(typeof(options.start_x)!="undefined"&&typeof(options.start_y)!="undefined"){this.allocBlock(options.start_x,options.start_y)}};Base.DEFAULT_OPTIONS={fill_style:"#000",block_size:10,start_x:undefined,start_y:undefined,animated:false,speed:Slither.SpeedLevels.Min,};var _Base=Base;Base=Base.prototype;Base.drawBlock=function(x,y){this.ctx.fillStyle=this.fill_style;this.ctx.fillRect(x,y,this.block_size,this.block_size)};Base.eraseBlock=function(x,y){this.ctx.clearRect(x,y,this.block_size,this.block_size);Slither.Core.EventManager.trigger("dealloc",this,{coord:{x:x,y:y}})};Base.allocBlock=function(x,y){if(typeof(x)=="undefined"||typeof(y)=="undefined"){return}Slither.Core.EventManager.trigger("alloc",this,{coord:{x:x,y:y}})};Base.adjustBlockWrap=function(coord){if(coord.y<0){coord.y=this.boundaries.height-this.block_size;return}if(coord.y>this.boundaries.height-this.block_size){coord.y=0;return}if(coord.x<0){coord.x=this.boundaries.width-this.block_size;return}if(coord.x>this.boundaries.width-this.block_size){coord.x=0}};Base.drawRoundedRect=function(x,y,width,height,radius){this.ctx.beginPath();this.ctx.moveTo(x,y+radius);this.ctx.lineTo(x,y+height-radius);this.ctx.quadraticCurveTo(x,y+height,x+radius,y+height);this.ctx.lineTo(x+width-radius,y+height);this.ctx.quadraticCurveTo(x+width,y+height,x+width,y+height-radius);this.ctx.lineTo(x+width,y+radius);this.ctx.quadraticCurveTo(x+width,y,x+width-radius,y);this.ctx.lineTo(x+radius,y);this.ctx.quadraticCurveTo(x,y,x,y+radius);this.ctx.fill()};Base.onAllocRecv=function(sender,data){if(data.target===this){this.body_blocks.push({x:data.coord.x,y:data.coord.y})}};Base.onCollide=function(sender,data){if(data.receiver===this&&data.initiator instanceof Slither.Objects.PlayerClass){Slither.Core.EventManager.trigger("eat",this,{coord:{x:data.coord.x,y:data.coord.y}})}};Base.toString=function(){return"Base"},Base.draw=function(){if(!this.animated&&this.drawn){return}for(var i=0;i<this.body_blocks.length;i++){this.drawBlock(this.body_blocks[i].x,this.body_blocks[i].y)}this.drawn=true};Base.clear=function(unregister){for(var i=0;i<this.body_blocks.length;i++){this.eraseBlock(this.body_blocks[i].x,this.body_blocks[i].y)}this.body_blocks=[];this.drawn=false;if(unregister!==false){Slither.Core.EventManager.unregister("collide",this.onCollide,this);Slither.Core.EventManager.unregister("alloc-recv",this.onAllocRecv,this)}};Base.getHeading=function(){return this.heading||Slither.Headings.Right};Base.setHeading=function(heading){if(!heading){heading=Slither.Headings.Right}else{if(heading=="random"){var keys=[];for(var key in Slither.Headings){keys.push(key)}var key=null,last=this.getHeading();do{key=keys[Slither.genNumber(0,keys.length-1)]}while(Slither.Headings[key]==last);heading=Slither.Headings[key]}}for(var i in Slither.Headings){if(heading==Slither.Headings[i]){this.heading=heading;break}}};Base.getSpeed=function(){return this.speed||Slither.SpeedLevels.Min};Base.setSpeed=function(speed){if(!speed||speed<Slither.SpeedLevels.Min){speed=Slither.SpeedLevels.Min}else{if(speed>Slither.SpeedLevels.Max){speed=Slither.SpeedLevels.Max}else{if(speed=="random"){speed=Slither.genNumber(Slither.SpeedLevels.Min,Slither.SpeedLevels.Max)}}}this.speed=speed},Base=_Base;Slither.Objects.Base=Base;Slither.Core.ObjectManager={objects:[],types:{},searchForObject:function(obj){if(!obj){return -1}for(var i=0;i<this.objects.length;i++){if(this.objects[i]===obj){return i}}return -1},handleFrames:function(obj){if(typeof(obj.draw)!="function"||typeof(obj.getSpeed)!="function"){return}var frame_diff=Math.round(Slither.FRAME_RATE/Slither.SpeedLevels.Max)*obj.getSpeed()-Slither.FRAME_RATE;if(Math.abs(frame_diff)>=Slither.FRAME_RATE){throw new Error("Slither expects a higher frame rate; try 30fps or better.")}var frame_count=Math.round(Slither.FRAME_RATE/(Slither.FRAME_RATE-Math.abs(frame_diff)));if(frame_diff<0){if(!obj.frame_skip_ctr){obj.frame_skip_ctr=0}if(++obj.frame_skip_ctr<=frame_count){return}obj.frame_skip_ctr=0}obj.draw()},draw:function(){for(var i=0;i<this.objects.length;i++){this.handleFrames(this.objects[i])}},toString:function(){return"ObjectManager"},add:function(obj){if(!obj){return false}if(this.searchForObject(obj)==-1){this.objects.push(obj);var type=typeof(obj);if(!(type in this.types)){this.types[type]=1}else{this.types[type]++}return true}return false},remove:function(obj){var index=this.searchForObject(obj);if(index==-1){return false}if(typeof(obj.clear)=="function"){obj.clear()}this.objects.splice(index,1);var type=typeof(obj);if(type in this.types){this.types[type]--;if(this.types[type]==0){delete this.types[type]}}return true},removeAll:function(){this.stop();for(var i=0;i<this.objects.length;i++){var obj=this.objects[i];if(typeof(obj.clear)=="function"){obj.clear()}}this.objects=[];this.types={}},getList:function(type){var obj_list=[];for(var i=0;i<this.objects.length;i++){var obj=this.objects[i];if(!type||obj instanceof type||typeof(obj)==type){obj_list.push(obj)}}return obj_list},getCount:function(type){if(!type){return this.objects.length}var count=0,type_str=(typeof(type)=="string")?type:typeof(type);if(type in this.types){return this.types[type]}for(var i=0;i<this.objects.length;i++){var obj=this.objects[i];if(obj instanceof type||typeof(obj)==type){count++}}return count},start:function(frame_rate){if(!this.started&&frame_rate>=0){var _this=this,last_time=(new Date()).getTime(),target_ms=Math.ceil(1000/frame_rate),animate=function(){if(!_this.started){return}var now=(new Date()).getTime();var diff=now-last_time;_this.draw();var delay=target_ms;if(diff>delay){delay=Math.max(2*delay-diff,1)}last_time=now;if(_this.timer_id){clearTimeout(_this.timer_id)}_this.timer_id=setTimeout(animate,delay)};this.started=true;animate()}},stop:function(){if(this.timer_id){clearTimeout(this.timer_id);this.timer_id=null}this.started=false}};Slither.Core.EventManager={events:{},searchForCallback:function(handlers,callback,owner){if(!handlers||typeof(callback)!="function"){return -1}for(var i=0;i<handlers.length;i++){if(owner){if(handlers[i].callback===callback&&handlers[i].owner===owner){return i}}else{if(handlers[i].callback===callback){return i}}}return -1},toString:function(){return"EventManager"},register:function(event_type,callback,owner){if(typeof(callback)!="function"){return false}if(!this.events[event_type]){this.events[event_type]=[]}var handlers=this.events[event_type];if(this.searchForCallback(handlers,callback,owner)==-1){var handler={callback:callback};if(owner){handler.owner=owner}handlers.push(handler);return true}return false},unregister:function(event_type,callback,owner){if(typeof(callback)!="function"){return false}if(this.events[event_type]){var handlers=this.events[event_type];var index=this.searchForCallback(handlers,callback,owner);if(index!=-1){handlers.splice(index,1);if(handlers.length==0){this.clear(event_type)}return true}}return false},trigger:function(event_type,sender,data,target_only){if(this.events[event_type]){for(var i=0;i<this.events[event_type].length;i++){var handler=this.events[event_type][i];if(target_only&&target_only!==handler.owner){continue}if(typeof(handler.callback)=="function"){var res;if(handler.owner){res=handler.callback.call(handler.owner,sender,data)}else{res=handler.callback(sender,data)}if(res===false){break}}}}},clear:function(event_type){if(this.events[event_type]){delete this.events[event_type];return true}return false},clearAll:function(){this.events={}}};Slither.Core.LevelManager={levels:[],level:-1,searchForLevel:function(callback){if(!this.levels||typeof(callback)!="function"){return -1}for(var i=0;i<this.levels.length;i++){if(this.levels[i]===callback){return i}}return -1},toString:function(){return"LevelManager"},add:function(callback,scope){if(typeof(callback)!="function"){return false}if(this.searchForLevel(callback)==-1){var activation_info={callback:callback};if(scope){activation_info.scope=scope}this.levels.push(activation_info);return true}return false},remove:function(callback){if(typeof(callback)!="function"){return false}var index=this.searchForLevel(callback);if(index!=-1){this.levels.splice(index,1);if(index<=this.level){this.level--}return true}return false},removeAll:function(){this.levels=[];this.level=-1},startOver:function(){this.level=-1;this.advance()},advance:function(dir){var restart=(dir===-1);if(!restart){this.level++}if(this.level>this.levels.length-1){this.level=this.levels.length-1;return}var activation_info=this.levels[this.level];if(typeof(activation_info.callback)=="function"){if(activation_info.scope){activation_info.callback.call(activation_info.scope,restart)}else{activation_info.callback(restart)}}},getCount:function(){return this.levels.length},getLevelNum:function(){return this.level+1}};var Game=function(canvas,options){if(!canvas||!canvas.tagName||canvas.tagName.toUpperCase()!="CANVAS"){throw new Error("Invalid Canvas element provided")}if(!canvas.getContext){throw new Error("Unsupported browser")}this.alloc_info={};this.ctx=canvas.getContext("2d");this.boundaries={width:canvas.clientWidth||parseInt(canvas.width.replace(/px/,""),10)||0,height:canvas.clientHeight||parseInt(canvas.height.replace(/px/,""),10)||0};options=Slither.setDefaults(Game.DEFAULT_OPTIONS,options);this.setupEventHandlers(options);this.setSpeed(options.speed);this.score=0;this.lives=options.lives;if(this.lives<=0){this.lives=Game.DEFAULT_OPTIONS.lives}this.prev_options=options;this.setupSnake();this.setupLevels()};Game.DEFAULT_OPTIONS={onEat:null,onDie:null,onScore:null,onUpgrade:null,onLoseUpgrade:null,onPoisoned:null,onGameOver:null,onLevelAdvance:null,onTimeWarp:null,speed:Slither.SpeedLevels.Min,lives:3};var _Game=Game;Game=Game.prototype;Game.event_man=Slither.Core.EventManager;Game.object_man=Slither.Core.ObjectManager;Game.level_man=Slither.Core.LevelManager;Game.setupControls=function(){var _this=this;var key_down=function(event){var KeyCodes={Left:37,Up:38,Right:39,Down:40};if(!_this.snake||_this.finished){return}var key_code=event?event.keyCode:null;var control_map={};control_map[KeyCodes.Up]=Slither.Headings.Up;control_map[KeyCodes.Down]=Slither.Headings.Down;control_map[KeyCodes.Left]=Slither.Headings.Left;control_map[KeyCodes.Right]=Slither.Headings.Right;if(_this.started&&key_code in control_map){_this.snake.setHeading(control_map[key_code])}};if(window.addEventListener){window.addEventListener("keydown",key_down,false)}else{window.onkeydown=key_down}};Game.setupEventHandlers=function(options){this.setupControls();var event_map={alloc:this.onAlloc,dealloc:this.onDealloc,eat:this.onEat,die:this.onDie,upgrade:this.onUpgrade,poisoned:this.onPoisoned};for(var name in event_map){this.event_man.register(name,event_map[name],this)}if(typeof(options.onEat)=="function"){this.event_man.register("eat",function(sender,data){options.onEat(sender)})}if(typeof(options.onScore)=="function"){this.event_man.register("score",function(sender,data){options.onScore(this.score)},this)}if(typeof(options.onGameOver)=="function"){this.event_man.register("gameover",function(sender,data){options.onGameOver(this.score,data.win)},this)}if(typeof(options.onUpgrade)=="function"){this.event_man.register("upgrade",function(sender,data){options.onUpgrade(data.type,this.lives,this.score)},this)}if(typeof(options.onLoseUpgrade)=="function"){this.event_man.register("lose-upgrade",function(sender,data){options.onLoseUpgrade(data.type)},this)}if(typeof(options.onPoisoned)=="function"){this.event_man.register("poisoned.after",function(sender,data){options.onPoisoned(data.type,this.lives,this.score)},this)}if(typeof(options.onTimeWarp)=="function"){this.event_man.register("timewarp",function(sender,data){options.onTimeWarp()},this)}var _this=this,nextLevel=function(){_this.level_man.advance()},afterDie=function(){_this.level_man.advance(-1)};if(typeof(options.onLevelAdvance)=="function"){this.event_man.register("level",function(sender,data){var proxy={defAction:nextLevel};return options.onLevelAdvance.call(proxy,this.level_man.getLevelNum()+1)},this)}if(typeof(options.onDie)=="function"){this.event_man.register("die.after",function(sender,data){var proxy={defAction:afterDie};return options.onDie.call(proxy,this.lives)},this)}this.event_man.register("level",nextLevel);this.event_man.register("die.after",afterDie)};Game.setupLevels=function(){this.level_man.removeAll();var O=Slither.Objects,_this=this,Walls=Slither.WallTypes,Upgrades=Slither.UpgradeItemTypes,Poison=Slither.PoisonItemTypes,prevObjInfo=[];var genObjects=function(range,classes,bias){var objs=_this.genObjects(range,classes,bias);if($.isArray(objs)){prevObjInfo=prevObjInfo.concat(objs)}};var addLevel=function(level){if(typeof(level)=="function"){_this.level_man.add(function(restart){this.pause();this.object_man.removeAll();this.setupSnake(restart);if(restart){for(var i=0;i<prevObjInfo.length;i++){var obj_info=prevObjInfo[i],constructor=obj_info.type;this.object_man.add(new constructor(_this.boundaries,_this.ctx,Slither.setDefaults(obj_info.options,{block_size:Slither.Objects.Base.DEFAULT_OPTIONS.block_size,start_x:obj_info.position.x,start_y:obj_info.position.y})))}}else{prevObjInfo=[];level.call(this)}if(this.snake.hasUpgrade(Slither.UpgradeItemTypes.RatParalyzer)){this.freezeRats(true)}this.start()},_this)}};addLevel(function(){genObjects([5,7],[O.Grain,O.Mouse],0.75)});addLevel(function(){genObjects(1,[[O.Wall,{type:Walls.FourCorner}]]);genObjects([10,13],[O.Mouse,O.Rat,O.Grain],[0.35,0.15])});addLevel(function(){genObjects(1,[[O.Wall,{type:Walls.CrossHair}]]);genObjects([15,22],[O.Mouse,O.Rat,O.Grain],[0.65,0.3]);genObjects(1,[[O.PoisonItem,{type:Poison.Deadly}]]);genObjects(2,[[O.PoisonItem,{type:Poison.Toxic}]])});addLevel(function(){genObjects(1,[[O.Wall,{type:Walls.CrossDoubleBar}]]);genObjects([17,25],[O.Mouse,O.Rat],0.55);genObjects([0,1],[[O.UpgradeItem,{type:Upgrades.ExtraLife}]]);genObjects(3,[[O.PoisonItem,{type:Poison.Toxic}]])});addLevel(function(){genObjects(1,[[O.Wall,{type:Walls.BrokenSquare}]]);genObjects([25,30],[O.Rat,O.Mouse,O.Grain],[0.45,0.35]);genObjects(1,[[O.UpgradeItem,{type:Upgrades.RatParalyzer}]]);genObjects(1,[[O.UpgradeItem,{type:Upgrades.ShieldsUp}]]);genObjects(1,[[O.PoisonItem,{type:Poison.Toxic}]]);genObjects(4,O.PoisonItem)});addLevel(function(){genObjects(1,[[O.Wall,{type:Walls.CornerSofa}]]);genObjects([32,38],[O.Rat,O.Mouse],0.6);genObjects(1,[[O.UpgradeItem,{type:Upgrades.WallBreaker}]]);genObjects(1,[[O.UpgradeItem,{type:Upgrades.TimeWarp}]]);genObjects(5,O.PoisonItem)});addLevel(function(){genObjects(1,[[O.Wall,{type:Walls.TripleTee}]]);genObjects([40,50],[O.Rat,O.Mouse,O.Grain],[0.7,0.1]);genObjects([0,1],[[O.UpgradeItem,{type:Upgrades.ExtraLife}]]);genObjects(1,O.UpgradeItem);genObjects(4,[[O.PoisonItem,{type:Upgrades.Deadly}]]);genObjects(3,O.PoisonItem)})};Game.setupSnake=function(reset){this.player_dead=false;var snake=this.snake;if(typeof(reset)=="undefined"){reset=true}var len,upgrades;if(snake){len=snake.getLength();upgrades=snake.getUpgrades();this.object_man.remove(snake)}this.snake=null;var opts={speed:this.getSpeed()};if(!reset&&len){opts.length=len}this.snake=new Slither.Objects.PlayerClass(this.boundaries,this.ctx,opts);if(!reset&&upgrades){for(var i=0;i<upgrades.length;i++){this.snake.addUpgrade(upgrades[i])}}this.object_man.add(this.snake)};Game.genObjects=function(range,classes,bias){range=Slither.makeArray(range);classes=Slither.makeArray(classes);bias=Slither.makeArray(bias);var num_high,num_low;if(range.length==1){num_high=parseInt(range[0],10);if(isNaN(num_high)){return false}num_low=num_high}else{num_low=parseInt(range[0],10);num_high=parseInt(range[1],10);if(isNaN(num_low)||isNaN(num_high)||num_low<0||num_high<0){return false}if(num_low>num_high){num_low=num_high}}var target_count=Slither.genNumber(num_low,num_high),perc_left=1,ttl_count=0,part_data=[],i,j;for(i=0;i<classes.length;i++){var target_bias;if(i<bias.length){target_bias=Math.abs(parseFloat(bias[i],10))}else{var div=classes.length-i;target_bias=(div>0)?perc_left/div:perc_left}if(target_bias>perc_left){target_bias=perc_left}perc_left-=target_bias;var bias_count=target_bias*target_count,lo_count=Math.floor(bias_count),hi_count=Math.ceil(bias_count),class_count=(bias_count-lo_count>hi_count-bias_count)?hi_count:lo_count;if(i==classes.length-1&&ttl_count<target_count){class_count=target_count-ttl_count}part_data.push(class_count);ttl_count+=class_count}var block_size=Slither.Objects.Base.DEFAULT_OPTIONS.block_size,block_seg_x=Math.floor(this.boundaries.width/block_size),block_seg_y=Math.floor(this.boundaries.height/block_size),return_info=[];for(i=0;i<part_data.length;i++){var constructor=classes[i],options={};if(!constructor){continue}if(typeof(constructor)!="function"){if(constructor.length>0){if(constructor.length>1){options=constructor[1]}constructor=constructor[0]}else{continue}}for(j=0;j<part_data[i];j++){var start_x,start_y;do{start_x=Slither.genNumber(0,block_seg_x-1)*block_size;start_y=Slither.genNumber(0,block_seg_y-1)*block_size}while(this.getAllocObj(start_x,start_y)!=null);this.object_man.add(new constructor(this.boundaries,this.ctx,Slither.setDefaults(options,{block_size:block_size,start_x:start_x,start_y:start_y})));return_info.push({type:constructor,options:options,position:{x:start_x,y:start_y}})}}return return_info};Game.getAllocObj=function(x,y){if((x in this.alloc_info)&&(y in this.alloc_info[x])){return this.alloc_info[x][y]}return null};Game.setAllocObj=function(x,y,owner){if(!(x in this.alloc_info)){this.alloc_info[x]={length:0}}var item=this.alloc_info[x];item[y]=owner;item.length++};Game.freezeRats=function(enable){var rats=this.object_man.getList(Slither.Objects.Rat);for(var i=0;i<rats.length;i++){rats[i].animated=!enable}};Game.onAlloc=function(sender,data){var owner=this.getAllocObj(data.coord.x,data.coord.y);if(owner!=null){this.event_man.trigger("collide",this,{initiator:sender,receiver:owner,coord:{x:data.coord.x,y:data.coord.y}})}else{this.setAllocObj(data.coord.x,data.coord.y,sender);this.event_man.trigger("alloc-recv",this,{target:sender,coord:{x:data.coord.x,y:data.coord.y}},sender)}};Game.onDealloc=function(sender,data){var owner=this.getAllocObj(data.coord.x,data.coord.y);if(owner===sender){if(!(data.coord.x in this.alloc_info)){return}var item=this.alloc_info[data.coord.x];if(--item.length==0){delete this.alloc_info[data.coord.x]}else{delete item[data.coord.y]}}};Game.onEat=function(sender,data){this.object_man.remove(sender);if(!this.snake){return}this.event_man.trigger("alloc",this.snake,{coord:{x:data.coord.x,y:data.coord.y}});var O=Slither.Objects;var basic_score=this.getSpeed()*3;var basic_growth_offset=Math.ceil(this.getSpeed()*0.5),growth_offset=basic_growth_offset;if(sender instanceof O.Mouse){this.score+=basic_score}else{if(sender instanceof O.Grain){var grain_score=Math.ceil(0.2*basic_score);this.score+=grain_score;growth_offset=Math.ceil(0.2*basic_growth_offset)}else{if(sender instanceof O.Rat){this.score+=10*basic_score;growth_offset=Math.floor(0.1*basic_growth_offset)+basic_growth_offset}}}this.snake.setLength(this.snake.getLength()+growth_offset);this.event_man.trigger("score",this);var O=Slither.Objects;var check_types=[O.Rat,O.Grain,O.Mouse];for(var i=0;i<check_types.length;i++){if(this.object_man.getCount(check_types[i])!=0){return}}if(this.level_man.getLevelNum()==this.level_man.getCount()){this.end();this.event_man.trigger("gameover",this,{win:true});return false}this.event_man.trigger("level",this)};Game.onDie=function(sender,data){if(sender instanceof Slither.Objects.Wall){if(this.snake.hasUpgrade(Slither.UpgradeItemTypes.WallBreaker)){this.object_man.remove(sender);this.event_man.trigger("alloc",this.snake,{coord:{x:data.coord.x,y:data.coord.y}});return}}var list=this.snake.getUpgrades();for(var i=0;i<list.length;i++){this.snake.removeUpgrade(list[i]);this.event_man.trigger("lose-upgrade",this,{type:list[i]})}if(--this.lives==0){this.end();this.event_man.trigger("gameover",this,{win:false});return false}this.freezeRats(false);this.pause();this.player_dead=true;this.event_man.trigger("die.after",sender)};Game.onUpgrade=function(sender,data){this.object_man.remove(sender);this.event_man.trigger("alloc",this.snake,{coord:{x:data.coord.x,y:data.coord.y}});var types=Slither.UpgradeItemTypes;switch(data.type){case types.ExtraLife:this.lives++;break;case types.TimeWarp:this.event_man.trigger("timewarp",this);this.level_man.startOver();break;case types.RatParalyzer:this.freezeRats(true);this.snake.addUpgrade(data.type);break;case types.WallBreaker:case types.ShieldsUp:this.snake.addUpgrade(data.type);break}};Game.onPoisoned=function(sender,data){this.object_man.remove(sender);this.event_man.trigger("alloc",this.snake,{coord:{x:data.coord.x,y:data.coord.y}});if(this.snake.hasUpgrade(Slither.UpgradeItemTypes.ShieldsUp)){this.snake.removeUpgrade(Slither.UpgradeItemTypes.ShieldsUp);this.event_man.trigger("lose-upgrade",this,{type:Slither.UpgradeItemTypes.ShieldsUp});return}var list=this.snake.getUpgrades();for(var i=0;i<list.length;i++){this.snake.removeUpgrade(list[i]);this.event_man.trigger("lose-upgrade",this,{type:list[i]})}this.freezeRats(false);var types=Slither.PoisonItemTypes;var died=false;switch(data.type){case types.Deadly:died=true;break;case types.Toxic:this.score-=20*this.getSpeed();if(this.score<0){this.score=0}break;case types.Obnoxious:break}this.event_man.trigger("poisoned.after",sender,data);if(died){this.event_man.trigger("die",sender,{coord:{x:data.coord.x,y:data.coord.y}})}};Game.toString=function(){return"Game"};Game.getSpeed=function(){var speed=this.snake?this.snake.getSpeed():this.speed;return speed||Slither.SpeedLevels.Min};Game.setSpeed=function(speed){if(speed<Slither.SpeedLevels.Min){speed=Slither.SpeedLevels.Min}else{if(speed>Slither.SpeedLevels.Max){speed=Slither.SpeedLevels.Max}}this.speed=speed;if(this.snake){this.snake.setSpeed(this.speed)}},Game.getSize=function(){return{width:this.boundaries.width,height:this.boundaries.height}};Game.start=function(){if(!this.started&&!this.finished&&!this.player_dead){if(this.level_man.getLevelNum()<1){this.level_man.advance()}this.object_man.start(Slither.FRAME_RATE);this.started=true}};Game.pause=function(){if(this.started&&!this.finished&&!this.player_dead){this.started=false;this.object_man.stop()}};Game.end=function(){if(this.started){this.pause();this.finished=true}};Game.restart=function(){this.started=false;this.finished=false;this.pause();this.setupSnake();this.score=0;this.alloc_info={};this.lives=this.prev_options?this.prev_options.lives:Game.DEFAULT_OPTIONS.lives;if(this.lives<=0){this.lives=Game.DEFAULT_OPTIONS.lives}this.level_man.startOver()};Game=_Game;Slither.Game=Slither.Core.Game=Game;var Grain=function(boundaries,context,options){Grain.base.init.call(this,boundaries,context,options)};Slither.extend(Slither.Objects.Base,Grain);var _Grain=Grain;Grain=Grain.prototype;Grain.toString=function(){return"Grain"};Grain.draw=function(){if(this.drawn||this.body_blocks.length<1){return}var block=this.body_blocks[this.body_blocks.length-1],mid_offset=Math.ceil(this.block_size/2-0.5),x=block.x,y=block.y;this.ctx.fillStyle="#a24cc8";this.ctx.beginPath();this.ctx.moveTo(x+mid_offset,y);this.ctx.quadraticCurveTo(x+this.block_size,y+mid_offset,x+mid_offset,y+this.block_size);this.ctx.quadraticCurveTo(x,y+mid_offset,x+mid_offset,y);this.ctx.fill();this.drawn=true};var Mouse=function(boundaries,context,options){if(!options){options={}}if(!options.fill_style){options.fill_style="#fc3d32"}if(typeof(options.heading)=="undefined"){options.heading="random"}this.setHeading(options.heading);Mouse.base.init.call(this,boundaries,context,options)};Slither.extend(Slither.Objects.Base,Mouse);var _Mouse=Mouse;Mouse=Mouse.prototype;Mouse.toString=function(){return"Mouse"};Mouse.draw=function(){if(this.drawn||this.body_blocks.length<1){return}var block=this.body_blocks[this.body_blocks.length-1],mid_offset=Math.ceil(this.block_size/2-0.5),dir,axis,x=block.x,y=block.y;switch(this.getHeading()){case Slither.Headings.Up:axis=1;dir=-1;break;case Slither.Headings.Down:axis=1;dir=1;break;case Slither.Headings.Left:axis=0;dir=-1;break;case Slither.Headings.Right:axis=0;dir=1;break}this.ctx.fillStyle=this.fill_style;this.ctx.beginPath();var offset=[(dir<0?0:this.block_size),mid_offset];var point=[x+offset[axis],y+offset[1-axis]];this.ctx.moveTo(point[0],point[1]);point[axis]+=(-1*dir*this.block_size);point[1-axis]-=mid_offset;this.ctx.lineTo(point[0],point[1]);point[1-axis]+=this.block_size;this.ctx.lineTo(point[0],point[1]);this.ctx.lineTo(x+offset[axis],y+offset[1-axis]);this.ctx.fill();this.drawn=true};Grain=_Grain;Mouse=_Mouse;Slither.Objects.Grain=Grain;Slither.Objects.Mouse=Mouse;Slither.Objects.Standard={};var Rat=function(boundaries,context,options){if(!options){options={}}if(!options.fill_style){options.fill_style="#6ff042"}if(options.speed){options.speed="random"}options.animated=true;if(typeof(options.heading)=="undefined"){options.heading="random"}this.setHeading(options.heading);Rat.base.init.call(this,boundaries,context,options)};Slither.extend(Slither.Objects.Base,Rat);var _Rat=Rat;Rat=Rat.prototype;Rat.getDrawInfo=function(){var axis=null,dir=null;switch(this.getHeading()){case Slither.Headings.Up:axis=1;dir=-1;break;case Slither.Headings.Down:axis=1;dir=1;break;case Slither.Headings.Left:axis=0;dir=-1;break;case Slither.Headings.Right:axis=0;dir=1;break;default:return false}return{axis:axis,dir:dir}};Rat.onCollide=function(sender,data){if(data.receiver===this&&data.initiator instanceof Slither.Objects.PlayerClass){Slither.Core.EventManager.trigger("eat",this,{coord:{x:data.coord.x,y:data.coord.y}})}else{if(data.initiator===this){var Headings=Slither.Headings,heading=this.getHeading(),flip=[Slither.genNumber(0,1),Slither.genNumber(0,1)];switch(heading){case Headings.Up:if(flip[0]){heading=Headings.Down}else{heading=flip[1]?Headings.Right:Headings.Left}break;case Headings.Down:if(flip[0]){heading=Headings.Up}else{heading=flip[1]?Headings.Right:Headings.Left}break;case Headings.Left:if(flip[0]){heading=Headings.Right}else{heading=flip[1]?Headings.Up:Headings.Down}break;case Headings.Right:if(flip[0]){heading=Headings.Left}else{heading=flip[1]?Headings.Up:Headings.Down}break}this.setHeading(heading)}}};Rat.onAllocRecv=function(sender,data){if(data.target===this){if(this.body_blocks.length==1){var old_block=this.body_blocks.shift();this.eraseBlock(old_block.x,old_block.y)}var new_block={x:data.coord.x,y:data.coord.y};this.body_blocks.push(new_block);this.drawBlock(new_block.x,new_block.y)}};Rat.drawBlock=function(x,y){var mid_offset=Math.ceil(this.block_size/2-0.5);var info=this.getDrawInfo();if(!info){return}this.ctx.fillStyle=this.fill_style;this.ctx.beginPath();var offset=[(info.dir<0?0:this.block_size),mid_offset];var point=[x+offset[info.axis],y+offset[1-info.axis]];this.ctx.moveTo(point[0],point[1]);point[info.axis]+=(-1*info.dir*this.block_size);point[1-info.axis]-=mid_offset;this.ctx.lineTo(point[0],point[1]);point[1-info.axis]+=this.block_size;this.ctx.lineTo(point[0],point[1]);this.ctx.lineTo(x+offset[info.axis],y+offset[1-info.axis]);this.ctx.fill()};Rat.toString=function(){return"Rat"};Rat.draw=function(){if(!this.animated&&this.drawn){return}if(!this.heading_switch_ctr){this.heading_switch_max=Slither.genNumber(25,100);this.heading_switch_ctr=0}if(++this.heading_switch_ctr>=this.heading_switch_max){this.heading_switch_ctr=0;this.setHeading("random")}var info=this.getDrawInfo();if(!info||this.body_blocks.length<1){return}var last_block=this.body_blocks[this.body_blocks.length-1];var new_block={x:last_block.x,y:last_block.y};new_block[info.axis==0?"x":"y"]+=info.dir*this.block_size;this.adjustBlockWrap(new_block);this.allocBlock(new_block.x,new_block.y);this.drawn=true};Rat.setSpeed=function(speed){if(speed=="random"){speed=Slither.genNumber(Slither.SpeedLevels.Min,Slither.SpeedLevels.Max-2)}if(speed>Slither.SpeedLevels.Max-2){speed=Slither.SpeedLevels.Max-2}Rat.base.setSpeed(speed)};Rat=_Rat;Slither.Objects.Rat=Rat;var UpgradeItem=function(boundaries,context,options){if(!options){options={}}this.setType(options.type||"random");UpgradeItem.base.init.call(this,boundaries,context,options)};Slither.extend(Slither.Objects.Base,UpgradeItem);var _UpgradeItem=UpgradeItem;UpgradeItem=UpgradeItem.prototype;UpgradeItem.onCollide=function(sender,data){if(data.receiver===this&&data.initiator instanceof Slither.Objects.PlayerClass){Slither.Core.EventManager.trigger("upgrade",this,{type:this.getType(),coord:{x:data.coord.x,y:data.coord.y}})}};UpgradeItem.toString=function(){return"UpgradeItem"};UpgradeItem.draw=function(){if(!this.animated&&this.drawn||this.body_blocks.length==0){return}var block=this.body_blocks[this.body_blocks.length-1],mid_offset=Math.ceil(this.block_size/2-0.5),x=block.x,y=block.y,types=Slither.UpgradeItemTypes;switch(this.getType()){case types.ExtraLife:this.ctx.fillStyle="#cc0000";this.ctx.fillRect(x,y,this.block_size,this.block_size);this.ctx.fillStyle="#ddd";var offset=Math.ceil(this.block_size/3);this.ctx.fillRect(x,y,offset,offset);this.ctx.fillRect(x+this.block_size-offset,y,offset,offset);this.ctx.fillRect(x,y+this.block_size-offset,offset,offset);this.ctx.fillRect(x+this.block_size-offset,y+this.block_size-offset,offset,offset);break;case types.TimeWarp:this.ctx.strokeStyle="#d8d817";var radius=this.block_size,center_x,center_y;for(var i=0;i<2;i++){radius=Math.floor(radius/2);if(i==0){center_x=x+radius;center_y=y+radius}this.ctx.beginPath();this.ctx.arc(center_x,center_y,radius-0.5,0,2*Math.PI,true);this.ctx.stroke()}break;case types.ShieldsUp:var offset=Math.ceil(this.block_size/8);this.ctx.beginPath();this.ctx.fillStyle="#00a1e1";this.ctx.moveTo(x,y);this.ctx.quadraticCurveTo(x+mid_offset/2,y+2*offset,x+mid_offset,y);this.ctx.quadraticCurveTo(x+mid_offset+mid_offset/2,y+2*offset,x+this.block_size,y);this.ctx.quadraticCurveTo(x+this.block_size,y+4*offset,x+mid_offset,y+this.block_size);this.ctx.quadraticCurveTo(x,y+4*offset,x,y);this.ctx.fill();break;case types.WallBreaker:var third=Math.ceil(this.block_size/3);var offset=Math.ceil(this.block_size/8);this.ctx.fillStyle="#dd8f4e";this.ctx.fillRect(x+mid_offset-offset/2,y,offset,this.block_size);this.ctx.beginPath();this.ctx.fillStyle="#aaa";this.ctx.moveTo(x+offset,y);this.ctx.lineTo(x+this.block_size-offset,y);this.ctx.quadraticCurveTo(x+this.block_size,y+offset,x+this.block_size-offset,y+third);this.ctx.lineTo(x+offset,y+third);this.ctx.quadraticCurveTo(x,y+offset,x+offset,y);this.ctx.fill();break;case types.RatParalyzer:x+=mid_offset;y+=mid_offset;var size=mid_offset,cos_pi_10=Math.cos(Math.PI/10),cos_pi_5=Math.cos(Math.PI/5),sin_pi_10=Math.sin(Math.PI/10),sin_pi_5=Math.sin(Math.PI/5);this.ctx.beginPath();this.ctx.fillStyle="#6cc2de";this.ctx.moveTo(x,y-size);this.ctx.lineTo(x+size*sin_pi_5,y+size*cos_pi_5);this.ctx.lineTo(x-size*cos_pi_10,y-size*sin_pi_10);this.ctx.lineTo(x+size*cos_pi_10,y-size*sin_pi_10);this.ctx.lineTo(x-size*sin_pi_5,y+size*cos_pi_5);this.ctx.lineTo(x,y-size);this.ctx.fill();break}this.drawn=true};UpgradeItem.getType=function(){return this.type||null};UpgradeItem.setType=function(type){var types=[],item_types=Slither.UpgradeItemTypes;for(var name in item_types){types.push(item_types[name])}if(type=="random"){type=types[Slither.genNumber(0,types.length-1)]}for(var i=0;i<types.length;i++){if(types[i]===type){this.type=type;return true}}return false};UpgradeItem=_UpgradeItem;Slither.Objects.UpgradeItem=UpgradeItem;var PoisonItem=function(boundaries,context,options){if(!options){options={}}this.setType(options.type||"random");PoisonItem.base.init.call(this,boundaries,context,options)};Slither.extend(Slither.Objects.Base,PoisonItem);var _PoisonItem=PoisonItem;PoisonItem=PoisonItem.prototype;PoisonItem.onCollide=function(sender,data){if(data.receiver===this&&data.initiator instanceof Slither.Objects.PlayerClass){Slither.Core.EventManager.trigger("poisoned",this,{type:this.getType(),coord:{x:data.coord.x,y:data.coord.y}})}};PoisonItem.toString=function(){return"PoisonItem"};PoisonItem.draw=function(){if(!this.animated&&this.drawn||this.body_blocks.length==0){return}var block=this.body_blocks[this.body_blocks.length-1],mid_offset=Math.ceil(this.block_size/2-0.5),x=block.x,y=block.y,types=Slither.PoisonItemTypes;switch(this.getType()){case types.Deadly:this.ctx.beginPath();this.ctx.fillStyle="#3878db";this.ctx.arc(x+mid_offset,y+mid_offset,mid_offset,0,2*Math.PI,false);this.ctx.fill();this.ctx.beginPath();this.ctx.strokeStyle="#ffb300";this.ctx.lineWidth=1;this.ctx.moveTo(x+1,y+1);this.ctx.lineTo(x+this.block_size-1,y+this.block_size-1);this.ctx.moveTo(x+this.block_size-1,y+1);this.ctx.lineTo(x+1,y+this.block_size-1);this.ctx.stroke();break;case types.Toxic:this.ctx.fillStyle="#f77105";this.ctx.fillRect(x+mid_offset,y,mid_offset,mid_offset);this.ctx.fillRect(x,y+mid_offset,mid_offset,mid_offset);this.ctx.fillStyle="#b0232a";this.ctx.fillRect(x,y,mid_offset,mid_offset);this.ctx.fillRect(x+mid_offset,y+mid_offset,mid_offset,mid_offset);this.ctx.fillStyle="#f9df78";this.ctx.beginPath();this.ctx.arc(x+mid_offset,y+mid_offset,Math.ceil(this.block_size/4),0,2*Math.PI,false);this.ctx.fill();break;case types.Obnoxious:this.ctx.fillStyle="#ff3484";this.drawRoundedRect(x,y,this.block_size,this.block_size,Math.ceil(this.block_size/4));this.ctx.fillStyle="#e1e400";this.ctx.fillRect(x,y,mid_offset,mid_offset);this.ctx.fillRect(x+mid_offset,y+mid_offset,mid_offset,mid_offset);break}this.drawn=true};PoisonItem.getType=function(){return this.type||null};PoisonItem.setType=function(type){var types=[],item_types=Slither.PoisonItemTypes;for(var name in item_types){types.push(item_types[name])}if(type=="random"){type=types[Slither.genNumber(0,types.length-1)]}for(var i=0;i<types.length;i++){if(types[i]===type){this.type=type;return true}}return false};PoisonItem=_PoisonItem;Slither.Objects.PoisonItem=PoisonItem;var Wall=function(boundaries,context,options){if(!options){options={}}if(!options.fill_style){options.fill_style="#63645b"}options.start_x=undefined;options.start_y=undefined;if(!options.type){throw new Error(this+' object requires "type" option')}Wall.base.init.call(this,boundaries,context,options);this.block_area={width:this.boundaries.width-this.block_size,height:this.boundaries.height-this.block_size};this.setType(options.type);this.allocate()};Slither.extend(Slither.Objects.Base,Wall);var _Wall=Wall;Wall=Wall.prototype;Wall.allocate=function(){var types=Slither.WallTypes,cur_type=this.getType();switch(cur_type){case types.FourCorner:case types.CrossHair:case types.CrossDoubleBar:case types.BrokenSquare:case types.CornerSofa:case types.TripleTee:case types.GoalSplit:case types.ClownInsanity:this.clear(false);for(var type_name in types){if(types[type_name]==cur_type){this["alloc"+type_name]();break}}break;default:return}};Wall.allocFourCorner=function(){var x_area=Math.round(this.block_area.width*0.1),y_area=Math.round(this.block_area.height*0.1),x_offset=Math.round(x_area/(2*this.block_size))*this.block_size,y_offset=Math.round(y_area/(2*this.block_size))*this.block_size;var start_info=[{x:x_offset,y:y_offset,x_dir:1,y_dir:1},{x:this.block_area.width-x_offset,y:y_offset,x_dir:-1,y_dir:1},{x:x_offset,y:this.block_area.height-y_offset,x_dir:1,y_dir:-1},{x:this.block_area.width-x_offset,y:this.block_area.height-y_offset,x_dir:-1,y_dir:-1}];for(var i=0;i<4;i++){var j,info=start_info[i];for(j=0;j<3;j++){this.allocBlock(info.x+info.x_dir*j*this.block_size,info.y)}for(j=1;j<3;j++){this.allocBlock(info.x,info.y+info.y_dir*j*this.block_size)}}};Wall.allocCrossHair=function(){var mid_x=Math.floor(this.block_area.width/(2*this.block_size))*this.block_size,mid_y=Math.floor(this.block_area.height/(2*this.block_size))*this.block_size,x_blocks=Math.round(this.block_area.width*0.15/this.block_size),y_blocks=Math.round(this.block_area.height*0.15/this.block_size);var start_info=[{x:mid_x,y:0,x_dir:0,y_dir:1},{x:this.block_area.width,y:mid_y,x_dir:-1,y_dir:0},{x:mid_x,y:this.block_area.height,x_dir:0,y_dir:-1},{x:0,y:mid_y,x_dir:1,y_dir:0}];for(var i=0;i<4;i++){var info=start_info[i],j;if(info.x_dir!=0){for(j=0;j<x_blocks;j++){this.allocBlock(info.x+info.x_dir*j*this.block_size,info.y)}}else{for(j=0;j<y_blocks;j++){this.allocBlock(info.x,info.y+info.y_dir*j*this.block_size)}}}};Wall.allocCrossDoubleBar=function(){var mid_x=Math.round(this.block_area.width/(2*this.block_size))*this.block_size,mid_y=Math.round(this.block_area.height/(2*this.block_size))*this.block_size,x_blocks=Math.round(this.block_area.width*0.1/this.block_size),y_blocks=Math.round(this.block_area.height*0.1/this.block_size);var start_info=[{x:x_blocks*this.block_size,y:y_blocks*this.block_size},{x:this.block_area.width-x_blocks*this.block_size,y:y_blocks*this.block_size},{x:mid_x,y:mid_y},];var bar_block_max=Math.round((this.block_area.height-2*y_blocks*this.block_size)/this.block_size);for(var i=0;i<2;i++){var info=start_info[i];for(var j=0;j<bar_block_max;j++){this.allocBlock(info.x,info.y+j*this.block_size)}}var info=start_info[2];for(var i=0;i<2;i++){if(i==0){var start_x=info.x-2*this.block_size;for(var j=0;j<5;j++){this.allocBlock(start_x+j*this.block_size,info.y)}}else{var start_y=info.y-2*this.block_size;for(var j=0;j<5;j++){if(j!=2){this.allocBlock(info.x,start_y+j*this.block_size)}}}}};Wall.allocBrokenSquare=function(){var x_start=Math.round(this.block_area.width*0.15/this.block_size)*this.block_size,y_start=Math.round(this.block_area.height*0.1/this.block_size)*this.block_size,y_blocks=Math.round(this.block_area.height*0.2/this.block_size);var start_info=[{x:x_start,y:y_start},{x:x_start,y:this.block_area.height-y_start}];for(var i=0;i<start_info.length;i++){var info=start_info[i],j;var block_max=Math.ceil((this.block_area.width-2*x_start)/this.block_size);for(j=0;j<block_max;j++){this.allocBlock(info.x+j*this.block_size,info.y)}var vert_x,y_mult;if(i==0){vert_x=info.x+(block_max-1)*this.block_size;y_mult=1}else{vert_x=info.x;y_mult=-1}for(j=1;j<y_blocks;j++){this.allocBlock(vert_x,info.y+y_mult*j*this.block_size)}}};Wall.allocCornerSofa=function(){var x_area=Math.round(this.block_area.width*0.1),y_area=Math.round(this.block_area.height*0.1),x_offset=Math.round(x_area/(2*this.block_size))*this.block_size,y_offset=Math.round(y_area/(2*this.block_size))*this.block_size,x_blocks=Math.round(this.block_area.width*0.35/this.block_size),y_blocks=Math.round(this.block_area.height*0.35/this.block_size),y_block_off=y_offset+(y_blocks-1)*this.block_size;var start_info=[{x:x_offset,y:y_offset,x_dir:1,y_dir:1},{x:this.block_area.width-x_offset,y:this.block_area.height-y_offset,x_dir:-1,y_dir:-1},{x:this.block_area.width-x_offset,y:y_block_off,x_dir:-1,y_dir:-1},{x:x_offset,y:this.block_area.height-y_block_off,x_dir:1,y_dir:1}];for(var i=0;i<4;i++){var j,k,info=start_info[i];if(i<2){for(j=0;j<5;j++){this.allocBlock(info.x+info.x_dir*j*this.block_size,info.y)}for(j=1;j<5;j++){this.allocBlock(info.x,info.y+info.y_dir*j*this.block_size)}}else{for(j=0;j<x_blocks;j++){this.allocBlock(info.x+info.x_dir*j*this.block_size,info.y)}for(j=0;j<2;j++){var x=info.x+info.x_dir*j*(x_blocks-1)*this.block_size;for(k=1;k<y_blocks;k++){this.allocBlock(x,info.y+info.y_dir*k*this.block_size)}}}}};Wall.allocTripleTee=function(){var mid_x=Math.round(this.block_area.width/(2*this.block_size))*this.block_size,x_blocks=Math.round(this.block_area.width*0.25/this.block_size),y_blocks=Math.round(this.block_area.height*0.3/this.block_size);var start_info=[{x:mid_x,y:0},{x:x_blocks*this.block_size,y:this.block_area.height},{x:this.block_area.width-x_blocks*this.block_size,y:this.block_area.height}];for(var i=0;i<3;i++){var j,info=start_info[i],y_dir=(i==0)?1:-1;var top_max=Math.ceil(y_blocks/2);for(j=1;j<=top_max;j++){this.allocBlock(info.x-j*this.block_size,info.y);this.allocBlock(info.x+j*this.block_size,info.y)}for(j=0;j<y_blocks;j++){this.allocBlock(info.x,info.y+y_dir*j*this.block_size)}}};Wall.allocGoalSplit=function(){};Wall.allocClownInsanity=function(){};Wall.getType=function(){return this.type||null};Wall.setType=function(type){var types=Slither.WallTypes;for(var name in types){if(types[name]===type){this.type=type;return true}}return false};Wall.onCollide=function(sender,data){if(data.receiver===this){if(data.initiator instanceof Slither.Objects.PlayerClass){Slither.Core.EventManager.trigger("die",this,{coord:{x:data.coord.x,y:data.coord.y}})}else{if(data.initiator===this){throw new Error(this+" object should be created before all other objects")}}}};Wall.toString=function(){return"Wall"};Wall=_Wall;Slither.Objects.Wall=Wall;var Snake=function(boundaries,context,options){this.heading=null;this.heading_queue=[];this.applied_upgrades={};this.head_fill_style="#e8671b";this.body_fill_style="#ffa500";options.animated=true;options.start_x=0;options.start_y=0;this.setHeading(options.heading||Slither.Headings.Right);this.setLength(options.length);Snake.base.init.call(this,boundaries,context,options)};Snake.MIN_LENGTH=2;Slither.extend(Slither.Objects.Base,Snake);var _Snake=Snake;Snake=Snake.prototype;Snake.getHeadingInfo=function(heading){var offset=this.block_size,dir;switch(heading){case Slither.Headings.Up:dir="y";offset*=-1;break;case Slither.Headings.Down:dir="y";break;case Slither.Headings.Left:dir="x";offset*=-1;break;case Slither.Headings.Right:dir="x";break;default:return false}return{dir:dir,offset:offset}};Snake.onCollide=function(sender,data){if(data.initiator===this&&data.initiator===data.receiver){Slither.Core.EventManager.trigger("die",this)}};Snake.onAllocRecv=function(sender,data){if(data.target===this){if(this.body_blocks.length==this.getLength()){var tail_block=this.body_blocks.shift();this.eraseBlock(tail_block.x,tail_block.y)}if(this.body_blocks.length>0){var prev_head_block=this.body_blocks[this.body_blocks.length-1];this.ctx.fillStyle=this.body_fill_style;this.ctx.fillRect(prev_head_block.x,prev_head_block.y,this.block_size,this.block_size)}var head_block={x:data.coord.x,y:data.coord.y};this.body_blocks.push(head_block);if(this.drawn){this.ctx.fillStyle=this.head_fill_style;this.ctx.fillRect(head_block.x,head_block.y,this.block_size,this.block_size)}}};Snake.toString=function(){return"Snake"};Snake.draw=function(){var heading=this.getHeading();var info=this.getHeadingInfo(heading);if(!info||this.body_blocks.length<1){return}var block=this.body_blocks[this.body_blocks.length-1],head_block={x:block.x,y:block.y};head_block[info.dir]+=info.offset;this.adjustBlockWrap(head_block);this.allocBlock(head_block.x,head_block.y);this.drawn=true};Snake.getHeading=function(){if(this.heading_queue.length>0){this.heading=this.heading_queue.shift()}return this.heading};Snake.setHeading=function(heading){if(!heading){return}if(this.heading_queue&&heading===this.heading_queue[this.heading_queue.length-1]){return}var allow=false;switch(heading){case Slither.Headings.Up:allow=this.heading!=Slither.Headings.Down;break;case Slither.Headings.Down:allow=this.heading!=Slither.Headings.Up;break;case Slither.Headings.Left:allow=this.heading!=Slither.Headings.Right;break;case Slither.Headings.Right:allow=this.heading!=Slither.Headings.Left;break}if(allow){this.heading_queue.push(heading)}};Snake.getLength=function(){return this.length||Snake.MIN_LENGTH};Snake.setLength=function(length){if(!length||length<Snake.MIN_LENGTH){length=Snake.MIN_LENGTH}if(!this.length){this.length=length;return}var diff=this.length-length;if(diff>1){for(var i=0;i<diff;i++){var tail_block=this.body_blocks.shift();this.eraseBlock(tail_block.x,tail_block.y)}}this.length=length};Snake.addUpgrade=function(upgrade_type){if(this.hasUpgrade(upgrade_type)){return false}this.applied_upgrades[upgrade_type]=true;return true};Snake.hasUpgrade=function(upgrade_type){return(upgrade_type in this.applied_upgrades)};Snake.getUpgrades=function(){var list=[];for(var type in this.applied_upgrades){list.push(type)}return list};Snake.removeUpgrade=function(upgrade_type){if(!this.hasUpgrade(upgrade_type)){return false}delete this.applied_upgrades[upgrade_type];return true};Snake=_Snake;Slither.Objects.Snake=Snake;if(Slither.AUTOLOAD_SCRIPTS){Slither.ready(false);Slither.loadScripts(Slither.AUTOLOAD_BASE_PATH,Slither.AUTOLOAD_SCRIPTS)}else{Slither.ready(true)}window.Slither=Slither})(window,document);(function(k,h,e){var l=["#game-area","#game-msg","#game-msg a","#help div.controls div.arrow div.icon"];var d,r,m,s,o,g,t,n,c,j;var q,i=false,p={};function f(x,B,z,v,A){if(!v){v="to resume."}q.pause();i=false;var y=function(C){var D=C?C.keyCode:null;if(D==13){i=true;r.hide();if(typeof(z)=="function"){z()}e(k).unbind("keydown",y);return false}};e(k).keydown(y);var w=r.attr("className").replace(/sprites/,"");r.hide().removeClass(w);if(x){r.addClass(x)}e("div.submsg",r).text("Press <Enter> "+v);m.html(B);if(A!==false){r.fadeIn(400)}else{r.show()}}function b(){g.text("0");t.text("3");c.text("1");j.text("-");n.text(q?q.getSpeed():"-")}function a(z,w){if(!p){p={}}var y=Slither.UpgradeItemTypes;var A={};A[y.WallBreaker]="WB";A[y.ShieldsUp]="SU";A[y.RatParalyzer]="RP";if(z in A){p[z]=w;var v=[];for(var x in p){if(p[x]){v.push(A[x])}}if(v.length==0){p={};v.push("-")}j.text(v.join(", "))}}function u(){Slither.ready(function(){q=new Slither.Game(d.get(0),{lives:3,speed:3,onTimeWarp:function(){c.text("1")},onLevelAdvance:function(w){var v=this.defAction;f("new-level","New Level Reached",function(){c.text(w);v()});return false},onDie:function(v){t.text(v);f("die","Slither Is Dead!",this.defAction);return false},onScore:function(v){g.text(v)},onUpgrade:function(w,v,x){a(w,true);t.text(v);g.text(x)},onLoseUpgrade:function(v){a(v,false)},onPoisoned:function(w,v,x){t.text(v);g.text(x)},onGameOver:function(y,x){if(!x){t.text("0")}s.addClass("disabled");i=false;var v=function(){s.removeClass("disabled");q.restart();b()};var w=x?"Win":"Lose";f(w.toLowerCase(),"Game Over, You "+w+"!",v,"to play again.")}});e(k).keydown(function(v){var w=v.keyCode;if(i&&w==27){q.pause();f("pause","Game Paused",function(){r.hide();q.start()},null,false)}});r.fadeOut(200,function(){r.removeClass("loading").addClass("speed");m.empty().html("Choose A Speed..click one to start game");var v=e('<div class="speed-btns" />').appendTo(m);for(var w=0;w<5;w++){e('<a class="key'+(w+1)+'" href="#">Key '+(w+1)+"</a>").appendTo(v)}var x=function(){var y=parseInt(this.className.replace(/[^\d]+/g,""),10);if(isNaN(y)){y=Slither.Game.DEFAULT_OPTIONS.speed}s.slideUp(350,function(){e(this).css({display:""}).addClass("hidden disabled")});var z=r.width();r.animate({width:0},350,"linear",function(){r.hide().removeClass("speed").width(z);var A=o.width();e("#content").animate({width:"+="+A+"px",marginLeft:"-="+(A/2)+"px"},{duration:500,easing:"linear",complete:function(){o.fadeIn(400,function(){s.removeClass("disabled")});q.setSpeed(y);n.text(q.getSpeed());q.start();i=true}})});return false};e("a",v).click(x);r.fadeIn(400);s.slideDown(600).removeClass()})})}e(h).load(function(){e(l.join(",")).addClass("sprites");m.html("Loading Game...Please wait");setTimeout(function(){r.hide().removeClass("invisible").fadeIn(200,function(){setTimeout(u,1500)})},500)});e(k).ready(function(){d=e("#game-area canvas.palette");r=e("#game-msg");m=e("div.msg",r);s=e("#help");o=e("#hud");g=e("#score",o);t=e("#lives",o);n=e("#speed",o);c=e("#level",o);j=e("#upgrades",o);b()})})(document,window,jQuery);
</script>
<!-- Styles -->
<style type="text/css">
* {
margin: 0;
padding: 0;
}

body {
background: #222 url("skin.gif") repeat;
}

.hidden {
display: none;
}

.invisible {
visibility: hidden
}

#wrapper {
height: 1px;
left: 0;
overflow: visible;
position: absolute;
top: 50%;
width: 100%;
}

#content {
height: 650px;
left: 50%;
margin-left: -260px;
position: absolute;
top: -325px;
width: 520px;
}

#header {
font-family: verdana, arial;
}

#header h1 {
background-position: 0 -5px !important;
display: block;
color: #596cc9;
font-size: 25px;
height: 90px;
margin: 0 0 0 90px;
padding: 0;
text-indent: -1999px;
width: 320px;
}

#header h3 {
color: #404e91;
display: none;
font-size: 13px;
margin: -5px 0 0 50px;
padding: 0;
text-transform: lowercase;
}

#footer {
background-position: 90px -125px !important;
clear: both;
color: #5997a8;
font-size: 10px;
height: 60px;
padding: 2px 0 0 0;
text-align: right;
letter-spacing: 1px;
text-transform: lowercase;
width: 500px;
}

#game-area {
background-color: #444 !important;
background-position: 0 0 !important;
border: 2px solid #1b1b1b;
float: left;
height: 500px;
overflow: hidden;
position: relative;
width: 500px;
}

#game-msg {
-moz-box-shadow: -2px -2px 40px rgba(0, 0, 0, 0.7);
-webkit-box-shadow: -2px -2px 40px rgba(0, 0, 0, 0.7);
background-color: #333 !important;
border-bottom: 2px solid #624144;
border-top: 2px solid #916166;
box-shadow: -2px -2px 40px rgba(0, 0, 0, 0.7);
font-family: arial, verdana, sans-serif;
height: 40px;
left: 0;
opacity: 0.95;
overflow: hidden;
padding: 20px 0;
position: absolute;
text-align: left;
text-indent: 25px;
top: 205px;
width: 500px;
z-index: 999;
}

#game-msg div.msg {
color: #4bafcc;
font-size: 36px;
font-weight: bold;
}

#game-msg div.submsg {
color: #8cafcc;
font-size: 12px;
margin: -25px 25px 0 0;
padding-bottom: 13px;
text-align: right;
}

#game-msg.loading {
background: #333 url("loading.gif") no-repeat 15px 10px !important;
text-align: left;
}

#game-msg.loading div.msg {
background: #333 url("loading-txt.gif") no-repeat !important;
height: 75px;
margin: -15px 0 0 75px;
text-indent: -1999px;
}

#game-msg.loading div.submsg {
display: none;
}

#game-msg.speed {
background-position: -498px 0 !important;
}

#game-msg.speed div.msg {
text-indent: -1999px;
}

#game-msg div.speed-btns {
clear: both;
margin: -43px 0 0 346px;
padding-bottom: 37px;
width: 150px;
}

#game-msg a {
border: none;
display: block;
float: left;
height: 23px;
margin-right: 5px;
overflow: hidden;
text-indent: -1999px;
width: 23px;
}

#game-msg a:active, #game-msg a:hover,
#game-msg a:visited {

border: none;
outline: none;
}

#game-msg a.key1 {
background-position: -501px -451px !important;
}

#game-msg a.key1:hover {
background-position: -616px -451px !important;
}

#game-msg a.key1:active {
background-position: -731px -451px !important;
}

#game-msg a.key2 {
background-position: -523px -451px !important;
}

#game-msg a.key2:hover {
background-position: -638px -451px !important;
}

#game-msg a.key2:active {
background-position: -753px -451px !important;
}

#game-msg a.key3 {
background-position: -546px -451px !important;
}

#game-msg a.key3:hover {
background-position: -661px -451px !important;
}

#game-msg a.key3:active {
background-position: -776px -451px !important;
}

#game-msg a.key4 {
background-position: -569px -451px !important;
}

#game-msg a.key4:hover {
background-position: -684px -451px !important;
}

#game-msg a.key4:active {
background-position: -799px -451px !important;
}

#game-msg a.key5 {
background-position: -592px -451px !important;
}

#game-msg a.key5:hover {
background-position: -707px -451px !important;
}

#game-msg a.key5:active {
background-position: -822px -451px !important;
}

#game-msg.pause {
background-position: -498px -72px !important;
}

#game-msg.pause div.msg {
text-indent: -1999px;
}

#game-msg.pause div.submsg {
display: none;
}

#game-msg.new-level {
background-position: -498px -145px !important;
}

#game-msg.new-level div.msg {
text-indent: -1999px;
}

#game-msg.new-level div.submsg {
display: none;
}

#game-msg.die {
background-position: -498px -223px !important;
}

#game-msg.die div.msg {
text-indent: -1999px;
}

#game-msg.die div.submsg {
display: none;
}

#game-msg.lose {
background-position: -498px -295px !important;
}

#game-msg.lose div.msg {
color: #aa310c;
font-size: 32px;
margin: 5px 0 0 90px;
text-indent: -1999px;
}

#game-msg.lose div.submsg {
display: none;
margin: 0 52px 0 0;
}

#game-msg.win {
background-position: -498px -370px !important;
}

#game-msg.win div.msg {
color: #72ad2d;
font-size: 32px;
margin: 5px 0 0 90px;
text-indent: -1999px;
}

#game-msg.win div.submsg {
display: none;
margin: 0 66px 0 0;
}

#help {
background-color: #000;
border-bottom: 1px dotted #003d63;
border-top: 1px dotted #005e99;
color: #fff;
font-family: arial, verdana, sans-serif;
font-size: 10px;
height: 85px;
opacity: 0.7;
position: relative;
padding: 10px 0;
margin-top: -107px;
width: 500px;
}

#game-area:hover #help {
display: block;
}

#game-area:hover #help.disabled {
display: none;
}

#help div.area {
color: #888;
float: left;
line-height: 20px;
margin: 0;
padding: 0;
width: 240px;
}

#help div.area div.header {
background-color: #111;
border-top: 1px dotted #333;
border-bottom: 1px dotted #333;
color: #ccc;
display: block;
font-weight: bold;
font-size: 10px;
line-height: 12px;
margin: 10px 0;
padding: 2px 0px !important;
text-align: center;
width: 210px;
}

#help div.controls {
border-right: 1px dotted #222;
margin-left: 10px;
padding-right: 5px;
width: 240px;
}

#help div.controls div.arrow,
#help div.controls div.esc,
#help div.controls div.enter {
float: left;
width: 80px;
}

#help div.controls div.arrow div.icon {
float: left;
height: 16px;
margin: 2px 3px 0 0;
overflow: hidden;
width: 16px;
}

#help div.controls div.header {
background: #222;
border-top: 1px dotted #444;
border-bottom: 1px dotted #444;
width: 230px;
}

#help div.controls div.arrow div.left {
background-position: -908px -451px !important;
}

#help div.controls div.arrow div.up {
background-position: -924px -451px !important;
}

#help div.controls div.arrow div.right {
background-position: -940px -451px !important;
padding-right: 0;
}

#help div.controls div.arrow div.down {
background-position: -956px -451px !important;
}

#help div.controls div.esc, 
#help div.controls div.enter {
background: none;
}

#help div.controls div.esc span, 
#help div.controls div.enter span{
color: #aaa;
}

#help div.instructions {
padding-left: 13px;
width: 220px;
}

#help div.instructions span {
padding: 0;
}

#help div.instructions div.header {
width: 215px;
}

#hud {
-moz-box-shadow: -2px -2px 100px rgba(5, 5, 5, 0.6) inset;
-webkit-box-shadow: -2px -2px 100px rgba(5, 5, 5, 0.6) inset;
background-color: #444;
border: 2px solid #1b1b1b;
box-shadow: -2px -2px 100px rgba(5, 5, 5, 0.6) inset;
color: #8cafcc;
float: left;
font-family: verdana, arial, sans-serif;
font-size: 11px;
font-weight: bold;
margin: 0 0 0 5px;
overflow: hidden;
text-align: center;
width: 150px;
}

#hud ul {
list-style-type: none;
margin: 5px;
padding: 0;
width: 140px;
}

#hud ul li {
border-bottom: 1px dotted #292929;
margin: 5px;
padding: 5px 0;
}

#hud ul li.no-border {
border-bottom: none !important;
}

#hud ul li div {
font-size: 20px;
}

#upgrades {
font-size: 18px !important;
}

#hud ul li div.header {
color: #4bafcc;
font-size: 11px;
}

#hud div.help-text {
color: #666;
font-size: 10px;
padding: 0 0 10px 0;
text-align: center;
}

#hud div.help-text span {
color: #4e6172;
}

#header h1, #footer {
background: url("logo.png") no-repeat 0 0;
}

#game-area, #game-msg, #game-msg a, #help div.controls div.arrow div.icon {
background: none;
background-repeat: no-repeat;
}

.sprites, #game-msg.sprites div.msg a {
background-image: url("sprites.gif") !important;
}

#game-area:after, #help:after, #hud:after,
#gamemsg div.speed-btns:after, #help div.controls:after {
clear: both;
content: ".";
display: block;
height: 0;
visibility: hidden;
}
</style>
</head>

<body>
<!-- wrapper container -->
<div id="wrapper">
<!-- content container -->
<div id="content">
<!-- header container -->
<div id="header">
<h1>Slither</h1>
<h3>:: Taking A Bite Out Of Canvas! ::</h3>
</div><!-- /#header/ -->
<!-- game area container -->
<div id="game-area">
<!-- canvas drawing palette -->
<canvas class="palette" width="500" height="500"></canvas>
<!-- inset game message container -->
<div id="game-msg" class="invisible loading">
<div class="msg"></div>
<div class="submsg"></div>
</div><!-- /#game-msg/ -->
<!-- inset help container -->
<div id="help" class="hidden disabled">
<div class="area controls">
<div class="header">Controls:</div>
<div class="arrow">
<div class="icon left"></div>Move Left
</div>
<div class="arrow">
<div class="icon up"></div>Move Up
</div>
<div class="arrow">
<div class="icon down"></div>Move Down
</div>
<div class="arrow">
<div class="icon right"></div>Move Right
</div>
<div class="enter">
<span>&lt;Enter&gt;</span>Start
</div>
<div class="esc">
<span>&lt;Esc&gt;</span>Pause
</div>
</div>
<div class="area instructions">
<div class="header">Instructions:</div>
<p>Navigate the snake, eating food to grow longer.
Master each level, beware the poison!</p>
</div>
</div><!-- /#help/ -->
</div><!-- /#game-area/ -->
<!-- heads-up display container -->
<div id="hud" class="hidden">
<ul>
<li>
<div class="header">Score:</div>
<div id="score">-</div>
</li>
<li>
<div class="header">Lives:</div>
<div id="lives">-</div>
</li>
<li>
<div class="header">Speed:</div>
<div id="speed">-</div>
</li>
<li>
<div class="header">Level:</div>
<div id="level">-</div>
</li>
<li class="no-border">
<div class="header">Upgrades:</div>
<div id="upgrades">-</div>
</li>
</ul>
<div class="help-text"><span>Help:</span> Hover over game.</div>
</div><!-- /#hud/ -->
<!-- footer container -->
<div id="footer">&copy;2010 :: Erik Johnson</div>
</div><!-- /#content/ -->
</div><!-- /#wrapper/ -->
</body>

</html>