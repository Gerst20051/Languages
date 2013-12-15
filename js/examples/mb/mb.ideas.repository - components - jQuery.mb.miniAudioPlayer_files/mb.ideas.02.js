/*******************************************************************************
 jquery.mb.components
 Copyright (c) 2001-2010. Matteo Bicocchi (Pupunzi); Open lab srl, Firenze - Italy
 email: mbicocchi@open-lab.com
 site: http://pupunzi.com

 Licences: MIT, GPL
 http://www.opensource.org/licenses/mit-license.php
 http://www.gnu.org/licenses/gpl.html
 ******************************************************************************/

/*
 * jQuery.mb.components: mb.ideas
 * version: 1- 11-dic-2010 - 34
 * © 2001 - 2011 Matteo Bicocchi (pupunzi), Open Lab
 */

/*global vars*/
var site={};
var siteLoaded=true;
var section=null;
var actualSection=null;
var subsection=null;
var actualRandomImage=null;
var bgnds=[];

var tt=700;
var unsupported=$.browser.msie && $.browser.version<9;

site.audioOn=true;

//looks for jpg files in filesystem
$.ajax({
	url:"media/bgnds/",
	async:false,
	dataType:"html",

	success:function(response){
		var tmp=$(response);
		var images= tmp.find("[href*='.jpg']");
		images.each(function(){
			var url= $(this).attr("href");
			bgnds.push("media/bgnds/"+url);
		});
		tmp.remove();
	}
});

if(bgnds.length==0 || ($.browser.msie && $.browser.version<9)){

	bgnds=[
		"media/bgnds/1.jpg",
		"media/bgnds/2.jpg",
		"media/bgnds/3.jpg",
		"media/bgnds/4.jpg",
		"media/bgnds/5.jpg",
		"media/bgnds/6.jpg",
		"media/bgnds/7.jpg",
		"media/bgnds/8.jpg",
		"media/bgnds/9.jpg",
		"media/bgnds/10.jpg",
		"media/bgnds/11.jpg",
		"media/bgnds/12.jpg",
		"media/bgnds/13.jpg",
		"media/bgnds/14.jpg",
		"media/bgnds/15.jpg",
		"media/bgnds/16.jpg",
		"media/bgnds/17.jpg"
	];
}

function IEsuks(){
	document.createElement('header');
	document.createElement('hgroup');
	document.createElement('nav');
	document.createElement('menu');
	document.createElement('section');
	document.createElement('article');
	document.createElement('aside');
	document.createElement('footer');
}




function pageload(hash) {
	$.showLoader();
	$(document).trigger("beforeunloadcontent");
	if(hash) {
		$.ajax({
			url:hash,
			dataType:"html",

			success:function(response){

				$.closeWindow();


				var tmp= $("<div/>").attr("id","temp").html(response);
				if(actualSection!=section)
					$("#"+section).changeMenuVoice();
				actualSection=section;


				if($("#"+section).length==0){
					$("#navigation h1").html(section);
					$("#navigation").find("a.sel").each(function(){
						$(this).removeClass("sel");
						$(this).slideDown();
					});
				}

				var content=tmp.find("#dinaContent");
				if(content.length==0){
					pageload("error.html");
					return;
				}

				var imageURL=content.data("image");
				var transition=content.data("transition");
				var music=content.data("music");

				if(transition==undefined) transition="fade";
				$.changeBackground(imageURL,true);

				var wrapper=$("#wrapper");

				$(document).one("bgReady",function(){
					switch(transition){
						case "slideright":
							wrapper.css({left:$(window).width()+"px"});
							break;
						case "slideleft":
							wrapper.css({left:-$(window).width()+"px"});
							break;
						case "slidedown":
							wrapper.css({top:-$(window).height()+"px"});
							break;
						case "slideup":
							wrapper.css({top:$(window).height()+"px"});
							break;
						case "fade":
							wrapper.css({opacity:0});
							break;
						default:
							wrapper.css({opacity:0});
					}


					if(!unsupported){
						var oldPlayer=$("audio[isPlaying]").get(0);
						if(oldPlayer && music)
							$.mbAudio.fadeOut(oldPlayer.id,4000);

						if(music && music!="none" ){
							$.mbAudio.play(music,4000);
						}
					}

					wrapper.find("#content").html(content).initContent();

					$(document).trigger("beforeContentShow");

					$("body").css({overflow:"hidden"});

					wrapper.animate({left:"0px",top:"0px",opacity:1},tt,"easeOutQuart",function(){
						$("body").css({overflow:"auto"}); $(document).trigger("contentloaded");
					}).show();

					$.hideLoader();
					retweetText= "mb.ideas.repository - "+$("#dinaContent h1").text();
					var el=$(".mbretweet").get(0);
					el.desc = retweetText.replace(/	/g,"-").replace(/\n/g,"");

					$(".mbretweet").updateRetweet();

					document.title= "mb.ideas.repository - " + section + (subsection ? " - "+content.find("h1").text() : "");

					/*GOOGLE ANALYTICS*/
					try {
						var pageTracker = _gat._getTracker("UA-10907260-1");
						pageTracker._trackPageview();
					} catch(err) {}


				})
			},
			error:function(){
				pageload("error.html");
				$("#navigation ").find("a.sel").fadeTo(500,1).removeClass("sel");
			}
		});
	} else {
		pageload("welcome.html");
	}
}

