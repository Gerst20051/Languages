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
 * jQuery.mb.components: jquery.mb.retweet
 * version: 1.3
 * © 2001 - 2009 Matteo Bicocchi (pupunzi), Open Lab
 *
 */

(function($){

	$.mbRetweet={
		name:"mb.retweet",
		author:"Matteo Bicocchi",
		version:"1.0",
		//----------------------------------------------------- INSERT YOUR BIT.LY USER AND KEY!!!
		// Found here: http://bit.ly/account/your_api_key
		BitlyAPIversion:"2.0.1",
		// Your Bit.ly Username
		user:"patapage",
		// Your Bit.ly API Key
		key:"R_65320846dbaf9103bdb23631255a0f9e",
		//-----------------------------------------------------END INSERT

		defaults:{
			label:"Retweet",
			prefix:"#pupunzi #jquery #html5",
			color:"#32CCFF",
			commentPanel:false
		},

		mbBitlyShorten:function(callback){
			var el=$(this).get(0);
			var url="http://api.bit.ly/shorten?version="+$.mbRetweet.BitlyAPIversion+"&longUrl="+encodeURIComponent(el.longUrl)+"&login="+$.mbRetweet.user+"&apiKey="+$.mbRetweet.key;
			$.ajax({
				url:url,
				dataType: "jsonp",
				success:function(data){
					for ( var u in data.results) {
						el.shortUrl = data.results[u].shortUrl;
					}
					if(typeof callback=="function")
						callback()
				},
				error:function(XMLHttpRequest, textStatus){
					alert(textStatus);
				}
			});
		},

		mbBitlyStats:function(){
			var el=$(this).get(0);
			if(!el.shortUrl) $(el).mbBitlyShorten();
			var out=0;
			var checkUrl = setInterval(function(){
				if ( typeof el.shortUrl !== "undefined" || out==20) {
					clearInterval( checkUrl );
					if (out==20) el.shortUrl = el.longUrl;
					var url="http://api.bit.ly/stats?version="+$.mbRetweet.BitlyAPIversion+"&shortUrl="+el.shortUrl+"&login="+$.mbRetweet.user+"&apiKey="+$.mbRetweet.key;
					$.ajax({
						url:url,
						dataType: "jsonp",
						async:true,
						success:function(data){
							el.stats="-";
							if(data.results)
								el.stats = data.results.clicks;
						},
						error:function(XMLHttpRequest, textStatus){
							alert(textStatus);
						}
					});
				}
				out++;
			},1);
		},

		buildMbRetweet:function(options){
			return this.each(function(){
				this.options = {};
				$.extend (this.options, $.mbRetweet.defaults, options);

				this.longUrl= $(this).hasClass("self")?self.location.href:$(this).attr("href");
				this.desc= $(this).text();
				this.desc=this.desc.length>=90?this.desc.substring(0,100)+"...":this.desc;
				var el=this;
				$(el).html(this.options.label);
				$(el).wrap("<span class='mbRetweetSpan' style='position:relative;width:160px'/>");

				//	if ($.mbRetweet.user){
				$(el).wrapInner("<span class='mbretweetLabel'/>");
				$(el).find(".mbretweetLabel").css("background-color",el.options.color).hover(
						function(){
							$(this).css("opacity",.7);
							$(document).unbind("click");
						},
						function(){
							$(this).css("opacity","");
							$(document).bind("click",function(){$(el).removeMbRetweetExtra();});
						});

				function openTwitterWin(){
					var newWin = window.open("","twitterWin");
					if ( typeof el.shortUrl !== "undefined" ) {
						if($("#mbretweetDesc").length>0) {
							el.desc= $("#mbretweetDesc").val();
						}

						//newWin.location.href = "http://twitter.com/home?status=" +	encodeURIComponent(el.options.prefix + " " + el.desc + ": " +el.shortUrl);
						newWin.location.href = "http://twitter.com/intent/tweet?text=" +	encodeURIComponent(el.options.prefix + " " + el.desc + ": " +el.shortUrl);
						$(".mbretweetExtra").remove();
						$(el).addClass("disabled");
					}
				}

				$(el).find(".mbretweetLabel").bind("click",function(){

					if($(el).hasClass("disabled"))
						return;

					if($.mbRetweet.user){
						if(!el.shortUrl)
							$(el).mbBitlyShorten(openTwitterWin);
						else
							openTwitterWin();

					}else{
						el.shortUrl=el.longUrl;
						openTwitterWin();
					}
				});


				if (el.options.commentPanel){
					$(el).find(".mbretweetLabel").append("<span class='mbopenExtra'>&#9660;</span>");
					$(el).find(".mbopenExtra").click(function(){
						$(".mbretweetExtra").remove();
						setTimeout(function(){$(el).buildMbRetweetExtra();},250);
						return false;});
				}else{
					$(el).find(".mbretweetLabel").append("<span>&nbsp;</span>");
				}
				$(el).attr("href","javascript:void(0)");

				$(el).mbBitlyStats();
				var checkStats = setInterval(function(){
					if ( typeof el.stats !== "undefined" ) {
						clearInterval( checkStats );
						$(el).append("<span class='mbretweetStats'>"+el.stats+"</span>");
					}
				}, 10);

				//	}
				/*else{
				 $(el).wrapInner("<span class='mbretweetLabel'/>");
				 $(el).find(".mbretweetLabel").css("background-color",el.options.color).hover(
				 function(){
				 $(document).unbind("click");
				 },
				 function(){
				 $(document).bind("click",function(){$(el).removeMbRetweetExtra();});
				 });
				 $(el).find(".mbretweetLabel").bind("click",function(){

				 if($(el).hasClass("disabled"))
				 return;

				 if($("#mbretweetDesc").length>0) {
				 el.desc= $("#mbretweetDesc").val();
				 }
				 //ar twitterUrl="http://twitter.com/home?status=" +	encodeURIComponent(el.options.prefix + " " + el.desc + ": " +el.longUrl);
				 var twitterUrl="http://twitter.com/intent/tweet??text=" +	encodeURIComponent(el.options.prefix + " " + el.desc + ": " +el.longUrl);
				 window.open(twitterUrl,"twitter");
				 $(".mbretweetExtra").fadeOut(200,function(){});
				 $(el).addClass("disabled");
				 });
				 $(el).attr("href","javascript:void(0)");
				 if (el.options.commentPanel){
				 $(el).find(".mbretweetLabel").append("<span class='mbopenExtra'>&#9660;</span>");
				 $(el).find(".mbopenExtra").click(function(){
				 $(".mbretweetExtra").remove();
				 setTimeout(function(){$(el).buildMbRetweetExtra();},250);
				 return false;});
				 }
				 }*/

			});
		},

		buildMbRetweetExtra:function(){
			var el=$(this).get(0);
			if ($(el).hasClass("disabled")) return;
			if(!el.shortUrl) $(el).mbBitlyShorten();
			var checkUrl = setInterval(function(){
				if ( typeof el.shortUrl !== "undefined" || !$.mbRetweet.user ) {
					clearInterval( checkUrl );
					var tweetUrl=$.mbRetweet.user?el.shortUrl:el.longUrl;
					var extracontent=$("<div class='mbretweetExtra'></div>").hide();
					//extracontent.css({left:0,top:14});
					extracontent.append("add a comment:<br><textarea maxlength='140' id='mbretweetDesc' >"+el.desc+"</textarea>");
					$(el).css({position:"relative"});
					$(el).after(extracontent);
					extracontent.fadeIn();
					$("#mbretweetDesc").css({color:el.options.color}).focus();/*.select()*/
					$(".mbretweetExtra").hover(
							function(){
								$(document).unbind("click");
							},
							function(){
								$(document).bind("click",function(){$(el).removeMbRetweetExtra();});
							});

					extracontent.keypress(function(e){
						if(e.which==13) $(el).find(".mbretweetLabel").click();
						var charLeft=(140-$("#mbretweetDesc").val().length-el.options.prefix.length-tweetUrl.length);
						$(".mbretweetExtra").find("#mbcounter").html(charLeft+" left &nbsp;&nbsp;");
						if (charLeft<0)
							$(".mbretweetExtra").find("#mbcounter").addClass("alert");
						else
							$(".mbretweetExtra").find("#mbcounter").removeClass("alert");
					});
					$("#mbretweetDesc").after(
							"<div class='extraretweet' style='text-align:right'><span id='mbcounter'>"+
									(140-el.desc.length-el.options.prefix.length-tweetUrl.length)+
									" left &nbsp;&nbsp;</span><span class='goretweet'>retweet!</span></div>");
					$(".goretweet").css({color:"white", backgroundColor:el.options.color}).click(function(){$(el).find(".mbretweetLabel").click();});
				}
			},100);
		},
		removeMbRetweetExtra:function(){
			$(".mbretweetExtra").fadeOut(400,function(){$(".mbretweetExtra").remove();});
		},
		updateRetweet: function(){
			var el=$(this).get(0);
			$(el).removeClass("disabled");
			el.longUrl=self.location.href;
			el.shortUrl=undefined;
			el.stats=undefined;
			$(el).mbBitlyShorten();
			var checkUrl = setInterval(function(){
				if ( typeof el.shortUrl !== "undefined" ) {
					clearInterval( checkUrl);
					//alert(el.shortUrl);
					$(el).mbBitlyStats();
					var checkStats = setInterval(function(){
						if ( typeof el.shortUrl !== "undefined" &&  typeof el.stats !== "undefined") {
							clearInterval( checkStats);
							//alert(el.longUrl+" --- "+el.shortUrl+" --- "+el.stats);
							$(".mbretweetStats").html(el.stats);
						}
					}, 10);}
			},50);
		}
	};

	$.fn.buildMbRetweet= $.mbRetweet.buildMbRetweet;
	$.fn.buildMbRetweetExtra= $.mbRetweet.buildMbRetweetExtra;
	$.fn.removeMbRetweetExtra= $.mbRetweet.removeMbRetweetExtra;
	$.fn.mbBitlyShorten= $.mbRetweet.mbBitlyShorten;
	$.fn.mbBitlyStats= $.mbRetweet.mbBitlyStats;
	$.fn.updateRetweet= $.mbRetweet.updateRetweet;

})(jQuery);