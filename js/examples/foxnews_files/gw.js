//Vermont-12.4.0-1133
var rsi_now= new Date();
var rsi_csid= 'E05510';if(typeof(csids)=="undefined"){var csids=[rsi_csid];}else{csids.push(rsi_csid);};function rsiClient(Da){this._rsiaa=Da;this._rsiba=1;this._rsica=1;this._rsida=0;this._rsiea=0;this._rsifa=0;this._rsiga="1003161";this._rsiha="pix04.revsci.net";this._rsiia="js";this._rsija="b";this._rsika="3";this._rsila=3;this._rsima=1;this._rsina=new Array();this._rsioa=0;this._rsipa=null;this._rsiqa=null;this._rsira=null;this._rsisa=null;this._rsita=null;this._rsiua=null;this._rsiva=0;this.DM_cat=function(Ea){this._rsipa=Ea;};this.DM_name=function(Fa){this._rsiqa=Fa;};this.DM_keywords=function(st){this._rsira=st;};this.DM_event=function(Ga){this._rsisa=Ga;};this.DM_addToLoc=function(n,v){this._rsita=_rsiwa(this._rsita,n,v);};this.DM_addEncToLoc=function(n,v){this.DM_addToLoc(_rsixa(n),_rsixa(v));};this.DM_setLoc=function(u){this._rsita=u;};this.rsi_c=function(Da){this._rsiaa=Da;};this.rsi_ral=function(Ha){this._rsiba=Ha;};this.rsi_riu=function(Ia){this._rsica=Ia;};this.rsi_tiu=function(Ja){this._rsida=Ja;};this.rsi_m=function(Ka){this._rsiea=Ka;};this.rsi_dw=function(La){this._rsifa=La;};this.rsi_s=function(Ma){this._rsiha=Ma;};this.rsi_t=function(Na){this._rsiia=Na;};this.rsi_en=function(Oa){this._rsija=Oa;};this.rsi_cn=function(Pa){this._rsika=Pa;};this.rsi_us=function(Qa){this._rsila=Qa;};this.rsi_ra=function(ra){this._rsima=ra;};this.DM_tag=function(){var Ra;if(this._rsioa==0||this._rsiea==1){if(typeof(DM_prepClient)=="function"){DM_prepClient(this._rsiaa,this);}var Sa=this._rsiya();if(this._rsiia=="gif"){Ra=new Image(2,3);Ra.src=Sa;this._rsina[this._rsina.length]=Ra;}else if(this._rsiia=="js"){if(this._rsifa==1){document.write("<script language=\"JavaScript\" type=\"text/javascript\" src=\""+Sa+"\"><"+"/script>");}else{var Ta=document.createElement("script");Ta.language="JavaScript";Ta.type="text/javascript";Ta.src=Sa;if(document.body==null){document.getElementsByTagName("head")[0].appendChild(Ta);}else{document.body.insertBefore(Ta,document.body.firstChild);}Ra=Ta;}}this._rsioa=1;}this.rsi_r();return Ra;};this._rsiya=function(){var Ua="";this.DM_addEncToLoc("_rsiL",this._rsiva);Ua="DM_LOC="+_rsixa(this._rsita);if(this._rsipa){Ua+="&DM_CAT="+_rsixa(this._rsipa);}if(this._rsisa){Ua+="&DM_EVT="+_rsixa(this._rsisa);}if(this._rsira){Ua+="&DM_KYW="+_rsixa(this._rsira);}if(this._rsica==1&&this._rsiua){Ua+="&DM_REF="+_rsixa(this._rsiua);}if(this._rsida==1){Ua+="&DM_TIT="+_rsixa(document.title);}if(this._rsiqa){Ua+="&DM_NAM="+_rsixa(this._rsiqa);}Ua+="&DM_EOM=1";var Va="http"+(location.protocol=="https:"?"s":"")+"://";var Wa="/"+this._rsiaa+"/"+this._rsija+this._rsika+"/0/"+this._rsila+"/"+this._rsiga+"/";var Xa=Math.floor(Math.random()*1000000000)+"."+this._rsiia;var Ya=Va+this._rsiha+Wa+Xa+"?D="+_rsixa(Ua)+"&C="+_rsixa(csids);var Za=Ya.length;if(Za>=2000){if(Ya.charAt(1998)=='%'){Ya=Ya.substr(0,1998);}else if(Ya.charAt(1999)=='%'){Ya=Ya.substr(0,1999);}else{Ya=Ya.substr(0,2000);}if(Ya.charAt(Ya.length-3)=='%'&&Ya.charAt(Ya.length-2)=='2'&&Ya.charAt(Ya.length-1)=='5'){Ya=Ya.substr(0,Ya.length-3);}}return Ya;};this.rsi_r=function(){var $a;var ab;var bb=0;var cb=0;if(this._rsiba==1){var db=window;while(true){try{$a=db.document.location;ab=db.document.referrer;bb=cb;}catch(notAllowed){}if(db==window.top||db==db.parent){break;}db=db.parent;cb++;}}else{$a=window.document.location;ab=window.document.referrer;}this._rsiva=cb-bb;this._rsiua=this._rsima?_rsiza(ab.toString()):ab.toString();if(this._rsiva==0){this._rsita=(this._rsima)?_rsiza($a.href):$a.href;}else{this._rsita=this._rsiua;}this._rsipa=null;this._rsiqa=null;this._rsira=null;this._rsisa=null;};this.rsi_r();}var _rsixa;if(typeof(encodeURIComponent)=="function"){_rsixa=encodeURIComponent;}else{var _rsiAa=new RegExp("[\x00-\x20]|[\x22-\x26]|[\x2B-\x2C]|\x2F|[\x3A-\x40]|[\x5B-\x5E]|\x60|[\x7B-\x7D]|[\x7F-\uFFFF]","g");_rsixa=function(v){return v.toString().replace(_rsiAa,_rsiBa);}}function _rsiwa(u,n,v){return u+(u.indexOf("?")==-1?"?":"&")+n+"="+v;}function _rsiza(u){var i=u.indexOf('#');return(i>=0)?u.substr(0,i):u;}function _rsiCa(i){var eb=i.toString(16).toUpperCase();return eb.length<2?"0"+eb:eb;}function _rsiBa(c){var i=c.charCodeAt(0);if(isNaN(i))return "";if(i<128)return "%"+_rsiCa(i);if(i<2048)return "%"+_rsiCa(0xC0+(i>>6))+"%"+_rsiCa(0x80+(i&0x3F));if(i<65536)return "%"+_rsiCa(0xE0+(i>>12))+"%"+_rsiCa(0x80+(i>>6&0x3F))+"%"+_rsiCa(0x80+(i&0x3F));return "%"+_rsiCa(0xF0+(i>>18))+"%"+_rsiCa(0x80+(i>>12&0x3F))+"%"+_rsiCa(0x80+(i>>6&0x3F))+"%"+_rsiCa(0x80+(i&0x3F));}window[rsi_csid]=new rsiClient(rsi_csid);
function DM_cat(aa){window[rsi_csid].DM_cat(aa);}function DM_name(ba){window[rsi_csid].DM_name(ba);}function DM_keywords(kw){window[rsi_csid].DM_keywords(kw);}function DM_event(ca){window[rsi_csid].DM_event(ca);}function DM_addToLoc(n,v){window[rsi_csid].DM_addToLoc(n,v);}function DM_addEncToLoc(n,v){window[rsi_csid].DM_addEncToLoc(n,v);}function DM_setLoc(u){window[rsi_csid].DM_setLoc(u);}function DM_tag(){window[rsi_csid].DM_tag();}
if(window[rsi_csid])window[rsi_csid].DM_tag();else DM_tag();
function asi_addElem(e){var p=document.body==null?document.getElementsByTagName('head')[0]:document.body;p.insertBefore(e,p.firstChild);}
function asi_makeIFrame(u,h,w){var e=document.createElement("iframe");e.height=h?h:0;e.width=w?w:0;e.frameBorder=0;e.src=u;return e;}
function asi_makeJS(u){var e=document.createElement("script");e.language="JavaScript";e.type="text/javascript";e.src=u;return e;}
function asi_makeGIF(u){var i=new Image(2,2);i.src=u;return i;}
function asi_scheme(){return "http"+(location.protocol=="https:"?"s":"");}

if(location.protocol=='http:'){asi_makeGIF('http://cm.g.doubleclick.net/pixel?nid=audsci');}
asi_addElem(asi_makeJS('' + location.protocol + '//ads.revsci.net/adserver/ako?activate&csid=E05510'));
