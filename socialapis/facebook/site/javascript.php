<?php
session_start();
header("Content-Type: application/x-javascript");
?>
/* ---------------------------------------------------- */
/* ----------- >>>  Global Javascript  <<< ------------ */
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
/* Site Name:    Facebook
/* Site Creator: Andrew Gerst
/* Site Created: Tue, 07 Dec 2010 16:40:05 -0400
/* Last Updated: <?php echo date(r, filemtime('javascript.php')) . "\n"; ?>
<?php if (isset($_SESSION['logged']) && ($_SESSION['logged'] == 1)) { ?>/* Current User: <?php echo $_SESSION['fullname'] . "\n"; } ?>
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
/* ------------ >>>  Table of Contents  <<< ----------- */
/* ---------------------------------------------------- */
/* Item 1
/* ---------------------------------------------------- */

/* begin browser detect */

var BrowserDetect = {
	init: function () {
		this.browser = this.searchString(this.dataBrowser) || "An unknown browser";
		this.version = this.searchVersion(navigator.userAgent) || this.searchVersion(navigator.appVersion) || "an unknown version";
		this.OS = this.searchString(this.dataOS) || "an unknown OS";
		this.network = this.dataNetwork();
		this.mobile = this.dataMobile(navigator.userAgent||navigator.vendor||window.opera);
	},

	searchString: function (data) {
		for (var i = 0; i < data.length; i++) {
			var dataString = data[i].string;
			var dataProp = data[i].prop;
			this.versionSearchString = data[i].versionSearch || data[i].identity;

			if (dataString) {
				if (dataString.indexOf(data[i].subString) != -1) return data[i].identity;
			} else if (dataProp) return data[i].identity;
		}
	},

	searchVersion: function (dataString) {
		var index = dataString.indexOf(this.versionSearchString);
		if (index == -1) return;
		return parseFloat(dataString.substring(index + this.versionSearchString.length + 1));
	},

	dataBrowser: [
		{
			string: navigator.userAgent,
			subString: "Chrome",
			identity: "Chrome"
		},
		{
			string: navigator.userAgent,
			subString: "OmniWeb",
			versionSearch: "OmniWeb/",
			identity: "OmniWeb"
		},
		{
			string: navigator.vendor,
			subString: "Apple",
			identity: "Safari"
			// versionSearch: "Version"
		},
		{
			prop: window.opera,
			identity: "Opera"
		},
		{
			string: navigator.vendor,
			subString: "iCab",
			identity: "iCab"
		},
		{
			string: navigator.vendor,
			subString: "KDE",
			identity: "Konqueror"
		},
		{
			string: navigator.userAgent,
			subString: "Firefox",
			identity: "Firefox"
		},
		{
			string: navigator.vendor,
			subString: "Camino",
			identity: "Camino"
		},
		{
			string: navigator.userAgent,
			subString: "Netscape",
			identity: "Netscape"
		},
		{
			string: navigator.userAgent,
			subString: "MSIE",
			identity: "Explorer",
			versionSearch: "MSIE"
		},
		{
			string: navigator.userAgent,
			subString: "Gecko",
			identity: "Mozilla",
			versionSearch: "rv"
		},
		{
			string: navigator.userAgent,
			subString: "Mozilla",
			identity: "Netscape",
			versionSearch: "Mozilla"
		}
	],

	dataOS: [
		{
			string: navigator.platform,
			subString: "Win",
			identity: "Windows"
		},
		{
			string: navigator.platform,
			subString: "Mac",
			identity: "Mac"
		},
		{
			string: navigator.platform,
			subString: "Linux",
			identity: "Linux"
		},
		{
			string: navigator.userAgent,
			subString: "iPhone",
			identity: "iPhone/iPod"
		}
	],
	
	dataNetwork: function() {
		return navigator.onLine ? true : false;
	},
	
	dataMobile: function(a) {
		return (/android|avantgo|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od)|iris|kindle|lge |maemo|midp|mmp|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\/|plucker|pocket|psp|symbian|treo|up\.(browser|link)|vodafone|wap|windows (ce|phone)|xda|xiino/i.test(a)||/1207|6310|6590|3gso|4thp|50[1-6]i|770s|802s|a wa|abac|ac(er|oo|s\-)|ai(ko|rn)|al(av|ca|co)|amoi|an(ex|ny|yw)|aptu|ar(ch|go)|as(te|us)|attw|au(di|\-m|r |s )|avan|be(ck|ll|nq)|bi(lb|rd)|bl(ac|az)|br(e|v)w|bumb|bw\-(n|u)|c55\/|capi|ccwa|cdm\-|cell|chtm|cldc|cmd\-|co(mp|nd)|craw|da(it|ll|ng)|dbte|dc\-s|devi|dica|dmob|do(c|p)o|ds(12|\-d)|el(49|ai)|em(l2|ul)|er(ic|k0)|esl8|ez([4-7]0|os|wa|ze)|fetc|fly(\-|_)|g1 u|g560|gene|gf\-5|g\-mo|go(\.w|od)|gr(ad|un)|haie|hcit|hd\-(m|p|t)|hei\-|hi(pt|ta)|hp( i|ip)|hs\-c|ht(c(\-| |_|a|g|p|s|t)|tp)|hu(aw|tc)|i\-(20|go|ma)|i230|iac( |\-|\/)|ibro|idea|ig01|ikom|im1k|inno|ipaq|iris|ja(t|v)a|jbro|jemu|jigs|kddi|keji|kgt( |\/)|klon|kpt |kwc\-|kyo(c|k)|le(no|xi)|lg( g|\/(k|l|u)|50|54|e\-|e\/|\-[a-w])|libw|lynx|m1\-w|m3ga|m50\/|ma(te|ui|xo)|mc(01|21|ca)|m\-cr|me(di|rc|ri)|mi(o8|oa|ts)|mmef|mo(01|02|bi|de|do|t(\-| |o|v)|zz)|mt(50|p1|v )|mwbp|mywa|n10[0-2]|n20[2-3]|n30(0|2)|n50(0|2|5)|n7(0(0|1)|10)|ne((c|m)\-|on|tf|wf|wg|wt)|nok(6|i)|nzph|o2im|op(ti|wv)|oran|owg1|p800|pan(a|d|t)|pdxg|pg(13|\-([1-8]|c))|phil|pire|pl(ay|uc)|pn\-2|po(ck|rt|se)|prox|psio|pt\-g|qa\-a|qc(07|12|21|32|60|\-[2-7]|i\-)|qtek|r380|r600|raks|rim9|ro(ve|zo)|s55\/|sa(ge|ma|mm|ms|ny|va)|sc(01|h\-|oo|p\-)|sdk\/|se(c(\-|0|1)|47|mc|nd|ri)|sgh\-|shar|sie(\-|m)|sk\-0|sl(45|id)|sm(al|ar|b3|it|t5)|so(ft|ny)|sp(01|h\-|v\-|v )|sy(01|mb)|t2(18|50)|t6(00|10|18)|ta(gt|lk)|tcl\-|tdg\-|tel(i|m)|tim\-|t\-mo|to(pl|sh)|ts(70|m\-|m3|m5)|tx\-9|up(\.b|g1|si)|utst|v400|v750|veri|vi(rg|te)|vk(40|5[0-3]|\-v)|vm40|voda|vulc|vx(52|53|60|61|70|80|81|83|85|98)|w3c(\-| )|webc|whit|wi(g |nc|nw)|wmlb|wonu|x700|xda(\-|2|g)|yas\-|your|zeto|zte\-/i.test(a.substr(0,4))) ? true : false;
	}
};

