/* Client-side access to querystring name=value pairs
Version 1.3
28 May 2008
	
License (Simplified BSD):
http://adamv.com/dev/javascript/qslicense.txt
*/
function Querystring(qs) { // optionally pass a querystring to parse
    this.params = {};

    if (qs == null) qs = location.search.substring(1, location.search.length);
    if (qs.length == 0) return;

    // Turn <plus> back to <space>
    // See: http://www.w3.org/TR/REC-html40/interact/forms.html#h-17.13.4.1
    qs = qs.replace(/\+/g, ' ');
    var args = qs.split('&'); // parse out name/value pairs separated via &

    // split out each name=value pair
    for (var i = 0; i < args.length; i++) {
        var pair = args[i].split('=');
        var name = decodeURIComponent(pair[0]);

        var value = (pair.length == 2)
			? decodeURIComponent(pair[1])
			: name;

        this.params[name] = value;
    }
}

Querystring.prototype.get = function (key, default_) {
    var value = this.params[key];
    return (value != null) ? value : default_;
}

Querystring.prototype.contains = function (key) {
    var value = this.params[key];
    return (value != null);
}
String.prototype.startsWith = function (str)
{ return (this.match("^" + str) == str) }

var timer; //needed to loop through thumbnails

function getVideoId(input) {

    var qs = new Querystring(input.split("?")[1]);
    return qs.get("v");
}

function clearList(ul) {
    var list = document.getElementById(ul);
    while (list.firstChild) {
        list.removeChild(list.firstChild);
    }
}

function hideOverlay() {
    var overlay = document.getElementById('embed_overlay');
    overlay.style.display = 'none';
    overlay.innerHTML = "";
}

var ytPlayerDiv;

function closesearch() {
    document.getElementById('youtubeDiv').innerHTML = "";
}


function listVideos(json, cfg) {
    if (!cfg.player) {
        cfg.player = 'embed';
    }
    if (!cfg.layout) {
        cfg.layout = 'full';
    }

    div = document.getElementById(cfg.block);
    var SearchTerm = document.getElementById('txtInput').value;
    if (SearchTerm.startsWith('YT link or keywords'))
        SearchTerm = document.location.href.replace("http://", "").split('/')[1].replace(/_/g, ' ');
    SearchTerm = decodeURIComponent(SearchTerm);
    div.innerHTML = '<h4 onclick=\"closesearch()\">[x] Search results for <span class="search-query-hig">' + SearchTerm + '</span> - <a href="/' + SearchTerm.replace(/ /g,'_') + '">result address</a></h4><div class="section-gradient-top"><div class="nearCap"></div><div class="farCap"></div></div>'; //empty

    var ul = document.createElement('ul');
    ul.setAttribute('id', 'youtubelist');
    if (json.feed.entry) {
        for (var i = 0; i < json.feed.entry.length; i++) {
            var entry = json.feed.entry[i];

            for (var k = 0; k < entry.link.length; k++) {
                if (entry.link[k].rel == 'alternate') {
                    url = entry.link[k].href;
                    break;
                }
            }


            var thumb = entry['media$group']['media$thumbnail'][1].url;

            var li = document.createElement('li');
            var overlayLink = 'javascript:videoOverlay(\'' + getVideoId(url) + '\',\'' + cfg.block + '\');';

            if (cfg.layout == 'thumbnails') {

                if (cfg.player == 'embed') {
                    li.innerHTML = '<a href="' + overlayLink + '"><img style="border:' + cfg.imgstyle + '" src="' + thumb + '" alt="' + entry.title.$t + '"  onmouseout="mouseOutImage(this,\'' + cfg.imgstyle + '\')" onmouseover="mousOverImage(this,\'' + getVideoId(url) + '\',2)"></a>';
                } else {
                    var EntryTitleSubstr;
                    if (entry.title.$t.length > 40)
                        EntryTitleSubstr = entry.title.$t.substr(0, 40);
                    else EntryTitleSubstr = entry.title.$t;
                    li.innerHTML = '<a href="/v/' + getVideoId(url) + '"><img onmouseover="changeThumb(\'' + getVideoId(url) + '\')" onmouseout="resetThumb(\'' + getVideoId(url) + '\')" id="' + getVideoId(url) + '" src="' + thumb + '" alt="' + entry.title.$t + '"></a><br /><a title="' + entry.title.$t + '" href="/v/' + getVideoId(url) + '">' + EntryTitleSubstr + '</a>';
                }
            } else {
                //cfg.layout = full
                if (cfg.player == 'embed') {
                    li.innerHTML = entry.content.$t.replace(url, overlayLink);
                } else {
                    //cfg.player = link
                    li.innerHTML = entry.content.$t;
                }
            }

            ul.appendChild(li);
        }
    } else {
        div.innerHTML = 'No Youtube videos found for your query:<br>Type:\'' + cfg.type + '\'<br>Query: \'' + cgf.q + '\'';

    }

    div.appendChild(ul);
}

var ytQuery = 0;
var ytInit = {}

function insertVideos(cfg) {
    cfg = cfg || {};
    if (!cfg.block) {
        //alert();
    } else {
        if (!cfg.type) {
            document.getElementById(cfg.block).innerHTML = 'You must provide a type: search, user, playlist, featured in the insertVideos function.';
        } else {
            document.getElementById(cfg.block).innerHTML = '<p style="text-align: center; margin: 10px 0 10px 0;"><img src="/loading.gif"> YouTubeLoop Instant Search is working. Please hold on for a second...</p>';
            //create a javascript element that returns our JSON data.
            var script = document.createElement('script');
            script.setAttribute('id', 'jsonScript');
            script.setAttribute('type', 'text/javascript');

            //a counter
            ytQuery++;

            if (!cfg.results) {
                cfg.results = 12;
            }
            if (!cfg.order) {
                cfg.orderby = 'relevance';
                cfg.sortorder = 'descending';
            }

            switch (cfg.order) {
                case "new_first":
                    cfg.orderby = 'published';
                    cfg.sortorder = 'ascending';
                    break;

                case "highest_rating":
                    cfg.orderby = 'rating';
                    cfg.sortorder = 'descending';
                    break;

                case "most_relevance":
                    cfg.orderby = 'relevance';
                    cfg.sortorder = 'descending';
                    break;
            }

            //what data do we need: a search, a user search, a playlist
            switch (cfg.type) {
                case "search":
                    script.setAttribute('src', 'http://gdata.youtube.com/feeds/videos?vq=' + cfg.q + '&max-results=' + cfg.results + '&format=5&alt=json-in-script&callback=ytInit[' + ytQuery + ']&orderby=' + cfg.orderby + '&sortorder=' + cfg.sortorder);
                    break;

                case "user":
                    script.setAttribute('src', 'http://gdata.youtube.com/feeds/users/' + cfg.q + '/uploads?max-results=' + cfg.results + '&alt=json-in-script&callback=ytInit[' + ytQuery + ']&orderby=' + cfg.orderby + '&sortorder=' + cfg.sortorder);
                    break;

                case "playlist":
                    script.setAttribute('src', 'http://gdata.youtube.com/feeds/playlists/' + cfg.q + '?max-results=' + cfg.results + '&alt=json-in-script&callback=ytInit[' + ytQuery + ']&orderby=' + cfg.orderby + '&sortorder=' + cfg.sortorder);
                    break;

                case "featured":
                    script.setAttribute('src', 'http://gdata.youtube.com/feeds/api/standardfeeds/recently_featured?alt=json-in-script&callback=ytInit[' + ytQuery + ']&start-index=1&max-results=' + cfg.results + '&orderby=' + cfg.orderby + '&sortorder=' + cfg.sortorder);
                    break;

            }

            ytInit[ytQuery] = function (root) { listVideos(root, cfg); };

            //attach script to page, this will load the data into our page and call the funtion ytInit[ytQuery]
            document.documentElement.firstChild.appendChild(script);
        }
    }
}

