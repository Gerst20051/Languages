/*
* YTPlaylist.com - Instant YouTube Playlist
* Developed by Yumefave team [www.yumefave.com]
* Alex Ho (vnitus) | Steven Ton | Ngoc Tran
* This application utilizes YouTube API
* Contact: yumefave@gmail.com
*/
var ymfPlyr,ymfPlyrExt,ymfPl=[],ymfPlUrls=[],ymfInitItem="VlOvgDr-M3c",ymfInitTitle="YTPlaylist - Instant YouTube Playlist",ymfCurItem=ymfInitItem,ymfCurItemPreview=ymfInitItem,ymfCurBufItem=ymfInitItem,ymfCurItemInfo=[];ymfCurItemInfo.duration=0;ymfCurItemInfo.currentTime=0;ymfCurItemInfo.bytesTotal=0;ymfCurItemInfo.bytesLoaded=0;var ymfItemStatus=[];ymfItemStatus.play=false;ymfItemStatus.mute=false;ymfItemStatus.repeat=false;ymfItemStatus.shuffle=false;ymfItemStatus.resize=4;
ymfItemStatus.resize_ext=4;ymfItemStatus.lights=false;ymfItemStatus.reload=false;var ymfExternal=[];ymfExternal.share=true;ymfExternal.embed=false;var ymfItemsData=[],ymfItemsSearchData=[],ymfMode="playlist",ymfInstStIdx=1,ymfParams={allowScriptAccess:"always",allowFullScreen:"true"},ymfAttr={id:"ymf_yt_player",style:"position:absolute;"},ymfAttrExt={id:"ymf_yt_player_extra",style:"position:absolute;"},ymfHold=[];ymfHold.swfobject=false;ymfHold.yt_player=false;var ymfExp=[];ymfExp.yt_urls=false;
var ymfSugSearchVal,ymfSugCur=0,ymfSugExp=false,ymfSugCount=0,ymfInstOrderBy="relevance";ymfInit();function ymfInit(){ymfHold.swfobject=window.setInterval("ymfLoadSwfobject()",250);ymfHold.yt_player=window.setInterval("ymfLoadYTPlayer()",250);document.onmouseup=function(){if(ymfSugExp)ymfGetEl("suggest_area").innerHTML="";ymfSugExp=false}}function ymfLoadSwfobject(){if(typeof swfobject!="undefined"){window.clearInterval(ymfHold.swfobject);ymfStart()}}

function ymfLoadFirstLi(){
var a=ymfGetEl("ymf_item_"+ymfCurItem);if(a){window.clearInterval(ymfHold.first_li);var c=ymfGup("auto");a=ymfGetEl("ymf_item_"+ymfCurItem);a!=null&&a.setAttribute("class","playing");a=ymfItemsData[ymfCurItem].title;if(a!=null)ymfGetEl("headline_title").innerHTML=a;c=="true"&&ymfPlayVideo()}}function ymfLoadYTPlayer(){if(typeof ymfPlyr!="undefined"){window.clearInterval(ymfHold.yt_player);setInterval(ymfUpdatePlayerInfo,5E3)}}

function ymfStart(){
var a=ymfGup("pl"),c="",b=ymfReadCookie("ymf_pl"),d=ymfReadCookie("ymf_pl_session"),e=ymfReadCookie("ymf_i_session"),f=ymfGup("auto");if(a.length>=11)c="pl";else if(d&&document.referrer.indexOf("ytplaylist")>0){a=d.replace(/\|/g,";");c="session"}else if(b){a=b.replace(/\|/g,";");c="save"}else{ymfEraseCookie("ymf_pl_session");ymfEraseCookie("ymf_i_session")}if(b){ymfGetEl("ymf_btn_save").style.color="#0033CC";ymfGetEl("ymf_btn_save").value="Unsave";ymfGetEl("ymf_btn_save").title=
"Clear the playlist for the next visit"}if(c!=""){b=[];b=a.split(";");ymfCurItem=c=="pl"?b[0]:c=="session"?e!==null?e:b[0]:b[0];ymfCreateCookie("ymf_i_session",ymfCurItem,7);for(var g in b)ymfPl.push(b[g]);for(g in b)ymfAddItem(b[g],"init");ymfGetEl("headline_title").innerHTML="Loading...";if(f=="true")ymfGetEl("share_autoplay").checked=true;ymfHold.first_li=window.setInterval("ymfLoadFirstLi()",250)}else{ymfGetEl("headline_title").innerHTML=ymfInitTitle;ymfSwitchSideBar("tab_search_results");ymfSearchYT("most_popular")}ymfEmbed()}

function ymfEmbed(){
swfobject.embedSWF("http://www.youtube.com/v/"+ymfCurItem+"?enablejsapi=1&playerapiid=ymfPlyr&fs=1&rel=0&showsearch=0&showinfo=0&iv_load_policy=3","ymf_yt_player_div","640","390","8",null,null,ymfParams,ymfAttr);swfobject.embedSWF("http://www.youtube.com/v/"+ymfCurItem+"?enablejsapi=1&playerapiid=ymfPlyrExt&fs=1&rel=0&showsearch=0&showinfo=0&iv_load_policy=3","ymf_yt_player_extra_div","0","0","8",null,null,ymfParams,ymfAttrExt)}

