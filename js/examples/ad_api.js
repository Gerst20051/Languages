if(typeof(window[ 'qwapi' ]) == "undefined") {
  qwapi = {};
}

if(typeof(window['qwapi.http']) == "undefined") {
  qwapi.http = {
    setCookie : function(name, value, expires) {
      // set time, it's in milliseconds
      var today = new Date();
      today.setTime(today.getTime());

      /*
        if the expires variable is set, make the correct
        expires time, the current script below will set
        it for x number of days, to make it for hours,
        delete * 24, for minutes, delete * 60 * 24
      */
      if(expires) {
        expires = expires * 1000 * 60;
      }
      var expires_date = new Date( today.getTime() + (expires) );
      var cookieString = name + "=" +escape(value)
      +
      ((expires) ? ";expires=" + expires_date.toGMTString() : "");

      document.cookie = cookieString;
    
      var newCookie = document.cookie;
      var i = 0;
    },
    getCookie : function(check_name) {  
      // first we'll split this cookie up into name/value pairs
      // note: document.cookie only returns name=value, not the other components
      var a_all_cookies = document.cookie.split( ';' );
      var a_temp_cookie = '';
      var cookie_name = '';
      var cookie_value = '';
      var b_cookie_found = false; // set boolean t/f default f

      for (i = 0; i < a_all_cookies.length; i++) {
        // now we'll split apart each name=value pair
        a_temp_cookie = a_all_cookies[i].split( '=' );
        // and trim left/right whitespace while we're at it
        cookie_name = a_temp_cookie[0].replace(/^\s+|\s+$/g, '');

        // if the extracted name matches passed check_name
        if(cookie_name == check_name) {
          b_cookie_found = true;
          // we need to handle case where cookie has no value but exists (no = sign, that is):
          if(a_temp_cookie.length > 1) {
            cookie_value = unescape(a_temp_cookie[1].replace(/^\s+|\s+$/g, ''));
          }
          // note that in cases where cookie is initialized but no value, null is returned
          return cookie_value;
          break;
        }
        a_temp_cookie = null;
        cookie_name = '';
      }
      if(!b_cookie_found) {
        return null;
      }  
    },
    deleteCookie : function(name) {
      if(qwapi.http.getCookie(name)) {
        document.cookie = name + "=" +
          ";expires=Thu, 01-Jan-1970 00:00:01 GMT";
       }
    }  
  };
}// Ad Client Functions --------------------
function QWAdClient() {
  var rand = Math.floor(Math.random()*1000000);
  this.scriptId = "ScriptTagID" + rand;
  this.divId = "ad" + rand;

  this.renderAd = function (url) {
    document.write('<div align="center" id="' + this.divId + '"></div>');
    document.write("<scr" + "ipt type='text/javascript' src='" + url + "'></scr" + "ipt>"); 
  },
  this.buildRequestURL = function (url, params) {
    // qw ad request url
    var qw_request = url + "?tag=" + this.divId;

    for (i in params) {
      qw_request += "&" + i + "=" + escape(params[i]);
    }
    
    // Interstitial cookie has been set so lets add a query param.
    var qw_cookie = qwapi.http.getCookie('qw-interstitial');
    if(qw_cookie) {
      // we found the cookie so we are going to add the request param.
      qw_request += "&iis=true";
    }
    var qw_jsenabled = qwapi.http.getCookie('x-quattro-jsenabled');
    if(qw_jsenabled) {
    	qw_request += "&jsenabled=true";
    }
    
    return qw_request;
  }
}

// Call Function --------------------
function qwAd(url,params){
  // note: commented this out as the browser will send the 
  // User-Agent header.  Ref defect [23160] for details. 
  //params.ua = navigator.userAgent; // User Agent
  params.code = "js2"; // Identifies source code type
  params.sas = "true";  
  var adClient = new QWAdClient();
  var adURL = adClient.buildRequestURL(url, params);
  adClient.renderAd(adURL);
}

function isValidAd(id) {
  	var el = document.getElementById(id);
  	if(!el) {
  	  return false;
  	}
  	var guts = el.innerHTML;
  	
  	if(guts.match(/message code \[[0-9]+\]/gim) ||
  	   guts.match(/<!-- no qualified ads found for this request -->/gim)) {
  		return false;
    }
  	return true;
}

function encodeParams(params) {
  var prms = "";
  for(i in params) {
     prms += "&" + i + "=" + escape(params[i]);
  }
  return prms;
}
