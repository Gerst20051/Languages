//
var _woo_settings={};
var _woo_visitor={};
var _woo_actions=[{'type':'pageview','title':document.title,'url':window.location.pathname}];

if(typeof(woo_settings) != 'undefined' && woo_settings != false){
	_woo_settings=woo_settings;
}

if(typeof(woo_visitor) != 'undefined' && woo_visitor!=false){
	_woo_visitor=woo_visitor;
}

if(typeof(woo_actions) != 'undefined' && woo_actions != false){
	_woo_actions=woo_actions;
}

var woopraTracker=false;

function WoopraScript(_src,_hook,_async){

        this.src=_src;
        this.hook=_hook;
        this.async=_async;

        var pntr=false;

        this.init=function(){
               	pntr=this;
        }

        this.load=function(){

                var script=document.createElement('script');
                script.type='text/javascript';
                script.src=pntr.src;
		script.async=pntr.async;

                if(pntr.hook){
                        script.onload=function(){
				setTimeout(function(){pntr.hook.apply();},400);
			}
                        script.onreadystatechange = function() {
                                if (this.readyState == 'complete'|| this.readyState=='loaded') {
					setTimeout(pntr.hook,400);
				}
			}
                }

                var ssc = document.getElementsByTagName('script')[0];
		ssc.parentNode.insertBefore(script, ssc);
	}
	this.init();
}

function WoopraEvent(_name, options){

	this.name=_name || 'unknown';
	this.options = options || {};
	this.mod=0;

	this.addProperty=function(key,value){
		this.options[key] = value;
	}

	this.fire=function(){
		var t=woopraTracker;
		var buffer='';

		this.addProperty('name',this.name);

		for (var key in this.options) {
			var item = this.options[key];
			
			if (typeof item == 'string') { // String
				buffer+= "&" + encodeURIComponent('ce_'+key)+'='+encodeURIComponent(this.options[key]);
			} else if (item instanceof Array) { // Array
				for (var i=0; i<item.length; i++) {
					for (var itemKey in item[i]) {
						buffer+= "&" + encodeURIComponent('ce_' + key + '[' + i + '].' + itemKey)+'='+encodeURIComponent(item[i][itemKey]);
					}
				}
			} else { // Object
				for (var itemKey in item) {
					buffer+= "&" + encodeURIComponent('ce_' + key + '.' + itemKey)+'='+encodeURIComponent(item[itemKey]);
				}
			}
		} 
		

		buffer+='&'+'alias'+'='+t.site();
		buffer+='&'+''+'cookie'+'='+t.readcookie('wooTracker');
		buffer+='&'+''+'vid'+'='+t.vid();
		buffer+='&'+''+'meta'+'='+encodeURIComponent(t.meta());
		buffer+='&'+''+'screen'+'='+encodeURIComponent(t.screeninfo());
		buffer+='&'+''+'language'+'='+encodeURIComponent(t.langinfo());
		
		if(buffer!=''){
			var _mod = ((document.location.protocol=="https:")?'/woopras/ce.jsp?':'/ce/');
			var _url= t.getEngine() + _mod +'ra='+t.randomstring()+buffer;
			t.request(_url);
		}
	}
}

