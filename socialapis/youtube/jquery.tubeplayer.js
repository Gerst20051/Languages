/*!
 * jQuery TubePlayer Plugin
 * 
 * version: 0.9.1 (30-JULY-2010)
 * @requires v1.3.2 or later
 * @requires SWFObject - http://code.google.com/p/swfobject/
 * 
 * Documentation:
 * 		http://www.tikku.com/jquery-youtube-tubeplayer-plugin
 *
 * Copyright 2010, Nirvana Tikku (ntikku@gmail.com)
 * 
 * Dual licensed under the MIT and GPL licenses:
 *   http://www.opensource.org/licenses/mit-license.php
 *   http://www.gnu.org/licenses/gpl.html 
 */
;(function($){
	
	var TUBEPLAYER = ".tubeplayer",
		TUBEPLAYER_CLZ = "-youtube-tubeplayer-",
		OPTS = TUBEPLAYER+"opts";
	
	// public facing defaults for tubeplayer
	$.tubeplayer = {};
	
	$.tubeplayer.defaults = {
		afterReady: function($player){},
		stateChange: function(player){
			var _this = this;
			var _ret = _this.onPlayer;
			return function(state){
				switch(state){
					case -1: return _ret.unstarted[player]();
					case 0: return _ret.ended[player]();
					case 1: return _ret.playing[player]();
					case 2: return _ret.paused[player]();
					case 3: return _ret.buffering[player]();
					case 5: return _ret.cued[player]();
					default: return null;
				}
			}
		},
		onError: function(player){
			var _this = this;
			return function(errorCode){
				switch(errorCode){
					case 100: case 150: 
						return _this.onErrorNotFound();
					case 101: return _this.onErrorNotEmbeddable();
					default: return null;
				}
			}
		},
		qualityChange: function(player){
			var _this = this;
			return function(suggested){
				return _this.onQualityChange[player](suggested);
			}
		},
		onQualityChange:{},
		onPlayer:{unstarted:{},ended:{},playing:{},paused:{},buffering:{},cued:{}},
		onErr:{notFound:{},notEmbeddable:{}}
	};
	
	// internal facing defaults to be merged w/ users
	var defaults = {
		// imperitive but not public facing
		allowScriptAccess: "always",
		// public facing
		width: 640,
		height: 385,
		allowFullScreen: "true",
		initialVideo: "VUaJTqSM-As",
		playerID: "youtube-player",
		preferredQuality: "default",
		otherOptions: "",
		// trigger fn's
		onPlay: function(id){},
		onPause: function(){},
		onStop: function(){},
		onSeek: function(time){},
		onMute: function(){},
		onUnMute: function(){},
		// player fn's
		onPlayerUnstarted: function(){},
		onPlayerEnded: function(){},
		onPlayerPlaying: function(){},
		onPlayerPaused: function(){},
		onPlayerBuffering: function(){},
		onPlayerCued: function(){},
		onQualityChange: function(){},
		// error fn's
		onErrorNotFound: function(){},
		onErrorNotEmbeddable: function(){}
	};
	
	// the actual plugin
	$.fn.tubeplayer = function(input, xtra){
		var $this = $(this);
		var type = typeof input;
		if(arguments.length == 0 || type == "object")
			return init($this, input);
		else if(type == "string")
			return $this.triggerHandler(input+TUBEPLAYER, xtra||null);
	};
	
	// bind events and init the player 
	function init($player, opts){
		var o = $.extend({}, defaults, opts);
		if($player.hasClass(TUBEPLAYER_CLZ))return;
		$player.addClass(TUBEPLAYER_CLZ).data(OPTS, o);
		for(e in PLAYER) $player.bind(e+TUBEPLAYER, $player, PLAYER[e]);
		var d = $.tubeplayer.defaults;
		var ID = o.playerID;
		// default onPlayer events
		var dp = d.onPlayer;
		dp.unstarted[ID] = o.onPlayerUnstarted;
		dp.ended[ID] = o.onPlayerEnded;
		dp.playing[ID] = o.onPlayerPlaying;
		dp.paused[ID] = o.onPlayerPaused;
		dp.buffering[ID] = o.onPlayerBuffering;
		dp.cued[ID] = o.onPlayerCued;
		// default onQualityChange
		d.onQualityChange[ID] = o.onQualityChange;
		// default onError events
		var de = d.onErr;
		de.notFound[ID] = o.onErrorNotFound;
		de.notEmbeddable[ID] = o.onErrorNotEmbeddable;
		// keep track of players
		players.push(ID);
		// embed the player
		var containerID = "tubeplayer-player-container-"+players.length;
		jQuery("<div><br/><a href='http://www.adobe.com/go/getflashplayer' border='0'><img src='http://www.adobe.com/macromedia/style_guide/images/160x41_Get_Flash_Player.jpg' alt='Get Adobe Flash player' border='0' /></a><br/><br/><br/></div>").attr("id", containerID).appendTo($player);
		swfobject.embedSWF(
			"http://www.youtube.com/v/"+o.initialVideo+"?fs=1&enablejsapi=1&version=3&playerapiid="+ID+opts.otherOptions, 
			containerID,o.width,o.height,"8",null,null,
			{ allowScriptAccess: o.allowScriptAccess, allowFullScreen: o.allowFullScreen },
			{ id: ID }
		);
	};
	
	// factor out the common structure of the function
	var buildFN = function(fn, after){
		return function(evt,param){
			var p = $.tubeplayer.getPkg(evt);
			if(p.ytplayer) { 
				return fn(evt, param, p) || p.$player;
			}
			return p.$player;
		};
	};
	
	// all the player events
	var PLAYER = {
		cue: buildFN(function(evt,param,p){ 
			p.ytplayer.cueVideoById(param, p.opts.preferredQuality);
		}),
		play: buildFN(function(evt,param,p){
			if(typeof(param)=='object') p.ytplayer.loadVideoById(param.id,param.time, p.opts.preferredQuality); 
			else if(param) p.ytplayer.loadVideoById(param, 0, p.opts.preferredQuality); 
			else p.ytplayer.playVideo(); 
			p.opts.onPlay(param);
		}),
		pause: buildFN(function(evt,param,p){
			p.ytplayer.pauseVideo();
			p.opts.onPause();
		}),
		stop: buildFN(function(evt,param,p){
			p.ytplayer.stopVideo();
			p.opts.onStop();
		}),
		seek: buildFN(function(evt,param,p){
			p.ytplayer.seekTo(param, true);
			p.opts.onSeek(param);
		}),
		mute: buildFN(function(evt,param,p){
			p.ytplayer.mute(); 
			p.opts.onMute();
		}),
		unmute: buildFN(function(evt,param,p){
			p.ytplayer.unMute(); 
			p.ytplayer.setVolume(100); // YT API bug? 24/Jul/10
			p.opts.onUnMute();
		}),
		isMuted: buildFN(function(evt,param,p){
			return p.ytplayer.isMuted();
		}),
		volume: buildFN(function(evt,param,p){
			if(param) p.ytplayer.setVolume(param);
			else return p.ytplayer.getVolume();
		}),
		quality: buildFN(function(evt,param,p){
			if(param) p.ytplayer.setPlaybackQuality(param); 
			else return p.ytplayer.getPlaybackQuality();
		}),
		data: buildFN(function(evt,param,p){
			var ret = {}; 
			var P = p.ytplayer;
			ret.bytesLoaded = P.getVideoBytesLoaded(); 
			ret.bytesTotal = P.getVideoBytesTotal();
			ret.startBytes= P.getVideoStartBytes();
			ret.state = P.getPlayerState();
			ret.currentTime = P.getCurrentTime();
			ret.availableQualityLevels = P.getAvailableQualityLevels();
			ret.duration = P.getDuration();
			ret.videoURL = P.getVideoUrl();
			return ret;
		}),
		destroy: buildFN(function(evt, param, p){
			p.$player.removeClass(TUBEPLAYER_CLZ)
				.data(OPTS, null)
				.unbind(TUBEPLAYER);
			$(p.ytplayer).remove();
			return null;		
		}),
		player: buildFN(function(evt, param, p){
			return p.ytplayer;
		})
	};
	
	// a helper to get the tubeplayer package
	$.tubeplayer.getPkg = function(evt){
		var $player = evt.data;
		var opts = $player.data(OPTS);
		return {
			$player: $player,
			opts: opts,
			ytplayer: opts ? document.getElementById(opts.playerID) : null
		}
	};
	
	/**
	 * Public Events for YouTube Player
	 */
	 
	var players = [];
	
	onYouTubePlayerReady = function(playerId) { 
		var player = document.getElementById(playerId);
		player.addEventListener("onStateChange", "$.tubeplayer.defaults.stateChange('"+playerId+"')");
		player.addEventListener("onError", "$.tubeplayer.defaults.onError('"+playerId+"')");
		player.addEventListener("onPlaybackQualityChange", "$.tubeplayer.defaults.qualityChange('"+playerId+"')");
		var $player = $(player).parents("."+TUBEPLAYER_CLZ);
		$.tubeplayer.defaults.afterReady($player);
	};
	
})(jQuery);