function onYouTubePlayerReady(a){
if(a=="ymfPlyr"){if(!ymfPlyr||ymfItemStatus.reload){if(ymfItemStatus.reload)ymfItemStatus.reload=false;ymfPlyr=ymfGetEl("ymf_yt_player");ymfPlyr.addEventListener("onStateChange","ymfOnytplayerStateChange");ymfPlyr.addEventListener("onError","ymfOnPlayerError")}}else if(a=="ymfPlyrExt")if(!ymfPlyrExt){ymfPlyrExt=ymfGetEl("ymf_yt_player_extra");ymfPlyrExt.addEventListener("onStateChange","ymfOnytplayerStateChangeExt")}}

function ymfOnPlayerError(a){
a==100&&ymfNextItem()}function ymfOnytplayerStateChange(a){a==0&&ymfPl.length>0&&ymfNextItem();if(a==1){ymfItemStatus.play=true;ymfSetEl("ymf_btn_play","Pause")}else{ymfItemStatus.play=false;ymfSetEl("ymf_btn_play","Play")}}function ymfOnytplayerStateChangeExt(a){ymfMode=="playlist"&&a==1&&ymfPauseVideoPreview()}

function ymfUpdatePlayerInfo(){
var a=ymfCurItemInfo.duration,c=ymfCurItemInfo.currentTime,b=ymfCurItemInfo.bytesTotal,d=ymfCurItemInfo.bytesLoaded;if(ymfPlyr&&ymfPlyr.getDuration){if(a<=0)ymfCurItemInfo.duration=ymfPlyr.getDuration();ymfCurItemInfo.currentTime=ymfPlyr.getCurrentTime();if(b<=0)ymfCurItemInfo.bytesTotal=ymfPlyr.getVideoBytesTotal();ymfCurItemInfo.bytesLoaded=ymfPlyr.getVideoBytesLoaded()}if(d==b&&d>0||c/a>=0.8&&c>0||ymfCurItem==ymfCurBufItem&&ymfCurItem!=ymfInitItem)if(ymfMode=="playlist"){a=
ymfGetNextItem(ymfCurItem);
if(a!==false&&a!=ymfCurBufItem&&a!=ymfInitItem)if(ymfPlyrExt){ymfPlyrExt.cueVideoById(a);ymfPlayVideoPreview();ymfPauseVideoPreview();ymfCurBufItem=a}}}

function ymfReadUrls(){
var a=ymfGetEl("ymf_yt_urls").getElementsByTagName("textarea")[0],c=a.value,b=false;if(c.length>11){var d=[];d=c.match(/v=([a-zA-Z0-9\-_]{11})(\&|\,|\n|\r|$)/gi);for(var e in d)if(isNaN(d[e])&&d[e].length<=14){c=d[e].substring(2,13);ymfPlUrls.push(c);ymfPl.join().indexOf(c)<0&&ymfPl.push(c);b=true}if(ymfPlUrls.length==0){ymfError("yt_urls_no_matches");return false}else b&&alert("Your YouTube videos have been successfully added to the playlist")}else{ymfError("yt_urls_invalid_input");
return false}a.value="";
for(e in ymfPlUrls){c=ymfPlUrls[e];c in ymfItemsData||ymfAddItem(c,"urls")}return false}

function ymfAddItem(a,c){
var b=ymfPl.join("|");ymfCreateCookie("ymf_pl_session",b,7);ymfUpdateSaveBtn();a in ymfItemsData||$.ajax({type:"GET",url:"http://gdata.youtube.com/feeds/api/videos/"+a+"?alt=jsonc&v=2",dataType:"jsonp",success:function(d){var e=d.data;d=e.title;var f=e.uploader,g=e.thumbnail.sqDefault,h=ymfAddCommasToNumber(e.viewCount);e=ymfConvertTime(e.duration);var j=[];j.title=d;j.author=f;j.thumbnail=g;j.views=h;j.duration=e;ymfItemsData[a]=j;if(ymfAssocArrLength(ymfItemsData)==ymfPl.length)if(c==
"init")ymfRenderPlaylist("init");
else c=="urls"?ymfRenderPlaylist("urls"):ymfRenderPlaylist(a)}})}

