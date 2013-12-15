$(document).ready(function() { 
	$('#playyear').selectToUISlider({
		labels: 5,
		sliderOptions: {
			stop: function(e,ui) {
				var year = $("#playyear").val();
				document.location.href="http://yttm.tv/y/"+year;
			}
		}
	});
	
	$("#watch_next").click(function() {
		var year = $("#playyear").val();
		document.location.href="http://yttm.tv/y/"+year;
		return false;
	});
	
	$("#searchterm").click(function() {
		if ($("#searchterm").val() == "Search YTTM...") {
			$("#searchterm").val("");
		} else if ($("#searchterm").val() != "") {
			$("#searchresults").slideDown("fast");
		}
	});
	
	$("#addvidurl").typeWatch( { highlight:false, wait:350, callback:checkVideo, captureLength:0 } );
	$('#searchterm').typeWatch( { highlight:false, wait:250, callback:liveSearch, captureLength:0 } );
	
	$(".highlight").focus(function() {
		$(this).select();
	});
	
	$("ul.side_nav a.filter").click(function(){
		if ($(this).hasClass('toggle')) {
			$(this).addClass('toggle_off');$(this).removeClass('toggle');
			$(this).prev("div").addClass("off");
			var setting = $(this).attr("rel");
			$("#"+setting).val("1");
		} else {
			$(this).addClass('toggle');$(this).removeClass('toggle_off');
			$(this).prev("div").removeClass("off");
			var setting = $(this).attr("rel");
			$("#"+setting).val("0");
		}
		// regenerate cookie and reload page
		var cookieval = $("#filt1").val()+","+$("#filt2").val()+","+$("#filt3").val()+","+$("#filt4").val()+","+$("#filt5").val()+","+$("#filt6").val()+","+$("#filt7").val();
		Set_Cookie('filter',cookieval,0,'/','.yttm.tv');
		var thisyear = $("#this_year").val();
		var newurl = "http://yttm.tv/y"+thisyear;
		
		var year = $("#playyear").val();
		$.get("/ajax_loadvid.php", { year: year }, function(data) {
			$("#video_load_area").html(data);
			var totalvids = $("#totalvidssorted").val();
			$("#totalsorted").html(totalvids);
			if (totalvids < 2) {
				$("#watch_next,#watch_next_off").addClass("disabled");
			} else {
				$("#watch_next,#watch_next_off").removeClass("disabled");
			}
		});		
		return false;
	});
	
	$("ul.side_nav a.autop").click(function(){
		if ($(this).hasClass('toggle')) {
			$(this).addClass('toggle_off');$(this).removeClass('toggle');
			$(this).prev("div").addClass("off");
			var setting = $(this).attr("rel");
			var cv = 0;
		} else {
			$(this).addClass('toggle');$(this).removeClass('toggle_off');
			$(this).prev("div").removeClass("off");
			var setting = $(this).attr("rel");
			var cv = 1;
		}
		
		Set_Cookie('autop',cv,0,'/','.yttm.tv');		
		return false;
	});
	
	$("#addavid").click(function() {
		$("#addvidurl").removeClass("loading bad");
		$("#addvidurl").val("Enter YouTube URL");
		$("#backer").fadeIn("fast");
		$("#addvideo").center().show();
		return false;
	});
	
	$("#sharevidbutton").click(function() {
		$("#backer").fadeIn("fast");
		$("#sharevideo").center().show();
		return false;
	});
	
	$("div.closebutton").click(function() {
		$("#addvideo,#sharevideo").hide();
		$("#backer").fadeOut("fast");
	});
	
	$("#addvidurl").click(function(){
		$("#addvidurl").removeClass("bad");
		if ($("#addvidurl").val() == "Enter YouTube URL") {
			$("#addvidurl").val("");
		}
	});
	
	$("#addvid").click(function(){
		var yturl = $("#add_url").val();
		var name = $("#add_name").val();
		var year = $("#add_year").val();
		$.post("/ajax_subvideo.php", { url: yturl, name: name, year: year }, function(data) {
			if (data == '1') {
				alert("This video has already been submitted for approval. Sorry!");
			} else if (data == '2') {
				alert("There was an error processing your submission. Please try again later!");
			} else {
				$("input:checkbox").removeAttr("checked");
				$("#add_url").val("Enter YouTube URL");
				$("#add_year").val("");
				$("#add_title").val("");
				$("#add_desc").val("");
				$("#add_user").val("");
				$("#add_views").val("");
			}
		});
	});
	
	$("#favicon,a.favlink").click(function() {
		var fbid = $("#this_user").val();
		var vid = $("#this_vid").val();
		$.post("/ajax_addfav.php", { user: fbid, vid: vid }, function(data) {
			if (data == '1') {
				$("#favicon").attr("src","/images/icon_favorite_on.png");
			} else {
				$("#favicon").attr("src","/images/icon_favorite_off.png");
			}
		});
		return false;
	});
	
	$("#favorites div.year,#lovedyears div.year").click(function() {
		var where = $(this).attr("rel");
		alert(where);
	});
	
	$("#fbdrop").mouseover(function() {
		$("#medrop").slideDown("fast");
	});
	
	$("#medrop").mouseleave(function() {
		$("#medrop").slideUp("fast");
	});
	
});

jQuery.fn.center = function () {
    this.css("position","absolute");
    this.css("top", ( $(window).height() - this.height() ) / 2+$(window).scrollTop() + "px");
    this.css("left", ( $(window).width() - this.width() ) / 2+$(window).scrollLeft() + "px");
    return this;
}

