function loadTrends()
{
	clearColumns();
	var url = "http://search.twitter.com/trends.json?callback=?";	
	
	$.getJSON(url, function(json)
	{
		var colcount = 3;
		
		for (var x = 0; x < colcount; x++)
		{
			addColumn(json.trends[x].name);					
		}
	});
	return(false);
}
function clearColumns()
{
	$('div.column:visible').find('a.close').click();
}
function getLocation(mycolumn)
{
	var column = jQuery(mycolumn);

	if(column.css('display') == 'none')
		return false;
	
	
	var locationstr 	= column.find('input.loc').val();
	var locationradius 	= column.find('select.radius').val();
	var locationunits 	= column.find('select.units').val();
	
	column.find('div.title small.location').html(locationstr);	
	
	if(locationstr != '' && window.geocodes[locationstr] != 'loading' && !window.geocodes[locationstr])
	{
		window.geocodes[locationstr] = 'loading';
		column.data('locationstr',locationstr);
		column.find('div.stream').prepend('<div class="changed">Location changed: '+locationstr+'</div>');	
		column.find('div.title small.location').html(locationstr);			
		column.data('paused',true);
		column.addClass('paused');
		$('div.pausedmessage span.msg',column).text('Geocoding...');
		window.geocoder.getLatLng(locationstr,function(point)
		{
			var locationstr =  column.find('input.loc').val();
//					console.log(locationstr);
			if (point) 
			{
				window.geocodes[locationstr] = point;	
				restartAllColumns();								
			}
			else
			{
				window.locationfail = 'YES';					
			}
		});
	}
	if(column.data('locationstr') != locationstr && locationstr != '' && locationstr != undefined)
	{
		//console.log(locationstr);
		column.data('locationstr', locationstr);	
		searchkey = column.attr('id')+'-'+locationstr;
		
		window.tweetstack[searchkey] = [];
		column.find('div.stream').prepend('<div class="changed">Location changed: '+locationstr+'</div>');
		column.find('div.title small.location').html(locationstr);
	}
}
function getLocations()
{
	if (GBrowserIsCompatible()) 
	{	
		$('div.column').each(function()
		{
			getLocation(this);
				
		});
	}
	
}
String.prototype.linkusers = function() 
{
  return this.replace(/[@]+[A-Za-z0-9-_]+/g, function(us) 
  {
    var username = us.replace("@","");
    
    us = us.link("http://twitter.com/"+username);
  	us = us.replace('href="','target="_blank"  title="'+username+'"  href="');
    return us;
  });
};

