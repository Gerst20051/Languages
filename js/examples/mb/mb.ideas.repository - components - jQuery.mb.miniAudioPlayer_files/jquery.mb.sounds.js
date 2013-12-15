/*******************************************************************************
 jquery.mb.components
 Copyright (c) 2001-2010. Matteo Bicocchi (Pupunzi); Open lab srl, Firenze - Italy
 email: info@pupunzi.com
 site: http://pupunzi.com

 Licences: MIT, GPL
 http://www.opensource.org/licenses/mit-license.php
 http://www.gnu.org/licenses/gpl.html
 ******************************************************************************/

/*
 * jQuery.mb.components: jquery.mb.sounds
 * version: 0.1- 9-giu-2010 - 30
 * © 2001 - 2010 Matteo Bicocchi (pupunzi), Open Lab
 *
 * HTML5 AUDIO TAG doesn't work i IE8 <
 */
String.prototype.asId = function () {
	return this.replace(/[^a-zA-Z0-9_]+/g, '');
};

(function($){
	$.mbAudio ={
		name:"mb.sounds",
		author:"Matteo Bicocchi",
		version:"0.1",
		defaults:{
			id:"",
			ogg:"",
			mp3:"",
			loop:false,
			fade:false,
			volume:1
		},
		sounds:{},
		loaded:new Object(),
		playing:[],

		build:function(sound){
			var sID=typeof sound == "string"?sound:sound.ogg.split(".")[0].asId();
			if($.mbAudio.loaded[sID]!=1){
				var soundEl= typeof sound == "string"?eval("$.mbAudio.sounds."+sound):sound;
				var audio=$("<audio>").attr({id:sID, preload:"auto"});
				var oggSource=$("<source>").attr({src:soundEl.ogg, type:"audio/ogg"});
				var mp3Source=$("<source>").attr({src:soundEl.mp3, type:"audio/mpeg"});
				audio.append(mp3Source).append(oggSource);
				audio.get(0).load();

				$("body").append(audio);
				$.mbAudio.loaded[sID]=1;
			}
		},

		preload:function(){
			for(var i in $.mbAudio.sounds){
				$.mbAudio.build(i);
			}
		},

		play:function(sound){
			var sID=typeof sound == "string"?sound:sound.ogg.split(".")[0].asId();
			var soundEl= typeof sound == "string"?eval("$.mbAudio.sounds."+sound):sound;
			var loop= soundEl.loop?soundEl.loop:$.mbAudio.defaults.loop;
			var fade= soundEl.fade?soundEl.fade:$.mbAudio.defaults.fade;
			var volume= typeof soundEl.volume == "number" ?soundEl.volume:$.mbAudio.defaults.volume;

			$.mbAudio.build(sound);

			var player= document.getElementById(sID);
			player.vol=volume;



			if(loop){
				counter=0;
				$(player).bind("ended",function(){
					this.currentTime = 0;
					if(typeof loop == "number"){
						counter++;
						if(counter==loop){
							$(this).unbind('ended');
						}
					}
				});
				player.addEventListener('timeupdate', function(){},false);
			}

			if(fade){
				player.volume=0;
				$.mbAudio.fadeIn(sound,4000);
			}
			else{
				player.volume=0;
				$.mbAudio.fadeIn(sound,1);
			}
		},

		stop:function(sound){
			var sID=typeof sound == "string"?sound:sound.ogg.split(".")[0].asId();
			var player= document.getElementById(sID);

			if(!player)
				return;

			$.mbAudio.fadeOut(sound,1);
			player.currentTime=0;
			$(player).unbind('ended');
		},

		destroy:function(sound){
			var sID=typeof sound == "string"?sound:sound.ogg.split(".")[0].asId();
			var player= document.getElementById(sID);

			$(player).remove();
			$.mbAudio.loaded[sID]=0;

		},

		muteAllSounds:function(){
			var sounds=$.mbAudio.loaded;
			for(var i in sounds){
/*
				var player= document.getElementById(i);
				player.volume=0;
*/
				$.mbAudio.fadeTo(i,0,1000, function(p){p.pause()});
			}
			$.mbAudio.allMuted=true;
		},

		unMuteAllSounds:function(){
			var sounds=$.mbAudio.playing;
			for(var i in sounds){
				var id=sounds[i];
				var player= document.getElementById(id);

				player.play();
				$.mbAudio.fadeTo(id,player.vol,1000);

				//player.volume=player.vol/100;
			}

			$.mbAudio.allMuted=false;
		},

		fadeIn:function(sound,duration,callback){
			if(!duration)
				duration=100;
			else
				duration=duration/100;

			$.mbAudio.build(sound);

			var sID=typeof sound == "string"?sound:sound.ogg.split(".")[0].asId();
			var player= document.getElementById(sID);

			var soundEl= typeof sound == "string"?eval("$.mbAudio.sounds."+sound):sound;
			var volume= typeof soundEl.volume == "number" ?soundEl.volume:$.mbAudio.defaults.volume;

			clearInterval(player.fade);
			player.play();
			player.volume=0;

			if(!$.mbAudio.allMuted){
				var v=0;
				player.fade=setInterval(function(){

					if(v==volume){
						clearInterval(player.fade);
						if (typeof (callback)=="function")
							callback();
					}
					if($.mbAudio.allMuted){
						clearInterval(player.fade);
						player.volume=0;
						return;
					}
					player.volume=v/100;
					v++
				},duration);
			}
			$.mbAudio.playing.push(sID);
			$(player).attr("isPlaying",true);

		},

		fadeOut:function(sound,duration,callback){

			if(!duration)
				duration=100;
			else
				duration=duration/100;

			var sID=typeof sound == "string"?sound:sound.ogg.split(".")[0].asId();
			var player= document.getElementById(sID);

			if(!player)
				return;

			clearInterval(player.fade);

			var v=player.volume>0?player.volume*100:1;
			if(!v) v=1;
			player.fade=setInterval(function(){

				if(v<=0){
					v=0;
					clearInterval(player.fade);
					player.pause();
					if (typeof (callback)=="function")
						callback();
					return;
				}
				player.volume=v/100;
				v--
			},duration);

			var idx=jQuery.inArray( sID, $.mbAudio.playing );
			$.mbAudio.playing.splice(idx,1);
			$(player).removeAttr("isPlaying");
		},

		fadeTo:function(sound,to,duration,callback){

			if(!duration)
				duration=100;
			else
				duration=duration/100;

			var sID=typeof sound == "string"?sound:sound.ogg.split(".")[0].asId();
			var player= document.getElementById(sID);

			if(!player)
				return;

			clearInterval(player.fade);

			var decrease= player.volume>=to;

			var v=player.volume>0?player.volume*100:1;
			if(!v) v=1;
			player.fade=setInterval(function(){

				var stop= decrease?v<=to:v>=to;
				if(stop){
					v=to;
					player.volume=v/100;
					clearInterval(player.fade);
					if (typeof (callback)=="function")
						callback(player);
					return;
				}
				player.volume=v/100;
				if(decrease)
					v--;
				else
					v++;
			},duration);
		}
	}
})(jQuery);