function searchme() {
    document.getElementById('HomepageText').style.display = "none";
    document.getElementById('BillOuter').style.display = "none";
    insertVideos({ 'block': 'youtubeDiv', 'q': document.getElementById('txtInput').value, 'type': 'search', 'results': 12, 'order': 'most_relevance', 'player': 'link', 'layout': 'thumbnails' });
}




function LoopIt(input) {
    document.getElementById('txtInputAlt').value = "";
    var qs = new Querystring(input.split("?")[1]);
    if (qs.get("v", "money") != "money") {
        window.location = "/v/" + qs.get("v");
    }
    else {
        searchme();

    }
}

function delquote(str) { return (str = str.replace(/["']{1}/gi, "")); }
function showRelated(data) {
    try {
        var feed = data.feed;
        var entries = feed.entry || [];
        var html = ['<h3>Related videos to this clip</h3><div class="section-gradient-top"><div class="nearCap"></div><div class="farCap"></div></div><ul id="videos">'];
        for (var i = 0; i < entries.length; i++) {
            var entry = entries[i];
            var title = entry.title.$t;
            if (title.length > 60) title = title.substr(0, 60) + "...";
            var titlel = entry.content.$t;
            titlel = delquote(titlel);
            var Rating = 0;
            //alert(typeof (entry.gd$rating.average));
            if (typeof (entry.gd$rating) != 'undefined') Rating = entry.gd$rating.average;
            var thumbnailUrl = entry.media$group.media$thumbnail[0].url;
            var VideoId = getVideoId(entry.media$group.media$player[0].url);
            var VideoItemString = "<li><a title=\"" + titlel + "\" href='/v/" + VideoId + "'><img onmouseover=\"changeThumb('" + VideoId + "')\" onmouseout=\"resetThumb('" + VideoId + "')\" id=\"" + VideoId + "\" align='left' src='" + thumbnailUrl + "' title='" + titlel + "' /></a><a class=big title=\"" + titlel + "\" href='/v/" + VideoId + "'>" + title + "</a><br /><span class=\"rating\">rating: " + Rating.toFixed(2) + "</span></li>";
            html.push(VideoItemString);
        }
        html.push('</ul><br style="clear: left;"/>');
        document.getElementById('videos2').innerHTML = html.join('');
    }
    catch (err) {
        alert(err.Message);
    }
}
function ShowVideoInformation(data) {
    document.getElementById("videoTitle").innerHTML = data.entry.title.$t;
    document.getElementById("videoDesc").innerHTML = data.entry.content.$t.substr(0, 180) + "...";

    var categories = data.entry.category || [];
    var html = ['<ul><li class=\"TermHeadline\">Related search terms to this song</li>'];
    for (var i = 1; i < categories.length; i++) {
        var SearchTerm = '<li><a onclick="searchTerm(\'' + categories[i].term + '\')" >' + categories[i].term + '</a></li>';
        html.push(SearchTerm);
    }
    html.push('</ul>');
    document.getElementById('searchTerm').innerHTML = html.join('');
    var VideoId = getVideoId(data.entry.link[0].href);
    document.getElementById('ToolBoxLink').innerHTML = "<a class=fbtool rel=nofollow target=_blank href=http://www.facebook.com/sharer.php?u=http://www.youtubeloop.com/v/" + VideoId + " >Share on Facebook</a><a class='SongChord' rel=nofollow target=_blank href=/lucky.html?q=site:ultimate-guitar.com+chords+" + RefineQueryString(document.getElementById('lbVideoTitle').innerHTML) + ">Song Chords</a><a class='SongFact' rel=nofollow target=_blank href=/lucky.html?q=songfacts+" + RefineQueryString(document.getElementById('lbVideoTitle').innerHTML) + ">Song Facts</a><a class=videotool rel=nofollow target=_blank href=http://keepvid.com/?url=http%3A%2F%2Fwww.youtube.com%2Fwatch%3Fv%3D" + VideoId + " >Download video</a><a class=audiotool rel=nofollow target=_blank href=http://www.listentoyoutube.com/index.php?url=http://www.youtube.com/watch?v=" + VideoId + ">Extract audio</a>";

}
function RefineQueryString(input) {
    input = input.replace(/ /g, '+');
    input = encodeURI(input);
    input = input.replace(/&amp;/g, 'and');
    
    return input;
}
function ShowComments(data) {
    var feed = data.feed;
    var entries = feed.entry || [];
    var itemCounter = 0;
    var html = ['<h3>Latest comments from YouTube users (roll over for full view)</h3><div class="section-gradient-top"><div class="nearCap"></div><div class="farCap"></div></div><ul>'];
    for (var i = 0; i < entries.length; i++) {
        var entry = entries[i];
        var title = entry.content.$t;
        var username = entry.author[0].name.$t;
        html.push("<li onmouseover=\"ListExpand(this)\" onmouseout=\"ListCollapse(this)\" ><b>" + username + ":</b><br />" + title + "</li>");
        itemCounter++;
    }
    html.push("</ul>");
    document.getElementById('CommentArea').innerHTML = html.join('');
    if (itemCounter == 0) document.getElementById('CommentArea').innerHTML = '';
}
function ParseBillboard(data) {
    var html = '<ul>';
    for (var i = 0; i < data.feed.entry.length; i++) {
        var title = data.feed.entry[i].title.$t;
        html += "<li><table><tr><td><img class='billavatar' height='30px' id='bill" + i.toString() + "'/></td><td><a href='\\" + title.replace(/ /g, '_') + "'>" + title + "</a></td></tr></table></li>";
    }
    
    html += "</ul>";
    document.getElementById('BillboardSample').innerHTML = html;

    for (var i = 0; i < data.feed.entry.length; i++) {
        var title = data.feed.entry[i].title.$t;
        title = encodeURI(title.replace(/ /g, '+'));
        var ShowComment = document.createElement("script");
        ShowComment.type = "text/javascript";
        ShowComment.src = "http://gdata.youtube.com/feeds/videos?vq=" + title + "&max-results=1&format=5&alt=json-in-script&callback=AvatarSearch" + i.toString() + "&orderby=relevance&sortorder=descending";
        document.getElementById('callbackjs').appendChild(ShowComment);
    }
}
function AvatarSearch0(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill0').src = Avatar; }
function AvatarSearch1(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill1').src = Avatar; }
function AvatarSearch2(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill2').src = Avatar; }
function AvatarSearch3(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill3').src = Avatar; }
function AvatarSearch4(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill4').src = Avatar; }
function AvatarSearch5(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill5').src = Avatar; }
function AvatarSearch6(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill6').src = Avatar; }
function AvatarSearch7(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill7').src = Avatar; }
function AvatarSearch8(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill8').src = Avatar; }
function AvatarSearch9(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill9').src = Avatar; }
function AvatarSearch10(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill10').src = Avatar; }
function AvatarSearch11(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill11').src = Avatar; }
function AvatarSearch12(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill12').src = Avatar; }
function AvatarSearch13(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill13').src = Avatar; }
function AvatarSearch14(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill14').src = Avatar; }
function AvatarSearch15(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill15').src = Avatar; }
function AvatarSearch16(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill16').src = Avatar; }
function AvatarSearch17(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill17').src = Avatar; }
function AvatarSearch18(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill18').src = Avatar; }
function AvatarSearch19(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill19').src = Avatar; }
function AvatarSearch20(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill20').src = Avatar; }
function AvatarSearch21(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill21').src = Avatar; }
function AvatarSearch22(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill22').src = Avatar; }
function AvatarSearch23(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill23').src = Avatar; }
function AvatarSearch24(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill24').src = Avatar; }
function AvatarSearch25(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill25').src = Avatar; }
function AvatarSearch26(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill26').src = Avatar; }
function AvatarSearch27(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill27').src = Avatar; }
function AvatarSearch28(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill28').src = Avatar; }
function AvatarSearch29(data) { var Avatar = data.feed.entry[0].media$group.media$thumbnail[1].url; document.getElementById('bill29').src = Avatar; }

var DescriptionExpand = 0;
var TermExpand = 0;

function PageOnLoad() {
    document.getElementById('txtInputAlt').value = "";
    var VideoId = document.location.href.split('/v/')[1];

    if (VideoId != null) {
        if (VideoId.indexOf("#") != -1) VideoId = VideoId.split("#")[0];
        var showRelated = document.createElement("script");
        showRelated.type = "text/javascript";
        showRelated.src = "http://gdata.youtube.com/feeds/api/videos/" + VideoId + "/related?alt=json-in-script&callback=showRelated&max-results=20&orderby=rating&format=5";
        document.getElementById('callbackjs').appendChild(showRelated);

        /*
        var ShowComment = document.createElement("script");
        ShowComment.type = "text/javascript";
        ShowComment.src = "http://gdata.youtube.com/feeds/api/videos/" + VideoId + "/comments?alt=json-in-script&callback=ShowComments";
        document.getElementById('callbackjs').appendChild(ShowComment);
        */
        ShowLike("likebutton", "like");
        document.getElementById('ToolBoxLink').innerHTML = "<a class=fbtool rel=nofollow target=_blank href=http://www.facebook.com/sharer.php?u=http://www.youtubeloop.com/v/" + VideoId + " >Share on Facebook</a><a class='SongChord' rel=nofollow target=_blank href=/lucky.html?q=site:ultimate-guitar.com+chords+" + RefineQueryString(document.getElementById('lbVideoTitle').innerHTML) + ">Song Chords</a><a class='SongFact' rel=nofollow target=_blank href=/lucky.html?q=songfacts+" + RefineQueryString(document.getElementById('lbVideoTitle').innerHTML) + ">Song Facts</a><a class=videotool rel=nofollow target=_blank href=http://keepvid.com/?url=http%3A%2F%2Fwww.youtube.com%2Fwatch%3Fv%3D" + VideoId + " >Download video</a><a class=audiotool rel=nofollow target=_blank href=http://www.listentoyoutube.com/index.php?url=http://www.youtube.com/watch?v=" + VideoId + ">Extract audio</a>";
        
        
        document.getElementById("lbVideoDesc").style.height = "22px";
        if(document.getElementById("lbVideoDesc").scrollHeight > 22)
            document.getElementById('ShowFullDesc').style.display = 'block';
        document.getElementById("FootNavigator").style.display = "block";
        var ckuikParam = document.getElementById('lbVideoTitle').innerHTML.replace(/ /g, '_');
        document.getElementById("ckuikLink").setAttribute('href', 'http://ckuik.com/' + ckuikParam);
        

    }
    else {
        //loadJson('top_rated');
        document.getElementById("HomepageText").style.display = "block";
        document.getElementById("SearchTag").style.display = "none";

        // billboard sample
        document.getElementById("BillOuter").style.display = "block";
        var billboard = document.createElement("script");
        billboard.type = "text/javascript";
        billboard.src = "http://gdata.youtube.com/feeds/api/channelstandardfeeds/US/most_viewed/-/Music?v=2&alt=json-in-script&callback=ParseBillboard&max-results=30";
        document.getElementById('callbackjs').appendChild(billboard);

    }

    //ShowFacebookPage("FacebookFanPage");
    var myInput = document.getElementById("txtInput");
    myInput.name = "R" + Math.random();
    // attach onkeydown for capture tab
    if (myInput.addEventListener) {
        myInput.addEventListener('keydown', this.keyHandler, false);
    } else if (myInput.attachEvent) {
        myInput.attachEvent('onkeydown', this.keyHandler); /* damn IE hack */
    }
}
function SearchPageOnLoad() {
    document.getElementById('txtInputAlt').value = "";
    var myInput = document.getElementById("txtInput");
    myInput.name = "R" + Math.random();
    // attach onkeydown for capture tab
    if (myInput.addEventListener) {
        myInput.addEventListener('keydown', this.keyHandler, false);
    } else if (myInput.attachEvent) {
        myInput.attachEvent('onkeydown', this.keyHandler); /* damn IE hack */
    }
    var SearchTerm = document.location.href.replace("http://", "").split('/')[1].replace(/_/g, ' ');
    insertVideos({ 'block': 'youtubeDiv', 'q': SearchTerm, 'type': 'search', 'results': 40, 'order': 'most_relevance', 'player': 'link', 'layout': 'thumbnails' });
}
function BillboardOnLoad() {
    document.getElementById('txtInputAlt').value = "";
    var myInput = document.getElementById("txtInput");
    myInput.name = "R" + Math.random();
    // attach onkeydown for capture tab
    if (myInput.addEventListener) {
        myInput.addEventListener('keydown', this.keyHandler, false);
    } else if (myInput.attachEvent) {
        myInput.attachEvent('onkeydown', this.keyHandler); /* damn IE hack */
    }
}

function ShowFullDesc() {
    document.getElementById('ShowFullDesc').style.display = 'none';
    document.getElementById('HideFullDesc').style.display = 'block';
    document.getElementById("lbVideoDesc").style.height = "auto";
}
function HideFullDesc() {
    document.getElementById('HideFullDesc').style.display = 'none';
    document.getElementById('ShowFullDesc').style.display = 'block';
    document.getElementById("lbVideoDesc").style.height = "22px";
}
function ToggleSearchTerm() {
    if (TermExpand == 0) {
        TermExpand = 1;
        if (document.getElementById("Terms").scrollHeight > 25)
            document.getElementById("SearchTag").style.height = document.getElementById("Terms").scrollHeight + 5 + "px";
    }
    else {
        TermExpand = 0;
        document.getElementById("SearchTag").style.height = "25px";
    }
}
function keyHandler(e) {
    var TABKEY = 9;
    if (e.keyCode == TABKEY) {
        var Search = document.getElementById('txtInput');
        if ((CurrentTopSuggestion != '') && (Search.value != CurrentTopSuggestion) && (CurrentSelectedListItem == -1)) Search.value = CurrentTopSuggestion;
        LoopIt(Search.value);

        if (e.preventDefault) {
            e.preventDefault();
        }
        return false;
    }
}
function loadJson(feedName) {
    var script = document.createElement('script');
    script.setAttribute('id', 'jsonScript');
    script.setAttribute('type', 'text/javascript');
    script.setAttribute('src', 'http://gdata.youtube.com/feeds/api/standardfeeds/' + feedName + '?alt=json-in-script&callback=StandardFeedCallBack&max-results=24');
    document.documentElement.firstChild.appendChild(script);
}
function StandardFeedCallBack(data) {
    var feed = data.feed;
    var entries = feed.entry || [];
    var html = ['<h4>'];
    if (feed.id.$t == "http://gdata.youtube.com/feeds/api/standardfeeds/top_rated") {
        html.push('<span class=\"spanSelected\" id=\"top_rated\" onclick=\"loadJson(\'top_rated\')\">Top rated</span><span id=\"top_favorites\" onclick=\"loadJson(\'top_favorites\')\">Top favorites</span><span id=\"most_viewed\" onclick=\"loadJson(\'most_viewed\')\">Most viewed</span><span id=\"most_popular\" onclick=\"loadJson(\'most_popular\')\">Most popular</span><span id=\"most_recent\" onclick=\"loadJson(\'most_recent\')\">Most recent</span><span id=\"most_discussed\" onclick=\"loadJson(\'most_discussed\')\">Most discussed</span><span id=\"most_responded\" onclick=\"loadJson(\'most_responded\')\">Most responded</span><span id=\"recently_featured\" onclick=\"loadJson(\'recently_featured\')\">Recently featured</span></h4><ul id=\'videos\'>');
    }
    else if (feed.id.$t == "http://gdata.youtube.com/feeds/api/standardfeeds/top_favorites") {
        html.push('<span id=\"top_rated\" onclick=\"loadJson(\'top_rated\')\">Top rated</span><span class=\"spanSelected\" id=\"top_favorites\" onclick=\"loadJson(\'top_favorites\')\">Top favorites</span><span id=\"most_viewed\" onclick=\"loadJson(\'most_viewed\')\">Most viewed</span><span id=\"most_popular\" onclick=\"loadJson(\'most_popular\')\">Most popular</span><span id=\"most_recent\" onclick=\"loadJson(\'most_recent\')\">Most recent</span><span id=\"most_discussed\" onclick=\"loadJson(\'most_discussed\')\">Most discussed</span><span id=\"most_responded\" onclick=\"loadJson(\'most_responded\')\">Most responded</span><span id=\"recently_featured\" onclick=\"loadJson(\'recently_featured\')\">Recently featured</span></h4><ul id=\'videos\'>');
    }
    else if (feed.id.$t == "http://gdata.youtube.com/feeds/api/standardfeeds/most_viewed") {
        html.push('<span id=\"top_rated\" onclick=\"loadJson(\'top_rated\')\">Top rated</span><span id=\"top_favorites\" onclick=\"loadJson(\'top_favorites\')\">Top favorites</span><span class=\"spanSelected\" id=\"most_viewed\" onclick=\"loadJson(\'most_viewed\')\">Most viewed</span><span id=\"most_popular\" onclick=\"loadJson(\'most_popular\')\">Most popular</span><span id=\"most_recent\" onclick=\"loadJson(\'most_recent\')\">Most recent</span><span id=\"most_discussed\" onclick=\"loadJson(\'most_discussed\')\">Most discussed</span><span id=\"most_responded\" onclick=\"loadJson(\'most_responded\')\">Most responded</span><span id=\"recently_featured\" onclick=\"loadJson(\'recently_featured\')\">Recently featured</span></h4><ul id=\'videos\'>');
    }
    else if (feed.id.$t == "http://gdata.youtube.com/feeds/api/standardfeeds/most_popular") {
        html.push('<span id=\"top_rated\" onclick=\"loadJson(\'top_rated\')\">Top rated</span><span id=\"top_favorites\" onclick=\"loadJson(\'top_favorites\')\">Top favorites</span><span id=\"most_viewed\" onclick=\"loadJson(\'most_viewed\')\">Most viewed</span><span class=\"spanSelected\" id=\"most_popular\" onclick=\"loadJson(\'most_popular\')\">Most popular</span><span id=\"most_recent\" onclick=\"loadJson(\'most_recent\')\">Most recent</span><span id=\"most_discussed\" onclick=\"loadJson(\'most_discussed\')\">Most discussed</span><span id=\"most_responded\" onclick=\"loadJson(\'most_responded\')\">Most responded</span><span id=\"recently_featured\" onclick=\"loadJson(\'recently_featured\')\">Recently featured</span></h4><ul id=\'videos\'>');
    }
    else if (feed.id.$t == "http://gdata.youtube.com/feeds/api/standardfeeds/most_recent") {
        html.push('<span id=\"top_rated\" onclick=\"loadJson(\'top_rated\')\">Top rated</span><span id=\"top_favorites\" onclick=\"loadJson(\'top_favorites\')\">Top favorites</span><span id=\"most_viewed\" onclick=\"loadJson(\'most_viewed\')\">Most viewed</span><span id=\"most_popular\" onclick=\"loadJson(\'most_popular\')\">Most popular</span><span class=\"spanSelected\" id=\"most_recent\" onclick=\"loadJson(\'most_recent\')\">Most recent</span><span id=\"most_discussed\" onclick=\"loadJson(\'most_discussed\')\">Most discussed</span><span id=\"most_responded\" onclick=\"loadJson(\'most_responded\')\">Most responded</span><span id=\"recently_featured\" onclick=\"loadJson(\'recently_featured\')\">Recently featured</span></h4><ul id=\'videos\'>');
    }
    else if (feed.id.$t == "http://gdata.youtube.com/feeds/api/standardfeeds/most_responded") {
        html.push('<span id=\"top_rated\" onclick=\"loadJson(\'top_rated\')\">Top rated</span><span id=\"top_favorites\" onclick=\"loadJson(\'top_favorites\')\">Top favorites</span><span id=\"most_viewed\" onclick=\"loadJson(\'most_viewed\')\">Most viewed</span><span id=\"most_popular\" onclick=\"loadJson(\'most_popular\')\">Most popular</span><span id=\"most_recent\" onclick=\"loadJson(\'most_recent\')\">Most recent</span><span id=\"most_discussed\" onclick=\"loadJson(\'most_discussed\')\">Most discussed</span><span class=\"spanSelected\" id=\"most_responded\" onclick=\"loadJson(\'most_responded\')\">Most responded</span><span id=\"recently_featured\" onclick=\"loadJson(\'recently_featured\')\">Recently featured</span></h4><ul id=\'videos\'>');
    }
    else if (feed.id.$t == "http://gdata.youtube.com/feeds/api/standardfeeds/recently_featured") {
        html.push('<span id=\"top_rated\" onclick=\"loadJson(\'top_rated\')\">Top rated</span><span id=\"top_favorites\" onclick=\"loadJson(\'top_favorites\')\">Top favorites</span><span id=\"most_viewed\" onclick=\"loadJson(\'most_viewed\')\">Most viewed</span><span id=\"most_popular\" onclick=\"loadJson(\'most_popular\')\">Most popular</span><span id=\"most_recent\" onclick=\"loadJson(\'most_recent\')\">Most recent</span><span id=\"most_discussed\" onclick=\"loadJson(\'most_discussed\')\">Most discussed</span><span id=\"most_responded\" onclick=\"loadJson(\'most_responded\')\">Most responded</span><span class=\"spanSelected\" id=\"recently_featured\" onclick=\"loadJson(\'recently_featured\')\">Recently featured</span></h4><ul id=\'videos\'>');
    }
    else if (feed.id.$t == "http://gdata.youtube.com/feeds/api/standardfeeds/most_discussed") {
        html.push('<span id=\"top_rated\" onclick=\"loadJson(\'top_rated\')\">Top rated</span><span id=\"top_favorites\" onclick=\"loadJson(\'top_favorites\')\">Top favorites</span><span id=\"most_viewed\" onclick=\"loadJson(\'most_viewed\')\">Most viewed</span><span id=\"most_popular\" onclick=\"loadJson(\'most_popular\')\">Most popular</span><span id=\"most_recent\" onclick=\"loadJson(\'most_recent\')\">Most recent</span><span class=\"spanSelected\" id=\"most_discussed\" onclick=\"loadJson(\'most_discussed\')\">Most discussed</span><span id=\"most_responded\" onclick=\"loadJson(\'most_responded\')\">Most responded</span><span id=\"recently_featured\" onclick=\"loadJson(\'recently_featured\')\">Recently featured</span></h4><ul id=\'videos\'>');
    }

    for (var i = 0; i < entries.length; i++) {
        var entry = entries[i];
        var title = entry.title.$t;
        if (title.length > 60) title = title.substr(0, 60) + "...";
        var titlel = entry.content.$t;
        var viewCount = entry.yt$statistics.viewCount;

        if (typeof (entry.app$control) == 'undefined') {
            var thumbnailUrl = entry.media$group.media$thumbnail[0].url;
            var VideoId = getVideoId(entry.media$group.media$player[0].url);
            var VideoItemString = "<li><a title=\"" + titlel + "\" href='/v/" + VideoId + "'><img onmouseover=\"changeThumb('" + VideoId + "')\" onmouseout=\"resetThumb('" + VideoId + "')\" id=\"" + VideoId + "\" align='left' src='" + thumbnailUrl + "' title='" + titlel + "' /></a><a title=\"" + titlel + "\" href='/v/" + VideoId + "'>" + title + "</a> <span class=\"rating\">view: " + addCommas(viewCount) + "</span></li>";
            html.push(VideoItemString);
        }
    }
    html.push('</ul>');
    document.getElementById('videos2').innerHTML = html.join('');
}
function addCommas(nStr) {
    nStr += '';
    x = nStr.split('.');
    x1 = x[0];
    x2 = x.length > 1 ? '.' + x[1] : '';
    var rgx = /(\d+)(\d{3})/;
    while (rgx.test(x1)) {
        x1 = x1.replace(rgx, '$1' + ',' + '$2');
    }
    return x1 + x2;
}

var CurrentSelectedListItem = -1;
var SuggestionListLength = 0;
var EnterPress = 0;
var CurrentTextboxQuery = "";
var CurrentTopSuggestion = "";

function SubmitOnEnter(myfield, e) {
    document.getElementById('txtInputAlt').value = "";
    var MyTextBox = document.getElementById('txtInput');
    var CurrentSearchQuery = MyTextBox.value;
    var SuggestionDiv = document.getElementById('SearchSuggestion');

    var keycode;
    if (window.event) keycode = window.event.keyCode;
    else if (e) keycode = e.which;
    else return true;


    if (keycode == 13) {
        EnterPress = 1;

        LoopIt(CurrentSearchQuery);
        document.getElementById("SearchSuggestion").innerHTML = "";
        document.getElementById("SearchSuggestion").style.visibility = "hidden";
        return false;

    }

}


function SubmitOnUp(e) {
    document.getElementById('txtInputAlt').value = "";
    var MyTextBox = document.getElementById('txtInput');
    var CurrentSearchQuery = MyTextBox.value;
    var SuggestionDiv = document.getElementById('SearchSuggestion');

    var keycode;
    if (window.event) keycode = window.event.keyCode;
    else if (e) keycode = e.which;
    else return true;
    // neu dang khong nagivate tren list thi update suggestion
    if (CurrentSelectedListItem == -1) {
        CurrentTextboxQuery = CurrentSearchQuery;
        AttachSuggestionScript(CurrentSearchQuery);
    }


    switch (keycode) {
        case 13:
            //alert("enter");
            EnterPress = 1;
            return false;
            break;
        case 32:
            EnterPress = 0;
            LoopIt(CurrentSearchQuery);
            break;
        case 40:
            EnterPress = 0;
            var DoSearch = false;
            if (SuggestionListLength > 0) {
                if (CurrentSelectedListItem < SuggestionListLength - 1) {
                    CurrentSelectedListItem++;
                    DoSearch = true;
                }
                else {
                    CurrentSelectedListItem = SuggestionListLength - 1;
                    DoSearch = false;
                }
            }
            var CurrentListItem = document.getElementById("suggest_" + CurrentSelectedListItem);
            CurrentListItem.style.backgroundColor = "#9DE1FB";
            if (CurrentSelectedListItem != 0) {
                var PrevId = CurrentSelectedListItem - 1;
                var PreviousListItem = document.getElementById("suggest_" + PrevId);
                PreviousListItem.style.backgroundColor = "#DAF3FD";
            }


            MyTextBox.value = CurrentListItem.title;

            if (DoSearch == true)
                LoopIt(MyTextBox.value);
            break;
        case 38:
            EnterPress = 0;
            var DoSearch = false;

            if (SuggestionListLength > 0) {
                if (CurrentSelectedListItem > 0) {
                    CurrentSelectedListItem--;
                    DoSearch = true;
                }
                else {
                    CurrentSelectedListItem = 0;
                    DoSearch = false;
                }
            }
            var CurrentListItem = document.getElementById('suggest_' + CurrentSelectedListItem);
            var PrevId = CurrentSelectedListItem + 1;
            var PreviousListItem = document.getElementById('suggest_' + PrevId);
            CurrentListItem.style.backgroundColor = "#9DE1FB";
            PreviousListItem.style.backgroundColor = "#DAF3FD";
            MyTextBox.value = CurrentListItem.title;
            if (DoSearch == true)
                LoopIt(MyTextBox.value);
            break;
        case 39: // right button
            var Search = document.getElementById('txtInput');
            if ((CurrentTopSuggestion != '') && (Search.value != CurrentTopSuggestion) && (CurrentSelectedListItem == -1)) Search.value = CurrentTopSuggestion;
            LoopIt(Search.value);
            break;
        default:
            CurrentSelectedListItem = -1;
    }
}
function StartTimeEnter(myfield, e) {
    var keycode;
    if (window.event) keycode = window.event.keyCode;
    else if (e) keycode = e.which;
    else return true;

    if (keycode == 13) {
        var EndTimeTextbox = document.getElementById("txtEndTime");
        EndTimeTextbox.focus();
        return false;
    }
}
function EndTimeEnter(myfield, e) {
    var keycode;
    if (window.event) keycode = window.event.keyCode;
    else if (e) keycode = e.which;
    else return true;

    if (keycode == 13) {
        TimeValidator();
        PartLoopIt();
        return false;
    }
}
function TimeValidator() {

    var StartTime = parseInt(TimeToInt(document.getElementById("txtStartTime").value, ":"));
    var EndTime = parseInt(TimeToInt(document.getElementById("txtEndTime").value, ":"));
    if (!isNaN(StartTime) && !isNaN(EndTime)) {
        if ((StartTime < 0) || (EndTime < 0)) {
            alert("Time cannot be negative! Time value will be reset!");
            document.getElementById("txtStartTime").value = "0:00";
            document.getElementById("txtEndTime").value = IntToTime(parseInt(ytplayer.getDuration()), ":");
        }
        if (StartTime >= EndTime) {
            alert("Starting time cannot be after ending time, the two values will be exchanged!");
            var a = document.getElementById("txtStartTime").value;
            document.getElementById("txtStartTime").value = document.getElementById("txtEndTime").value;
            document.getElementById("txtEndTime").value = a;
        }
        if (EndTime > parseInt(ytplayer.getDuration())) {
            alert("Ending time cannot be after the end of the clip.");
            document.getElementById("txtEndTime").value = IntToTime(parseInt(ytplayer.getDuration()), ":");
        }
    }
    else {
        alert("Time format must be m:ss, where m is the number of minute and ss is the number of second!Time value will be reset!");
        document.getElementById("txtStartTime").value = "0:00";
        document.getElementById("txtEndTime").value = IntToTime(parseInt(ytplayer.getDuration()), ":");
    }

}
function searchTerm(term) {
    document.getElementById('txtInput').value = term;
    insertVideos({ 'block': 'youtubeDiv', 'q': term, 'type': 'search', 'results': 12, 'order': 'most_relevance', 'player': 'link', 'layout': 'thumbnails' });
}
function updateHTML(elmId, value) {
    document.getElementById(elmId).innerHTML = value;
}

function doSomething() {
    var CurrentTime = parseInt(ytplayer.getCurrentTime());
    document.getElementById("CurrentLocation").innerHTML = IntToTime(parseInt(ytplayer.getCurrentTime() + 0.51), ":");
    var StartTime = TimeToInt(document.getElementById("txtStartTime").value, ":");
    var EndTime = TimeToInt(document.getElementById("txtEndTime").value, ":");
    if (CurrentTime == EndTime) {
        if (StartTime > 0) StartTime = StartTime - 1;
        ytplayer.seekTo(StartTime, true);
    }
}

function onYouTubePlayerReady(playerId) {
    ytplayer = document.getElementById("myytplayer");
    ytplayer.addEventListener("onStateChange", "onytplayerStateChange");
    //update time spans

    if (document.location.href.indexOf("#") != -1) {
        var BookmarkQuery = document.location.href.split("#")[1].replace(/\&amp;/g, '&');
        var qs = new Querystring(BookmarkQuery);
        var StartTime = qs.get("s");
        var EndTime = qs.get("e");
        document.getElementById("txtStartTime").value = IntToTime(parseInt(StartTime), ":");
        document.getElementById("txtEndTime").value = IntToTime(parseInt(EndTime), ":");
    }
    else {
        document.getElementById("txtStartTime").value = "0:00";
        document.getElementById("txtEndTime").value = IntToTime(parseInt(ytplayer.getDuration()), ":");
    }
    document.getElementById("LoopPartShareLink").innerHTML = document.location.href;

    setInterval(doSomething, 600);
    doSomething();


}
var FarPreviousState = -1;
var ClosePreviousState = -1;
function onytplayerStateChange(newState) {
    //alert("far: " + FarPreviousState + " close: " + ClosePreviousState + " current: " + newState);
    //alert(newState);
    var StartTime = parseInt(TimeToInt(document.getElementById("txtStartTime").value, ":"));

    if (newState == 0) {
        if ((ClosePreviousState == 1) && (StartTime != 0)) {
            ytplayer.seekTo(StartTime, true);
        }
    }

    if ((newState == 1) && (FarPreviousState == -1) && (ClosePreviousState == 3)) {
        if (StartTime > 0)
            ytplayer.seekTo(StartTime, true);
    }
    FarPreviousState = ClosePreviousState;
    ClosePreviousState = newState;
}


function TimeToInt(input, splitter) {
    var TimeArray = input.split(splitter);
    return parseInt(TimeArray[0]) * 60 + parseInt(TimeArray[1]);
}
function IntToTime(input, splitter) {
    if (input - parseInt(input / 60) * 60 < 10)
        return parseInt(input / 60) + splitter + "0" + (input - parseInt(input / 60) * 60);
    else
        return parseInt(input / 60) + splitter + (input - parseInt(input / 60) * 60);
}

function PartLoopIt() {
    var StartTime = TimeToInt(document.getElementById("txtStartTime").value, ":");
    var EndTime = TimeToInt(document.getElementById("txtEndTime").value, ":");
    var CurrentLink = document.location.href;
    if (CurrentLink.indexOf("#") != -1) {
        CurrentLink = CurrentLink.split("#")[0];
    }
    document.location.href = CurrentLink + "#s=" + StartTime + "&e=" + EndTime;
    document.getElementById("LoopPartShareLink").innerHTML = document.location.href;
    ytplayer.playVideo();
    if (StartTime > 0) StartTime = StartTime - 1;
    ytplayer.seekTo(StartTime, true);
    return false;
}
function RemovePartLoopIt() {
    document.getElementById("txtStartTime").value = "0:00";
    document.getElementById("txtEndTime").value = IntToTime(parseInt(ytplayer.getDuration()), ":");
    var CurrentLink = document.location.href;
    if (CurrentLink.indexOf("#") != -1) {
        CurrentLink = CurrentLink.split("#")[0];
    }
    document.location.href = CurrentLink + "#s=0&e=" + parseInt(ytplayer.getDuration());
    document.getElementById("LoopPartShareLink").innerHTML = document.location.href;
}
function encodeMyHtml(encodedHtml) {
    encodedHtml = escape(encodedHtml);
    encodedHtml = encodedHtml.replace(/\//g, "%2F");
    encodedHtml = encodedHtml.replace(/\?/g, "%3F");
    encodedHtml = encodedHtml.replace(/=/g, "%3D");
    encodedHtml = encodedHtml.replace(/&/g, "%26");
    encodedHtml = encodedHtml.replace(/@/g, "%40");
    return encodedHtml;
}
function ShowLike(divid, type) {
    ifrm = document.createElement("IFRAME");
    ifrm.setAttribute("src", "http://www.facebook.com/plugins/like.php?href=" + encodeMyHtml(window.location.href) + "&layout=standard&show_faces=false&width=300&action=" + type + "&colorscheme=light&height=31");
    ifrm.style.width = 310 + "px";
    ifrm.style.height = 31 + "px";
    ifrm.style.border = "0";
    ifrm.style.overflow = "hidden";
    ifrm.setAttribute("frameBorder", "0px");
    ifrm.setAttribute("allowTransparency", "true");
    ifrm.setAttribute("scrolling", "no");
    document.getElementById(divid).appendChild(ifrm);
}
function ShowFacebookPage(divid) {
    ifrm = document.createElement("IFRAME");
    ifrm.setAttribute("src", "http://www.facebook.com/plugins/likebox.php?id=154563927907505&width=238&connections=8&stream=false&header=false&height=284&border_color=%23e5e5e5");
    ifrm.style.width = 238 + "px";
    ifrm.style.height = 284 + "px";
    ifrm.style.border = "0";
    ifrm.style.paddingLeft = 1 + "px";
    ifrm.style.overflow = "hidden";
    ifrm.setAttribute("frameBorder", "0px");
    ifrm.setAttribute("allowTransparency", "true");
    ifrm.setAttribute("scrolling", "no");
    document.getElementById(divid).appendChild(ifrm);
}

// copy share link

function selectText(TextboxId) {
    document.getElementById(TextboxId).focus();
    document.getElementById(TextboxId).select();
}

function RelatedLoad() {
    var qs = new Querystring(document.location.href.split("?")[1]);
    var VideoId = qs.get("v");
    var showRelatedSmall = document.createElement("script");
    showRelatedSmall.type = "text/javascript";
    showRelatedSmall.src = "http://gdata.youtube.com/feeds/api/videos/" + VideoId + "/related?alt=json-in-script&callback=showRelatedSmall";
    document.getElementById('callbackjs').appendChild(showRelatedSmall);
}

function showRelatedSmall(data) {
    var feed = data.feed;
    var entries = feed.entry || [];
    var html = ['<ul>'];
    var Counter = 0, Max = 5;

    for (var i = 0; i < entries.length; i++) {
        if (Counter < Max) {
            var entry = entries[i];
            var title = entry.title.$t;
            if (title.length > 60) title = title.substr(0, 60) + "...";
            if (typeof (entry.app$control) == 'undefined') {
                var thumbnailUrl = entry.media$group.media$thumbnail[0].url;
                var VideoId = getVideoId(entry.media$group.media$player[0].url);
                var VideoItemString = "<li><a target=\"_blank\" href='/v/" + VideoId + "'><img align='left' src='" + thumbnailUrl + "' /></a><a target=\"_blank\" href='/v/" + VideoId + "'>" + title + "</a></li>";
                html.push(VideoItemString);
                Counter++;
            }
        }
    }
    html.push('</ul><br style="clear: left;"/>');
    document.getElementById('related').innerHTML = html.join('');

}


var Changing = 0;
var Sequence = 1;
var Id;
var TimeOutId;
function ChangePic(id) {
    document.getElementById(id).src = "http://i.ytimg.com/vi/" + id + "/" + Sequence + ".jpg";
    if (Sequence == 1) Sequence = 2;
    else if (Sequence == 2) Sequence = 3;
    else if (Sequence == 3) Sequence = 1;
}
function LoadPic() {
    if (Changing == 1) {
        ChangePic(Id);
        clearTimeout(TimeOutId);
        TimeOutId = setTimeout('LoadPic()', 1000);
    }
    else
        clearTimeout(TimeOutId);
}
function changeThumb(id) {
    Changing = 1;
    Id = id;
    LoadPic();
}
function resetThumb(id) {
    Changing = 0;
    document.getElementById(id).src = "http://i.ytimg.com/vi/" + id + "/1.jpg";
}
function ListExpand(object) {
    object.style.height = "250px";
    object.style.fontSize = "15px";
}
function ListCollapse(object) {
    object.style.height = "50px";
    object.style.fontSize = "10.5px";
}
function AttachSuggestionScript(query) {
    var showSuggestion = document.createElement("script");
    showSuggestion.type = "text/javascript";
    showSuggestion.src = "http://suggestqueries.google.com/complete/search?hl=en&ds=yt&client=youtube&cp=1&callback=ParseSuggestion&q=" + query;
    document.getElementById('callbackjs').appendChild(showSuggestion);
}
function ParseSuggestion(data) {
    var SearchCorrection = document.getElementById('txtInputAlt');
    var ItemCounter = 0;
    SearchCorrection.value = "";
    var FoundBestSuggest = 0;

    var CurrentQuery = data[0];
    var SuggestionDiv = document.getElementById('SearchSuggestion');
    var html = ['<ul>']
    for (var i = 0; i < data[1].length; i++) {
        if ((data[1][i][0].startsWith(CurrentQuery)) && (FoundBestSuggest == 0)) {
            SearchCorrection.value = data[1][i][0];
            CurrentTopSuggestion = data[1][i][0];
            FoundBestSuggest = 1;
        }
        if (data[1][i][0] != CurrentQuery) {

            html.push("<li title=\"" + data[1][i][0] + "\" id=\"suggest_" + ItemCounter + "\" onclick=\"SuggestListClick(this)\" >" + data[1][i][0].replace(CurrentTextboxQuery, "<b>" + CurrentTextboxQuery + "</b>") + "<span onclick=\"LoopClick('" + escape(data[1][i][0]) + "')\" class=\"looply\">I'm feeling looply</span></li>");
            ItemCounter++;
        }
    }
    html.push("</ul><h4 onclick=\"SuggestionClose()\">x close</h4>");
    SuggestionDiv.innerHTML = html.join('');

    if (ItemCounter > 0) {
        SuggestionDiv.style.visibility = "visible";
        SuggestionListLength = data[1].length;
    }
    else {

        SuggestionDiv.innerHTML = "";
        SuggestionDiv.style.visibility = "hidden";
        SuggestionListLength = 0;
    }
    if (EnterPress == 1) {
        SuggestionDiv.innerHTML = "";
        SuggestionDiv.style.visibility = "hidden";
        SuggestionListLength = 0;
    }
}

function SuggestListClick(ListItem) {
    document.getElementById("txtInput").value = ListItem.title;
    LoopIt(document.getElementById("txtInput").value);
    document.getElementById("SearchSuggestion").innerHTML = "";
    document.getElementById("SearchSuggestion").style.visibility = "hidden";
}
function SuggestionClose() {
    document.getElementById("SearchSuggestion").innerHTML = "";
    document.getElementById("SearchSuggestion").style.visibility = "hidden";
}
function LoopClick(QueryTerm) {
    var showRelated = document.createElement("script");
    showRelated.type = "text/javascript";
    showRelated.src = "http://gdata.youtube.com/feeds/videos?vq=" + QueryTerm + "&max-results=1&alt=json-in-script&callback=LooplyLoad";
    document.getElementById('callbackjs').appendChild(showRelated);
}
function LooplyLoad(data) {
    document.location = "/v/" + getVideoId(data.feed.entry[0].link[0].href);
}
function AltFocus() {
    document.getElementById('txtInput').focus();
}
function translateText(response) {
        document.getElementById("TranslatedSongLyric").innerHTML += " " + response.data.translations[0].translatedText;
}
function translateTitle(response) {
    document.getElementById("TranslatedTitle").innerHTML += response.data.translations[0].translatedText;
}

// bad implementation
function sleep(milliSeconds){
	var startTime = new Date().getTime(); // get the current time
	while (new Date().getTime() < startTime + milliSeconds); // hog cpu
}
function set_cookie(name, value) {
    var cookie_string = name + "=" + escape(value);
    var expires = new Date();
    expires.setDate(expires.getDate() + 18000000);
    cookie_string += "; expires=" + expires.toGMTString();
    document.cookie = cookie_string;
}
function get_cookie(cookie_name) {
    var results = document.cookie.match('(^|;) ?' + cookie_name + '=([^;]*)(;|$)');

    if (results)
        return (unescape(results[2]));
    else
        return null;
}

function LanguageOnChange() {
    var SelectedLanguage = document.getElementById('TranslateLanguage').value;
    set_cookie("ChosenLanguage", SelectedLanguage);
    Translate();
}
function Select(value) {
    var id = document.getElementById(value).getAttribute('title');
    document.getElementById('TranslateLanguage').selectedIndex = parseInt(id);
}

function ChangeToSavedLanguage() {
    if (get_cookie("ChosenLanguage") != null) {
        var value = get_cookie("ChosenLanguage");
        if (document.getElementById('TranslateLanguage').value != value)
            Select(value);
    }
    else
        Select('de');
    //end set cookie
}
function Translate() {
    var TranslateAPI = ["AIzaSyCJvrXleqCXy5cZ5mCy9ASbLHc4t7QwpB0", "AIzaSyAKclW1ypE1xgsxO98ZkIlkWcFp3l7jmFs", "AIzaSyDwI0GkaWeea1rkeIG0eXvtAf_ZcqMhIrw", "AIzaSyDFjnZMnNPc5h5lp4HTPT65Xdrr1vVKAn8", "AIzaSyDP_18moODgOWSW1Kj5ae_XRp0fbpIkE2Q"];
    var randomnumber = Math.floor(Math.random() * (TranslateAPI.length + 1));
    var ChosenAPIKey = TranslateAPI[randomnumber];
    document.getElementById("TranslatedSongLyric").innerHTML = "";
    document.getElementById("TranslatedTitle").innerHTML = "";
    var TranslateLanguage = document.getElementById("TranslateLanguage").value;
    var TranslateTitle = document.getElementById('lbSongName').innerHTML;
    
    // translate song title
    var TitleTranslate = document.createElement('script');
    TitleTranslate.type = 'text/javascript';
    var source = 'https://www.googleapis.com/language/translate/v2?key='+ChosenAPIKey+'&target=' + TranslateLanguage + '&callback=translateTitle&q=' + encodeURI(TranslateTitle);
    TitleTranslate.src = source;
    document.getElementsByTagName('head')[0].appendChild(TitleTranslate);

    var SongLyricSplit = document.getElementById("SongLyric").innerHTML.split(" ");
    var counter = 0;
    var Paragraph = "";
    for (var i = 0; i < SongLyricSplit.length; i++) {
        if (i == 0) Paragraph += SongLyricSplit[i];
        else Paragraph += " " + SongLyricSplit[i];
        counter++;
        if (((counter % 90) == 0) || (counter == SongLyricSplit.length - 1)) {
            
            var Transcript = document.createElement('script');
            Transcript.type = 'text/javascript';
            Transcript.src = 'https://www.googleapis.com/language/translate/v2?key=' + ChosenAPIKey + '&target=' + TranslateLanguage + '&callback=translateText&q=' + encodeURI(Paragraph);
            document.getElementsByTagName('head')[0].appendChild(Transcript);
            Paragraph = "";
            sleep(500);
        }
    }
}

function HideModalPopup() {
    document.getElementById('ToHide').style.display = "none";
}
