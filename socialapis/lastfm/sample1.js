$(document).ready(function(){
	var user = 'user name';
	var limit = '10';
	var nowp = "true"
	var akey = 'your api key';
	var lfmurl = 'http://ws.audioscrobbler.com/2.0/?method=user.getrecenttracks&user='+user+'&limit='+limit+'&nowplaying='+nowp+'&api_key='+akey+'&format=json';

	if ($('.lastfm ul').length <= 0) {
		$('.lastfm').append('<ul />');
	}

	$.getJSON(lfmurl, function(data){
		if (data && data.length >= 1) {
			$.each(data.recenttracks, function(index, item){
				$('.lastfm ul').append('<li><a href="'+item.url+'">'+item.name+'</a>- <cite>'+item.artist.text+'</cite></li>');
			});
		}
	});
});