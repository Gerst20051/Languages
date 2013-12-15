
function xhrRequest(ro)
{var r;this.init=function()
{if(ro['method']=='POST'){var pa=[];if(ro['params']){for(p in ro['params']){pa.push(p+"="+ro['params'][p]);};};};try
{if(window.XMLHttpRequest)
{r=new XMLHttpRequest();}
else
{r=new ActiveXObject("Msxml2.XMLHTTP");}
if(r==undefined)
{throw new Error("You do not have a compatible browser to view this site.");}}
catch(_ignore)
{};r.onreadystatechange=function()
{if(r.readyState==4&&r.status==200)
{switch(ro['responseType']){case'json':var d=eval('('+r.responseText+')');break;case'text':var d=r.responseText;break;default:var d=r.responseXML;break;};if(ro['callback'])
{var my_func=eval(ro['callback']);new my_func(d,r.status);}
else
{new xhrRequestComplete(d,r.status);};}
else
{if(ro['status'])
{var my_func=eval(ro['status']);new my_func(r);}
else
{new xhrRequestStatus(r);};};};if(!ro['async']){ro['async']=true;};r.open(ro['method'],ro['url'],ro['async']);if(ro['method']=="GET"){r.send(null);}
else{r.setRequestHeader("Content-type","application/x-www-form-urlencoded");r.setRequestHeader("Content-length",pa.join("&").length);r.setRequestHeader("Connection","close");r.send(pa.join("&"));};};};function xhrRequestStatus(st)
{};function xhrRequestComplete(d,st,h)
{};if(window.addEventListener){window.addEventListener("load",function(){sf('query');$('#mask').click(function(){$(this).hide();$('.window').hide();});},false);}
else if(window.attachEvent){window.attachEvent("onload",function(){sf('query');$('#mask').click(function(){$(this).hide();$('.window').hide();});});};function ge(f,t){var e;switch(t){case"name":e=document.getElementsByName(f)[0];break;default:e=document.getElementById(f);break;};return e;};function sf(f,t){var e;e=ge(f,t);e.focus();};var q="";var t;function gs(str){var p=[];p.push("method=GetSuggestions");p.push("q="+ge("query").value);var gs=new xhrRequest({'url':'app/YT.php?'+p.join('&'),'method':'GET','callback':'ds','responseType':'json'});gs.init();};function s(str){var p=[];if(t){clearTimeout(t);};p.push("method=DoSearch");if(str==undefined||str==null){q=ge("query").value;}
else{q=str;};p.push("q="+q);var gr=new xhrRequest({'url':'app/YT.php?'+p.join('&'),'method':'GET','callback':'dr','responseType':'json'});gr.init();return false;};function sk(str){s(str);sf("query");};function ec(c){var e=ge(c);if(e.hasChildNodes()){while(e.hasChildNodes()){e.removeChild(e.childNodes[0]);};};};function dr(d,st){if(t){clearTimeout(t);};q=d["query"];ec("contentContainer");if(d["total"]>0){var si=document.createElement("span");si.innerHTML="You searched for: &quot;<b>"+q+"<\/b>&quot; - We found "+d["total"]+" results in <i>"+d["time"]+" seconds<\/i>";ge("contentContainer").appendChild(si);for(p in d["videos"]){var v=d["videos"][p];var r=document.createElement("div");r.className="resultRow";var lt=document.createTextNode(v["title"]);var l=document.createElement("a");l.href="javascript:;";l.onclick=function(){DoPopup(v["id"]);};l.appendChild(lt);var ic=document.createElement("div");ic.className="resultImg";var i=document.createElement("img");i.src="http://img.youtube.com/vi/"+v["id"]+"/default.jpg";i.setAttribute("onclick","DoPopup( '"+v["id"]+"' )");ic.appendChild(i);r.appendChild(ic);var br=document.createElement("br");var de=document.createElement("div");de.className="resultDesc";de.appendChild(l);de.appendChild(br);var a=document.createElement("div");a.innerHTML="Posted on "+v["published"]+" by <a href=\"http://www.youtube.com/"+v["author"]["name"]+"\" target=\"_blank\">"+v["author"]["name"]+"<\/a>";de.appendChild(a);var c=document.createElement("span");c.innerHTML=v["category"];de.appendChild(c);r.appendChild(de);var br=document.createElement("br");br.clear="all";r.appendChild(br);ge("contentContainer").appendChild(r);}}
else{ge("contentContainer").appendChild(document.createTextNode("No results were found for your search."));};};function ds(d,st){if(d["firstResult"]&&d["firstResult"].indexOf(ge("query").value)!=-1){ge("suggestion").value=d["firstResult"];sk(d["firstResult"]);}
else{ge("suggestion").value="";ge("moreSuggestions").style.display="none";s();};if(d["suggestions"][1]!=""){ge("moreSuggestions").style.display="block";ec("moreSuggestions");for(p in d["suggestions"]){var di=document.createElement("div");di.onmouseover=function(){this.style.backgroundColor="#d6e1ff";};di.onmouseout=function(){this.style.backgroundColor="#FFFFFF";}
di.onclick=function(){var sq=this.innerHTML.replace(/<b>/gi,'').replace(/<\/b>/gi,'').replace(/<\/span>/gi,'').replace(/<span>/gi,'');sk(sq);ge("query").value=sq;ge("suggestion").value="";ge("moreSuggestions").style.display="none";cl();}
var sp=document.createElement("span");var cq=ge('query').value;var re=new RegExp(cq,'gi');sp.innerHTML=d["suggestions"][p].replace(re,cq+"<b>")+"<\/b>";di.appendChild(sp);ge("moreSuggestions").appendChild(di);};};};function st(ti){document.title=ti;}
function ps(e){var kc;var kchr;var kchk;if(window.event){kc=e.keyCode;}
else if(e.which){kc=e.which;};kchr=String.fromCharCode(kc);kchk=/[a-zA-Z0-9 ]/;if(kc==13){clearTimeout(t);ge("moreSuggestions").style.display="none";ge("suggestion").value="";cl();}
else if(kchk.test(kchr)||kc==8){if(t){clearTimeout(t);}
t=setTimeout(function(){gs(ge("query").value);},175);if(ge("query").value.length>0){ge("x").style.display="block";}
else{cf();}};};function cl(){window.location='#!/'+escape(ge("query").value.replace(/ /gi,'+'));}
function cf(){if(t){clearTimeout(t);}
window.location='/';return false;};