jQuery(document.body).click(function(event) {
	var clicked = jQuery(event.target);

	if (!(clicked.is('#search') || clicked.is('div.searchbox') || clicked.parents('#searchresults').length || clicked.is('#searchterm'))) {
		$("#searchresults").slideUp("fast");
	}

});

function Set_Cookie( name, value, expires, path, domain, secure ) 
{
// set time, it's in milliseconds
var today = new Date();
today.setTime( today.getTime() );

if ( expires )
{
expires = expires * 1000 * 60;
}
var expires_date = new Date( today.getTime() + (expires) );

document.cookie = name + "=" +escape( value ) +
( ( expires ) ? ";expires=" + expires_date.toGMTString() : "" ) + 
( ( path ) ? ";path=" + path : "" ) + 
( ( domain ) ? ";domain=" + domain : "" ) +
( ( secure ) ? ";secure" : "" );
}

function checkVideo() {
	$("#addvidurl").addClass("loading");
	var vidurl = $("#addvidurl").val();
	vidurl = $.trim(vidurl);
	
	$.post("/ajax_vidinfo.php", { url: vidurl }, function(data) {
		if (data == '0') {
			$("#addvidurl").removeClass("loading");$("#addvidurl").addClass("bad");
		} else if (data == "1") {
			$("#addvidurl").removeClass("loading");$("#addvidurl").addClass("bad");
			$("#addvidurl").val("Video already exists! Try something else!");
		} else {
			$("#addvida").hide();
			$("#addvidb").show();
			$("#addvidb").html(data);
			$("#addvideo").center();
		}
	});
}

function checkFormVal(field,what) {
	if ($("#"+field).val() == what) {
		$("#"+field).val("");
		$("#"+field).removeClass("bad");
	}
}

function sendAdd() {

	// first check our fields
	var error = 0;
	
	if ($("#add_year").val() == "" || $("#add_year").val() == "Video year") {
		$("#add_year").addClass("bad");
		error++;
	}
	
	if ($("#add_name").val() == "" || $("#add_name").val() == "Your name") {
		$("#add_name").addClass("bad");
		error++;
	}
	
	if ($("#add_email").val() == "" || $("#add_email").val() == "Your email address") {
		$("#add_email").addClass("bad");
		error++;
	}
	
	if ($("#add_country").val() == "") {
		$("#add_country").addClass("bad");
		error++;
	}
	
	if (error == 0) {

		var title = $("p.title").text();
		var yturl = $("#add_url").val();
		var ytid = $("#ytid").val();
		var name = $("#add_name").val();
		var year = $("#add_year").val();
		var email = $("#add_email").val();
		var country = $("#add_country").val();
		$.post("/ajax_subvideo.php", { url: yturl, name: name, email: email, year: year, country: country }, function(data) {
			$("#addvidb").hide();
			$("#addvidc").show();
			$("#addvidc").html(data);
			$("#addvideo").center();
		});
	}
}

function liveSearch() {
	var term = $("#searchterm").val();
	if (term.length < 1) $("#searchresults").slideUp("fast");
	$.post("/ajax_search.php", { st: term }, function (data) {
		$("#searchresults").html(data);
		$("#searchresults").slideDown("fast");
	});
}

function addMore() {
	$("#addvidb,#addvidc").hide();
	$("#addvida").show();
	$("#addvidurl").removeClass("loading");
	$("#addvidurl").val("");
}

function closeAdd() {
	$("#addvideo,#sharevideo").hide();
	$("#addvidb,#addvidc").hide();
	$("#addvida").show();
	$("#backer").fadeOut("fast");
}

function htmlspecialchars_decode (string, quote_style) {
    var optTemp = 0, i = 0, noquotes= false;
    if (typeof quote_style === 'undefined') {
        quote_style = 2;
    }
    string = string.toString().replace(/&lt;/g, '<').replace(/&gt;/g, '>');
    var OPTS = {
        'ENT_NOQUOTES': 0,
        'ENT_HTML_QUOTE_SINGLE' : 1,
        'ENT_HTML_QUOTE_DOUBLE' : 2,
        'ENT_COMPAT': 2,
        'ENT_QUOTES': 3,
        'ENT_IGNORE' : 4
    };
    if (quote_style === 0) {
        noquotes = true;
    }
    if (typeof quote_style !== 'number') { // Allow for a single string or an array of string flags
        quote_style = [].concat(quote_style);
        for (i=0; i < quote_style.length; i++) {
            // Resolve string input to bitwise e.g. 'PATHINFO_EXTENSION' becomes 4
            if (OPTS[quote_style[i]] === 0) {
                noquotes = true;
            }
            else if (OPTS[quote_style[i]]) {
                optTemp = optTemp | OPTS[quote_style[i]];
            }
        }
        quote_style = optTemp;
    }
    if (quote_style & OPTS.ENT_HTML_QUOTE_SINGLE) {
        string = string.replace(/&#0*39;/g, "'"); // PHP doesn't currently escape if more than one 0, but it should
        // string = string.replace(/&apos;|&#x0*27;/g, "'"); // This would also be useful here, but not a part of PHP
    }
    if (!noquotes) {
        string = string.replace(/&quot;/g, '"');
    }
    // Put this in last place to avoid escape being double-decoded
    string = string.replace(/&amp;/g, '&');

    return string;
}