function ymfRenderPlaylist(a){
var c=ymfGetEl("playlist");
if(a=="init")for(var b in ymfPl){a=ymfPl[b];var d=ymfItemsData[a],e=document.createElement("li");e.setAttribute("id","ymf_item_"+a);e.innerHTML=ymfItemContent("playlist",a,d.title,d.views,d.author,d.duration,d.thumbnail);c.appendChild(e)}else if(a=="urls"){for(b in ymfPlUrls){a=ymfPlUrls[b];if(!ymfGetEl("ymf_item_"+a)){d=ymfItemsData[a];e=document.createElement("li");e.setAttribute("id","ymf_item_"+a);e.innerHTML=ymfItemContent("playlist",a,
d.title,d.views,d.author,d.duration,d.thumbnail);c.appendChild(e)}}ymfPlUrls=[]}else{a=a;d=ymfItemsData[a];e=document.createElement("li");e.setAttribute("id","ymf_item_"+a);e.innerHTML=ymfItemContent("playlist",a,d.title,d.views,d.author,d.duration,d.thumbnail);c.appendChild(e)}ymfShareURL();ymfEmbedText();$("#tab_playlist a").text("Playlist")}

function ymfShareURL(){
var a="http://www.ytplaylist.com/";if(ymfPl.length>0){a+="?pl="+ymfPl.join(";");if(ymfGetEl("share_autoplay").checked)a+="&auto=true";ymfShortenURL(a)}else ymfShareUpdate(a)}

function ymfShortenURL(a){
(function(c){var b={version:"2.0.1",login:"yumefave",apiKey:"R_917975cea6a1da3fde40dc446adf32e4",history:"1",longUrl:encodeURIComponent(a)};c.getJSON("http://api.bit.ly/shorten?version="+b.version+"&longUrl="+b.longUrl+"&login="+b.login+"&apiKey="+b.apiKey+"&history="+b.history+"&format=json&callback=?",function(d){ymfShareUpdate(d.results[a].shortUrl)})})(jQuery)}

function ymfShareUpdate(a){
ymfGetEl("share_url").value=a.indexOf("bit.ly")>0?a+"#YTPlaylist.com":a;$("#fb-share").attr("share_url",a);$("#fb-share").attr("href","http://www.facebook.com/sharer.php?u="+encodeURIComponent(a)+"&src=sp");var c=$('iframe[class="twitter-share-button twitter-count-horizontal"]'),b=c.attr("src");b=b.substr(0,b.indexOf("&url="))+"&url="+encodeURIComponent(a);c.attr("src",b)}function ymfSocialBookmark(){if(ymfPl.length>0){ymfPl.join(";");ymfGetEl("share_autoplay")}}

function ymfEmbedText(){
var a="",c="";c="false";a+=ymfPl.join(";");if(ymfGetEl("embed_autoplay").checked)c="true";c="<script src=\"http://ytplaylist.com/js/ytp_embed.js\"><\/script>\n<script>\nvar ymf_embed = new ymfEmbed({id:'ymf_embed',playlist:'"+a+"',auto:"+c+"});\nymf_embed.init();\n<\/script>";ymfGetEl("embed_text").value=c}

function ymfSwitchMode(a){
ymfMode=a;
if(ymfMode=="preview"){
if(ymfPlyr){ymfPlyr.getPlayerState()==1&&ymfPauseVideo();ymfPlyr.setSize(0,0);ymfGetEl("ymf_yt_player").style.width="0px";ymfGetEl("ymf_yt_player").style.height="0px"}if(ymfPlyrExt){ymfPlyrExt.setSize(640,390);ymfGetEl("ymf_yt_player_extra").style.width="640px";ymfGetEl("ymf_yt_player_extra").style.height="390px"}}else{if(ymfPlyrExt){ymfPlyrExt.getPlayerState()==1&&ymfPauseVideoPreview();ymfPlyrExt.setSize(0,0);ymfGetEl("ymf_yt_player_extra").style.width=
"0px";ymfGetEl("ymf_yt_player_extra").style.height="0px"}
if(ymfPlyr){ymfPlyr.setSize(640,390);ymfGetEl("ymf_yt_player").style.width="640px";ymfGetEl("ymf_yt_player").style.height="390px"}}}

function ymfSwitchVideo(a){
if(ymfMode=="preview"){ymfGetEl("ymf_item_preview_"+ymfCurItemPreview)!==null&&ymfGetEl("ymf_item_preview_"+ymfCurItemPreview).removeAttribute("class");ymfGetEl("ymf_item_preview_"+a).setAttribute("class","playing");ymfGetEl("headline_title").innerHTML=ymfItemsSearchData[a].title;ymfPlayVideoPreview();ymfCurItemPreview=a}else{ymfPlyr&&ymfPlyr.cueVideoById(a);ymfGetEl("ymf_item_"+ymfCurItem)!==null&&ymfGetEl("ymf_item_"+ymfCurItem).removeAttribute("class");ymfGetEl("ymf_item_"+
a).setAttribute("class","playing");
ymfGetEl("headline_title").innerHTML=ymfItemsData[a].title;ymfCurItem=a;ymfCreateCookie("ymf_i_session",a,7);if(ymfCurItem==ymfCurItemPreview){ymfGetEl("ymf_item_preview_"+ymfCurItemPreview)!==null&&ymfGetEl("ymf_item_preview_"+ymfCurItemPreview).removeAttribute("class");ymfCurItemPreview=ymfInitItem;ymfPlyrExt&&ymfPlyrExt.cueVideoById(ymfCurItemPreview)}ymfPlayVideo();ymfCurItemInfo.duration=0;ymfCurItemInfo.currentTime=0;ymfCurItemInfo.bytesTotal=0;ymfCurItemInfo.bytesLoaded=
0}}