String.prototype.linktags = function() 
{
  return this.replace(/[#]+[A-Za-z0-9-_]+/g, function(t) 
  {
    var tag = t.replace("#","%23");
    t = t.link("http://search.twitter.com/search?q="+tag);
  	t = t.replace('href="','target="_blank" href="');
  	return t;
  });

}; 

String.prototype.linkify = function() 
{
  return this.replace(/[A-Za-z]+:\/\/[A-Za-z0-9-_]+\.[A-Za-z0-9-_:%&\?\/.=]+/g, function(m) 
  {
  	m = m.link(m);
  	m = m.replace('href="','target="_blank" href="');
  	return m;
  });
};

function sizeColumns()
{

	var columns = $('div.column',$('#column-manager'));
	var w = Math.floor($(window).width()/columns.length);
	columns.width(w);
	var h = Math.floor($(window).height()-$('#toolbar').outerHeight());
	//columns.height(h);
	//$('div.column div.stream').height(h);
}

function addColumn(search,thelocation)
{
		//var column = $('<div class="column" id="'+search+'"><div class="controls"><a class="close">x</a><a class="pause">p</a><a class="settings">s</a></div><div class="title">'+search+'</div><div class="settings-panel"><input type="text" name="location" value="" class="loc" /><select name="radius" class="radius" > <option>100</option><option>20</option><option>30</option></select><select name="units" class="units"><option value="km">Km</option><option value="mi">Mi</option></select></div><div class="pausedmessage"><span class="msg">Paused</span><div class="counter"></div></div><div class="stream"></div></div>');		
		
		var column = $('#columntemplate div.column').clone();
		$('#welcome').fadeOut();
		var titlehtml = search+' <small class="location"></small>'
		
		column.find('div.title').html(titlehtml);
		column.attr('id',search);
		column.data('search',search);

		//console.log('add: '+thelocation);
		if(thelocation != false)
		{
			column.find('input.loc').val(thelocation);
			column.data('locationstr',thelocation);
		}
		$('#column-manager').append(column);	
		
		sizeColumns();
		getLocations();
		
		updateHashes();
		
		column.oneTime(1,'firstloader',getTweets);	
		column.everyTime(10000,'loader',getTweets);		
		try
		{
			var clicky_pause_timer = 200;
			clicky.log('#ADD COLUMN','TERM: '+search);
			var clicky_pause_timer = 500;			
		} catch(e){}	
}
MonitterColumns = $.klass(
{
	initialize: function()
	{
		this.columns = [];
//		this.element.append('');
		manager = this;
		$('#addcolumn',this.element).click(function()
		{
			var search = $('#search').val();
			manager._addColumn(search,false);
			return(false);
		});
		$(window).resize(this._sizeColumns);
		
		this.element.sortable({axis:'x',scroll:false,tolerance: 'pointer',handle:'div.title',items:'div.column' });
	},
	_sizeColumns: function()
	{
		sizeColumns();
	},
	_addColumn: function(search)
	{
		addColumn(search,false);
	},
});
function getTweets()
{
	var column = $(this);	
	if($(this).data('search') == undefined)
		$(this).data('search',$(this).attr('id'));
		
	var locationurl = '';
	var locationstr = column.data('locationstr');
	if(window.geocodes[locationstr] == 'loading')
	{
		column.data('paused',true);
		column.addClass('paused');
		$('div.pausedmessage span.msg',column).text('Geocoding...');
		return false;
	}
	
	var search = $(this).data('search');
	if(locationstr ==undefined) locationstr = '';
	var searchkey = search+'-'+locationstr;
	since = 0;
	if(!window.tweetstack[searchkey])
		window.tweetstack[searchkey] = [];

	column.addClass('loading');
	

	rpp = '&rpp=100';
	
	if(!window.tweeturls[searchkey])
	{
		window.tweeturls[searchkey] = '?q='+search.replace('#','%23');
		rpp = '&rpp=20';
	}
	searchstr = window.tweeturls[searchkey];

	if(window.geocodes[locationstr] != undefined && window.geocodes[locationstr] != 'loading')
	{
		point = window.geocodes[locationstr];
		locationurl = '&geocode='+point.lat()+','+point.lng()+','+$('select.radius',column).val()+$('select.units',column).val();		
	}
	
	var url = "http://search.twitter.com/search.json"+searchstr+rpp+locationurl+"&lang=en&callback=?";
	$.getJSON(url, function(json)
	{		
		window.tweeturls[searchkey] = json.refresh_url;
		
		adjustSpeeds();
		
		$(json.results).each(function() 
		{
			window.tweetstack[searchkey].push(this);
		});	
		
		
		column.everyTime(500,'drawer',drawTweets);	
	});	
	
	
	
}
function adjustSpeeds(column)
{
		var column = jQuery(column);
		if(column.data('search') == undefined)
			column.data('search',column.attr('id'));
		
		var search = column.data('search');
		var locationstr = column.find('input.loc').val();
		var searchkey = search+'-'+locationstr;
		var queue_length = 1;
		if(window.tweetstack[searchkey] != undefined)
		{
			 queue_length = window.tweetstack[searchkey].length+1;
		}	
		
		var time = 400+(10000/queue_length);	
			
		column.data('speed',time);
		//console.log(search+ ' speed: '+time);
		
}
function loadProfile(username)
{
	var url = "http://twitter.com/status/user_timeline/"+username+".json?count=1&callback=?";
    var info = {};
    $.getJSON(url, function(data)
    {
		var user = data[0].user;
		
		
		//console.log(data);
		// Profile Template:
		locstr = '';
		template = 	'<div class="inner"><span class="closer">X</span>'+
					'<img class="avatar" title="'+user.screen_name+'" src="'+user.profile_image_url+'" />';
		
		template += '<div class="info">'+
					'<div class="name">'+user.name+' <small>('+user.screen_name+')</small> '+locstr+'</div>';
		if(user.location != null) template 		+= '<div class="location">'+user.location+'</div>';
		if(user.url != null) template 			+= '<div class="url"><a target="_blank" href="'+user.url+'">'+user.url+'</a></div>';
		if(user.description != null) template 	+= '<div class="description">'+user.description+'</div>';
		
		template += '<div class="updates">Updates: '+user.statuses_count+'</div>';
		template += '<div class="followers">Followers: '+user.followers_count+'</div>';
		template += '<div class="favourites">Favourites: '+user.favourites_count+'</div>';
		template += '<p style="margin-top: 4px;"><a style="color:#0BA3EC" href="http://twitter.com/'+user.screen_name+'" target="_blank">View complete profile on Twitter &raquo;</a></p>';
		template += '<span id="follow-button"></span>';
		template 	+= '</div></div>';
		
		$('#profile').html(template);
		$('#profile span.closer').click(function(e){$('#profile').fadeOut(); $('#tweetblockout').fadeOut(); e.stopPropagation();});
		
		
		twttr.anywhere(function (T) {
		   T('#follow-button').followButton(user.screen_name);
		 });
		
		
		resizeBlockout();
		$('#tweetblockout').fadeTo('fast',0.8);
		$('#profile').fadeIn();
    });
    return(false);			
}
function resizeBlockout()
{
	var w = jQuery(window).width();
	var h = jQuery(window).height();
	
	var boxtop = h/2-(140);
	$('#tweetblockout').css({width:w,height:h});
	$('#profile').css('top',boxtop)
}
function drawTweets()
{

	if($(this).data('prevspeed') != $(this).data('speed'))
	{
		$(this).stopTime('drawer');
		$(this).everyTime($(this).data('speed'), 'drawer',drawTweets);
	}
	
	$(this).data('prevspeed',$(this).data('speed'));
	
	if($(this).data('search') == undefined)
		$(this).data('search',$(this).attr('id'));
		
	var search = $(this).data('search');
	var locationstr = $(this).find('input.loc').val();
	var searchkey = search+'-'+locationstr;
	
	$(this).removeClass('loading');
	
	$('div.pausedmessage div.counter',this).text(window.tweetstack[searchkey].length+' new');
	if(!$(this).data('paused'))
	{		
		if(window.tweetstack[searchkey].length > 0)
		{
			var tweet = window.tweetstack[searchkey].pop();
			
			var thedate = new Date(Date.parse(tweet.created_at));
			var hourstr = thedate.getHours();
			var minustr = new String(thedate.getMinutes());
			if(minustr.length<=1)
				minustr = '0'+minustr;
			var thedatestr = hourstr+':'+minustr;
			var locstr = '';
			//console.log(tweet);
			src = tweet.source.replace(/&gt;/gi,'>').replace(/&lt;/gi,'<');
			
			if(tweet.location != undefined)
				locstr = ' <span class="locstr">@ '+tweet.location+'</span>';
			var divstr = '<div id="tw'+tweet.id+'" class="tweet"><a class="profile_button twitter-anywhere-user"  title="'+tweet.from_user+'" href="http://www.twitter.com/'+tweet.from_user+'"><img class="profile" title="'+tweet.from_user+'" width="48" height="48" src="'+tweet.profile_image_url+'" ></a><div class="text"><p class="text">'+tweet.text.linkify().linkusers().linktags()+'</p><div class="buttons"><a class="miniicon retweetbutton"  target="_blank"></a><a class="replybutton miniicon" target="_blank"></a></div><b class="lastline"><a class="profile_button" title="'+tweet.from_user+'">'+tweet.from_user+'</a></b>&nbsp;&nbsp;<b><a href="http://twitter.com/'+tweet.from_user+'/status/'+tweet.id+'" target="_blank">'+thedatestr+'</a></b>'+locstr+'<span class="src">'+src+'</span><div style="display:none" class="rawtweettext">'+tweet.text+'</div></div></div>';
			
			$('div.stream',this).prepend(divstr);
			sizeColumns();
			$('div.stream #tw'+tweet.id,this).show(window.animmode,window.animoptions,window.animspeed);			
		}
	}
	
	$('div.column').each(function()
	{ 
		$('div.stream div.tweet:gt(100)',$(this)).remove();
	});
}
function restartAllColumns()
{
	$('div.column').each(function()
	{
		var column = $(this);
		
		column.stopTime('drawer');
		column.stopTime('loader');		
		
		
		column.data('paused',false);
		column.removeClass('paused');
		
		column.oneTime(1,'firstloader',getTweets);	
		column.everyTime(10000,'loader',getTweets);	
	});
}
jQuery(document).ready(function($)
{

	$('#toolbarform').live('submit',function(e)
	{
		$(this).find('a#addcolumn').click();
		return(false);
	});
	$('#column-manager').attach(MonitterColumns);
	
	window.geocoder =  new GClientGeocoder();	
	window.geocodes = {};
	 getLocations();
	 
	$('input.geocode').live('click',function()
	{
		$(this).next().toggle();
	});
	$('a.closer').click(function()
	{
		$('#tweetblockout').fadeOut();
		$('#tweetbox').fadeOut();
		return(false);
	});
	$('div.column div.controls a.close').live('click',function()
	{
		var column = $(this).parent().parent();
		
		column.stopTime('drawer');
		column.stopTime('loader');		
		
		column.remove();
		updateHashes();		
		sizeColumns();
	});
	$('div.column div.settings-panel div.hide-settings').live('click',function()
	{
		var column = $(this).parent().parent();
		
		getLocations();
		
		search = column.data('search');
		var locationstr = column.find('input.loc').val();
		var searchkey = search+'-'+locationstr;

		window.tweetstack[searchkey] = [];
		window.tweeturls[searchkey] = '';
		
		column.find('div.settings-panel').slideToggle();		
	});
	$('div.column div.controls a.settings').live('click',function()
	{
		var column = $(this).parent().parent();
		
		$('div.settings-panel',column).slideToggle();
	});
	$('div.column div.controls a.pause').live('click',function()
	{
		var column = $(this).parent().parent();
		
		column.data('paused',true);
		column.addClass('paused');	
		column.fadeTo('fast',0.6);	
		
		$('div.pausedmessage span.msg',column).text('Paused');
		$(this).addClass('play').removeClass('pause');
	});
	$('div.column div.controls a.play').live('click',function()
	{
		var column = $(this).parent().parent();
		
		column.data('paused',false);
		column.removeClass('paused');
		column.fadeTo('fast',1.0)		;	
		$(this).addClass('pause').removeClass('play');
	});
	
	$('#tweetblockout').click(function()
	{
		$(this).fadeOut();
		$('#tweetbox').fadeOut();
	});
	$('div.column a.profile_button').live('click',function()
	{
		var username = $(this).attr('title');
		
		loadProfile(username);
		
		return(false);
	});
	$('div.column a.replybutton').live('click',function()
	{
		var the_tweet = $(this).closest('div.tweet');
		var tweeter = $('.profile_button',the_tweet).text();
		var text = '@'+tweeter+' ';
		var data = {'in_reply_to_status_id':the_tweet.attr('id')};
		tweet(text,data);
	});
	
	$('div.column a.retweetbutton').live('click',function()
	{
		var the_tweet = $(this).closest('div.tweet');
		var tweeter = $('.profile_button',the_tweet).text();
		var text = $('div.rawtweettext',the_tweet).text();
		text = 'RT '+tweeter+': '+text;
		var data = {'retweet':the_tweet.attr('id')};
		tweet(text,data);
	});
	
	$('#share #loadTrends').click(loadTrends);
	$('#share a.tweet').click(function()
	{
		var msg = 'Using @monitter to watch twitter: ';
		var terms = [];
		$('div.column div.title').each(function()
		{
			terms.push($(this).text());
		});
			
		msg += terms.join(', ');
		msg += ' - ';
		
		urlstring = 'http://www.monitter.com/?'+jQuery.cookie('monitter-search');

		window.twitterstatus = msg;

		BitlyClient.call('shorten', {'longUrl': urlstring}, 'announce');		
		
		return(false);
	});
	
	$('div.settings-panel input,div.settings-panel select').live('change',function()
	{
		getLocations();
	});
	
	var init = '';
	if(window.location.href.indexOf('#')> 0)
	{
		init = window.location.href.substring(window.location.href.indexOf('#')+1)
	}
	else if(window.location.href.indexOf('?')> 0)
	{
		init = unescape(window.location.search.substring(1));
	}
	else
	{
		if(jQuery.cookie('monitter-search'))
			init = jQuery.cookie('monitter-search');
	}
	if(init != '')
	{
		//console.log(init);
		var searches = init.split('&');
		loc = false;
		for(var i in searches)
		{
			var search = searches[i];
			;
			if(search.indexOf('((') > 0)
			{
				search = search.replace('))','');
				var bits  = search.split('((');
				search = bits[0];
				loc = bits[1];
			}
			
			addColumn(search,loc);
		}
	}
});	
function updateHashes()
{
	window.newhashes = [];
	$('div.column:visible').each(function(i,col)
	{
		col = $(col);
		var search = col.data('search');
		var loc = col.data('locationstr');
		
		var key = search;
		
		if(loc != '' && loc != undefined)
			key += '%28%28'+loc+'%29%29';
			
		window.newhashes.push(key);
	});
	hash = window.newhashes.join('&');
	window.newhashes = [];
	
	jQuery.cookie('monitter-search',hash,999);
}
function announce(data)
{
    var first_result;
    // Results are keyed by longUrl, so we need to grab the first one.
    for     (var r in data.results) {
            first_result = data.results[r]; break;
    }
    //console.log(first_result);
    var url = first_result.shortUrl;
	window.twitterstatus += url;
	tweet(window.twitterstatus);
}
function tweet(defaulttext,data)
{
	T = window.anywhere;
	var w = jQuery(window).width();
	var h = jQuery(window).height();
	var boxtop = h/2-(140);
	$('#tweetblockout').css({width:w,height:h}).fadeTo('fast',0.8);
	
	if(data != undefined && data['in_reply_to_status_id'] != undefined)
	{
		the_tweet = $('#'+data['in_reply_to_status_id']);
		the_copy = the_tweet.clone();
		
		$('#tweetbox div.reply').html(the_copy);
		$('#tweetbox div.title').html('Replying to tweet..');
	}
	else if(data != undefined && data['retweet'] != undefined)
	{
		the_tweet = $('#'+data['retweet']);
		the_copy = the_tweet.clone();
		
		data = {};
		
		$('#tweetbox div.reply').html(the_copy);
		$('#tweetbox div.title').html('Retweeting tweet..');		
	}
	else
	{
		$('#tweetbox div.reply').html('');
		$('#tweetbox div.title').html('Send a tweet..');
	}
	$('#tweetbox div.frame').html('');
	$('#tweetbox').css('top',boxtop).fadeIn();
	T('#tweetbox div.frame').tweetBox({
	    label: "Send a tweet..",
	      height: 100,
	      width: 400,
	      defaultContent: defaulttext,
	      data: data
	    });	
}
$(window).resize(function()
{
	var w = jQuery(window).width();
	var h = jQuery(window).height();
	var boxtop = h/2-(140);
	$('#tweetblockout').css({width:w,height:h});
	$('#tweetbox').css('top',boxtop);
	resizeBlockout();
});

  //twttr.anywhere.config({ callbackURL: "http://monitter.com/aw/" });
  twttr.anywhere(function (T) {
    T("#login").connectButton();  
	T.linkifyUsers();
	
	window.anywhere = T;	
   });
   
  /* twttr.anywhere(function (T) {
   
       T("div.tweet img.profile").hovercards({
         username: function(node) {
           return jQuery(node).attr('title');
         }
       });
   
     });*/