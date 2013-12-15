function loadScript(url, callback) { /* loadScript("http://your.cdn.com/second.js", function() {}); */
	var script = document.createElement("script"); script.type = "text/javascript";
	if (callback) {
		if (script.readyState) { // IE
			script.onreadystatechange = function() {
				if (script.readyState == "loaded" || script.readyState == "complete") { script.onreadystatechange = null; callback(); }
			};
		} else { script.onload = function() { callback(); };	} // Others
	}
	script.src = url; document.getElementsByTagName("head")[0].appendChild(script);
	// or head.insertBefore(script, head.firstChild);
}



(function() {
	var s = [
	"/javascripts/script1.js",
	"/javascripts/script2.js",
	"/javascripts/script3.js",
	"/javascripts/script4.js",
	"/javascripts/script5.js"
	];

	var sc = "script", tp = "text/javascript", sa = "setAttribute", doc = document, ua = window.navigator.userAgent;
	for (var i = 0, l = s.length; i < l; ++i) {
		if (ua.indexOf("Firefox") !== -1 || ua.indexOf("Opera") !== -1) {
			var t = doc.createElement(sc);
			t[sa]("src", s[i]);
			t[sa]("type", tp);
			doc.getElementsByTagName("head")[0].appendChild(t);
		} else doc.writeln("<" + sc + " type=\"" + tp + "\" src=\"" + s[i] + "\"></" + sc + ">");
	}
})();



/** include - including .js files from JS - bfults@gmail.com - 2005-02-09
 ** Code licensed under Creative Commons Attribution-ShareAlike License 
 ** http://creativecommons.org/licenses/by-sa/2.0/
 **/              
var hIncludes = null;
function include(sURI)
{   
  if (document.getElementsByTagName)
  {   
    if (!hIncludes)
    {
      hIncludes = {}; 
      var cScripts = document.getElementsByTagName("script");
      for (var i=0,len=cScripts.length; i < len; i++)
        if (cScripts[i].src) hIncludes[cScripts[i].src] = true;
    }
    if (!hIncludes[sURI])
    {
      var oNew = document.createElement("script");
      oNew.type = "text/javascript";
      oNew.src = sURI;
      hIncludes[sURI]=true;
      document.getElementsByTagName("head")[0].appendChild(oNew);
    }
  }   
}



// MSN Perallel Scripts
var p = g.getElementsByTagName("HEAD")[0];
var c = g.createElement("script");
c.type = "type/javascript";
c.onreadystatechange = n;
c.onerror = c.onload = k;
c.src = e;
p.appendChild(c);



/* Asynchronous Script Loading
XHR Eval
XHR Injection
Script In IFrame
Script DOM Element
Script Defer
document.write Script Tag
*/



// XHR Eval
var xhrObj = getXHRObject();
xhr.Obj.onreadystatechange = function() {
	if (xhrObj.readyState != 4) return;
	eval(xhrObj.responseText);
);
xhrObj.open('GET', 'A.js', true);
xhrObj.send('');



// XHR Injection
var xhrObj = getXHRObject();
xhr.Obj.onreadystatechange = function() {
	if (xhrObj.readyState != 4) return;
	var se = document.createElement('script');
	document.getElementByTagName('head')[0].appendChild(se);
	se.text = xhrObj.responseText;
);
xhrObj.open('GET', 'A.js', true);
xhrObj.send('');



// Script In IFrame
<iframe src='A.html' width=0 height=0 frameborder=0 id=frame1></iframe>
// access iframe from main page
window.frames[0].createNewDiv();
//access main page from iframe
parent.document.createElement('div');



// Script DOM Element
var se = document.createElement('script');
se.src = 'http://anydomain.com/A.js';
document.getElementsByTagName('head')[0].appendChild(se);



// Script Defer (Only IE)
<script defer src='A.js'></script>



// document.write Script Tag
document.write("<scr" + "ipt type='text/javascript' src='A.js'>" + "</scr" + "ipt>");