function WoopraTracker(){

	var pntr=false;
	var chat=false;

	var wx_static=false;
	var wx_engine=false;

	var alias=false;

	var vs=0;

	var vid=-1;

	this.initialize=function(){

		pntr=this;
		
		var _c=false;
		_c=pntr.readcookie('wooTracker'); 
		if(!_c){
			_c=pntr.randomstring();
			pntr.createcookie('wooTracker', _c, 10*1000);
		}

		if(document.location.protocol=="https:"){
			wx_engine="https://sec1.woopra.com";
			wx_static="https://static.woopra.com";
		}else{
			wx_engine='http://'+pntr.site()+'.woopra-ns.com';
			wx_static="http://static.woopra.com";
		}

		var domain=_woo_settings['domain'];
		if(domain){
			alias=pntr.trimSite(domain);
        	        if(document.location.protocol=="http:"){
                	        wx_engine='http://'+alias+'.woopra-ns.com';
	                }
        	        var _c=pntr.readcookie('wooTracker');
                	if(!_c){
                        	_c=pntr.randomstring();
	                }
        	        pntr.createcookie('wooTracker', _c, 10*1000);
		}


		if(document.addEventListener){
			document.addEventListener("mousedown",pntr.clicked,false);
			document.addEventListener("mousemove",pntr.moved,false);
			document.addEventListener("keydown",pntr.typed,false);
		}else{
			document.attachEvent("onmousedown",pntr.clicked);
			document.attachEvent("onmousemove",pntr.moved);
			document.attachEvent("onkeydown",pntr.typed);
		}
	}

	this.site=function(){
		if(alias){
			return alias;
		}
		return pntr.trimSite(location.hostname);
	}

	this.trimSite=function(s){
		return ((s.indexOf('www.')<0)?s:s.substring(4));
	}

	this.getStatic=function(){
		return wx_static;
	}
	
	this.vid=function(s){
		if(s){
			vid=s;
		}
		return vid;
	}

	this.getEngine=function(){
		return wx_engine;
	}

	this.setEngine=function(e){
		wx_engine=e;
	}

	this.sleep=function(millis){
		var date = new Date();
		var curDate = new Date();
		while(curDate-date < millis){
			curDate=new Date();
		}
	}

	this.randomstring=function(){
		var chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		var s = '';
		for (var i = 0; i < 32; i++) {
			var rnum = Math.floor(Math.random() * chars.length);
			s += chars.substring(rnum, rnum + 1);
		}
		return s;
	}

	this.langinfo=function(){
		return (navigator.browserLanguage || navigator.language || "");
	}

	this.screeninfo=function(){
		return screen.width + 'x' + screen.height;
	}

	this.readcookie=function(k) {
		var c=""+document.cookie;
		var ind=c.indexOf(k);
		if (ind==-1 || k==""){
			return "";
		}
		var ind1=c.indexOf(';',ind);
		if (ind1==-1){
			ind1=c.length;
		}
		return unescape(c.substring(ind+k.length+1,ind1));
	}

	this.createcookie=function(k,v,days){
		var exp='';
		if(days>0){
			var expires = new Date();
			expires.setDate(expires.getDate() + days);
			exp = expires.toGMTString();
		}
		cookieval = k + '=' + v + '; ' + 'expires=' + exp + ';' + 'path=/'+';domain=.'+pntr.site();
		document.cookie = cookieval;
	}

	this.request=function(url,hook){
		var script=new WoopraScript(url,hook,true);
		script.load();
	}

	this.verify=function(){
	}

	this.rescue=function(){
	}

	this.meta=function(){
		var meta='';
		if(pntr.readcookie('wooMeta')){
			meta=pntr.readcookie('wooMeta');
		}
		return meta;
	}

	var pvTracked=0;

	this.trackPageview=function(action){

		pvTracked++;

		var date=new Date();

		var req=new Object();


		req.cookie=pntr.readcookie('wooTracker');
		req.vid=pntr.vid();
		req.meta=pntr.meta();
		req.alias=pntr.site();
		req.language=pntr.langinfo();

		req.page=action.url;
		req.pagetitle=action.title;

		req.referer=document.referrer;
		req.screen=pntr.screeninfo();
		req.localtime=date.getHours()+':'+date.getMinutes();

		for (var ite in action) {
			if(ite=='url' || ite=='title' || ite=='type'){
			}else{
				req['ce_'+ite]=action[ite];
			}
		}
	
		for (var ite in _woo_visitor){
			req['cv_'+ite]=_woo_visitor[ite];
		}

		var url='';
		for (var ite in req){
			url+="&"+encodeURIComponent(ite)+"="+encodeURIComponent(req[ite]);
		}

		if(pvTracked>1){
			url+="&mode=nil";
		}

		var _mod = ((document.location.protocol=="https:")?'/woopras/visit.jsp?':'/visit/');

		pntr.request(wx_engine + _mod +'ra='+pntr.randomstring()+url);
	}

	this.pingServer=function(){
		var _mod = ((document.location.protocol=="https:")?'/woopras/ping.jsp?':'/ping/');
		var _url = wx_engine + _mod;
		_url+='vid='+pntr.vid();
		_url+='&';
		_url+='cookie='+pntr.readcookie('wooTracker');
		_url+='&';
		_url+='alias='+pntr.site();
		_url+='&';
		_url+='idle='+parseInt(idle/1000);
		if(vs==2){
			_url+='&vs=w';
			vs=0;
		}else{
			if(idle==0){
				_url+='&vs=r';
			}else{
				_url+='&vs=i';
			}
		}
		_url+='&ra='+pntr.randomstring();
		pntr.request(_url);
	}

	this.typed=function(e){
		vs=2;
	}

	this.clicked=function(e) {
		pntr.moved();

		var cElem = (e.srcElement) ? e.srcElement : e.target;

		 while (typeof cElem != 'undefined' && cElem != null) {
		         if (cElem.tagName == "A") {
           			  break;
             		 }
         		cElem = cElem.parentNode;
         	}

		if(typeof cElem != 'undefined' && cElem != null){

			var link=cElem;
			var _download = link.pathname.match(/(?:doc|eps|jpg|png|svg|xls|ppt|pdf|xls|zip|txt|vsd|vxd|js|css|rar|exe|wma|mov|avi|wmv|mp3)($|\&)/);
			var ev=false;
			if(_download && (link.href.toString().indexOf('woopra-ns.com')<0)){
				ev=new WoopraEvent('download',{});
				ev.addProperty('url',link.href);
				ev.fire();
				pntr.sleep(100);
			}
			if (!_download&&link.hostname != location.host && link.hostname.indexOf('javascript')==-1 && link.hostname!=''){
				ev=new WoopraEvent('exit',{});
				ev.addProperty('url',link.href);
				ev.fire();
				pntr.sleep(400);
			}
		}
	}

	var last_activity=new Date();
	var idle=0;

	this.moved=function(){
		last_activity=new Date();       
		idle=0;
	}

	this.ping=function(){
		var timeout=4*60*1000;
		if(_woo_settings['idle_timeout']){
			timeout=parseInt(_woo_settings['idle_timeout']);
		}
		if(idle>timeout){
			return;
		}
		pntr.pingServer();
		var now=new Date();
		if(now-last_activity>10000){
			idle=now-last_activity;
		}
	}
    
	this.loadScript=function(src,hook){
		pntr.request(src,hook);
	}

	this.pushEvent=function(ev){
		var ce=new WoopraEvent(ev.name,ev);
                ce.fire();
	}
}

woopraTracker=new WoopraTracker();
woopraTracker.initialize();


var wx=0;

for(wx=0;wx<_woo_actions.length;wx++){
	var action=_woo_actions[wx];
	if(action.type=='pageview'){
		woopraTracker.trackPageview(action);
	}
	if(action.type=='event'){
		woopraTracker.pushEvent(action);
	}
}
