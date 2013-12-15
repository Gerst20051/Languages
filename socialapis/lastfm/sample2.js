$(document).ready(function(){
	var user = 'user name';
	var akey = 'your api key'
	var lfmurl = 'http://ws.audioscrobbler.com/2.0/?method=user.getrecenttracks&user='+user+'&api_key='+akey+'&format=json';

	$.getJSON(lfmurl, function(data){
		if (data && data.length >= 1) {
			try {
				$.each(data.recenttracks, function(index, item){
					$('.recenttracks').append('<li><h2><a href="'+item.url+'" >'+item.name+'</a></h2><p>'+item.artist.text+'</p></li>');
				});
			} catch(e) {
				$('.recenttracks').html(e);
			} finally {
				$(".recenttracks").fadeIn(2000);
			}
		}
	});
});