BrowserDetect.init();

/* end browser detect */
/* begin screen dimensions */

var myHeight = 0, myWidth = 0;

if (typeof(window.innerWidth) == 'number') {
	myHeight = window.innerHeight;
	myWidth = window.innerWidth;
} else if (document.documentElement && (document.documentElement.clientWidth || document.documentElement.clientHeight)) {
	myHeight = document.documentElement.clientHeight;
	myWidth = document.documentElement.clientWidth;
} else if (document.body && (document.body.clientWidth || document.body.clientHeight)) {
	myHeight = document.body.clientHeight;
	myWidth = document.body.clientWidth;
}

/* end screen dimensions */
<?php
/* begin dC database query */
if (isset($_SESSION['logged']) && ($_SESSION['logged'] == 1)) {
include ("db.inc.php");

//$query = 'SELECT * FROM socialhns u JOIN info i ON u.user_id = i.user_id WHERE u.user_id = "' . mysql_real_escape_string($_SESSION['user_id'], $db) . '"';
$query = 'SELECT * FROM hns_desktop u JOIN info i ON u.user_id = i.user_id WHERE u.user_id = "' . mysql_real_escape_string($_SESSION['user_id'], $db) . '"';
$result = mysql_query($query, $db) or die(mysql_error($db));

if (mysql_num_rows($result) > 0) {
$row = mysql_fetch_assoc($result);
extract($row);
mysql_free_result($result);
}
}
/* end dC database query */
?>
/* begin desktop config variable arrays */

