/* Check that search box has something typed into it before submitting. */
function checkSearch() {
  if (document.getElementById && document.getElementById('mainsearch').value == "") {
    alert("First enter some words to search for, yo.");
    document.getElementById('mainsearch').focus();
    return false;
  } else {
    return true;
  }
}

/* Add to Favorites button in IE, or instruction for other browsers. */
function faves() {
   var faver = document.getElementById('faver');
   if (document.all && navigator.userAgent.indexOf("Opera") == -1) {
       faver.innerHTML = "<a href=\"javascript:window.external.addfavorite('http://www.yourhtmlsource.com','HTML Source: HTML Tutorials');\" class=\"nav\">Add HTML Source to your favorites!</a>";
   } else {
       if (navigator.appVersion.toLowerCase().indexOf("mac") != -1) {    
       faver.innerHTML = "Press &#8984;D to add HTML Source to your bookmarks!";
   } else {
       faver.innerHTML = "Press Ctrl+D to add HTML Source to your bookmarks!";
   }
 }
}
   
/* Install into search bar for capable browsers, or return. */
function installSearchEngine(e) {
 if (window.external && ("AddSearchProvider" in window.external)) {
   // Firefox 2 and IE 7, OpenSearch
   window.external.AddSearchProvider('http://www.yourhtmlsource.com/htmlsource-search.xml')
 } else {
   // No search engine support (IE 6, Opera, etc).
   alert("Sorry, your browser doesn't support this. Search engines are supported by Firefox 2 onwards and Internet Explorer 7 onwards.");
 }
 createCookie('fxsearch', '1', 365);
 
 knackerEvent(e);
}

// Kills an event's propagation and default action
function knackerEvent(e) {
  if (window.event && window.event.cancelBubble)
    window.event.cancelBubble = true;
  if (e && e.stopPropagation)
    e.stopPropagation();
    
  if (window.event && window.event.returnValue)
    window.event.returnValue = false;
  if (e && e.preventDefault)
    e.preventDefault();
}

function createCookie(name,value,days) {
  if (days) {
    var date = new Date();
    date.setTime(date.getTime()+(days*24*60*60*1000));
    var expires = "; expires="+date.toGMTString();
  }
  else expires = "";
  document.cookie = name+"="+value+expires+"; path=/";
}

function readCookie(name) {
  var nameEQ = name + "=";
  var ca = document.cookie.split(';');
  for(var i=0;i < ca.length;i++) {
    var c = ca[i];
    while (c.charAt(0)==' ') c = c.substring(1,c.length);
    if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
  }
  return null;
}

/**
 * Move the AdSense skyscraper ad into the right gutter, between the wrapper
 * edge and the scrollbar.
 
position_adsense_skyscraper = function() {
  var viewportwidth = YAHOO.util.Dom.getViewportWidth();
  var usefulwidth = Math.ceil((viewportwidth - 780) / 15) - 8;
  var adblock = YAHOO.util.Dom.get('adsense-side');

  // Only move the ad into the side gutter if we have space. 
  if (viewportwidth > 1160) {
    YAHOO.util.Dom.setStyle(adblock, 'position', 'fixed');
    YAHOO.util.Dom.setStyle(adblock, 'top', '90px');
    YAHOO.util.Dom.setStyle(adblock, 'right', usefulwidth + 'px');
  } else {
    YAHOO.util.Dom.setStyle(adblock, 'position', 'static');
  }
}

YAHOO.util.Event.on(window, 'load', position_adsense_skyscraper, null, false);
YAHOO.util.Event.on(window, 'resize', position_adsense_skyscraper, null, false);

*/

window.onLoad = function() {
    window.defaultStatus="HTML Source: HTML Tutorials, just for you.";
}

$(document).ready(function () {
    faves();
});