function ymfPlayVideo(){
ymfItemStatus.resize=4;ymfPlyr&&ymfPlyr.playVideo()}function ymfPlayVideoPreview(){ymfItemStatus.resize_ext=4;ymfPlyrExt&&ymfPlyrExt.playVideo()}function ymfPauseVideo(){ymfItemStatus.resize=4;ymfPlyr&&ymfPlyr.pauseVideo()}function ymfPauseVideoPreview(){ymfItemStatus.resize_ext=4;ymfPlyrExt&&ymfPlyrExt.pauseVideo()}function ymfMuteVideo(){ymfPlyr&&ymfPlyr.mute()}function ymfUnMuteVideo(){ymfPlyr&&ymfPlyr.unMute()}

function ymfGetPrevItem(a){
var c=false;
if(ymfPl.length==1)c=a;
else if(ymfPl.length>1)for(var b=0;b<ymfPl.length;b++)if(ymfPl[b]==a)c=b==0?ymfPl[ymfPl.length-1]:ymfPl[b-1];return c}function ymfPrevItem(){var a=ymfGetPrevItem(ymfCurItem);if(a!==false){ymfSwitchVideo(a);ymfCurItem=a;ymfCreateCookie("ymf_i_session",a,7)}else ymfError("no_prev_item")}

function ymfGetNextItem(a){
var c=false;if(ymfPl.length==1)c=ymfPl[0];else if(ymfPl.length>1){for(var b=0;b<ymfPl.length;b++)if(ymfPl[b]==a)c=b==ymfPl.length-1?ymfPl[0]:ymfPl[b+1];if(!c&&a==ymfInitItem)c=ymfPl[0]}return c}

function ymfNextItem(){
var a=$("#ymf_yt_player").attr("data"),c=$("#ymf_yt_player param[name=movie]").attr("value"),b=ymfItemStatus.shuffle?ymfPl[Math.floor(Math.random()*ymfPl.length)]:ymfItemStatus.repeat?ymfCurItem:ymfGetNextItem(ymfCurItem);if(b!==false){if(b==ymfCurItem)(a+c).indexOf("loop=1")<0&&ymfReloadSwf("ymf_yt_player","ymf_yt_player_div",b,{enablejsapi:1,playerapiid:"ymfPlyr",fs:1,rel:0,showsearch:0,showinfo:0,iv_load_policy:3,autoplay:1,loop:1},ymfAttr);else(a+c).indexOf("loop=1")>0&&
ymfReloadSwf("ymf_yt_player","ymf_yt_player_div",b,{enablejsapi:1,playerapiid:"ymfPlyr",fs:1,rel:0,showsearch:0,showinfo:0,iv_load_policy:3,autoplay:1},ymfAttr);ymfSwitchVideo(b)}else ymfError("no_next_item")}function ymfReloadSwf(a,c,b,d,e){var f=[],g="",h;for(h in d)f.push(h+"="+d[h]);g=f.join("&");ymfReplaceSwfWithEmptyDiv(a,c);swfobject.embedSWF("http://www.youtube.com/v/"+b+"?"+g,c,"640","390","8",null,null,ymfParams,e);ymfItemStatus.reload=true}

function ymfError(a){
switch(a){
case "yt_urls_no_matches":a="Your input does not contain any You Tube Video ID, please try again!";break;
case "yt_urls_invalid_input":a="Your input is invalid, please try again!";break;
case "no_prev_item":a="There is no previous item in the playlist to be played!";break;
case "no_next_item":a="There is no next item in the playlist to be played!";break;
case "empty_playlist":a="Your playlist is currently empty!";break;
case "preview_same_item":a="Please wait until this video finishes buffering since it is being played in the playlist!";
break;case "no_control_no_playlist":a="Sorry, this button can only be used in Playlist mode!"
}
alert(a);
return false
}

function ymfSwapContent(a,c){
var b=ymfGetEl("ymf_item_"+a),d=ymfGetEl("ymf_item_"+c);
b=b.innerHTML;
ymfSetEl("ymf_item_"+a,d.innerHTML);
ymfSetEl("ymf_item_"+c,b)
}

function ymfSwapEls(a,c){
var b=ymfGetEl("ymf_item_"+a),d=ymfGetEl("ymf_item_"+c);b.setAttribute("id","ymf_item_"+c);
d.setAttribute("id","ymf_item_"+a);ymfSwapContent(a,c);for(var e in ymfPl)ymfGetEl("ymf_item_"+ymfPl[e]).removeAttribute("class");if(ymfCurItem!=ymfInitItem)ymfGetEl("ymf_item_"+ymfCurItem).className="playing"}function ymfSwapPlEls(a,c){var b=ymfIndexOf(ymfPl,a),d=ymfIndexOf(ymfPl,c),e=ymfPl[b];ymfPl[b]=ymfPl[d];ymfPl[d]=e}

