var url = "http://en.wikipedia.org/wiki/";
var searchBox = $('#searchBox');
var currentSearch = '';
var currentSuggestion = '';
var hashTimeout = false;
searchBox.keyup(doInstantSearch);

function doInstantSearch() {
var keyword = searchBox.val();
if (keyword == currentSearch) {
return;
}

currentSearch = keyword;
alert("hey");
if (keyword == '') {
updateHash('');
currentSuggestion = '';
updateKeyword('<strong>Search Wikipedia Instantly</strong>');
return;
}

var the_url = url + encodeURIComponent(keyword);
$.ajax({type:"GET",url:the_url,dataType:"jsonp",success:function(responseData,textStatus,XMLHttpRequest) {
if (responseData) {
var article = responseData;
updateArticle(article);
} else {
updateKeyword('No results for "' + keyword + '"');
}
}
});
}

function updateHTML(elmId,value) {
document.getElementById(elmId).innerHTML = value;
}

function updateKeyword(keyword) {
updateHTML('searchTermKeyword',keyword);
}

function updateHash(hash) {
var timeDelay = 1000;

if (hashTimeout) {
clearTimeout(hashTimeout);
}

hashTimeout = setTimeout(function() {
window.location.replace("#" + encodeURI(hash))

$('#linkUrl').val(window.location);

if (currentSuggestion != '') {
document.title = '"' + currentSuggestion.toTitleCase() + '" on Wikipedia Instant!';
} else {
document.title = 'Wikipedia Instant - Real-time Wikipedia article surfing.';
}
},timeDelay);
}

function getHash() {
return decodeURIComponent(window.location.hash.substring(1));
}

if (window.location.hash) {
$('#searchBox').val(getHash()).focus();
} else {
var defaultSearches = ['YouTube','AutoTune','Rihanna','Far East Movement','Glee Cast','Nelly','Usher','Katy Perry','Taio Cruz','Eminem','Shakira','Kesha','B.o.B.','Taylor Swift','Akon','Bon Jovi','Michael Jackson','Lady Gaga','Paramore','Jay Z','My Chemical Romance','The Beatles','Led Zepplin','Guns N Roses','AC DC','System of a Down','Aerosmith','Tetris','8-bit','Borat','Basshunter','Fallout Boy','Blink 182','Pink Floyd','Still Alive','Men at Work','MGMT','Justin Bieber','The Killers','Bed intruder song','Baba O Riley','Billy Joel','Drake','Jay Sean'];
var randomNumber = Math.floor(Math.random() * defaultSearches.length);
$('#searchBox').val(defaultSearches[randomNumber]).select().focus();
}

function updateArticle(videos) {

}

function getResult(keyword) {
var the_url = url + encodeURIComponent(keyword);

$.ajax({type:"GET",url:the_url,dataType:"jsonp",success:function(responseData,textStatus,XMLHttpRequest) {
if (responseData) {
var article = responseData;
updateArticle(article);
} else {
updateKeyword('No results for "' + keyword + '"');
}
}
});
}

String.prototype.toTitleCase = function() {
return this.replace(/([\w&`'‘’"“.@:\/\{\(\[<>_]+-? *)/g,function(match,p1,index,title) {
if (index > 0 && title.charAt(index - 2) !== ":" && match.search(/^(a(nd?|s|t)?|b(ut|y)|en|for|i[fn]|o[fnr]|t(he|o)|vs?\.?|via)[ \-]/i) > -1)
return match.toLowerCase();

if (title.substring(index - 1,index + 1).search(/['"_{(\[]/) > -1)
return match.charAt(0) + match.charAt(1).toUpperCase() + match.substr(2);

if (match.substr(1).search(/[A-Z]+|&|[\w]+[._][\w]+/) > -1 || title.substring(index-1,index + 1).search(/[\])}]/) > -1)
return match;

return match.charAt(0).toUpperCase() + match.substr(1);
});
};