var dC = {
"settings":{
"title":"Social HnS | Welcome to Homenet Spaces!",
"ip":"<?php echo $_SERVER['HTTP_HOST']; ?>"
},

"config":{
"browser":BrowserDetect.browser,
"version":BrowserDetect.version,
"OS":BrowserDetect.OS,
"network":BrowserDetect.network,
"mobile":BrowserDetect.mobile,
"getID":document.getElementById ? true : false,
"docAll":document.all ? true : false,
"height":myHeight,
"width":myWidth
},
<?php if (isset($_SESSION['logged']) && ($_SESSION['logged'] == 1)) { ?>

"user":{
"logged":true,
"id":<?php echo $_SESSION['user_id']; ?>,
"username":"<?php echo $_SESSION['username']; ?>",
"access_level":<?php echo $_SESSION['access_level']; ?>,
"fullname":"<?php echo $_SESSION['fullname']; ?>",
"firstname":"<?php echo $_SESSION['firstname']; ?>",
"middlename":"<?php echo $_SESSION['middlename']; ?>",
"lastname":"<?php echo $_SESSION['lastname']; ?>",
"image":"<?php echo $row['default_image']; ?>",
"mouseX":0,
"mouseY":0,
"time_inactive":0
}
<?php } else { ?>

"user":{
"logged":false,
"mouseX":0,
"mouseY":0,
"time_inactive":0
}
<?php } ?>
}

/* end desktop config variable arrays */

var stylesheet = '<link rel="stylesheet" type="text/css" href="css.php" media="all" />';
if ($) $('head').append(stylesheet);
else alert("We Are Sorry! Social HnS Requires A Browser That Supports jQuery.");

var blackout = document.createElement('div'); blackout.setAttribute('id','blackout');
var whiteout = document.createElement('div'); whiteout.setAttribute('id','whiteout');

/* begin misc functions */

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

String.prototype.capitalize = function() { return this.replace(/(^|\s)([a-z])/g, function(m, p1, p2) { return p1 + p2.toUpperCase(); }); };
String.prototype.trim = function() { return this.replace(/^\s+|\s+$/g,""); }
String.prototype.ltrim = function() { return this.replace(/^\s+/,""); }
String.prototype.rtrim = function() { return this.replace(/\s+$/,""); }

function getHash() { return decodeURIComponent(window.location.hash.substring(1)); }
function updateHash(hash) { window.location.replace("#" + encodeURI(hash)); }

function rotateTitle(title, reps) {
	rotateTitle.flag = !rotateTitle.flag;

	if (rotateTitle.flag) document.title = title;
	else document.title = dC.settings.title;

	reps++;

	if (reps < 10) setTimeout(rotateTitle(title, reps), 1800);
	else document.title = dC.settings.title;
}

function in_array(string, array) {
	for (i = 0; i < array.length; i++) return (array[i] == string) ? true : false;
}

function removeKey(arrayName, key) {
	var x, tmpArray = new Array();
	for (x in arrayName) { if (x != key) tmpArray[x] = arrayName[x]; }
	return tmpArray;
}