function ymfItemClick(a,c){
if(c=="preview")
if(a==ymfCurItem&&ymfCurItemInfo.bytesLoaded<ymfCurItemInfo.bytesTotal)ymfError("preview_same_item");
else{ymfSwitchMode("preview");
if(a==ymfCurItemPreview)
if(ymfPlyrExt&&ymfPlyrExt.getPlayerState()==1)ymfPauseVideoPreview();
else{ymfPlyrExt&&ymfPlyrExt.getPlayerState();ymfPlayVideoPreview()
}else{
if(ymfPlyrExt){ymfReloadSwf("ymf_yt_player_extra","ymf_yt_player_extra_div",a,{enablejsapi:1,playerapiid:"ymfPlyrExt",fs:1,rel:0,showsearch:0,showinfo:0,iv_load_policy:3,
autoplay:1},ymfAttrExt);ymfPlyrExt=null}ymfSwitchVideo(a)}}
else{ymfSwitchMode("playlist");
if(a==ymfCurItem)
if(ymfPlyr&&ymfPlyr.getPlayerState()==1)ymfPauseVideo();
else{ymfPlyr&&ymfPlyr.getPlayerState();ymfPlayVideo()}
else{ymfGetEl("headline_title").innerHTML=ymfItemsData[a].title;ymfSwitchVideo(a)}
}
}

function ymfMoveItem(a,c){
var b;b=ymfPl.length;
if(b>1){
var d=ymfIndexOf(ymfPl,a);b=c=="down"?d==b-1?ymfPl[0]:ymfGetNextItem(a):d==0?ymfPl[b-1]:ymfGetPrevItem(a);ymfSwapEls(a,b);ymfSwapPlEls(a,b)}ymfShareURL();ymfUpdateSaveBtn()}

function ymfRemoveItem(a){
var c=ymfGetNextItem(ymfCurItem),b=ymfIndexOf(ymfPl,a);
if(b!=-1){ymfPl.splice(b,1);b=ymfPl.join("|");ymfCreateCookie("ymf_pl_session",b,7);ymfUpdateSaveBtn()}
delete ymfItemsData[a];ymfRemoveEl("ymf_item_"+a);
if(ymfGetEl("ymf_item_preview_"+a))ymfGetEl("ymf_item_preview_"+a).getElementsByTagName("div")[0].innerHTML='<input type="button" id="ymf_btn_add" class="ymf_btns" value="" title="Add to playlist" onclick="ymfBtnAdd(\''+a+"')\">";ymfShareURL();ymfPl.length>0&&c!==false&&
c!=ymfInitItem&&a==ymfCurItem&&ymfSwitchVideo(c)}
function ymfUpdateSaveBtn(){
var a=ymfReadCookie("ymf_pl"),c=ymfPl.join("|"),b=ymfGetEl("ymf_btn_save");
if(a&&b.value=="Unsave"&&c!=a){b.style.color="#333";b.value="Save";b.title="Update your modified playlist for the next visit"}}

function ymfBtnAdd(a){
$("#tab_playlist a").html('<img src="img/ytp_loader.gif"/> Adding...');ymfGetEl("ymf_item_preview_"+a).getElementsByTagName("div")[0].innerHTML='<div id="ymf_btn_checked" class="ymf_btns" alt="" title="This item is already in the playlist"></div>';ymfPl.push(a);ymfAddItem(a)}
function ymfBtnPrev(){ymfMode=="playlist"?ymfPrevItem():ymfError("no_control_no_playlist")}

function ymfBtnPlay(){
if(ymfMode=="playlist"){
ymfGetEl("ymf_btn_play").innerHTML=="Play"?ymfSetEl("ymf_btn_play","Pause"):ymfSetEl("ymf_btn_play","Play");
if(ymfItemStatus.play){ymfItemStatus.play=false;ymfPauseVideo()}else{ymfItemStatus.play=true;ymfPlayVideo()}}
else ymfError("no_control_no_playlist")
}

function ymfBtnNext(){
if(ymfMode=="playlist"){ymfItemStatus.repeat&&ymfBtnRepeat();ymfNextItem()}
else ymfError("no_control_no_playlist")
}

function ymfBtnMute(){
if(ymfMode=="playlist")
if(ymfItemStatus.mute){
ymfItemStatus.mute=false;ymfGetEl("ymf_btn_mute").removeAttribute("class");ymfUnMuteVideo()}else{ymfItemStatus.mute=true;ymfGetEl("ymf_btn_mute").setAttribute("class","active");ymfMuteVideo()}else ymfError("no_control_no_playlist")}