(function($) {

	$.fn.initContent=function(){
		var a=$(this).find("a.ajax");
		a.each ($(this).setURL);

		var aext=$(this).find("a.external");
		aext.each ($(this).setExternalUrl);

		var ademo=$(this).find("a.demo");
		ademo.each ($(this).setDemoUrl);

		var aexpl=$(this).find("a.expl");
		aexpl.each ($(this).setExplUrl);
	};

	preloadBgnds=function(bgnds,onbeforeload,onloadeach, onload){

		if(onbeforeload) onbeforeload(bgnds);

		for( var i = 0; i < bgnds.length; i++ ) {
			var imgUrl=bgnds[i];
			var newImg=$("<img/>").load(function(){
				$(this).remove();
				if(typeof onloadeach=="function")
					onloadeach($(this))
			}).attr({src:imgUrl,idx:i});

			if(i==bgnds.length-1)
				if(typeof onload=="function")
					onload()
		}
	};
	$.changeBackground=function(imageURL,isChangingPage){
		if(imageURL==undefined) {

			var rnd=actualRandomImage;

			while(rnd==actualRandomImage){
				rnd=Math.floor(Math.random()*(bgnds.length));
			}

			imageURL=bgnds[rnd];
			actualRandomImage=rnd;
		}

		setTimeout(function(){
			var newBgnd=$("<div/>").addClass("bgnd").hide();
			$("body").prepend(newBgnd);

			var zidx=isChangingPage?2:1;
			var cssRnd = Math.floor(Math.random()*3);

			var bgStart= cssRnd==1?{bottom:0+"px",height:0+"px",zIndex:zidx,opacity:.5}:cssRnd==2?{top:0+"px",height:0+"px",zIndex:zidx,opacity:.5}:{top:0+"px",width:0+"px",zIndex:zidx,opacity:.5};
			var bgEnd= cssRnd==1 || cssRnd==2 ?{height:$(window).height()+"px",opacity:1}:{width:$(window).width()+"px",opacity:1};
			var newImg=$("<img/>")
					.load(function(){
				newBgnd.css({backgroundImage:"url("+imageURL+")"});
				newBgnd.css(bgStart);/*, opacity:.5*/
				newBgnd.show().animate(bgEnd,tt,"easeOutQuart",function(){/*, opacity:1*/
					$("body").css({backgroundImage:"url("+imageURL+")"});
					newBgnd.remove();
					if(isChangingPage)
						$(document).trigger("bgReady");
				});
			}).attr("src",imageURL);

		},0);

	};

	$.fn.setURL=function(){
		var url=$(this).attr("href");
		$(this).attr("href","#"+url);
		$(this).bind("click", function(){
			var hash = $(this).attr("href");
			hash = hash.replace(/^.*#/, '');
			$.history.load(hash);
		});
	};

	$.fn.setExternalUrl=function(){
		$(this).bind("click", function(){
			var hash = $(this).attr("href");
			$.openWindow(hash);
			return false;
		});
	};

	$.fn.setDemoUrl=function(){
		$(this).bind("click", function(){
			var hash = $(this).attr("href");
			$.openWindow(hash);
			return false;
		});
	};

	$.fn.setExplUrl=function(){
		var url=$(this).attr("href");
		$(this).bind("click", function(){
			var hash = $(this).attr("href");
			$.openWindow(hash,true);
			return false;
		});
	};

	$.fn.changeMenuVoice=function(){

		var voice=this;
		voice.parent().find("a.sel").each(function(){
			$(this).removeClass("sel");
			$(this).fadeTo(500,1);
		});
		$(this).fadeTo(500,.1);
		$(this).addClass("sel");
		var label=$(this).text().replace(".","/");
		var newTitle=$("<h1/>").css({marginLeft:2400});
		var oldTitle=$("#navigation h1");
		$("#navigation").prepend(newTitle);
		newTitle.html(label).animate({marginLeft:0+"px"},350,function(){
			oldTitle.animate({marginLeft:1000+"px"},350,function(){oldTitle.remove();});
		});
	};

	$.showSubMenu=function(){

		var subMenu=$(".sideContent");

		$(document).bind("beforeunloadcontent",function(){
			subMenu.fadeOut(tt);
		});

		$(document).bind("beforeContentShow",function(){
			if(subsection){
				if (subsection!= section)
					subMenu.find("h2").not(".dontRefill").empty();
				setTimeout(function(){

					if (subsection!= section){
						var subPath= $("article#dinaContent h1").html();
						subMenu.find("h2").not(".dontRefill").html(subPath);
					}

					$("[data-id="+section+"]").fadeIn(tt);
					$("#navigation .sideContent .selected").removeClass("selected");
					$("#"+subsection).addClass("selected");

				},1000);
			}
		});

	};

	$.openWindow=function(URL, isInternal){

		$.showLoader();

		$("#header").fadeOut();

		var win="";
		$("#content").animate({left:"-100%"},tt,"easeOutQuart",function(){

			var closeWin=$("<div/>").addClass("close_mbWin").html("Close").hide().click(function(){$.closeWindow()});
			var newWin=$("<div/>").addClass("new_mbWin").html("Open in a new window").hide().click(function(){window.open(URL,"mbNewWin"); $.closeWindow() });

			if(!isInternal){
				win=$("<iframe/>").attr("src","about:blank");
				win.one("load",function(){
					win.show().animate({top:0+"px"},tt,"easeOutQuart",function(){win.attr("src",URL)});
					closeWin.fadeIn();
					newWin.fadeIn();
					$.hideLoader();
				})

			}else{
				win=$("<div/>");
				win.load(URL,function(){
					win.show().animate({top:0+"px"},tt,"easeOutQuart");
					closeWin.fadeIn();
					$.hideLoader();
				});
			}
			win.addClass("mbWin").css("top",-3000+"px").hide();
			$("body").append(win).append(closeWin).append(newWin);

			//press ESC to close the win
			$(document).bind("keydown",function(e){if(e.keyCode==27)$.closeWindow();})

		});
	};

	$.closeWindow=function(){

		$("#header").fadeIn();

		var h=-$(".mbWin").height()+"px";
		$(".mbWin").animate({top:h},tt,"easeOutQuart",function(){$(".mbWin").remove(); $("#content").css({left:-$(window).width()+"px"}).animate({left:0+"px"},tt,"easeOutQuart");});
		$(".close_mbWin, .new_mbWin").fadeOut(tt,function(){$(".close_mbWin, .new_mbWin").remove()});
	};

	$.showLoader=function(){
		$("#loader").remove();
		var loader=$("<div/>").attr("id","loader").hide();
		$("#navigation").prepend(loader);
		if(!unsupported)
			$('#loader').activity({color: 'rgb(255,255,255)'});
		loader.fadeIn(tt);
	};

	$.hideLoader=function(){
		var loader=$("#loader");
		loader.fadeOut(tt,function(){$(this).remove()});
	};

	$.getSpotlight=function(){
		var url="http://pupunzi.open-lab.com/category/jquery/feed/?_r="+new Date().getTime();

		var spotlightN= $(window).height()>800?3:1;

		$.jGFeed( url ,
				function(feeds) {
					if(!feeds){
						return false;
					}

					var rss_placeHolder= $("#spotlight");
					rss_placeHolder.empty();

					var rss_container=$("<div/>").addClass("rss_container");
					rss_placeHolder.append(rss_container);

					// do whatever you want with feeds here
					for(var i=0; i<feeds.entries.length; i++){
						var entry = feeds.entries[i];
						var rss__entry_placeHolder= $("<div class='rss__entry_placeHolder'/>");
						rss__entry_placeHolder.data("url",entry.link);
						rss_container.append(rss__entry_placeHolder);


						rss__entry_placeHolder.append("<span class='rss_entry_date'>"+dateToRelative(new Date(entry.publishedDate))+"</span>");
						/*
						 if(entry.author != feeds.author){
						 rss__entry_placeHolder.append("<span class='rss_entry_author'> - "+entry.author+"</span>");
						 }
						 */

						rss__entry_placeHolder.append("<h3 class='rss_entry_title'>"+entry.title+"</h3>");
						rss__entry_placeHolder.click(function(){
							openLink($(this).data("url"));
						})
					}

					function openLink(url){window.open(url,"pupunzinews");}

					rss_placeHolder.find("*").attr("style","");
					rss_placeHolder.find("a").attr("target","_blank");

				}, spotlightN );
	}

})(jQuery);

function dateToRelative(localTime){
	var diff=new Date().getTime()-localTime;
	var ret="";

	var min=60000;
	var hour=3600000;
	var day=86400000;
	var wee=604800000;
	var mon=2629800000;
	var yea=31557600000;

	if (diff<-yea*2)
		ret ="in ## years".replace("##",(-diff/yea).toFixed(0));
	else if (diff<-mon*9)
		ret ="in ## months".replace("##",(-diff/mon).toFixed(0));
	else if (diff<-wee*5)
		ret ="in ## weeks".replace("##",(-diff/wee).toFixed(0));
	else if (diff<-day*2)
		ret ="in ## days".replace("##",(-diff/day).toFixed(0));
	else if (diff<-hour)
		ret ="in ## hours".replace("##",(-diff/hour).toFixed(0));
	else if (diff<-min*35)
		ret ="in about one hour";
	else if (diff<-min*25)
		ret ="in about half hour";
	else if (diff<-min*10)
		ret ="in some minutes";
	else if (diff<-min*2)
		ret ="in few minutes";
	else if (diff<=min)
		ret ="just now";
	else if (diff<=min*5)
		ret ="few minutes ago";
	else if (diff<=min*15)
		ret ="some minutes ago";
	else if (diff<=min*35)
		ret ="about half hour ago";
	else if (diff<=min*75)
		ret ="about an hour ago";
	else if (diff<=hour*5)
		ret ="few hours ago";
	else if (diff<=hour*24)
		ret ="## hours ago".replace("##",(diff/hour).toFixed(0));
	else if (diff<=day*7)
		ret ="## days ago".replace("##",(diff/day).toFixed(0));
	else if (diff<=wee*5)
		ret ="## weeks ago".replace("##",(diff/wee).toFixed(0));
	else if (diff<=mon*12)
		ret ="## months ago".replace("##",(diff/mon).toFixed(0));
	else
		ret ="## years ago".replace("##",(diff/yea).toFixed(0));
	return ret;
}

function audioMessage(){
	var audioState= $.fn.mb_getCookie("audioOff")=="true"?"audio is OFF": "audio is ON";
	$(".turnOnAudio").html(audioState);
	$(".turnOnAudio").clearQueue();
	$(".turnOnAudio").fadeIn().delay(6000).fadeOut();
}

//COOKIES
jQuery.fn.mb_setCookie = function(name,value,days) {
	var id=$(this).attr("id");
	if(!id) id="";
	if (!days) days=7;
	var date = new Date(), expires;
	date.setTime(date.getTime()+(days*24*60*60*1000));
	expires = "; expires="+date.toGMTString();
	document.cookie = name+"_"+id+"="+value+expires+"; path=/";
};

jQuery.fn.mb_getCookie = function(name) {
	var id=$(this).attr("id");
	if(!id) id="";
	var nameEQ = name+"_"+id + "=";
	var ca = document.cookie.split(';');
	for(var i=0;i < ca.length;i++) {
		var c = ca[i];
		while (c.charAt(0)==' ') c = c.substring(1,c.length);
		if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
	}
	return null;
};

jQuery.fn.mb_removeCookie = function(name) {
	$(this).mb_setCookie(name,"",-1);
};


$(function(){

	$(window).blur(function(){
		if($.fn.mb_getCookie("audioOff")==null){
			$.mbAudio.muteAllSounds();
			$("#soundManager").html("V");
		}
	});

	$(window).focus(function(){
		if($.fn.mb_getCookie("audioOff")==null){
			$.mbAudio.unMuteAllSounds();
			$("#soundManager").html("Vm");
		}
	});

	$(".mbretweet").hide();
	setTimeout(function(){
		$(".mbretweet").buildMbRetweet({color:'rgba(0,0,0,.6)', label:""}).fadeIn();
		retweetText= "mb.ideas.repository - "+$("#dinaContent h1").text();
		var el=$(".mbretweet").get(0);
		el.desc = retweetText.replace(/	/g,"-").replace(/\n/g,"");
	},4000);

	$.showSubMenu();
	$.getSpotlight();

	$(document).one("contentloaded", function(){

		setTimeout(function(){
			if(!($.browser.msie && $.browser.version<9)){
				var preloader=$("<iframe/>").attr("src","preloader.html").css({position:"absolute",top:-5000, left:-5000});
				$("body").append(preloader);
			}

		},3000);

	});

	$("#wrapper").hide();
	$("body").initContent();

	$.history.init(pageload);
	if(!window.location.hash)
		pageload("welcome.html");

});