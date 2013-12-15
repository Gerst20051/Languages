$.fn.setData = function(obj){
	if (typeof obj != "object") return this;
	return this.each(function(){
		for (key in obj) {
			$(this).data(key, obj[key]);
		}
	});
};

var mainplaylist = [], playlistLength = 0, search = "", sresultLength = 0;

function searchPlaylist(a,b){
	var retArr = [];
	if (typeof a === 'string') a = eval(a);
	$.each(b, function(i,v){
		if (typeof v['id'] !== "string" || typeof v['artist'] !== "string" || typeof v['track'] !== "string") return;
		var artist = v['artist'].replace(/[^a-zA-Z 0-9]+/g,'').replace('   ',' ').replace('  ',' ');
		var track = v['track'].replace(/[^a-zA-Z 0-9]+/g,'').replace('   ',' ').replace('  ',' ');
		if (artist.match(a) || track.match(a)) retArr.push(i);
	});
	sresultLength = retArr.length;
	return retArr;
}

function loadPlaylist(playlist){
	if (0 < playlist.length) {
		var list = [];
		$.each(playlist, function(i,v){
			if (typeof v == "object") {
				var liclass="i",
					html = $("<div/>"),
					itemlist = $("<ul/>").attr('class','ti'),
					item = '<li class="tt">'+(i+1)+'. '+v.track+'</li><li class="a">'+v.artist+'</li>';
				if (v.duration == 0 || v.id == "" || v.id == 0 || v.img == "") liclass = liclass + " un";
				list.push($("<li/>").attr('class',liclass).setData(v).html(html).append(itemlist.append(item))[0]);
			}
		});
		$("#content").html(list);
	} else {
		if (0 < mainplaylist.length) {
			$("#sresultcount").empty();
			$("#content").html($("<li/>").attr('class','i none').text('No Search Results'));
		} else console.log('Playlist is empty');
	}
}

function checkPlaylist(){
	$("#sB").keyup(doSearch);
	$.getJSON("playlist.json", function(a){
		var data = a.data;
		if ($.isArray(data) && 0 < data.length) {
			mainplaylist = data;
			playlistLength = data.length;
			loadPlaylist(data);
		} else console.log("Error loading playlist");
	});
}

function doSearch(){
	var val = $.trim($("#sB").val().replace(/[^a-zA-Z 0-9]+/g,'').replace('   ',' ').replace('  ',' '));
	if (1 < val.length && val != search) {
		search = val;
		var rlength = sresultLength;
		var results = searchPlaylist("/" + search + "/i", mainplaylist);
		if (sresultLength > 0) $("#sresultcount").text(sresultLength);
		if (rlength != sresultLength) {
			var searchplaylist = $.grep(mainplaylist, function(v,i){
				return $.inArray(i,results) > -1;
			});
			loadPlaylist(searchplaylist);
		}			
	} else if (0 == val.length) {
		search = "";
		sresultLength = 0;
		$("#sresultcount").empty();
		loadPlaylist(mainplaylist);
	}
}

$(document).ready(checkPlaylist);