function ymfBtnRepeat(){
if(ymfMode=="playlist")
if(ymfItemStatus.repeat){
ymfItemStatus.repeat=false;ymfGetEl("ymf_btn_repeat").removeAttribute("class")}else{ymfItemStatus.repeat=true;ymfGetEl("ymf_btn_repeat").setAttribute("class","active");if(ymfItemStatus.shuffle){ymfItemStatus.shuffle=false;ymfGetEl("ymf_btn_shuffle").removeAttribute("class")}}else ymfError("no_control_no_playlist")}

function ymfBtnShuffle(){
if(ymfMode=="playlist")
if(ymfItemStatus.shuffle){
ymfItemStatus.shuffle=false;
ymfGetEl("ymf_btn_shuffle").removeAttribute("class")
}else{
ymfItemStatus.shuffle=true;
ymfGetEl("ymf_btn_shuffle").setAttribute("class","active");
if(ymfItemStatus.repeat){
ymfItemStatus.repeat=false;
ymfGetEl("ymf_btn_repeat").removeAttribute("class")}
}else ymfError("no_control_no_playlist")}

function ymfBtnShare(){
if(ymfExternal.share){
ymfExternal.share=false;
ymfGetEl("share_area").style.display="none"
}else{
ymfShareURL();ymfExternal.share=true;ymfGetEl("share_area").style.display="block";ymfExternal.embed=false;ymfGetEl("embed_area").style.display="none"
}
}

function ymfBtnEmbed(){
if(ymfExternal.embed){ymfExternal.embed=false;ymfGetEl("embed_area").style.display="none"}
else if(ymfPl.length>0){ymfExternal.embed=true;ymfGetEl("embed_area").style.display="block";ymfEmbedText();ymfExternal.share=false;ymfGetEl("share_area").style.display="none"}
else ymfError("empty_playlist")
}

function ymfBtnResize(){
if(ymfMode=="playlist")
switch(ymfItemStatus.resize){
case 4:ymfItemStatus.resize=3;ymfPlyr.setSize(480,293);break;
case 3:ymfItemStatus.resize=2;ymfPlyr.setSize(320,195);break;
case 2:ymfItemStatus.resize=1;ymfPlyr.setSize(640,25);break;
case 1:ymfItemStatus.resize=4;ymfPlyr.setSize(640,390)
}else switch(ymfItemStatus.resize_ext){
case 4:ymfItemStatus.resize_ext=3;ymfPlyrExt.setSize(480,293);break;
case 3:ymfItemStatus.resize_ext=2;ymfPlyrExt.setSize(320,195);break;
case 2:ymfItemStatus.resize_ext=
1;ymfPlyrExt.setSize(640,25);break;
case 1:ymfItemStatus.resize_ext=4;ymfPlyrExt.setSize(640,390)
}
}

function ymfBtnLights(){
var a=["#tabs .tab","#yt_video_urls textarea","#search_box input","#share_url","#embed_text","#external_area"];if(ymfItemStatus.lights){document.getElementById("ymf_btn_lights_on").id="ymf_btn_lights_off";document.body.style.backgroundColor="#FFFFFF";for(c in a)$(a[c]).css("background-color","#FFFFFF");ymfItemStatus.lights=false}else{document.getElementById("ymf_btn_lights_off").id="ymf_btn_lights_on";document.body.style.backgroundColor="#111111";for(var c in a)$(a[c]).css("background-color",
"#111111");ymfItemStatus.lights=true}}

function ymfBtnSave(){
var a=ymfGetEl("ymf_btn_save");
if(a.value=="Save"&&ymfPl.length>0){
a.style.color="#0033CC";a.value="Unsave";a.title="Clear the playlist for the next visit";
a=ymfPl.join("|");
ymfCreateCookie("ymf_pl",a,7);
alert("Your playlist has been saved for the next visit")}
else if(a.value=="Save"&&ymfPl.length==0)alert("Nothing has been saved since your playlist is currently empty.");
else{a.style.color="#333";a.value="Save";a.title="Save the playlist for the next visit";ymfEraseCookie("ymf_pl");
alert("Your playlist has been cleared from the next visit")}}

function ymfInstSearch(){
var a="s";
if(ymfGetEl("search_area").value.length>0)ymfSearchYT("search");
else if(ymfPl.length>0)a="p";
else ymfSearchYT("most_popular");
if(a=="s"){ymfSwitchSideBar("tab_search_results");ymfGetEl("search_results").scrollTop=0}
else ymfSwitchSideBar("tab_playlist")}