Array.prototype.remove = function(from, to) {
	var rest = this.slice((to || from) + 1 || this.length);
	this.length = from < 0 ? this.length + from : from;
	return this.push.apply(this, rest);
};

Array.prototype.kIndex = function(key) {
	for (i = 0; i < this.length; i++) { if (this[i].key == key) return i; }
	return -1;
};

Array.prototype.vIndex = function(value) {
	for (i = 0; i < this.length; i++) { if (this[i] == value) return i; }
	return -1;
};

function IsRightButtonClicked(e) {
	var rightclick = false;
	e = e || window.event;
	if (e.which) rightclick = (e.which == 3);
	else if (e.button) rightclick = (e.button == 2);
	return rightclick;
}

function getthedate() {
	var dayarray = new Array("Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday");
	var montharray = new Array("January","February","March","April","May","June","July","August","September","October","November","December");
	var mydate = new Date();
	var year = mydate.getYear();
	var day = mydate.getDay();
	var month = mydate.getMonth();
	var daym = mydate.getDate();
	var hours = mydate.getHours();
	var minutes = mydate.getMinutes();
	var seconds = mydate.getSeconds();
	var dn = "AM";

	if (year < 1000) year += 1900;
	if (daym < 10) daym = "0" + daym;
	if (hours >= 12) dn = "PM";
	if (hours > 12) hours = hours - 12;
	
	{
	d = new Date();
	Time24H = new Date();
	Time24H.setTime(d.getTime() + (d.getTimezoneOffset() * 60000) + 3600000);
	InternetTime = Math.round((Time24H.getHours() * 60 + Time24H.getMinutes()) / 1.44);
	if (InternetTime < 10) InternetTime = '00' + InternetTime;
	else if (InternetTime < 100) InternetTime = '0' + InternetTime;
	}

	if (hours == 0) hours = 12;
	if (minutes <= 9) minutes = "0" + minutes;
	if (seconds <= 9) seconds = "0" + seconds;
	var cdate = dayarray[day] + ", " + montharray[month] + " " + daym + " " + year + " | " + hours + ":" + minutes + ":" + seconds + " " + dn + " | @" + InternetTime + "";
	var mytime = hours + ":" + minutes + ":" + seconds + " " + dn;
	var mytime2 = hours + ":" + minutes + " " + dn;
	var mydate = (month + 1) + "/" + daym + "/" + year;
	var myfulldate = dayarray[day] + ", " + montharray[month] + " " + daym + ", " + year;
}

function getdate() {
	if (document.all || document.getElementById) setInterval("getthedate()", 1000);
	else getthedate();
}

function updatecaptchaimg() { document.captchaimg.src += '?'; }

function div_selection(e, eq) {
	this.e = e;
	this.eq = eq;
	if (this.e.target) this.event = this.e.target;
	else this.event = this.e.srcElement;
	if (this.event.nodeType == 3) this.event = this.event.parentNode;
	this.selection = $(this.event).parents().eq(this.eq).attr('id');
	this.target_class = function() { return $(this.event).attr('class'); }
	this.target_id = function() { return $(this.event).attr('id'); }
	this.target_html = function() { return $(this.event).html(); }
	this.main = function() { return this.selection; }
	this.div_main = function() { return ['div#', this.selection].join(''); }
}

 function maxWindow() {
	window.moveTo(0, 0);

	if (document.all) {
		top.window.resizeTo(screen.availWidth, screen.availHeight);
	} else if (document.layers || document.getElementById) {
		if (top.window.outerHeight < screen.availHeight || top.window.outerWidth < screen.availWidth) {
			top.window.outerHeight = screen.availHeight;
			top.window.outerWidth = screen.availWidth;
		}
	}
}

function noFrame() {
	if (top.location != self.location) {
		var topURL = document.referrer;
		var subURL = topURL.slice(7,10);
		var chars = topURL.length;
		var hash = window.location.hash

		/* begin location hashes */

		/* end location hashes */
		
		top.location = self.location;
	}
}

