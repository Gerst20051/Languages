var d=45000;var timer= null;
var langs=[{'c':'all','d':'Any language'},{'c':'en','d':'English'},{'c':'de','d':'Deutsch'},{'c':'es','d':'español'},{'c':'fr','d':'français'},{'c':'ja', 'd':'日本語 (Japanese)'},{'c':'no','d':'Norsk'},{'c':'ru','d':'русский язык'},{'c':'sv','d':'svenska'}];
var dL='all';var dExc='porn';var dExcUsr='hollyfanz,wifemilfs';
var cL=dL;var cExc=dExc;var cEU=dExcUsr;

var sp=String.prototype;sp.trim=function(){return this.replace(/^\s*/, "").replace(/\s*$/, "")};sp.e=function(){return escape(this)};

$(function(){
	var eE=$('#exc');
	var eEU=$('#excUsr');
	for (var i in langs){var cl= langs[i]; $('#lang').append('<option value="'+cl.c+'">'+cl.d);}
	eE.val(dExc);
	eEU.val(dExcUsr);
	$('.hlp').text('separate with commas');
	$('#show-opts a').click(function(){ 
		if($('#opts').is(':visible')){$('#opts').slideUp(200);$(this).text('Show options');} 
		else{$('#opts').slideDown(200);$(this).text('Hide options');}
		return false});
	$('#lang').change(chCr);eE.keypress(chCr);eEU.keypress(chCr);
	rstrCr();
	$('#updt').click(updateUsingNewCriteria);
	sTmr();update()});
function sTmr(){timer=setInterval(update,d)}

function chCr(){$('#updt').removeAttr('disabled')}
function sCr(){
	cL=$('#lang').val();cExc=$('#exc').val();cEU=$('#excUsr').val();
	sC('twgrl',cL);sC('twgrexc',cExc);sC('twgrexcUsr',cEU);}
function rstrCr(){
	var lC= gC('twgrl');var eC= gC('twgrexc');var eUC= gC('twgrexcUsr');
	if(lC!=null){cL=lC;$('#lang').val(lC);};
	if(eC!=null){cExc=eC;$('#exc').val(eC);};
	if(eUC!=null){cEU=eUC;$('#excUsr').val(eUC);};}
function updateUsingNewCriteria(){sCr();
	clearInterval(timer);sTmr();update();
	$('#updt').attr('disabled', 'disabled')}

function update(){ searchTweets("blogged http",cExc,cEU,true,cL); }
function searchTweets(q,exc,excUsr,fRT,lang){
	q+=prepExc(exc);
	q+=prepExcUsrs(excUsr);
	if(fRT) q+=' -RT';
	var url="http://search.twitter.com/search.json?q="+q.e()+'&rpp=10&result_type=recent';
	if(lang!='all') url+='&lang='+lang;
	url+='&callback=?';
	$('#ldr').show();$.getJSON(url, updateTweets)}
function updateTweets(ts){
	$('#ldr').hide();$('#ts').empty();
	for(var i in ts.results){$('#ts').append(createResultOutput(ts.results[i]));}}
function createResultOutput(t){
	var usr= t.from_user;
	var img= t.profile_image_url;
	var dt= new Date(t.created_at);
	var linkMatches= t.text.match(/https?:\/\/([-\w\.]+)+(:\d+)?(\/([\w\/_\.-]*(\?\S+)?)?)?/i);
	var lnk= (linkMatches != null) ? linkMatches[0] : null;
	var usrL= 'http://twitter.com/'+usr;
	var txt= (lnk != null) ? t.text.replace(lnk,'<a href="'+lnk+'">'+lnk+'</a>') : t.text;
	var mu= '<li><a class="auth-icon" href="'+usrL+'"><img src="'+img+'" alt="'+usr+'" height="48" width="48"></a><p class="text">'+txt+'<p class="info">';
	if(lnk != null) mu+= '<a class="visit" href="'+lnk+'">Read this</a>';
	mu+= 'By '+usr+' at '+dt.toLocaleTimeString()+' on '+dt.toLocaleDateString();
	return mu}

function prepExc(exclusionsStr){
	var ar= exclusionsStr.trim().split(',');var q= '';
	for(var i in ar) q+= ' -'+ar[i].trim();
	return q}
function prepExcUsrs(exclusionsStr){
	var ar= exclusionsStr.trim().split(',');var q= '';
	for(var i in ar) q+= ' -from:' + ar[i].trim();
	return q}

function gC(k){var s,e;var c=document.cookie;
	if(c.length>0){s=c.indexOf(k + "=");
		if(s!=-1){s=s + k.length+1;e=c.indexOf(";",s);
			if (e==-1) e=c.length;
			return unescape(c.substring(s,e));}} return null}
function sC(k,v){var exp= new Date();exp.setDate(exp.getDate()+7);document.cookie= k+"="+v.e()+";expires="+exp.toUTCString()}