function ymfSearchYT(a){
var c=ymfGetEl("search_area").value;
if(c.length>0)c=c.replace(/^\s+|\s+$/g,"").replace(/[\s]+/g,"+");var b=encodeURIComponent(c);if(a=="more")ymfInstStIdx+=10;else ymfInstStIdx=1;a=="search"&&$("#tab_search_results a").html('<img src="img/ytp_loader.gif"/> Searching...');b="http://gdata.youtube.com/feeds/api/videos?q="+b+"&format=5&start-index="+ymfInstStIdx+"&max-results=10&orderby="+ymfInstOrderBy+"&v=2&alt=jsonc";if(a=="most_popular")b="http://gdata.youtube.com/feeds/api/standardfeeds/most_popular?time=today&format=5&start-index="+
ymfInstStIdx+"&max-results=10&orderby=relevance&v=2&alt=jsonc";var d=ymfGetEl("search_results");$.ajax({type:"GET",url:b,dataType:"jsonp",success:function(e){if(a=="more")ymfRemoveEl("ymf_more_results");else d.innerHTML="";if(e.data.items){$.each(e.data.items,function(f,g){var h=g.id,j=g.title,l=g.uploader,m=g.thumbnail.sqDefault,n=ymfAddCommasToNumber(g.viewCount),o=ymfConvertTime(g.duration),k=document.createElement("li");k.setAttribute("id","ymf_item_preview_"+h);k.innerHTML=ymfItemContent("preview",
h,j,n,l,o,m);d.appendChild(k);
if(!(h in ymfItemsSearchData)){k=[];k.title=j;k.author=l;k.thumbnail=m;k.views=n;k.duration=o;ymfItemsSearchData[h]=k}});if(a!="most_popular"&&e.data.startIndex+e.data.itemsPerPage<e.data.totalItems){e=document.createElement("li");e.setAttribute("id","ymf_more_results");e.innerHTML="<a href=\"javascript:ymfSearchYT('more');\">More results...</a>";d.appendChild(e)}}else{e=document.createElement("li");e.setAttribute("id","ymf_no_results");e.innerHTML='No video results for "<b>'+
c+'</b>"';d.appendChild(e)}$("#tab_search_results a").text("Search Results")}})}

function ymfSwitchSideBar(a){
var c="";if(a=="tab_playlist")c="tab_search_results";else{c="tab_playlist";ymfGetEl("search_area").value.length<=0&&ymfSearchYT("most_popular")}ymfGetEl(a).style.borderBottom="1px solid #FFF";ymfGetEl(c).style.borderBottom="1px solid #D3D3D3";ymfGetEl(a.substr(4)).style.display="block";ymfGetEl(c.substr(4)).style.display="none"}

function ymfSuggestion(a){
var c;if(!a)a=window.event;if(a.keyCode)c=a.keyCode;else if(a.which)c=a.which;if(c==13||c==38||c==40)return true;inp=ymfGetEl("search_area").value;if(inp.length>0)inp=inp.replace(/^\s+|\s+$/g,"");if(inp.length<1){ymfGetEl("suggest_area").innerHTML="";return true}inp=encodeURIComponent(inp);$.ajax({type:"GET",url:"http://suggestqueries.google.com/complete/search?hl=en&ds=yt&client=ytplaylist.com&hjson=t&q="+inp,dataType:"jsonp",success:function(b){b=b[1];var d;if(b.length>
0&&ymfGetEl("search_area").value.length>0){for(var e='<ul id="select_suggest">',f=0;b.length>0;){f++;d=b.shift();e+='<li id="suggest_'+f+'" value="'+d[0]+'" onmouseover="ymfSugHover(\''+f+"');\" onmousedown=\"ymfSugApply('"+f+"');\">"+d[0]+"</li>"}e+="</ul>";ymfGetEl("suggest_area").innerHTML=e;e=ymfGetEl("select_suggest");if(f<=4){e.style.height=18*f+"px";ymfSugCount=f}else{e.style.height="72px";ymfSugCount=4}}}})}

function ymfSugHover(a){$('li[id^="suggest_"]').css("background-color","#FAFAFA");ymfGetEl("suggest_"+a).style.backgroundColor="#8DB2ED";ymfSugCur=a}

function ymfSugKeyPress(a){
var c=ymfGetEl("suggest_area");a=a?a:window.event;a=a.which?a.which:a.keyCode;if(a!=13&&a!=40&&a!=38){ymfSugCur=0;ymfSugExp=true}ymfGetEl("search_area").onkeydown=function(b){b=b?b:window.event;b=b.which?b.which:b.keyCode;if(b==13){c.innerHTML="";ymfInstSearch()}else if(b==40||b==38)if(c.innerHTML!=""){if(b==40)if(ymfSugCur<ymfSugCount&&ymfGetEl("suggest_"+(ymfSugCur+1))!=null)ymfSugCur++;else{if(ymfSugCur==ymfSugCount)ymfSugCur=0}else if(ymfSugCur>1)ymfSugCur--;else if(ymfSugCur==
1)ymfSugCur=0;else if(ymfSugCur==0&&ymfGetEl("suggest_"+ymfSugCount)!=null)ymfSugCur=ymfSugCount;if(ymfSugCur>0){$('li[id^="suggest_"]').css("background-color","#FAFAFA");ymfGetEl("suggest_"+ymfSugCur).style.backgroundColor="#8DB2ED";b=ymfGetEl("suggest_"+ymfSugCur).innerHTML;ymfGetEl("search_area").value=b}else{ymfGetEl("search_area").value=ymfSugSearchVal;$('li[id^="suggest_"]').css("background-color","#FAFAFA")}}};ymfGetEl("search_area").onkeyup=function(b){b=b?b:window.event;var d=b.which?b.which:
b.keyCode;if(d!=40&&d!=38){ymfSugSearchVal=ymfGetEl("search_area").value;ymfInstSearch();ymfSuggestion(b)}}}