function extractHost(url) {
	var returnArry = /^(?:[^:\/?#]+):\/\/([^\/?#]+)(?::\d+)?(?:[^?#]*)\//i.exec(url);
	if (returnArry && typeof returnArry === "object") return returnArry[1];
	else return "";
}

function bustFrame() {
	var blacklist = ['homenetspaces.tk','hnsdesktop.tk'];

	if (top.location != window.location) {
		var topURL = extractHost(document.referrer);

		if (topURL) {
			for (var i=0; i < blacklist.length; i++) {
				if (topURL.indexOf(blacklist[i]) != -1) {
					top.location.replace(window.location);
					return;
				}
			}
		}
	}
}

function setCookie(name, value, expires, path, domain, secure) {
	var today = new Date();
	today.setTime(today.getTime());
	if (expires) expires = (expires * 1000 * 60 * 60 * 24);
	var expires_date = new Date(today.getTime() + (expires));
	document.cookie = name + "=" + escape(value) + ((expires) ? ";expires=" + expires_date.toGMTString() : "") + ((path) ? ";path=" + path : "") + ((domain) ? ";domain=" + domain : "") + ((secure) ? ";secure" : "");
}

function getCookie(check_name) {
	var a_all_cookies = document.cookie.split(';');
	var a_temp_cookie = '';
	var cookie_name = '';
	var cookie_value = '';
	var b_cookie_found = false;

	for (i = 0; i < a_all_cookies.length; i++) {
		a_temp_cookie = a_all_cookies[i].split('=');
		cookie_name = a_temp_cookie[0].replace(/^\s+|\s+$/g, '');

		if (cookie_name == check_name) {
			b_cookie_found = true;
			if (a_temp_cookie.length > 1) cookie_value = unescape(a_temp_cookie[1].replace(/^\s+|\s+$/g, ''));
			return cookie_value;
			break;
		}

		a_temp_cookie = null;
		cookie_name = '';
	}

	if (!b_cookie_found) return null;
}

function deleteCookie(name, path, domain) {
	if (getCookie(name)) document.cookie = name + "=" + ((path) ? ";path=" + path : "") + ((domain) ? ";domain=" + domain : "") + ";expires=Thu, 01-Jan-1970 00:00:01 GMT";
}

/* end misc functions */
/* begin cookie functions */

setCookie('hnsmaintheme', 1, 365, '/');
setCookie('hnslanguage', 'en', 365, '/');

/* end cookie functions*/

$(document).ready(function() {

noFrame();

if ($) {
	$("div.noscript").remove();
	$("body").html('<h1 style="color: #fff; text-align: center;">Social HnS | Welcome to Homenet Spaces. Please remember this is in the planning stages.</h1>');
}

});

<?php if (isset($_SESSION['logged']) && ($_SESSION['logged'] == 1)) { ?>
/* begin dragresize functions */

if (typeof addEvent != 'function') {
	var addEvent = function(o,t,f,l) {
		var d = 'addEventListener', n = 'on' + t, rO = o, rT = t, rF = f, rL = l;

		if (o[d] && !l) return o[d](t,f,false);
		if (!o._evts) o._evts = {};
		if (!o._evts[t]) {
			o._evts[t] = o[n] ? {b:o[n]} : {};
			o[n] = new Function('e','var r = true, o = this, a = o._evts["' + t + '"], i; for (i in a) { o._f = a[i]; r = o._f(e || window.event) != false && r; o._f = null } return r;');
			if (t != 'unload') addEvent(window,'unload',function() { removeEvent(rO,rT,rF,rL); })
		}

		if (!f._i) f._i = addEvent._i++;
		o._evts[t][f._i] = f;
	};

	addEvent._i = 1;

	var removeEvent = function(o,t,f,l) {
		var d = 'removeEventListener';

		if (o[d] && !l) return o[d](t,f,false);
		if (o._evts && o._evts[t] && f._i) delete o._evts[t][f._i];
	}
}

function cancelEvent(e,c) {
	e.returnValue = false;

	if (e.preventDefault) e.preventDefault();
	if (c) {
		e.cancelBubble = true;
		if (e.stopPropagation) e.stopPropagation();
	}
};

function DragResize(myName,config) {
	var props = {
		myName:myName,
		enabled:true,
		handles:['tl','tm','tr','ml','mr','bl','bm','br'],
		isElement:null,
		isHandle:null,
		element:null,
		handle:null,
		minWidth:10,
		minHeight:10,
		minLeft:0,
		maxLeft:9999,
		minTop:0,
		maxTop:9999,
		zIndex:1,	
		mouseX:0,
		mouseY:0,
		lastMouseX:0,
		lastMouseY:0,
		mOffX:0,
		mOffY:0,
		elmX:0,
		elmY:0,	
		elmW:0,
		elmH:0,
		allowBlur:true,
		ondragfocus:null,
		ondragstart:null,
		ondragmove:null,
		ondragend:null,
		ondragblur:null
	};	

	for (var p in props) this[p] = (typeof config[p] == 'undefined') ? props[p] : config[p];
};

DragResize.prototype.apply = function(node) {
	var obj = this;
	addEvent(node,'mousedown',function(e) { obj.mouseDown(e); });
	addEvent(node,'mousemove',function(e) { obj.mouseMove(e); });
	addEvent(node,'mouseup',function(e) { obj.mouseUp(e); })
};

DragResize.prototype.select = function(newElement) {
	with (this) {
		if (!document.getElementById || !enabled) return;

		if (newElement && (newElement != element) && enabled) {
			element = newElement;
			var zmax = 0, cur = 0;
			if (this.resizeHandleSet) this.resizeHandleSet(element,true);
			elmX = parseInt(element.style.left);
			elmY = parseInt(element.style.top);
			elmW = element.offsetWidth;
			elmH = element.offsetHeight;
			if (ondragfocus) this.ondragfocus();
		}
	}
};

DragResize.prototype.deselect = function(delHandles) {
	with (this) {
		if (!document.getElementById || !enabled) return;
		if (delHandles) {
			if (ondragblur) this.ondragblur();
			if (this.resizeHandleSet) this.resizeHandleSet(element,false);
			element = null;
		}

		handle = null;
		mOffX = 0;
		mOffY = 0;
	}
};

DragResize.prototype.mouseDown = function(e) {
	with (this) {
		if (!document.getElementById || !enabled) return true;
		var elm = e.target || e.srcElement, newElement = null, newHandle = null, hRE = new RegExp(myName + '-([trmbl]{2})','');
		
		while (elm) {
			if (elm.className) {
				if (!newHandle && (hRE.test(elm.className) || isHandle(elm))) newHandle = elm;
				if (isElement(elm)) {
					newElement = elm;
					break;
				}
			}

			elm = elm.parentNode;
		}

		if (element && (element != newElement) && allowBlur) deselect(true);
		if (newElement && (!element || (newElement == element))) {
			if (newHandle) cancelEvent(e);
			select(newElement,newHandle);
			handle = newHandle;
			if (handle && ondragstart) this.ondragstart(hRE.test(handle.className));
		}
	}
};

DragResize.prototype.mouseMove = function(e) {
	with (this) {
		if (!document.getElementById || !enabled) return true;
		mouseX = e.pageX || (e.clientX + document.documentElement.scrollLeft);
		mouseY = e.pageY || (e.clientY + document.documentElement.scrollTop);
		var diffX = (mouseX - lastMouseX + mOffX);
		var diffY = (mouseY - lastMouseY + mOffY);
		mOffX = mOffY = 0;
		lastMouseX = mouseX;
		lastMouseY = mouseY;
		if (!handle) return true;
		var isResize = false;

		if (this.resizeHandleDrag && this.resizeHandleDrag(diffX,diffY)) {
			isResize = true;
		} else {
			var dX = diffX, dY = diffY;
			if ((elmX + dX) < minLeft) mOffX = (dX - (diffX = minLeft - elmX));
			else if ((elmX + elmW + dX) > maxLeft) mOffX = (dX - (diffX = (maxLeft - elmX - elmW)));
			if ((elmY + dY) < minTop) mOffY = (dY - (diffY = (minTop - elmY)));
			else if ((elmY + elmH + dY) > maxTop) mOffY = (dY - (diffY = (maxTop - elmY - elmH)));
			elmX += diffX;
			elmY += diffY;
		}

		with (element.style) {
			left = elmX + 'px';
			width = elmW + 'px';
			top = elmY + 'px';
			height = elmH + 'px';
		}

		if (window.opera && document.documentElement) {
			var oDF = document.getElementById('op-drag-fix');

			if (!oDF) {
				var oDF = document.createElement('input');
				oDF.id = 'op-drag-fix';
				oDF.style.display = 'none';
				document.body.appendChild(oDF);
			}

			oDF.focus();
		}

		if (ondragmove) this.ondragmove(isResize);
		cancelEvent(e);
	}
};

DragResize.prototype.mouseUp = function(e) {
	with (this) {
		if (!document.getElementById || !enabled) return;
		var hRE = new RegExp(myName + '-([trmbl]{2})','');
		if (handle && ondragend) this.ondragend(hRE.test(handle.className));
		deselect(false);
	}
};

DragResize.prototype.resizeHandleSet = function(elm,show) {
	with (this) {
		if (!elm._handle_tr) {
			for (var h = 0; h < handles.length; h++) {
				var hDiv = document.createElement('div');
				hDiv.className = myName + ' ' + myName + '-' + handles[h];
				elm['_handle_' + handles[h]] = elm.appendChild(hDiv);
			}
		}

		for (var h = 0; h < handles.length; h++) {
			elm['_handle_' + handles[h]].style.visibility = show ? 'inherit' : 'hidden';
		}
	}
};

DragResize.prototype.resizeHandleDrag = function(diffX,diffY) {
	with (this) {
		var hClass = handle && handle.className && handle.className.match(new RegExp(myName + '-([tmblr]{2})')) ? RegExp.$1 : '';
		var dY = diffY, dX = diffX, processed = false;

		if (hClass.indexOf('t') >= 0) {
			rs = 1;
			if ((elmH - dY) < minHeight) mOffY = (dY - (diffY = elmH - minHeight));
			else if ((elmY + dY) < minTop) mOffY = (dY - (diffY = minTop - elmY));
			elmY += diffY;
			elmH -= diffY;
			processed = true;
		}

		if (hClass.indexOf('b') >= 0) {
			rs = 1;
			if ((elmH + dY) < minHeight) mOffY = (dY - (diffY = (minHeight - elmH)));
			else if ((elmY + elmH + dY) > maxTop) mOffY = (dY - (diffY = (maxTop - elmY - elmH)));
			elmH += diffY;
			processed = true;
		}

		if (hClass.indexOf('l') >= 0) {
			rs = 1;
			if ((elmW - dX) < minWidth) mOffX = (dX - (diffX = (elmW - minWidth)));
			else if ((elmX + dX) < minLeft) mOffX = (dX - (diffX = (minLeft - elmX)));
			elmX += diffX;
			elmW -= diffX;
			processed = true;
		}

		if (hClass.indexOf('r') >= 0) {
			rs = 1;
			if (elmW + dX < minWidth) mOffX = (dX - (diffX = minWidth - elmW));
			else if ((elmX + elmW + dX) > maxLeft) mOffX = (dX - (diffX = maxLeft - elmX - elmW));
			elmW += diffX;
			processed = true;
		}

		return processed;
	}
};

var dragresize = new DragResize('dragresize', { minWidth: 10, minHeight: 10, minLeft: 0, maxLeft: dC.config.width, minTop: 0, maxTop: dC.config.height });

dragresize.isElement = function(elm) { if (elm.className && elm.className.indexOf('drsElement') > -1) return true; };
dragresize.isHandle = function(elm) { if (elm.className && elm.className.indexOf('drsMoveHandle') > -1) return true; };
dragresize.ondragfocus = function() { };
dragresize.ondragstart = function(isResize) { };
dragresize.ondragmove = function(isResize) { };
dragresize.ondragend = function(isResize) { };
dragresize.ondragblur = function() { };
dragresize.apply(document);

/* end dragresize functions */
<?php } ?>