function ymfSugApply(a){a=ymfGetEl("suggest_"+a).innerHTML;ymfGetEl("search_area").value=a;ymfGetEl("suggest_area").innerHTML="";ymfInstSearch()}

function ymfItemContent(a,c,b,d,e,f,g){
var h="",j="";h=b.length>40?b.substring(0,b.substring(0,41).lastIndexOf(" "))+"...":b;
if(a=="preview"){
j+="<a href=\"javascript:ymfItemClick('"+c+"','preview')\">\t<p><img src=\""+g+'"/><span class="time">'+f+'</span></p>\t<p><span class="video_title" title="'+b+'">'+h+'</span>\t<span class="stat">by '+e+'</span>\t<span class="stat">'+ymfAddCommasToNumber(d)+" views</span></p></a>";h=ymfPl.join().indexOf(c)>=0?'<div id="ymf_btn_checked" class="ymf_btns" alt="" title="This item is already in the playlist"></div>':
'<input type="button" id="ymf_btn_add" class="ymf_btns" value="" title="Add to playlist" onclick="ymfBtnAdd(\''+c+"')\"/>";j+='<div class="item_buttons">'+h+"</div>"
}else{
j+="<a href=\"javascript:ymfItemClick('"+c+"','playlist')\">\t<p><img src=\""+g+'"/><span class="time">'+f+'</span></p>\t<p><span class="video_title" title="'+b+'">'+h+'</span>\t<span class="stat">by '+e+'</span>\t<span class="stat">'+ymfAddCommasToNumber(d)+" views</span></p></a>";j+='<div class="item_buttons"><input type="button" id="ymf_btn_up" class="ymf_btns" value="" title="Move up" onclick="ymfMoveItem(\''+
c+'\',\'up\')"/><input type="button" id="ymf_btn_down" class="ymf_btns" value="" title="Move down" onclick="ymfMoveItem(\''+c+'\',\'down\')"/><input type="button" id="ymf_btn_remove" class="ymf_btns" value="" title="Remove from playlist" onclick="ymfRemoveItem(\''+c+"')\"/></div>"}j+='<div class="clear"></div>';
return j
}

function ymfExpand(a){
var c=ymfGetEl("ymf_"+a),b=ymfGetEl("ymf_expand_"+a);
if(ymfExp[a]){b.style.backgroundPosition="0px 0px";ymfExp[a]=false;c.style.display="none"}else{b.style.backgroundPosition="-14px 0px";ymfExp[a]=true;c.style.display="block"}}function ymfGetEl(a){return document.getElementById(a)}function ymfSetEl(a,c){ymfGetEl(a).innerHTML=c}function ymfRemoveEl(a){a=ymfGetEl(a);a.parentNode.removeChild(a)}

function ymfCreateEl(a,c){
var b=document.createElement(c);b.setAttribute("id",a);
return b}

function ymfCreateCookie(a,c,b){
if(b){
var d=new Date;d.setTime(d.getTime()+b*24*60*60*1E3);b="; expires="+d.toGMTString()}
else b="";document.cookie=a+"="+c+b+"; path=/"}
function ymfReadCookie(a){a=a+"=";
for(var c=document.cookie.split(";"),b=0;b<c.length;b++){
for(var d=c[b],e=0;d.charAt(e)==" ";)e++;d=d.substring(e,d.length);
if(d.indexOf(a)==0)return d.substring(a.length,d.length)
}
return null
}

function ymfEraseCookie(a){
ymfCreateCookie(a,"",-1)
}

function ymfReplaceSwfWithEmptyDiv(a,c){
var b=ymfGetEl(a);
if(b){
var d=document.createElement("div");b.parentNode.insertBefore(d,b);swfobject.removeSWF(a);d.setAttribute("id",c)}}

function ymfGup(a){
a=a.replace(/[\[]/,"\\[").replace(/[\]]/,"\\]");a=RegExp("[\\?&]"+a+"=([^&#]*)").exec(window.location.href);
return a==null?"":a[1]
}

function ymfIndexOf(a,c){
if(a.indexOf)return a.indexOf(c);
else{
for(var b=0;b<a.length;b++)if(a[b]==c)return b;
return-1
}
}

function ymfAddCommasToNumber(a){
a+="";x=a.split(".");x1=x[0];x2=x.length>1?"."+x[1]:"";
for(a=/(\d+)(\d{3})/;a.test(x1);)x1=x1.replace(a,"$1,$2");
return x1+x2
}

function ymfConvertTime(a){
var c=Math.floor(a/60);a=Math.floor(a-c*60);
if(a<10)a="0"+a;return c+":"+a
}

function ymfAssocArrLength(a){
var c=0;
for(i in a)c++;
return c;
}