<?php

#
# Surrogafier v1.0-rc2
#
# Author: Brad Cable
# Email: brad@bcable.net
# License: Modified BSD
# License Details:
# http://bcable.net/license.php
#


# CONFIG {{{

# Default to simple mode when the page is loaded. [false]
define('DEFAULT_SIMPLE',false);
# Force the page to always be in simple mode (no advanced mode option). [false]
define('FORCE_SIMPLE',false);
# Width for the URL box when in simple mode (CSS "width" attribute). [300px]
define('SIMPLE_MODE_URLWIDTH','300px');

# Default value for tunnel server. []
define('DEFAULT_TUNNEL_PIP','');
# Default value for tunnel port. []
define('DEFAULT_TUNNEL_PPORT','');
# Should the tunnel fields be displayed? "false" value here will force the defaults above [true]
define('FORCE_DEFAULT_TUNNEL',true);

# Default value for "Persistent URL" checkbox [true]
define('DEFAULT_URL_FORM',true);
# Default value for "Remove Cookies" checkbox [false]
define('DEFAULT_REMOVE_COOKIES',false);
# Default value for "Remove Referer Field" checkbox [false]
define('DEFAULT_REMOVE_REFERER',false);
# Default value for "Remove Scripts" checkbox [false]
define('DEFAULT_REMOVE_SCRIPTS',false);
# Default value for "Remove Objects" checkbox [false]
define('DEFAULT_REMOVE_OBJECTS',false);
# Default value for "Encrypt URLs" checkbox [false]
define('DEFAULT_ENCRYPT_URLS',false);
# Default value for "Encrypt Cookies" checkbox [false]
define('DEFAULT_ENCRYPT_COOKS',false);

/*/ Address Blocking Notes \*\

Formats for address blocking are as follows:

  1.2.3.4     - plain IP address
  1.0.0.0/16  - subnet blocking
  1.0/16      - subnet blocking
  1/8         - subnet blocking
  php.net     - domain blocking

Default Value: '10/8','172/8','192.168/16','127/8','169.254/16'

\*\ End Address Blocking Notes /*/

$blocked_addresses=array('10/8','172/8','192.168/16','127/8','169.254/16');

# }}}

# ADVANCED CONFIG {{{

# The following options alter the way documents are parsed on the page.  ONLY EDIT THIS STUFF IF YOU REALLY KNOW WHAT YOU ARE DOING!

# 500 is the most reasonable number I could come up with as a maximum URL length limit
# I ran into a 1200+ character long URL once and it nearly melted the processor on my laptop trying to parse it
# Honestly, who needs this long of a URL anyway?
define('MAXIMUM_URL_LENGTH',500);

# Time limit in seconds for a single request and parse. [10]
define('TIME_LIMIT',10);
# Time limit in minutes for a DNS entry to be kept in the cache. [10]
define('DNS_CACHE_EXPIRE',10);

# Use gzip (if possible) to compress the connection between the proxy and the user (less bandwidth, more CPU) [false]
define('GZIP_PROXY_USER',false);
# Use gzip (if possible) to compress the connection between the proxy and the server (less bandwidth, more CPU) [false]
define('GZIP_PROXY_SERVER',false);

# Protocol that proxy is running on.  Uncomment this line to define it manually.
# If you leave this line commented, the code detects if you are running on an
# HTTPS connection.  If you are, then 'https' is used as the PROTO value,
# otherwise 'http' is used.  If you need a different value here, then define it.
#define('PROTO','http');

# }}}


// DON'T EDIT ANYTHING AFTER THIS POINT \\


#
# (unless you absolutely know what you are doing...)
#

# COOKIE & SESSION SETUP {{{

//$totstarttime=microtime(true); # BENCHMARK
//$blocked_addresses=array(); # DEBUG

# set error level to not display notices
error_reporting(E_ALL^E_NOTICE);

# set time limit to the defined time limit, if not in safe mode
if(!ini_get('safe_mode')) set_time_limit(TIME_LIMIT);

# use gzip compression if available
if(GZIP_PROXY_USER && extension_loaded('zlib') && !ini_get('zlib.output_compression')) ob_start('ob_gzhandler'); # use gzip encoding to compress all data, if possible

# reverse magic quotes if enabled
if(get_magic_quotes_gpc()){
	function stripslashes_recurse($var){
		if(is_array($var)) $var=array_map('stripslashes_recurse',$var);
		else $var=stripslashes($var);
		return $var;
	}
	$_GET=stripslashes_recurse($_GET);
	$_POST=stripslashes_recurse($_POST);
	$_COOKIE=stripslashes_recurse($_COOKIE);
}

# script environment constants
if(!defined('PROTO')) define('PROTO',($_SERVER['HTTPS']=='on'?'https':'http'));
define('VERSION','1.0-rc2');
define('THIS_SCRIPT',PROTO."://{$_SERVER['HTTP_HOST']}{$_SERVER['PHP_SELF']}");
define('SIMPLE_MODE',DEFAULT_SIMPLE || FORCE_SIMPLE);

# Randomized cookie prefixes #
function gen_randstr($len){
	$chars=null;
	for($i=0;$i<$len;$i++){
		$char=rand(0,25);
		$char=chr($char+97);
		$chars.=$char;
	}
	return $chars;
}

function dosetcookie($cookname,$cookval,$expire=null){
	$_COOKIE[$cookname]=$cookval;
	if($expire===null) setcookie($cookname,$cookval);
	else setcookie($cookname,$cookval,$expire);
}

define('FIRST_LOAD',empty($_COOKIE['PHPSESSID']));
session_start();
if(empty($_SESSION['sesspref'])){
	$sesspref=gen_randstr(30);
	$_SESSION['sesspref']=$sesspref;
}
else $sesspref=$_SESSION['sesspref'];

if(empty($_COOKIE['user'])){
	$cookpref=gen_randstr(12);
	dosetcookie('user',$cookpref);
}
else $cookpref=$_COOKIE['user'];

define('SESS_PREF',$sesspref);
define('COOK_PREF',$cookpref);
define('COOKIE_SEPARATOR','__'.COOK_PREF.'__');
unset($sesspref,$cookpref);

if(FIRST_LOAD){
	if(DEFAULT_URL_FORM) dosetcookie(COOK_PREF.'_url_form',true);
	if(DEFAULT_REMOVE_COOKIES) dosetcookie(COOK_PREF.'_remove_cookies',true);
	if(DEFAULT_REMOVE_REFERER) dosetcookie(COOK_PREF.'_remove_referer',true);
	if(DEFAULT_REMOVE_SCRIPTS) dosetcookie(COOK_PREF.'_remove_scripts',true);
	if(DEFAULT_REMOVE_OBJECTS) dosetcookie(COOK_PREF.'_remove_objects',true);
	if(DEFAULT_ENCRYPT_URLS) dosetcookie(COOK_PREF.'_encrypt_urls',true);
	if(DEFAULT_ENCRYPT_COOKS) dosetcookie(COOK_PREF.'_encrypt_cooks',true);
}

# }}}

# ENVIRONMENT SETUP {{{

global $postandget,$blocked_addresses,$dns_cache_array;
$postandget=array_merge($_GET,$_POST);

define('PAGETYPE_MINIREGEXP','(=[_\.\-]?\&=|=)?');
define('PAGETYPE_REGEXP','/^'.PAGETYPE_MINIREGEXP.'(.*)$/');
if(!empty($postandget[COOK_PREF])) $oenc_url=$postandget[COOK_PREF];
else{
	$pagetype_str=preg_replace(PAGETYPE_REGEXP,'\1',$_SERVER['QUERY_STRING']);
	define('QUERY_STRING',substr($_SERVER['QUERY_STRING'],strlen($pagetype_str),strlen($_SERVER['QUERY_STRING'])-strlen($pagetype_str)));
	define('PAGETYPE_NULL',0);
	define('PAGETYPE_FORCE_MAIN',1);
	define('PAGETYPE_FRAME_TOP',2);
	define('PAGETYPE_FRAMED_PAGE',3);
	define('PAGETYPE_FRAMED_CHILD',4); # framing children for crimes isn't very nice, but the script does it anyway
	switch($pagetype_str){
		case '=&=': define('PAGETYPE_ID',PAGETYPE_FRAME_TOP); break;
		case '=_&=': define('PAGETYPE_ID',PAGETYPE_FRAMED_PAGE); break;
		case '=-&=': define('PAGETYPE_ID',PAGETYPE_FORCE_MAIN); break;
		case '=.&=': define('PAGETYPE_ID',PAGETYPE_FRAMED_CHILD); break;
# this is one more unencoded string for future features
#		case '=*&=': define('PAGETYPE_ID',); break;
		default: define('PAGETYPE_ID',PAGETYPE_NULL); break;
	}
	unset($pagetype_str);

	define('NEW_PAGETYPE_FRAME_TOP',(PAGETYPE_ID===PAGETYPE_FRAMED_CHILD?PAGETYPE_FRAMED_CHILD:PAGETYPE_FRAME_TOP));
	define('NEW_PAGETYPE_FRAMED_PAGE',(PAGETYPE_ID===PAGETYPE_FRAMED_CHILD?PAGETYPE_FRAMED_CHILD:PAGETYPE_FRAMED_PAGE));

	$oenc_url=QUERY_STRING;
	//define('OENC_URL',urldecode(preg_replace('/^([^&]*).*?$/i','\1',QUERY_STRING)));
}
if(strpos(substr($oenc_url,0,6),'%')!==false || strpos($oenc_url,'%')<strpos($oenc_url,'/') || strpos($oenc_url,'%')<strpos($oenc_url,':')) $oenc_url=urldecode($oenc_url);
define('OENC_URL',preg_replace('/^([^\?\&]+)\&/i','\1?',$oenc_url));
unset($oenc_url);
define('ORIG_URL',proxdec(OENC_URL));
global $curr_url;
$curr_url=ORIG_URL;

function gethardattr($attr){
	global $postandget;
	return (empty($postandget[COOK_PREF.'_set_values'])?!empty($_COOKIE[COOK_PREF."_{$attr}"]):!empty($postandget[COOK_PREF."_{$attr}"]));
}
define('ENCRYPT_URLS',gethardattr('encrypt_urls'));
define('URL_FORM',gethardattr('url_form'));
define('PAGE_FRAMED',(PAGETYPE_ID===PAGETYPE_FRAMED_PAGE || PAGETYPE_ID===PAGETYPE_FRAMED_CHILD || QUERY_STRING=='js_regexps_framed' || QUERY_STRING=='js_funcs_framed'));
#define('URLVAR',(ENCRYPT_URLS?'e':null).'url');

# }}}

# PHP DECODING FUNCTIONS {{{

function my_base64_decode($string){ return base64_decode(str_replace(' ','+',urldecode($string))); }

function proxdec($url){
	if($url{0}!='~' && strtolower(substr($url,0,3))!='%7e') return $url;
	#while(strpos($url,'%')!==false) $url=urldecode($url);
	#$url=urldecode($url);
	while($url{0}=='~' || strtolower(substr($url,0,3))=='%7e'){
		$url=substr($url,1);
		$url=my_base64_decode($url);
		$new_url=null;
		for($i=0;$i<strlen($url);$i++){
			$char=ord($url{$i});
			$char-=ord(substr(SESS_PREF,$i%strlen(SESS_PREF),1));
			while($char<32) $char+=94;
			$new_url.=chr($char);
		}
		$url=$new_url;
	}
	return urldecode($url);
}

# }}}

# JAVASCRIPT ENCODING FUNCTIONS {{{

function js_proxenc(){ ?>
//<script>
<?php echo(COOK_PREF); ?>_pe={
expon:function(a,b){
	var num;
	if(b==0) return 1;
	num=a; b--;
	while(b>0){ num*=a; b--; }
	return num;
},

dectobin:function(){
	var dec=arguments[0],chars=arguments[1]||8,binrep="";
	for(j=chars-1;j>=0;j--){
		if(dec>=this.expon(2,j)){
			binrep+="1"; dec-=this.expon(2,j);
		}
		else binrep+="0";
	}
	return binrep;
},

bintodec:function(){
	var bin=arguments[0],chars=arguments[1]||8,dec=0;
	for(var j=0;j<chars;j++) if(bin.substring(j,j+1)=="1") dec+=this.expon(2,chars-1-j);
	return dec;
},

b64e:function(string){
	var encstr="",binrep="";
	var charbin,charnum;
	for(var i=0;i<string.length;i++){
		charnum=string.charCodeAt(i);
		binrep+=this.dectobin(charnum);
	}
	while(binrep.length%6) binrep+="00";
	for(var i=1;i*6<=binrep.length;i++){
		charbin=binrep.substring((i-1)*6,i*6);
		charnum=this.bintodec(charbin,6);
		if(charnum<=25) charnum+=65;
		else if(charnum<=51) charnum+=71;
		else if(charnum<=61) charnum-=4;
		else if(charnum==62) charnum=43;
		else if(charnum==63) charnum=47;
		encstr+=String.fromCharCode(charnum);
	}
	while(encstr.length%8) encstr+="=";
	return encstr;
},

proxenc:function(url){
	var new_url="";
	var charnum;
	if(url.substring(0,1)=="~" || url.substring(0,3).toLowerCase()=="%7e") return url;
	url=encodeURIComponent(url);
	var sess_pref="<?php echo(SESS_PREF); ?>";
	for(i=0;i<url.length;i++){
		charnum=url.charCodeAt(i);
		charnum+=sess_pref.charCodeAt(i%sess_pref.length);
		while(charnum>126) charnum-=94;
		new_url+=String.fromCharCode(charnum);
	}
	return "~"+encodeURIComponent(this.b64e(new_url));
}
}
<? }

# }}}

# FIRST PAGE DISPLAYED WHEN ACCESSING PROXY {{{

if(PAGETYPE_ID===PAGETYPE_FORCE_MAIN || (substr(QUERY_STRING,0,3)!='js_' && ORIG_URL==null)){

$useragentinfo=null;
if(preg_match('/linux/i',$_SERVER['HTTP_USER_AGENT'])) $useragentinfo.='Linux';
elseif(preg_match('/win(?:dows|32)/i',$_SERVER['HTTP_USER_AGENT'])) $useragentinfo.='Windows';
elseif(preg_match('/mac(?:intosh|_powerpc)/i',$_SERVER['HTTP_USER_AGENT'])) $useragentinfo.='Macintosh';
elseif(preg_match('/bsd/i',$_SERVER['HTTP_USER_AGENT'])) $useragentinfo.='BSD';
else $useragentinfo.='Unknown';

$useragentinfo.=' / ';

if(preg_match('/msie/i',$_SERVER['HTTP_USER_AGENT'])) $useragentinfo.='Internet Explorer';
elseif(preg_match('/firefox/i',$_SERVER['HTTP_USER_AGENT'])) $useragentinfo.='Firefox';
elseif(preg_match('/netscape/i',$_SERVER['HTTP_USER_AGENT'])) $useragentinfo.='Netscape';
elseif(preg_match('/konqueror/i',$_SERVER['HTTP_USER_AGENT'])) $useragentinfo.='Konqueror';
elseif(preg_match('/seamonkey/i',$_SERVER['HTTP_USER_AGENT'])) $useragentinfo.='SeaMonkey';
elseif(preg_match('/opera/i',$_SERVER['HTTP_USER_AGENT'])) $useragentinfo.='Opera';
else $useragentinfo.='Unknown';

$useragent_array=array(
	array(null,"Actual ({$useragentinfo})"),
	array('-1',' [ Don\'t Send ] '),
	array('Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.8.1) Gecko/20061024 Firefox/2.0','Windows XP / Firefox 2.0'),
	array('Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; SV1)','Windows XP / Internet Explorer 7'),
	array('Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)','Windows XP / Internet Explorer 6'),
	array('Opera/9.02 (Windows NT 5.1; U; en)','Windows XP / Opera 9.02'),
	array('Mozilla/5.0 (Macintosh; U; PPC Mac OS X; en-US; rv:1.8.1) Gecko/20061024 Firefox/2.0','Mac OS X / Firefox 2.0'),
	array('Mozilla/5.0 (Macintosh; U; PPC Mac OS X; en) AppleWebKit/521.25 (KHTML, like Gecko) Safari/521.24','Mac OS X / Safari 3.0'),
	array('Opera/9.02 (Macintosh; PPC Mac OS X; U; en)','Mac OS X / Opera 9.02'),
	array('Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.8.1) Gecko/20061024 Firefox/2.0','Linux / Firefox 2.0'),
	array('Opera/9.02 (X11; Linux i686; U; en)','Linux / Opera 9.02'),
	array('Mozilla/5.0 (compatible; Konqueror/3.5; Linux) KHTML/3.5.5 (like Gecko)','Linux / Konqueror 3.5.5'),
	array('Links (2.1pre19; Linux 2.6 i686; x)','Linux / Links (2.1pre19)'),
	array('Lynx/2.8.5rel.1','Any / Lynx 2.8.5rel.1'),
	array('Dillo/0.8.6','Any / Dillo 0.8.6'),
	array('Wget/1.10.2','Any / Wget 1.10.2'),
	array('1',' [ Custom ] <noscript><b>**</b></noscript>')
);

define('IPREGEXP','/^((?:[0-2]{0,2}[0-9]{1,2}\.){3}[0-2]{0,2}[0-9]{1,2})\:([0-9]{1,5})$/');

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" 
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html>
<head>
<title>Surrogafier</title>
<meta name="robots" content="index, nofollow" />
<style>
	body{font-family: bitstream vera sans, trebuchet ms}
	input{border: 1px solid #000000}
	select{border: 1px solid #000000}
	a{color: #000000}
	a:hover{text-decoration: none}
	.advanced_stuff{display: <?php echo(SIMPLE_MODE?'none':'table-row'); ?>}
	.simple_stuff{display: <?php echo(SIMPLE_MODE?'table-row':'none'); ?>}
	.url{width: <?php echo(SIMPLE_MODE?SIMPLE_MODE_URLWIDTH:'99%'); ?>}
	.signature{float: left}
<?php if(FORCE_SIMPLE){ ?>
	.noscript_stuff{display: none}
	.signature{text-align: center; float: none}
<?php } ?>
</style>
<?php if(!FORCE_SIMPLE){ ?><noscript><style>
	.advanced_stuff{display: table-row}
	.simple_stuff{display: none}
	.noscript_stuff{display: none}
	.noscripturl{width: 99%}
	.url{display: none}
	.signature{text-align: center; float: none}
</style></noscript><?php } ?>
<script language="javascript">
<!--

<?php js_proxenc(); ?>

function useragent_check(focus){
	if(document.getElementsByName('<?php echo(COOK_PREF); ?>_useragent')[0].value=='1'){
		document.getElementById('useragent_texttr').style.display="";
		if(focus) document.getElementById('<?php echo(COOK_PREF); ?>_useragenttext').focus();
	}
	else document.getElementById('useragent_texttr').style.display='none';
}

<?php if(!FORCE_SIMPLE){ ?>
advanced_mode=true;
function toggle_mode(){
	document.getElementById("mode_toggler").innerHTML=(advanced_mode?"Advanced Mode":"Simple Mode");
	var advanced_stuff=document.getElementsByTagName("tr");
	for(var i=1;i<=12;i++) advanced_stuff[i].style.display=(advanced_mode?"none":"");
	document.getElementById("simple_submit").style.display=(advanced_mode?"inline":"none");
	document.getElementById("url").style.width=(advanced_mode?"<?php echo(SIMPLE_MODE_URLWIDTH); ?>":"99%");
	advanced_mode=!advanced_mode;
	if(advanced_mode) useragent_check(false);
	setTimeout("document.getElementById('url').focus();",100);
}
<?php } ?>

function submit_code(){
	document.forms[0].<?php echo(COOK_PREF); ?>.disabled=false;
	if(document.forms[0].<?php echo(COOK_PREF); ?>_encrypt_urls.checked)
		document.forms[0].<?php echo(COOK_PREF); ?>.value=<?php echo(COOK_PREF); ?>_pe.proxenc(document.getElementById('url').value);
	else
		document.forms[0].<?php echo(COOK_PREF); ?>.value=document.getElementById('url').value;
	return true;
}

//-->
</script>
</head>
<body<?php echo(SIMPLE_MODE?' onload="toggle_mode();"':null); ?>>
<center>
<span style="font-size: 18pt; font-weight: bold; margin-bottom: 5px">Surrogafier</span>
<form method="post" onsubmit="return submit_code();" style="margin: 0px; padding: 0px">
<input type="hidden" name="<?php echo(COOK_PREF); ?>_set_values" value="1" />
<input type="hidden" name="<?php echo(COOK_PREF); ?>" disabled="disabled" />
<table>
<tr>
<td style="text-align: left">URL:&nbsp;&nbsp;</td>
<td>
	<input type="text" class="url" id="url" value="<?php echo(ORIG_URL); ?>" />
	<noscript><input type="text" class="noscripturl" name="<?php echo(COOK_PREF); ?>" id="url" value="<?php echo(ORIG_URL); ?>" /></noscript>
	<input type="submit" class="simple_stuff" id="simple_submit" value="Surrogafy" style="background-color: #F0F0F0" />
</td>
</tr>
<tr class="advanced_stuff"<?php if(FORCE_DEFAULT_TUNNEL){ ?> style="display: none"><?php } ?>
<td style="text-align: left">Tunnel Proxy:</td>
<td><table cellspacing="0" cellpadding="0">
<tr>
	<td style="width: 100%"><input type="text" name="<?php echo(COOK_PREF); ?>_pip" onkeyup="if(this.value.match(<?php echo(IPREGEXP); ?>)){ document.forms[0].<?php echo(COOK_PREF); ?>_pport.value=this.value.replace(<?php echo(IPREGEXP); ?>,'\$2'); this.value=this.value.replace(<?php echo(IPREGEXP); ?>,'\$1'); document.forms[0].<?php echo(COOK_PREF); ?>_pport.focus(); };" style="width: 100%; text-align: left" value="<?php echo(empty($_COOKIE[COOK_PREF.'_pip'])?DEFAULT_TUNNEL_PIP:$_COOKIE[COOK_PREF.'_pip']); ?>" /></td>
	<td style="width: 5px">&nbsp;&nbsp;</td>
	<td style="width: 50px"><input type="text" name="<?php echo(COOK_PREF); ?>_pport" maxlength="5" size="5" style="width: 50px" value="<?php echo(empty($_COOKIE[COOK_PREF.'_pport'])?DEFAULT_TUNNEL_PPORT:$_COOKIE[COOK_PREF.'_pport']); ?>" /></td>
</tr>
</table></td>
</tr>
<tr class="advanced_stuff">
<td style="text-align: left">User-Agent:</td>
<td><select name="<?php echo(COOK_PREF); ?>_useragent" style="width: 100%" onchange="useragent_check(true);">
<?php foreach($useragent_array as $useragent){ ?>
	<option value="<?php echo($useragent[0]); ?>"<?php if($_COOKIE[COOK_PREF.'_useragent']==$useragent[0]) echo ' selected="selected"'; ?>><?php echo($useragent[1]); ?></option>
<?php } ?>
</select></td>
</tr>
<tr class="advanced_stuff" id="useragent_texttr"<?php echo($_COOKIE[COOK_PREF.'_useragent']=='1'?null:' style="display: none"'); ?>>
	<td>&nbsp;</td>
	<td><input type="text" id="<?php echo(COOK_PREF); ?>_useragenttext" name="<?php echo(COOK_PREF); ?>_useragenttext" value="<?php echo($_COOKIE[COOK_PREF.'_useragenttext']); ?>" style="width: 99%" /></td>
</tr>
<tr class="advanced_stuff"><td>&nbsp;</td><td style="text-align: left"><input type="checkbox" name="<?php echo(COOK_PREF); ?>_url_form" style="border: 0px" <?php if(!empty($_COOKIE[COOK_PREF.'_url_form'])) echo 'checked="checked" '; ?>/>&nbsp;Persistent URL Form</td></tr>
<tr class="advanced_stuff"><td>&nbsp;</td><td style="text-align: left"><input type="checkbox" name="<?php echo(COOK_PREF); ?>_remove_cookies" style="border: 0px" <?php if(!empty($_COOKIE[COOK_PREF.'_remove_cookies'])) echo 'checked="checked" '; ?>/>&nbsp;Remove Cookies</td></tr>
<tr class="advanced_stuff"><td>&nbsp;</td><td style="text-align: left"><input type="checkbox" name="<?php echo(COOK_PREF); ?>_remove_referer" style="border: 0px" <?php if(!empty($_COOKIE[COOK_PREF.'_remove_referer'])) echo 'checked="checked" '; ?>/>&nbsp;Remove Referer Field</td></tr>
<tr class="advanced_stuff"><td>&nbsp;</td><td style="text-align: left"><input type="checkbox" name="<?php echo(COOK_PREF); ?>_remove_scripts" style="border: 0px" <?php if(!empty($_COOKIE[COOK_PREF.'_remove_scripts'])) echo 'checked="checked" '; ?>/>&nbsp;Remove Scripts (JS, VBS, etc)</td></tr>
<tr class="advanced_stuff"><td>&nbsp;</td><td style="text-align: left"><input type="checkbox" name="<?php echo(COOK_PREF); ?>_remove_objects" style="border: 0px" <?php if(!empty($_COOKIE[COOK_PREF.'_remove_objects'])) echo 'checked="checked" '; ?>/>&nbsp;Remove Objects (Flash, Java, etc)</td></tr>
<tr class="advanced_stuff"><td>&nbsp;</td><td style="text-align: left"><input type="checkbox" name="<?php echo(COOK_PREF); ?>_encrypt_urls" style="border: 0px" <?php if(!empty($_COOKIE[COOK_PREF.'_encrypt_urls'])) echo 'checked="checked" '; ?>/>&nbsp;Encrypt URLs<noscript><b>**</b></noscript></td></tr>
<tr class="advanced_stuff"><td>&nbsp;</td><td style="text-align: left"><input type="checkbox" name="<?php echo(COOK_PREF); ?>_encrypt_cooks" style="border: 0px" <?php if(!empty($_COOKIE[COOK_PREF.'_encrypt_cooks'])) echo 'checked="checked" '; ?>/>&nbsp;Encrypt Cookies<noscript><b>**</b></noscript></td></tr>
<tr class="advanced_stuff"><td colspan="2"><input type="submit" value="Surrogafy" style="width: 100%; background-color: #F0F0F0" /></td></tr>
<tr><td style="font-size: 8pt" colspan="2">
<div class="signature"><a href="http://bcable.net/">Surrogafier v<?php echo(VERSION); ?> <b>&middot;</b> Brad Cable</a></div>
<div class="noscript_stuff" style="float: right"><a href="#" onclick="toggle_mode();" id="mode_toggler"><?php echo(SIMPLE_MODE?'Advanced':'Simple'); ?> Mode</a></div>
</td></tr>
</table>
<noscript>
<br />
<b>**</b> Surrogafier has detected that your browser does not have Javascript enabled. <b>**</b>
<br />
<b>**</b> Surrogafier requires Javascript in order to function to its full potential. <b>**</b>
</noscript>
</form>
</center>
</body>
</html>

<?php exit(); }

# }}}

# FRAMED PAGE WITH URL FORM {{{
if(PAGETYPE_ID===PAGETYPE_FRAME_TOP && ORIG_URL!=null){ ?>
<html>
<head>
<title><?php echo(ORIG_URL); ?></title>
<style>
	body{font-family: bitstream vera sans, trebuchet ms; margin: 0px; padding: 0px; font-size: 12px; overflow: hidden}
	input{border: 1px solid #000000}
	td{font-size: 12px}
	a{text-decoration: none; color: #000000}
	a:hover{text-decoration: underline}
</style>
<script>
<!--

<?php echo(COOK_PREF); ?>=true;

<?php if(ENCRYPT_URLS) js_proxenc(); ?>

function submit_code(){
<?php if(ENCRYPT_URLS){ ?>
	document.forms[0].<?php echo(COOK_PREF); ?>.value=<?php echo(COOK_PREF); ?>_pe.proxenc(document.forms[0].<?php echo(COOK_PREF); ?>.value);
<?php } ?>
	return true;
}

//-->
</script>
</head>
<body>
<form method="get" onsubmit="return submit_code();">
<input type="hidden" name="" value="" />
<table cellpadding="0" cellspacing="0" style="width: 100%; height: 100%; padding: 0px; margin: 0px">
<tr><td><table cellpadding="0" cellspacing="0" style="width: 100%; padding: 3px">
<tr>
	<td>&nbsp;<b><a id="proxy_link" href="<?php echo(THIS_SCRIPT.'?=-&='.OENC_URL); ?>">Surrogafier</a></b>&nbsp;&nbsp;</td>
	<td style="width: 100%"><input type="text" class="url" name="" style="width: 100%; padding-left: 4px" id="url" value="<?php echo(ORIG_URL); ?>" /></td>
	<td>&nbsp;</td>
	<td><input type="submit" class="simple_stuff" id="simple_submit" value="Surrogafy" style="background-color: #F0F0F0" /></td>
</tr>
</table></td></tr>
<tr><td style="height: 100%; border-top: 1px solid #000000">
<iframe name="<?php echo(COOK_PREF); ?>_top" src="<?php echo(THIS_SCRIPT.'?=_&='.OENC_URL); ?>" frameborder="0" style="border: 0px; width: 100%; height: 100%"></iframe>
</td></tr>
</table>
</form>
</body>
</html>
<?php exit(); }

# }}}

# PRE-JAVASCRIPT CONSTANTS & FUNCTIONS {{{
# these constants and functions must be defined before JS is output, but would be more readably located later

#define('AURL_LOCK_REGEXP','(?:(?:javascript|mailto|about):|~|%7e)');
define('FRAME_LOCK_REGEXP','/^(?:(?:javascript|mailto|about):|#)/i');
define('AURL_LOCK_REGEXP','/^(?:(?:javascript|mailto|about):|#|'.str_replace(array('/','.'),array('\/','\.'),addslashes(THIS_SCRIPT)).')/i');
define('URLREG','/^'.
	'(?:([a-z]*)?(?:\:?\/\/))'.		# proto
	'(?:([^\@\/]*)\@)?'.			# userpass
	'([^\/:\?\#\&]*)'.			# servername
	'(?:\:([0-9]+))?'.			# portval
	'(\/[^\&\?\#]*?)?'.			# path
	'([^\/\?\#\&]*(?:\&[^\?\#]*)?)'.	# file
	'(?:\?([\s\S]*?))?'.			# query
	'(?:\#([\s\S]*))?'.			# label
'$/ix');

function escape_regexp($regexp,$dollar=false){
	$regexp=str_replace('\\','\\\\',str_replace('\'','\\\'',str_replace('"','\\"',str_replace(chr(10),'\n',str_replace(chr(13),'\r',str_replace(chr(9),'\t',$regexp))))));
	return ($dollar?preg_replace('/[\\\\]+(?=[0-9])/','\\\\$',$regexp):preg_replace('/[\\\\]+(?=[0-9])/','\\\\\\\\',$regexp)); #*
}

# }}}

# JAVASCRIPT FUNCS {{{

if(QUERY_STRING=='js_funcs' || QUERY_STRING=='js_funcs_framed'){ ?>//<script>

// JAVASCRIPT FUNCS: DECODING {{{
<?php js_proxenc(); ?>

<?php echo(COOK_PREF); ?>_pe.b64d=function(string){
	var binrep="",decstr="";
	var charnum,charbin;
	string=string.replace(/[=]*$/,"");
	for(var i=0;i<string.length;i++){
		charnum=string.charCodeAt(i);
		if(charnum>=97) charnum-=71;
		else if(charnum>=65) charnum-=65;
		else if(charnum>=48) charnum+=4;
		else if(charnum==43) charnum=62;
		else if(charnum==47) charnum=63;
		binrep+=this.dectobin(charnum,6);
	}
	for(var i=0;i+8<binrep.length;i+=8){
		charbin=binrep.substr(i,8);
		decstr+=String.fromCharCode(this.bintodec(charbin));
	}
	return decstr;
}

<?php echo(COOK_PREF); ?>_pe.proxdec=function(url){
	var new_url,charnum;
	if(url.substr(0,1)!='~' && url.substr(0,3).toLowerCase()!='%7e') return url;
	while(url.substr(0,1)=='~' || url.substr(0,3).toLowerCase()=='%7e'){
		url=url.substr(1,url.length-1);
		url=this.b64d(url);
		new_url="";
		for(i=0;i<url.length;i++){
			charnum=url.charCodeAt(i);
			charnum-="<?php echo(SESS_PREF); ?>".charCodeAt(i%"<?php echo(SESS_PREF); ?>".length);
			while(charnum<32) charnum+=94;
			new_url+=String.fromCharCode(charnum);
		}
		url=new_url;
	}
	return decodeURIComponent(url); // urldecode()
}

// }}}

// JAVASCRIPT FUNCS: COOK_PREF OBJECT {{{

<?php echo(COOK_PREF); ?>={

URLREG:<?php echo(substr(URLREG,0,strlen(URLREG)-1)); ?>,
THIS_SCRIPT:"<?php echo(THIS_SCRIPT); ?>",
COOK_PREF:"<?php echo(COOK_PREF); ?>",
pe:<?php echo(COOK_PREF); ?>_pe,
gen_curr_urlobj:function(){ this.curr_urlobj=new this.aurl(this.CURR_URL); },

getCookieArr:function(){ return document.cookie.split("; "); },

aurl:function(url,topurl){
	this.URLREG=<?php echo(COOK_PREF); ?>.URLREG;
	this.THIS_SCRIPT=<?php echo(COOK_PREF); ?>.THIS_SCRIPT;
	this.ENCRYPT_URLS=<?php echo(COOK_PREF); ?>.ENCRYPT_URLS;

	this.trim=function(str){ return str.replace(/^\s*([\s\S]*?)\s*$/,"$1"); }

	this.get_fieldreq=function(fieldno,value){
		var fieldreqs=new Array();
		fieldreqs[2]="://"+(value!=""?value+"@":"");
		fieldreqs[4]=(value!="" && parseInt(value)!=80?":"+parseInt(value):"");
		fieldreqs[7]=(value!=""?"?"+value:"");
		fieldreqs[8]=(value!=""?"#"+value:"");
		if(fieldreqs[fieldno]!=undefined) return value;
		// return (value!=""?null:value);
		else return fieldreqs[fieldno];
	}

	this.set_proto=function(proto){
		if(proto==undefined) proto="http";
		if(this.locked) return;
		this.proto=proto;
	}
	this.get_proto=function(){ return this.proto; }

	this.get_userpass=function(){ return this.userpass; }
	this.set_userpass=function(userpass){ if(userpass==undefined) userpass=""; this.userpass=userpass; }
	this.get_servername=function(){ return this.servername; }
	this.set_servername=function(servername){ if(servername==undefined) servername=""; this.servername=servername; }
	this.get_portval=function(){ return ((this.portval=="")?(this.get_proto()=="https"?"443":"80"):this.portval); }
	this.set_portval=function(port){ if(port==undefined) port=""; this.portval=((parseInt(port)!=80)?port:"").toString(); }
	this.get_path=function(){ // ***
		if(this.path.indexOf("/../")!=-1) this.path=this.path.replace(/(?:\/[^\/]+){0,1}\/\.\.\//g,"/");
		if(this.path.indexOf("/./")!=-1) while((path=this.path.replace("/./","/")) && path!=this.path) this.path=path;
		return this.path;
	}
	this.set_path=function(path){ if(path==undefined) path="/"; this.path=path; }
	this.get_file=function(){ return this.file; }
	this.set_file=function(file){ if(file==undefined) file=""; this.file=file; }
	this.get_query=function(){ return this.query; }
	this.set_query=function(query){ if(query==undefined) query=""; this.query=query; }
	this.get_label=function(){ return this.label; }
	this.set_label=function(label){ if(label==undefined) label=""; this.label=label; }

	this.get_url=function(){
		if(this.locked) return this.url;
		return this.get_proto()+"://"+
		       (this.get_userpass()==""?"":this.get_userpass()+"@")+
		       this.get_servername()+
		       (parseInt(this.get_portval())==80?"":":"+parseInt(this.get_portval()))+
		       this.get_path()+this.get_file()+
		       (this.get_query()==""?"":"?"+this.get_query())+
		       (this.get_label()==""?"":"#"+this.get_label())
		;
	}

	this.surrogafy=function(){
		var url=this.get_url();
		if(this.locked || this.get_proto()+this.get_fieldreq(2,this.get_userpass())+this.get_servername()+this.get_path()+this.get_file()==this.THIS_SCRIPT) return url;
		var label=this.get_label();
		this.set_label();
		if(this.ENCRYPT_URLS && !this.locked) url=<?php echo(COOK_PREF); ?>.pe.proxenc(url);
		//url=this.THIS_SCRIPT+"?="+(!this.ENCRYPT_URLS?escape(url):url); // urlencode()d
		url=this.THIS_SCRIPT+"?="+url; // urlencode()d
		this.set_label(label);
		return url;
	}

	if(url.length><?php echo(MAXIMUM_URL_LENGTH)?>){
		//alert(this.url); // DEBUG
		//alert(this.url.length); // DEBUG
		this.url="";
	}
	else{
		//this.url=preg_replace("/&#([0-9]+);/e","chr(\\1)" // parse like PHP does for &#num; HTML entities? // TODO?
		this.url=this.trim(url.replace("&amp;","&").replace("\r","").replace("\n",""));
	}

	this.topurl=topurl;
	this.locked=url.match(<?php echo(AURL_LOCK_REGEXP); ?>); //*

	if(!this.locked){
		var urlwasvalid=true;
		if(!this.url.match(this.URLREG)){
			urlwasvalid=false;
			if(this.topurl==undefined) this.url="http://"+((this.url.charAt(0)==":" || this.url.charAt(0)=="/")?this.url.substring(1):this.url)+(this.url.indexOf("/")!=-1?"":"/");
			else{
				var newurl=this.topurl.get_proto()+"://"+this.get_fieldreq(2,this.topurl.get_userpass())+this.topurl.get_servername()+((this.topurl.get_portval()!=80 && (this.topurl.get_proto()=="https"?this.topurl.get_portval()!=443:true))?":"+this.topurl.get_portval():"");
				if(this.url.substring(0,1)!="/") newurl+=this.topurl.get_path();
				this.url=newurl+this.url;
			}
		}

		this.set_proto((urlwasvalid || this.topurl==undefined?this.url.replace(/^([^:]+).*$/,"\$1"):this.topurl.get_proto()));
		this.set_userpass(this.url.replace(this.URLREG,"\$2"));
		this.set_servername(this.url.replace(this.URLREG,"\$3"));
		this.set_portval(this.url.replace(this.URLREG,"\$4"));
		this.set_path(this.url.replace(this.URLREG,"\$5"));
		this.set_file(this.url.replace(this.URLREG,"\$6"));
		this.set_query(this.url.replace(this.URLREG,"\$7"));
		this.set_label(this.url.replace(this.URLREG,"\$8"));
	}

	//if(!this.locked && !this.url.match(this.URLREG)) havok(7,this.url); //*
},

surrogafy_url:function(url,topurl,addproxy){
	url=url.toString();
	if(!url.substring) return;
	if(addproxy==undefined) addproxy=true;
	var urlquote="";
	if((url.substring(0,1)=="\"" || url.substring(0,1)=="'") && url.substring(0,1)==url.substring(url.length-1,url.length)){
		urlquote=url.substring(0,1);
		url=url.substring(1,url.length-1);
	}
	if(topurl==undefined) topurl=this.curr_urlobj;
	var urlobj=new this.aurl(url,topurl);
	var new_url=(addproxy?urlobj.surrogafy():urlobj.get_url());
	if(urlquote!="") new_url=urlquote+new_url+urlquote;
	return new_url;
},

surrogafy_url_toobj:function(url,topurl,addproxy){
	url=url.toString();
	if(!url.substring) return;
	if(addproxy==undefined) addproxy=true;
	if((url.substring(0,1)=="\"" || url.substring(0,1)=="'") && url.substring(0,1)==url.substring(url.length-1,url.length)) url=url.substring(1,url.length-1);
	if(topurl==undefined) topurl=this.curr_urlobj;
	return new this.aurl(url,topurl);
},

de_surrogafy_url:function(url){
	if(url==undefined) return "";
	url=url.toString();
	if(url.match(<?php echo(FRAME_LOCK_REGEXP); ?>) || !url.match(<?php echo(AURL_LOCK_REGEXP); ?>)) return url;
	return this.pe.proxdec(decodeURIComponent(url.substring(url.indexOf('?')+1).replace(<?php echo(PAGETYPE_REGEXP); ?>,"\$2"))); // urldecode()
},

add_querystuff:function(url,querystuff){
	var pos=url.indexOf('?');
	return url.substr(0,pos+1)+querystuff+url.substr(pos+1,url.length-pos);
},

preg_match_all:function(regexpstr,string){
	var matcharr=new Array();
	var regexp=new RegExp(regexpstr);
	var result;
	while(true){
		result=regexp.exec(string);
		if(result!=null) matcharr.push(result);
		else break;
	}
	return matcharr;
},

framify_url:function(url,frame_type){
	if((frame_type!==<?php echo(PAGETYPE_FRAME_TOP); ?> || !this.URL_FORM) && (frame_type!==<?php echo(PAGETYPE_FRAMED_PAGE); ?> && !this.PAGE_FRAMED)) return url;
	var urlquote="";
	if((url.substring(0,1)=="\"" || url.substring(0,1)=="'") && url.substring(0,1)==url.substring(url.length-1,url.length)){
		urlquote=url.substring(0,1);
		url=url.substring(1,url.length-1);
	}
	if(!url.match(<?php echo(FRAME_LOCK_REGEXP); ?>)){
		var query;
		if(frame_type===<?php echo(PAGETYPE_FRAME_TOP); ?> && this.URL_FORM) query='&=';
		else if(frame_type===<?php echo(PAGETYPE_FRAMED_CHILD); ?>) query='.&=';
		else if(frame_type===<?php echo(PAGETYPE_FRAMED_PAGE); ?> || this.PAGE_FRAMED) query='_&=';
		else query='';
		url=url.replace(/^([^\?]*)[\?]?<?php echo(PAGETYPE_MINIREGEXP); ?>([^#]*?[#]?.*?)$/,'\$1?='+query+'\$3');
	}
	if(urlquote!="") url=urlquote+url+urlquote;
	return url;
},

parse_html:function(regexp,partoparse,html,addproxy,framify){
	var match,begin,end,nurl;
	if(html.match(regexp)){
		var matcharr=this.preg_match_all(regexp,html);
		var newhtml="";
		for(var key in matcharr){
			/*match=matcharr[i];
			nurl=this.surrogafy_url(match[partoparse],undefined,addproxy);
			nhtml=match[0].replace(match[partoparse],nurl);
			html=html.replace(match[0],nhtml);*/
			match=matcharr[key];
			if(match[partoparse]!=undefined){
				begin=html.indexOf(match[partoparse]);
				end=begin+match[partoparse].length;
				nurl=this.surrogafy_url(match[partoparse],undefined,addproxy);
				if(framify) nurl=this.framify_url(nurl,framify);
				newhtml+=html.substring(0,begin)+nurl;
				html=html.substring(end);
			}
		}
		html=newhtml+html;
	}
	return html;
},

parse_all_html:function(){
	if(arguments[0]==null) return;
	var html=arguments[0].toString();
	var key;
	for(var key in regexp_arrays){
		if((arguments.length>1 && key!=arguments[1]) || key=='text/javascript') continue;
		arr=regexp_arrays[key];
		for(var regexp_arraykey in arr){
			regexp_array=arr[regexp_arraykey];
			if(regexp_array[0]==undefined) continue;
			if(regexp_array[0]==1) html=html.replace(regexp_array[1],regexp_array[2]);
			else if(regexp_array[0]==2){
				addproxy=(regexp_array.length>3?regexp_array[3]:true);
				framify=(regexp_array.length>4?regexp_array[4]:false);
				html=this.parse_html(regexp_array[1],regexp_array[2],html,addproxy,framify);
			}
		}
	}
	return html;
},

form_button:null,
form_encrypt:function(form){
	if(form.method=='post') return true;
	//action=form.<php echo(COOK_PREF); ?>.value;
	var action=form.getElementsByName(this.COOK_PREF)[0].value;
	for(var i=1;i<form.elements.length;i++){
		if(form.elements[i].disabled || form.elements[i].name=='' || form.elements[i].value=='' || form.elements[i].type=='reset') continue;
		if(form.elements[i].type=='submit'){
			if(form.elements[i].name!=this.form_button) continue;
			this.form_button=null;
		}
		var pref;
		if(!action.match(/\?/)) pref="?";
		else pref="&";
		action+=pref+form.elements[i].name+"="+form.elements[i].value;
	}
	location.href=this.surrogafy_url(action);
	return false;
},

setAttr:function(obj,attr,val){
	if(typeof(attr)!=typeof("")){
		attr=attr.toString();
		attr=attr.substr(1,attr.length-2);
	}

	if(attr=="innerHTML"){
		obj[attr]=this.parse_all_html(val);
		return obj[attr];
	}

	if(obj==location && attr=="hostname") return this.LOCATION_HOSTNAME;

	if(obj==document && attr=="cookie"){
		const COOK_REG=/^([^=]*)=([^;]*)(?:;[\s\S]*?)?$/i;
		var realhost=this.LOCATION_HOSTNAME.replace("/^www/i","").replace(".","_");
		var cookkey=val.replace(COOK_REG,"\$1");
		var cookval=val.replace(COOK_REG,"\$2");
		if(this.ENCRYPT_COOKS){
			cookkey=proxenc(cookkey);
			cookval=proxenc(cookval);
		}
		var newcookie=realhost+"<?php echo(COOKIE_SEPARATOR); ?>"+cookkey+"="+cookval+"; ";
		document.cookie=newcookie;
		return newcookie;
	}

	if(obj==location && attr=="search"){
		if(val.substr(0,1)=="?") val=val.substr(1);
		this.curr_urlobj.set_query(val);
		val=this.curr_urlobj.get_url();
		attr="href";
	}

	var proxurl=val;
	if(attr!="cookie" && attr!="search" && attr!="hostname"){
		proxurl=this.surrogafy_url(val);

		// tags framified  must match REGEXPS with regexp_array[5]
		if(obj.tagName=="A" || obj.tagName=="AREA")
			proxurl=this.framify_url(proxurl,<?php echo(NEW_PAGETYPE_FRAME_TOP); ?>);
		else if(obj.tagName=="FRAME" || obj.tagName=="IFRAME")
			proxurl=this.framify_url(proxurl,<?php echo(PAGETYPE_FRAMED_CHILD); ?>);
	}

	if(this.URL_FORM){
		if((obj==location && attr=="href") || attr=="location"){
			urlobj=this.surrogafy_url_toobj(val);
			if(!urlobj.locked) proxurl=this.add_querystuff(proxurl,"=&");
			this.thetop.location.href=proxurl;
		}
		else obj[attr]=proxurl;
	}
	else obj[attr]=proxurl;
},

getAttr:function(obj,attr){
	if(typeof(attr)!=typeof("")){
		attr=attr.toString();
		attr=attr.substr(1,attr.length-2);
	}

	if(obj==document && attr=="cookie"){
		var ocookies=this.getCookieArr();
		var cookies="",ocook;
		const COOK_REG=/^([\s\S]*)<?php echo(COOKIE_SEPARATOR); ?>([^=]*)=([\s\S]*)(?:; )?$/i;
		for(var key in ocookies){
			ocook=ocookies[key];
			if(typeof(ocook)!=typeof("")) continue;
			if(ocook.match(COOK_REG)==null) continue;
			var realhost=this.LOCATION_HOSTNAME.replace("/^www/i","").replace(".","_");
			var cookhost=ocook.replace(COOK_REG,"\$1");
			if(cookhost==realhost){
				if(this.ENCRYPT_COOKS){
					var cookkey=this.pe.proxdec(ocook.replace(COOK_REG,"\$2"));
					var cookval=this.pe.proxdec(ocook.replace(COOK_REG,"\$3"));
					cookies+=cookkey+"="+cookval+"; ";
				}
				else cookies+=ocook.replace(COOK_REG,"\$2=\$3; ");
			}
		}
		return cookies;
	}

	if(obj==navigator){
		if(this.USERAGENT=="-1" && (attr!="plugins" && attr!="mimeType")) return undefined;
		if(this.USERAGENT=="") return obj[attr];
		var msie=this.USERAGENT.match(/msie/i);
		const UA_REG=/^([^\/\(]*)\/?([^ \(]*)[ ]*(\(?([^;\)]*);?([^;\)]*);?([^;\)]*);?([^;\)]*);?([^;\)]*);?[^\)]*\)?)[ ]*([^ \/]*)\/?([^ \/]*).*$/i;
		switch(attr){
			case "userAgent": return this.USERAGENT;
			case "appCodeName": return this.USERAGENT.replace(UA_REG,"\$1");
			case "appVersion": return (msie?this.USERAGENT.replace(UA_REG,"\$2 \$3"):this.USERAGENT.replace(UA_REG,"\$2 (\$4; \$7)"));
			case "platform":
				var tempplatform=this.USERAGENT.replace(UA_REG,"\$4");
				return (tempplatform=="compatible" || tempplatform=="Windows"?"Win32":this.USERAGENT.replace(UA_REG,"\$6"));
			case "oscpu": return (msie?undefined:this.USERAGENT.replace(UA_REG,"\$6"));
			case "language": return (msie?undefined:this.USERAGENT.replace(UA_REG,"\$7"));
			case "appName":
				var tempappname=(msie?"Microsoft Internet Explorer":this.USERAGENT.replace(UA_REG,"\$1"));
				if(tempappname=="Opera" || tempappname=="Mozilla") tempappname="Netscape";
				return tempappname;
			case "product": return (msie?undefined:this.USERAGENT.replace(UA_REG,"\$9"));
			case "productSub": return (msie?undefined:this.USERAGENT.replace(UA_REG,"\$10"));
			case "plugins": return (<?php echo((empty($_COOKIE[COOK_PREF.'_remove_objects'])?'1':'0')); ?>==1?navigator.plugins:undefined);
			case "mimeType": return navigator.mimeType;
			default: return undefined;
		}
	}

	if(obj==location && attr=="search") url=location.href;
	else url=obj[attr];
	url=this.de_surrogafy_url(url);
	if(obj==location && attr=="search") url=url.replace(/^[^?]*/,"");
	return url;
},

eventify:function(a1,a2){
	document.getElementsByTagName("head")[0].addEventListener("load",function(){<?php echo(COOK_PREF); ?>.setParentStuff(a1,a2);},false);
	window.addEventListener("load",function(){<?php echo(COOK_PREF); ?>.setParentStuff(a1,a2);},false);
	this.setParentURL(this.CURR_URL);
},

setParentURL:function(url){
	if(this.thetop!=null && this.thetop!=window){
		this.thetop.document.getElementById('url').value=url;
		this.thetop.document.getElementById('proxy_link').href=this.add_querystuff(this.surrogafy_url(url),"=-&");
	}
},

setParentStuff:function(proto,server){ // amazing creativity with the name on my part
	var topdoc=this.thetop.document;
	topdoc.title=document.title;

	// find and set shortcut icon
	var tophead=topdoc.getElementsByTagName("head")[0];
	var links=tophead.getElementsByTagName("link");
	var link=null;
	for(var i=0; i<links.length; i++){ if(links[i].type=="image/x-icon" && links[i].rel=="shortcut icon") link=links[i]; }

	if(tophead.getElementsByTagName("link").length>0) tophead.removeChild(topdoc.getElementsByTagName("link")[0]);

	var favicon=topdoc.createElement("link");
	favicon.type="image/x-icon";
	favicon.rel="shortcut icon";
	favicon.href=(link==null?this.surrogafy_url(proto+"://"+server+"/favicon.ico"):link.href);
	tophead.appendChild(favicon);
},

XMLHttpRequest_wrap:function(xmlhttpobj){
	xmlhttpobj.<?php echo(COOK_PREF); ?>_open=xmlhttpobj.open;
	xmlhttpobj.open=<?php echo(COOK_PREF); ?>.XMLHttpRequest_open;
	return xmlhttpobj;
},

XMLHttpRequest_open:function(){
	if(arguments.length<2) return;
	arguments[1]=<?php echo(COOK_PREF); ?>.surrogafy_url(arguments[1]);
	return this.<?php echo(COOK_PREF); ?>_open.apply(this,arguments);
},

// WRAPPED FUNCTIONS AND OBJECTS
thetop:top,
theparent:parent,
setTimeout:window.setTimeout,
setInterval:window.setInterval,
document_write_queue:"",
purge:function(){
	thehtml=this.document_write_queue;
	if(thehtml=="") return;
	thehtml=this.parse_all_html(thehtml);
	this.document_write_queue="";
	//alert(thehtml); // DEBUG
	document.write_<?php echo(COOK_PREF); ?>(thehtml);
},

purge_noparse:function(){
	thehtml=this.document_write_queue;
	if(thehtml=="") return;
	this.document_write_queue="";
	document.write_<?php echo(COOK_PREF); ?>(thehtml);
}

}

// }}}

// JAVASCRIPT FUNCS: WRAPPING {{{

document.write_<?php echo(COOK_PREF); ?>=document.write;
document.writeln_<?php echo(COOK_PREF); ?>=document.writeln;
document.write=function(html){ <?php echo(COOK_PREF); ?>.document_write_queue+=html; }
document.writeln=function(html){ <?php echo(COOK_PREF); ?>.document_write_queue+=html+"\n"; }

window.open_<?php echo(COOK_PREF); ?>=window.open;
window.open=document.open=function(){
	if(arguments.length<1) return;
	var url=<?php echo(COOK_PREF); ?>.surrogafy_url(arguments[0]);
	if((url.substring(0,1)=="\"" || url.substring(0,1)=="'") && url.substring(0,1)==url.substring(url.length-1,url.length)) url=url.substring(1,url.length-1);
	arguments[0]=url;
	return window.open_<?php echo(COOK_PREF); ?>.apply(this.caller,arguments);
}

setTimeout=function(){
	if(arguments.length<2) return;
	arguments[0]=<?php echo(COOK_PREF); ?>.parse_all_html(arguments[0],"application/x-javascript");
	return <?php echo(COOK_PREF); ?>.setTimeout.apply(this,arguments);
}

setInterval=function(){
	if(arguments.length<2) return;
	arguments[0]=<?php echo(COOK_PREF); ?>.parse_all_html(arguments[0],"application/x-javascript");
	return <?php echo(COOK_PREF); ?>.setInterval.apply(this,arguments);
}

/* hooking for eval(), not necessary anymore, but worked relatively well in the past
/*eval_<?php echo(COOK_PREF); ?>=eval;
eval=function(){
	if(arguments.length<1) return;
	arguments[0]=<?php echo(COOK_PREF); ?>.parse_all_html(arguments[0],"application/x-javascript");
	return eval_<?php echo(COOK_PREF); ?>.apply(this.caller,arguments);
}*/

// wrap top and parent objects for anti-frame breaking
if(<?php echo(COOK_PREF); ?>.PAGE_FRAMED){
	if(parent==top) parent=self;
	if(top!=self) top=<?php echo(COOK_PREF); ?>.thetop.frames[0];
}

// }}}

//</script><?php exit(); }

# }}}

# REGEXPS {{{

# This is where all the parsing is defined.  If a site isn't being
# parsed properly, the problem is more than likely in this section.
# The rest of the code is just there to set up this wonderful bunch
# of incomprehensible regular expressions.


# REGEXPS: CONVERSION TO JAVASCRIPT {{{

function bool_to_js($bool){ return ($bool?'true':'false'); }
function convertarray_to_javascript(){
	global $regexp_arrays;
	$js='regexp_arrays=new Array('.count($regexp_arrays).");\n";
	reset($regexp_arrays);
	while(list($key,$arr)=each($regexp_arrays)){
		$js.="regexp_arrays[\"$key\"]=new Array(".count($arr).");\n";
		for($i=0;$i<count($arr);$i++){
			$js.="regexp_arrays[\"$key\"][$i]=new Array(";
			if($arr[$i][0]==1) $js.='1,'.escape_regexp($arr[$i][2]).'g,"'.escape_regexp($arr[$i][3],true).'"';
			elseif($arr[$i][0]==2) $js.='2,'.escape_regexp($arr[$i][2])."g,{$arr[$i][3]}".(count($arr[$i])<5?null:','.bool_to_js($arr[$i][4])).(count($arr[$i])<6?null:",{$arr[$i][5]}");
			$js.=");\n";
		}
	}
	return stripslashes($js);
}

# }}}

# REGEXPS: VARIABLES {{{

global $regexp_arrays;

# 'img' was in $jsattrs... what's that for?
$jsattrs='(?:href|src|location|action|backgroundImage|pluginspage|codebase|location\.href|innerHTML)';
$jshookattrs="(?:{$jsattrs}|cookie|search|hostname)";
$jshookgetattrs="(?:{$jshookattrs}|userAgent|platform|appCodeName|appName|appVersion|language|oscpu|product|productSub|plugins)";
//$jshtmlattrs='(innerHTML)';
$jsmethods='(location\.(?:replace|assign))';
$jslochost='(location\.host(?:name){0,1})';
//$jslocsearch='(location\.search)';
//$jsrealpage='((?:(?:document|window)\.){0,1}location(?:(?=[^\.])|\.href)|document\.documentURI|[a-z]+\.referrer)';
$htmlattrs='(data|href|src|background|pluginspage|codebase|action)';

$justspace="[\t ]*";
$plusjustspace="[\t ]+";
$anyspace="[\t\r\n ]*";
$plusspace="[\t\r\n ]+";
$operands='[\+\-\/\*]';
$notoperands='[^\+\-\/\*]';

$quoteseg='(?:"(?:[^"]|[\\\\]")*?"|\'(?:[^\']|[\\\\]\')*?\'';
$regseg='\/(?:[^\/]|[\\\\]\/)*?\/';

//$jsobjsect="{$jsvarsect}(?:\((?:{$quoteseg}|{$jsvarsect}|))\))?";
//$jsobjsect="{$jsvarsect}(?:\({$anyspace}(?:{$quoteseg}|{$jsvarsect}|))(?:{$anyspace},{$anyspace}{$quoteseg}|{$jsvarsect}|))*{$anyspace}\))?(?:\[(?:{$quoteseg}|{$jsvarsect}|))\])?";
//$jsobjsect="{$jsvarsect}(?:\((?:[^\(\)\"']*(?:{$quoteseg}|(?R))))\))?(?:\[(?:[^\[\]\"']*(?:{$quoteseg}|(?R))))\])?";
//$jsvarobj='(?:[a-zA-Z0-9\._\(\)\[\]\+\-]+)';
$jsvarsect='[a-zA-Z0-9_\$](?:[a-zA-Z0-9\$\._\/\[\]\+-]*[a-zA-Z0-9_\/\]])?';
$jsobjsect="{$jsvarsect}(?:\((?:{$quoteseg}|{$jsvarsect}|))\))?(?:\[(?:{$quoteseg}|{$jsvarsect}|))\])?";
$jsvarobj="{$jsobjsect}(?:\.{$jsobjsect})*";
//$jsquotereg="((?:(?:{$anyspace}{$quoteseg}|{$jsvarobj}){$anyspace}\+)*){$anyspace}{$quoteseg}|{$jsvarobj}){$justspace}(?=[;\}\n\r]))"; # HUH?
$jsquotesect="(?:{$anyspace}{$quoteseg}|{$jsvarobj}))";
$jsquotereg="{$jsquotesect}(?:\+{$jsquotesect})*";

//$notjsvarsect='[^a-zA-Z0-9\._\[\]\+-]';
//$notjsvarsect='[^a-zA-Z0-9\._\[\]\/]';
$notjsvarsect='[^a-zA-Z0-9\._\[\]]';

//$jsend="(?={$anyspace}[;\}\n\r\'\"])";
//$jsend="(?={$anyspace}(?:[;\}]|{$notoperands}[\n\r]))";
$jsend="(?={$justspace}(?:[;\}\n\r]|{$notoperands}[\n\r]))";
$jsbegin="((?:[;\{\}\n\r\(\)]|[\!=]=){$anyspace})";
//$jsbeginright="((?:[;\{\}\n\r\(\)=\+\-\/\*]){$anyspace})";
$jsbeginright="((?:[;\{\}\(\)=\+\-\/\*]){$justspace})";

# one of two for notjsendsect, either parse (?:) statements correctly, or allow for [?:] (currently [?:])
$notjsendsect="(?:(?!<\/script>)(?:[^;\{\}\n\r\"'\+\-\/\*]|{$operands}{$anyspace}))";
$notjsend="{$notjsendsect}*(?:(?:{$quoteseg}|{$regseg}))?{$notjsendsect}*)*";

$htmlnoquot='(?:[^"\'\\\\][^> ]*)';
$htmlnoquotnoqm='(?:[^\?"\'\\\\][^\?> ]*)';
$htmlreg="({$quoteseg}|{$htmlnoquot}))";
$xmlhttpreq="(?:XMLHttpRequest{$anyspace}(?:\({$anyspace}\)|)|ActiveXObject{$anyspace}\({$anyspace}[^\)]+\.XMLHTTP['\"]{$anyspace}\))(?=;)";
$jsnewobj="(?:{$anyspace}new{$plusspace}|{$anyspace})";
$formnotpost="(?:(?!method{$anyspace}={$anyspace}(?:'|\")?post)[^>])";
$frametargets='_(?:top|parent|self)';

$js_string_methods='(?:anchor|big|blink|bold|charAt|charCodeAt|concat|fixed|fontcolor|fontsize|fromCharCode|indexOf|italics|lastIndexOf|link|match|replace|search|slice|small|split|strike|sub|substr|substring|sup|toLowerCase|toUpperCase|toSource|valueOf)';
$js_string_attrs='(?:constructor|length|prototype)';

# }}}

# REGEXPS: JAVASCRIPT PARSING {{{

$js_regexp_arrays=array(
	array(1,2,"/{$jsbegin}({$jsvarobj})\.({$jshookgetattrs}){$anyspace}\+=/i",'\1\2.\3='.COOK_PREF.'.getAttr(\2,/\3/)+'),
	array(1,2,"/{$jsbegin}({$jsvarobj})\.(({$jshookattrs}){$anyspace}=(?:{$anyspace}{$jsvarobj}{$anyspace}=)*{$anyspace})((?!\=)(?!{$jsend}){$notjsend}){$jsend}/i",'\1'.COOK_PREF.'.setAttr(\2,/\4/,\5)'),
	array(1,2,"/{$jsbeginright}({$jsvarobj})\.({$jshookgetattrs})([^\.=a-z0-9_\[\]\t\r\n]|\.{$js_string_methods}\(|\.{$js_string_attrs}{$notjsvarsect})/i",'\1'.COOK_PREF.'.getAttr(\2,/\3/)\4'),

	array(1,2,"/([^a-z0-9]{$jsmethods}{$anyspace}\()([^)]*)\)/i",'\1'.COOK_PREF.'.surrogafy_url(\3))'),
	array(1,2,"/([^a-z0-9])eval{$anyspace}\(({$anyspace}{$jsvarobj})\)/i",'\1eval('.COOK_PREF.'.parse_all_html(\2,"application/x-javascript"))'),

	array(1,2,"/{$jsbegin}\.action{$anyspace}=/i",'\1.'.COOK_PREF.'.value='),
	//array(1,2,"/{$jsbegin}(\.setAttribute{$anyspace}\({$anyspace}(\"|')({$jsattrs})(\\2){$anyspace},{$anyspace})(.*?){$jsend}/i",'\1\2'.COOK_PREF.'.surrogafy_url(\6)'),
	//array(1,2,"/{$jsbegin}(\.setAttribute{$anyspace}\({$anyspace}(\"|')({$jsattrs})(\\2){$anyspace},{$anyspace})(.*?){$jsend}/i",'\1\2'.COOK_PREF.'.surrogafy_url(\6)'),
	array(1,2,"/{$jsbegin}({$jsvarobj})\.setAttribute{$anyspace}\({$anyspace}({$jsquotereg}){$anyspace},{$anyspace}({$jsquotereg}){$anyspace}\)/i",'\1'.COOK_PREF.'.setAttr(\2,\3,\4)'),
	array(1,2,"/{$jsbegin}([^\ {>\t\r\n=;]+{$anyspace}=)({$jsnewobj}{$xmlhttpreq})/i",'\1\2'.COOK_PREF.'.XMLHttpRequest_wrap(\3)'),
	array(1,2,"/{$jsbegin}(return{$plusspace})({$jsnewobj}{$xmlhttpreq})/i",'\1\2'.COOK_PREF.'.XMLHttpRequest_wrap(\3)'),
	(ENCRYPT_URLS?array(1,2,"/{$jsbegin}((?:[^\) \{\}]*(?:\)\.{0,1}))+)(\.submit{$anyspace}\(\)){$jsend}/i",'\1void((\2.method=="post"?null:\2\3));'):null),
);

# }}}

# REGEXPS: HTML/CSS PARSING {{{

$regexp_arrays=array(
	'text/html' => array(
		## do HTML based javascript stuff # OLDWAY
		#array(1,1,"/( on[a-z]{3,20}{$anyspace}={$anyspace})(?:(\")([^\"]+)(\")|(')([^']+)('))/i",'\1\2\5;\3\6;\4\7'), # OLDWAY
		#array(1,2,"/(<script(?:(?:(?! src{$anyspace}=)[^>])*)>)([\s\S]*?)(?:{$anyspace}(?:\/\/)?{$anyspace}-->{$anyspace})?<\/script>/i",'\1\2;'.COOK_PREF.'.purge();//--></script>'), # OLDWAY

		# target attr
		(PAGETYPE_ID===PAGETYPE_FRAMED_PAGE?array(1,1,"/(<[a-z][^>]*{$anyspace}) target{$anyspace}={$anyspace}(?:{$frametargets}|('){$frametargets}'|(\"){$frametargets}\")/i",'\1'):null),
		(PAGETYPE_ID===PAGETYPE_FRAMED_CHILD?array(1,1,"/(<[a-z][^>]*{$anyspace} target{$anyspace}={$anyspace})(?:_top|(')_top'|(\")_top\")/i",'\1\2\3'.COOK_PREF.'_top\2\3'):null),

		# deal with <form>s
		array(1,1,"/(<form{$formnotpost}*?)(?:{$plusspace}action{$anyspace}={$anyspace}{$htmlreg})({$formnotpost}*)>/i",'\1\3><input type="hidden" name="" class="'.COOK_PREF.'" value=\2 />'),
		array(2,1,"/<input type=\"hidden\" name=\"\" class=\"".COOK_PREF."\" value{$anyspace}={$anyspace}{$htmlreg} \/>/i",1,false),
		array(1,1,'/(<form[^>]*?)>/i','\1 target="_self"'.(ENCRYPT_URLS?' onsubmit="return '.COOK_PREF.'.form_encrypt(this);">':'>')),
		array(1,1,"/(<form{$formnotpost}+)>(?!<!--".COOK_PREF.'-->)/i','\1 target="_parent"><!--'.COOK_PREF.'--><input type="hidden" name="" value="_">'),

		# deal with the form button for encrypted URLs
		(ENCRYPT_URLS?array(1,1,"/(<input[^>]*? type{$anyspace}={$anyspace}(?:\"submit\"|'submit'|submit)[^>]*?[^\/])((?:[ ]?[\/])?>)/i",'\1 onclick="'.COOK_PREF.'_form_button=this.name;"\2'):null),

		# parse all the other tags
		array(2,1,"/<[a-z][^>]*{$plusspace}{$htmlattrs}{$anyspace}={$anyspace}{$htmlreg}/i",2),
		array(2,1,"/<param[^>]*{$plusspace}name{$anyspace}={$anyspace}[\"']?movie[^>]*{$plusspace}value{$anyspace}={$anyspace}{$htmlreg}/i",1),
		array(2,2,"/<script[^>]*?{$plusspace}src{$anyspace}={$anyspace}([\"']){$anyspace}(.*?[^\\\\])\\1[^>]*>{$anyspace}<\/script>/i",2),
		(URL_FORM && PAGE_FRAMED?array(2,1,"/<a(?:rea)?{$plusspace}[^>]*href{$anyspace}={$anyspace}{$htmlreg}/i",1,false,NEW_PAGETYPE_FRAME_TOP):null),
		(URL_FORM && PAGE_FRAMED?array(2,1,"/<[i]?frame{$plusspace}[^>]*src{$anyspace}={$anyspace}{$htmlreg}/i",1,false,PAGETYPE_FRAMED_CHILD):null)
	),

	'text/css' => array(
		array(2,1,"/[^a-z]url\({$anyspace}(&(?:quot|#(?:3[49]));|\"|')(.*?[^\\\\])(\\1){$anyspace}\)/i",2),
		array(2,1,"/[^a-z]url\({$anyspace}((?!&(?:quot|#(?:3[49]));)[^\"'\\\\].*?[^\\\\]){$anyspace}\)/i",1),
		array(2,1,"/@import{$plusspace}(&(?:quot|#(?:3[49]));|\"|')(.*?[^\\\\])(\\1);/i",2)
	),

	'application/x-javascript' => $js_regexp_arrays,
	'text/javascript' => $js_regexp_arrays
);

# }}}

# REGEXPS: INSERT JAVASCRIPT REGEXPS {{{

if(QUERY_STRING=='js_regexps' || QUERY_STRING=='js_regexps_framed'){ ?>//<script>
<?php echo(convertarray_to_javascript().((!empty($_COOKIE[COOK_PREF.'_remove_objects']))?'regexp_arrays["text/html"].push(Array(1,/<[\\\\/]?(embed|param|object)[^>]*>/ig,""));':null)); ?>
//</script><?php exit(); }

# }}}

# REGEXPS: SERVER-SIDE ONLY PARSING {{{

array_push($regexp_arrays['text/html'],
	array(2,1,"/<meta[^>]*{$plusspace}http-equiv{$anyspace}={$anyspace}([\"']|)refresh\\1[^>]* content{$anyspace}={$anyspace}([\"']|)[ 0-9\.;\t\\r\n]*url=(.*?)\\2[^>]*>/i",3,true,NEW_PAGETYPE_FRAMED_PAGE),
	array(1,1,"/(<meta[^>]*{$plusspace}http-equiv{$anyspace}={$anyspace}([\"']|)set-cookie\\2[^>]* content{$anyspace}={$anyspace})([\"'])(.*?[^\\\\]){$anyspace}\\3/i",'\1\3'.PAGECOOK_PREFIX.'\4\3')
);

# }}}

# REGEXPS: CLEANUP {{{

# needed later, but $anyspace and $htmlreg are unset below
define('BASE_REGEXP',"<base[^>]* href{$anyspace}={$anyspace}{$htmlreg}[^>]*>");
define('END_OF_SCRIPT_TAG',"(?:{$anyspace}(?:\/\/)?{$anyspace}-->{$anyspace})?<\/script>");
define('REGEXP_SCRIPT_ONEVENT',"( on[a-z]{3,20}=(?:\"[^\"]+\"|'[^']+'|[^\"' >][^ >]+[^\"' >])| href=(?:\"{$anyspace}javascript:[^\"]+\"|'{$anyspace}javascript:[^']+'|{$anyspace}javascript:[^\"' >][^ >]+[^\"' >]))");

unset($jsattrs,$jshookattrs,$jsmethods,$jslochost,$htmlattrs,
      $anyspace,$plusspace,$operands,$notoperands,
      $quoteseg,$regseg,
      $jsvarsect,$jsobjsect,$jsvarobj,$jsquotesect,$jsquotereg,
      $notjsvarsect,
      $jsend,$jsbegin,$jsbeginright,
      $notjsendsect,$notjsend,
      $htmlnoquot,$htmlnoquotnoqm,$htmlreg,$xmlhttpreq,$jsnewobj,$formnotpost,$frametargets,
      $js_string_methods,$js_string_attrs,
      $js_regexp_arrays
);

# }}}

# }}}

# PROXY FUNCTIONS {{{

# PROXY FUNCTIONS: AURL CLASS {{{

# class for URL
class aurl{
	var $url,$topurl,$locked;
	var $proto,$userpass,$servername,$portval,$path,$file,$query,$label;

	function aurl($url,$topurl=null){

		if(strlen($url)>MAXIMUM_URL_LENGTH) $this->url=null;
		else $this->url=preg_replace('/&#([0-9]+);/e','chr(\1)',trim(str_replace('&amp;','&',str_replace(chr(13),null,str_replace(chr(10),null,$url)))));
		$this->topurl=$topurl;

		$this->determine_locked();
		if($this->locked) return;

		$urlwasvalid=true;
		if(!preg_match(URLREG,$this->url)){
			$urlwasvalid=false;
			if($this->topurl==null) $this->url='http://'.(($this->url{0}==':' || $this->url{0}=='/')?substr($this->url,1):$this->url).(strpos($this->url,'/')!==false?null:'/');
			#if($this->topurl==null) $this->url='http://'.preg_replace("/^:?\/{0,2}/","",$this->url).(strpos($this->url,'/')!==false?null:'/');
			else{
				$newurl=$this->topurl->get_proto().$this->get_fieldreq(2,$this->topurl->get_userpass()).$this->topurl->get_servername().(($this->topurl->get_portval()!=80 && ($this->topurl->get_proto()=='https'?$this->topurl->get_portval()!=443:true))?':'.$this->topurl->get_portval():null);
				if($this->url{0}!='/') $newurl.=$this->topurl->get_path();
				$this->url=$newurl.$this->url;
			}
		}

		$this->set_proto(($urlwasvalid || $this->topurl==null?preg_replace('/^([^:\/]*).*$/','\1',$this->url):$this->topurl->get_proto()));
		$this->set_userpass(preg_replace(URLREG,'\2',$this->url));
		$this->set_servername(preg_replace(URLREG,'\3',$this->url));
		$this->set_portval(preg_replace(URLREG,'\4',$this->url));
		$this->set_path(preg_replace(URLREG,'\5',$this->url));
		$this->set_file(preg_replace(URLREG,'\6',$this->url));
		$this->set_query(preg_replace(URLREG,'\7',$this->url));
		$this->set_label(preg_replace(URLREG,'\8',$this->url));

		if(!$this->locked && !preg_match(URLREG,$this->url)) havok(7,$this->url); #*
	}

	function determine_locked(){ $this->locked=preg_match(AURL_LOCK_REGEXP,$this->url)>0; } #*

	function get_fieldreq($fieldno,$value){
		$fieldreqs=array(2 => '://'.($value!=null?"$value@":null), 4 => ($value!=null && intval($value)!=80?':'.intval($value):null), 7 => ($value!=null?"?$value":null), 8 => ($value!=null?"#$value":null));
		if(!array_key_exists($fieldno,$fieldreqs)) return (empty($value)?null:$value);
		else return $fieldreqs[$fieldno];
	}

	function set_proto($proto=''){ if($this->locked) return; $this->proto=(!empty($proto)?$proto:'http'); }
	function get_proto(){ return $this->proto; }
	function get_userpass(){ return $this->userpass; }
	function set_userpass($userpass=null){ $this->userpass=$userpass; }
	function get_servername(){ return $this->servername; }
	function set_servername($servername=null){ $this->servername=$servername; }
	function get_portval(){ return (empty($this->portval)?($this->get_proto()=='https'?'443':'80'):$this->portval); }
	function set_portval($port=null){ $this->portval=strval((intval($port)!=80)?$port:null); }
	function get_path(){
		if(strpos($this->path,'/../')!==false) $this->path=preg_replace('/(?:\/[^\/]+){0,1}\/\.\.\//','/',$this->path);
		if(strpos($this->path,'/./')!==false) while(($path=str_replace('/./','/',$this->path)) && $path!=$this->path) $this->path=$path;
		return $this->path;
	}
	function set_path($path=null){ $this->path=(empty($path)?'/':$path); }
	function get_file(){ return $this->file; }
	function set_file($file=null){ $this->file=$file; }
	function get_query(){ return $this->query; }
	function set_query($query=null){ $this->query=$query; }
	function get_label(){ return $this->label; }
	function set_label($label=null){ $this->label=$label; }

	function get_url($withlabel=true){
		if($this->locked) return $this->url;
		return $this->get_proto().'://'.
		       ($this->get_userpass()==null?null:$this->get_userpass().'@').
		       $this->get_servername().
		       (($this->get_proto()=='https' && intval($this->get_portval())==443) || intval($this->get_portval())==80?null:':'.intval($this->get_portval())).
		       $this->get_path().$this->get_file().
		       ($this->get_query()==null?null:'?'.$this->get_query()).
		       ($withlabel && $this->get_label()==null?null:'#'.$this->get_label())
		;
	}

	function surrogafy(){
		$label=$this->get_label();
		$this->set_label();
		$url=$this->get_url();
		$this->set_label($label);

		#$this->determine_locked();
		if($this->locked) return $url;
		# || $this->get_proto().$this->get_fieldreq(2,$this->get_userpass()).$this->get_servername().$this->get_path().$this->get_file()==THIS_SCRIPT) return $url;

		if(ENCRYPT_URLS && !$this->locked) $url=proxenc($url);
		#$url=THIS_SCRIPT.'?='.(!ENCRYPT_URLS?urlencode($url):$url).(!empty($label)?"#$label":null); # urlencoded
		$url=THIS_SCRIPT."?={$url}".(!empty($label)?"#$label":null);
		return $url;
	}
} 

# }}}

# PROXY FUNCTIONS: URL PARSING {{{
function surrogafy_url($url,$topurl=false,$addproxy=true){
	global $curr_urlobj;
	if(preg_match('/^(["\']).*\1$/is',$url)>0){
		$urlquote=$url{0};
		$url=substr($url,1,strlen($url)-2);
	}
	if($topurl===false) $topurl=$curr_urlobj;
	$urlobj=new aurl($url,$topurl);
	$new_url=($addproxy?$urlobj->surrogafy():$urlobj->get_url());
	if(!empty($urlquote)) $new_url="{$urlquote}{$new_url}{$urlquote}";
	return $new_url;
}

function framify_url($url,$frame_type=false){
	if(($frame_type!==PAGETYPE_FRAME_TOP || !URL_FORM) && ($frame_type!==PAGETYPE_FRAMED_PAGE && !PAGE_FRAMED)) return $url;
	if(preg_match('/^(["\']).*\1$/is',$url)>0){
		$urlquote=$url{0};
		$url=substr($url,1,strlen($url)-2);
	}
	if(preg_match(FRAME_LOCK_REGEXP,$url)<=0){
		if($frame_type===PAGETYPE_FRAME_TOP && URL_FORM) $query='&=';
		elseif($frame_type===PAGETYPE_FRAMED_CHILD) $query='.&=';
		elseif($frame_type===PAGETYPE_FRAMED_PAGE || PAGE_FRAMED) $query='_&=';
		else $query=null;
		$url=preg_replace('/^([^\?]*)[\?]?'.PAGETYPE_MINIREGEXP.'([^#]*?[#]?.*?)$/',"\\1?={$query}\\3",$url,1);
	}
	if(!empty($urlquote)) $url="{$urlquote}{$url}{$urlquote}";
	return $url;
}

function proxenc($url){
	if($url{0}=='~' || strtolower(substr($url,0,3))=='%7e') return $url;
	$url=urlencode($url);
	$new_url=null;
	for($i=0;$i<strlen($url);$i++){
		$char=ord($url{$i});
		$char+=ord(substr(SESS_PREF,$i%strlen(SESS_PREF),1));
		while($char>126) $char-=94;
		$new_url.=chr($char);
	}
	#return '~'.base64_encode($new_url);
	return '~'.urlencode(base64_encode($new_url));
}

# }}}

# PROXY FUNCTIONS: ERRORS & EXITING {{{

function finish_noexit(){
	global $dns_cache_array;
	# save DNS Cache before exiting
	$_SESSION['DNS_CACHE_ARRAY']=$dns_cache_array;
}

function finish(){
	finish_noexit();
	exit();
}

function havok($errorno,$arg1=null,$arg2=null,$arg3=null){
	global $curr_url;
	$url=$curr_url;
	switch($errorno){
		case 1:
			$et='Bad IP Address';
			$ed="The IP address given ({$arg2}) is an impossible IP address, or the domain given ({$arg1}) was resolved to an impossible IP address.";
			break;
		case 2:
			$et='Address is Blocked';
			$ed="The administrator of this proxy service has decided to block this address, domain, or subnet.\n<br /><br />\nDomain: {$arg1}\n<br />\nAddress: {$arg2}";
			break;
		case 3:
			$et='Could Not Resolve Domain';
			$ed="The domain of the URL given ({$arg1}) could not be resolved due to DNS issues or an errorneous domain name.";
			break;
		case 4:
			$et='Bad Filters';
			$ed='The administrator of this proxy has incorrectly configured his domain filters, or a domain given could not be resolved.';
			break;
		case 5:
			$et='Domain is Blocked';
			$ed='The administrator of this proxy has decided to block this domain.';
			break;
		case 6:
			$et='Could Not Connect to Server';
			$ed="An error has occurred while attempting to connect to \"{$arg1}\" on port \"{$arg2}\".";
			break;
		case 7:
			$et='Invalid URL';
			$ed='The URL below was detected to be an invalid URL.';
			$url=$arg1;
			break;
	}
	$ed.="\n<br /><br />\nURL:&nbsp;{$url}";
?>
<div style="font-family: bitstream vera sans, trebuchet ms"><div style="border: 3px solid #FFFFFF; padding: 2px">
	<div style="float: left; border: 1px solid #602020; padding: 1px; background-color: #FFFFFF">
	<div style="float: left; background-color: #801010; color: #FFFFFF; font-weight: bold; font-size: 54px; padding: 2px; padding-left: 12px; padding-right: 12px">!</div>
	</div>
	<div style="float: left; width: 500px; padding-left: 20px">
		<div style="border-bottom: 1px solid #000000; font-size: 12pt; text-align: center; font-weight: bold; padding: 2px">Error: <?php echo($et); ?></div>
		<div style="padding: 6px"><?php echo($ed); ?></div>
	</div>
</div></div>
<?php finish(); }

# }}}

# PROXY FUNCTIONS: TCP/IP {{{

function ipbitter($ipaddr){
	$ipsplit=explode('.',$ipaddr);
	for($i=0;$i<4;$i++){
		$ipsplit[$i]=decbin($ipsplit[$i]);
		$ipsplit[$i]=str_repeat('0',8-strlen($ipsplit[$i])).$ipsplit[$i];
	}
	return implode(null,$ipsplit);
}

function ipcompare($iprange,$ip){
	$iprarr=split('/',$iprange);
	$ipaddr=$iprarr[0];
	$mask=$iprarr[1];
	$maskbits=str_repeat('1',$mask).str_repeat('0',$mask);
	$ipbits=ipbitter($ipaddr);
	$ipbits2=ipbitter($ip);
	return (($ipbits & $maskbits)==($ipbits2 & $maskbits));
}

function ip_check($ip,$mask=false){
	$ipseg='(?:[01]?[0-9]{1,2}|2(?:5[0-5]|[0-4][0-9]))';
	return preg_match("/^(?:$ipseg\.){0,3}$ipseg".($mask?'\/[0-9]{1,2}':null).'$/i',$ip); #*
}

function gethostbyname_cacheit($address){
	global $dns_cache_array;
	$ipaddr=gethostbyname($address);
	$dns_cache_array[$address]=array('time'=>time(), 'ipaddr'=>$ipaddr);
	return $ipaddr;
}

function gethostbyname_cached($address){
	global $dns_cache_array;
	if(isset($dns_cache_array[$address])) return $dns_cache_array[$address]['ipaddr'];
	return gethostbyname_cacheit($address);
}

function get_check($address){
	global $blocked_addresses;
	if(strrchr($address,'/')) $address=substr(strrchr($address,'/'),1);
	$ipc=ip_check($address);
	$addressip=(ip_check($address)?$address:gethostbyname_cached($address));
	if(!ip_check($addressip)) havok(1,$address,$addressip);
	foreach($blocked_addresses as $badd){
		if(!$ipc) if(strlen($badd)<=strlen($address) && substr($address,strlen($address)-strlen($badd),strlen($badd))==$badd) havok(5);
		if($badd==$addressip) havok(2,$address,$addressip);
		elseif(ip_check($badd,true)){ if(ipcompare($badd,$addressip)) havok(2,$address,$addressip); }
		else{
			$baddip=gethostbyname_cached($badd);
			if(empty($baddip)) havok(4);
			if($baddip==$addressip) havok(2,$address,$addressip);
		}
	}
	return $addressip;
}

# }}}

# PROXY FUNCTIONS: HTTP {{{

function httpclean($str){ return str_replace(' ','+',preg_replace('/([^":\-_\.0-9a-z ])/ie','\'%\'.(strlen(dechex(ord(\'\1\')))==1?\'0\':null).strtoupper(dechex(ord(\'\1\')))',$str)); }

function getpage($url){
	global $headers,$out,$post_vars,$proxy_variables,$referer;

	$urlobj=new aurl($url);
	$query=$urlobj->get_query();
	$requrl=$urlobj->get_path().$urlobj->get_file().(!empty($query)?"?{$query}":null);

	$http_auth=null;
	if(extension_loaded('apache')){
		$fail=false;
		$cheaders=getallheaders();
		$http_auth=$reqarray['Authorization'];
	}
	else $fail=true;

	$authorization=($fail?$_SERVER['HTTP_AUTHORIZATION']:$cheaders['Authorization']);
	$cache_control=($fail?$_SERVER['HTTP_CACHE_CONTROL']:$cheaders['Cache-Control']);
	$if_modified=($fail?$_SERVER['HTTP_IF_MODIFIED_SINCE']:$cheaders['If-Modified-Since']);
	$if_none_match=($fail?$_SERVER['HTTP_IF_NONE_MATCH']:$cheaders['If-None-Match']);

	if($fail){
		if(!empty($authorization)) $http_auth=$authorization;
		elseif(!empty($_SERVER['PHP_AUTH_USER']) && !empty($_SERVER['PHP_AUTH_PW']))
			$http_auth='Basic '.base64_encode("{$_SERVER['PHP_AUTH_USER']}:{$_SERVER['PHP_AUTH_PW']}");
		elseif(!empty($_SERVER['PHP_AUTH_DIGEST'])) $http_auth="Digest {$_SERVER['PHP_AUTH_DIGEST']}";
	}

	if(PIP!=null && PPORT!=null){
		$servername=PIP;
		$ipaddress=get_check(PIP);
		$portval=PPORT;
		$requrl=$urlobj->get_url(false);
	}
	else{
		$servername=$urlobj->get_servername();
		$ipaddress=($urlobj->get_proto()=='ssl' || $urlobj->get_proto()=='https'?'ssl://':null).get_check($servername);
		$portval=$urlobj->get_portval();
	}

	$out="{$_SERVER['REQUEST_METHOD']} ".str_replace(' ','%20',$requrl)." HTTP/1.1\r\nHost: ".$urlobj->get_servername().(($portval!=80 && ($urlobj->get_proto()=='https'?$portval!=443:true))?":$portval":null)."\r\n";

	global $useragent;
	$useragent=null;
	if($_COOKIE[COOK_PREF.'_useragent']!='-1'){
		$useragent=$_COOKIE[COOK_PREF.'_useragent'];
		if(empty($useragent)) $useragent=$_SERVER['HTTP_USER_AGENT'];
		$useragent_cook=($useragent==1?$_COOKIE[COOK_PREF.'_useragenttext']:$useragent);
		if(!empty($useragent_cook)) $out.="User-Agent: $useragent_cook\r\n";
	}
	if(!empty($http_auth)) $out.="Authorization: $http_auth\r\n";

	if(empty($_COOKIE[COOK_PREF.'_remove_referer']) && !empty($referer)) $out.='Referer: '.str_replace(' ','+',$referer)."\r\n";
	if($_SERVER['REQUEST_METHOD']=='POST') $out.='Content-Length: '.strlen($post_vars)."\r\nContent-Type: application/x-www-form-urlencoded\r\n";

	$cook_prefdomain=preg_replace('/^www\./i',null,$urlobj->get_servername()); #*
	$cook_prefix=str_replace('.','_',$cook_prefdomain).COOKIE_SEPARATOR;
	if(count($_COOKIE)>0 && empty($_COOKIE[COOK_PREF.'_remove_cookies'])){
		$addtoout=null;
		reset($_COOKIE);
		while(list($key,$val)=each($_COOKIE)){
			if(ENCRYPT_COOKS){
				$key=proxdec($key);
				$val=proxdec($val); #urldecode($val));
			}
			if(str_replace(COOKIE_SEPARATOR,null,$key)==$key) continue;
			#$cook_domain=preg_replace('/^(.*'.COOKIE_SEPARATOR.').*$/','\1',$key); #**
			$cook_domain=substr($key,0,strpos($key,COOKIE_SEPARATOR)).COOKIE_SEPARATOR;
			if(substr($cook_prefix,strlen($cook_prefix)-strlen($cook_domain),strlen($cook_domain))!=$cook_domain) continue;
			$key=substr($key,strlen($cook_domain),strlen($key)-strlen($cook_domain));
			if(!in_array($key,$proxy_variables)) $addtoout.=" $key=$val;";
		}
		if(!empty($addtoout)){
			$addtoout.="\r\n";
			$out.="Cookie:{$addtoout}";
		}
	}

	$out.="Accept: */*;q=0.1\r\n".
	      (GZIP_PROXY_SERVER?"Accept-Encoding: gzip\r\n":null).
	      //"Accept-Charset: ISO-8859-1,utf-8;q=0.1,*;q=0.1\r\n".
	      /*/
	      "Keep-Alive: 300\r\n".
	      "Connection: keep-alive\r\n".                          /*/
	      "Connection: close\r\n".                               //*/
	      ($cache_control!=null?"Cache-Control: $cache_control\r\n":null).
	      ($if_modified!=null?"If-Modified-Since: $if_modified\r\n":null).
	      ($if_none_match!=null?"If-None-Match: $if_none_match\r\n":null).
	      "\r\n{$post_vars}"
	;

	// This part ignores any "SSL: fatal protocol error" errors, and makes sure other errors are still triggered correctly
	function errorHandle($errno,$errmsg){
		if($errno<=E_PARSE && ($errno!=E_WARNING || substr($errmsg,-25)!='SSL: fatal protocol error')){
			restore_error_handler();
			trigger_error($errmsg,$errno<<8);
			set_error_handler('errorHandle');
		}
	}
	set_error_handler('errorHandle');

	$fp=@fsockopen($ipaddress,$portval,$errno,$errval,5) or havok(6,$servername,$portval);
	stream_set_timeout($fp,5);
	# for persistent connections, this may be necessary
	/*
	$ub=stream_get_meta_data($fp);
	$ub=$ub['unread_bytes'];
	if($ub>0) fread($fp,$ub);
	*/
	fwrite($fp,$out);

	$response='100';
	while($response=='100'){
		$responseline=fgets($fp,8192);
		$response=substr($responseline,9,3);

		$headers=array();
		while($curline!="\r\n" && $curline=fgets($fp,8192)){
			$harr=explode(':',$curline,2);
			$headers[strtolower($harr[0])][]=trim($harr[1]);
		}
	}

	#if($headers['pragma'][0]==null) header('Pragma: public');
	#if($headers['cache-control'][0]==null) header('Cache-Control: public');
	#if($headers['last-modified'][0]==null && $headers['expires']==null) header('Expires: '.date('D, d M Y H:i:s e',time()+86400));

	# read and store cookies
	if(empty($_COOKIE[COOK_PREF.'_remove_cookies'])){
		for($i=0;$i<count($headers['set-cookie']);$i++){
			$thiscook=explode('=',$headers['set-cookie'][$i],2);
			if(!strpos($thiscook[1],';')) $thiscook[1].=';';
			$cook_val=substr($thiscook[1],0,strpos($thiscook[1],';'));
			$cook_domain=preg_replace('/^.*domain=[	 ]*\.?([^;]+).*?$/i','\1',$thiscook[1]); #*
			if($cook_domain==$thiscook[1]) $cook_domain=$cook_prefdomain;
			elseif(substr($cook_prefdomain,strlen($cook_prefdomain)-strlen($cook_domain),strlen($cook_domain))!=$cook_domain) continue;
			$cook_name=str_replace('.','_',$cook_domain).COOKIE_SEPARATOR.$thiscook[0];
			if(ENCRYPT_COOKS){
				$cook_name=proxenc($cook_name);
				$cook_val=proxenc($cook_val);
			}
			dosetcookie($cook_name,$cook_val);
		}
	}

	if($response{0}=='3' && $response{1}=='0' && $response{2}!='4'){
		$urlobj=new aurl($url);
		$redirurl=framify_url(surrogafy_url($headers['location'][0],$urlobj),NEW_PAGETYPE_FRAMED_PAGE);

		fclose($fp);
		restore_error_handler();

		finish_noexit();
		header("Location: {$redirurl}");
		exit();
	}

	$oheaders=$headers;
	$oheaders['location']=$oheaders['content-length']=$oheaders['content-encoding']=
	$oheaders['set-cookie']=$oheaders['transfer-encoding']=$oheaders['connection']=
	$oheaders['keep-alive']=$oheaders['pragma']=$oheaders['cache-control']=$oheaders['expires']=null;

	while(list($key,$val)=each($oheaders)) if(!empty($val[0])) header("{$key}: {$val[0]}");
	unset($oheaders);
	header("Status: {$response}");

	if(substr($headers['content-type'][0],0,4)=='text' || substr($headers['content-type'][0],0,24)=='application/x-javascript'){
		$justoutput=false;
		$justoutputnow=false;
	}
	else{
		$justoutputnow=($headers['content-encoding'][0]=='gzip'?false:true);
		$justoutput=true;
	}

	if($headers['transfer-encoding'][0]=='chunked'){
		$body=null;
		$chunksize=null;
		while($chunksize!==0){
			$chunksize=intval(fgets($fp,8192),16);
			$bufsize=$chunksize;
			while($bufsize>=1){
				$chunk=fread($fp,$bufsize);
				if($justoutputnow) echo $chunk;
				else $body.=$chunk;
				$bufsize-=strlen($chunk);
			}
			fread($fp,2);
		}
	}

	// Content-Length stuff - commented for even more chocolatey goodness
	/*elseif($headers['content-length'][0]!=null){
		$conlen=$headers['content-length'][0];
		$body=null;
		for($i=0;$i<$conlen;$i+=$read){
			$read=($conlen-$i<8192?$conlen-$i:8192);
			$byte=fread($fp,$read);
			if($justoutputnow) echo $byte;
			else $body.=$byte;
		}
	}*/

	else{
		if(function_exists('stream_get_contents')){
			if($justoutputnow) echo stream_get_contents($fp);
			else $body=stream_get_contents($fp);
		}
		else{
			$body=null;
			while(true){
				$chunk=fread($fp,8192);
				if(empty($chunk)) break;
				if($justoutputnow) echo $chunk;
				else $body.=$chunk;
			}
		}
	}

	fclose($fp);
	restore_error_handler();

	if(GZIP_PROXY_SERVER && $headers['content-encoding'][0]=='gzip') $body=gzinflate(substr($body,10));
	if($justoutput){
		if(!$justoutputnow) echo $body;
		finish();
	}
	return array($body,$url,$cook_prefix);

}

# }}}

# }}}

# PROXY EXECUTION {{{

# PROXY EXECUTION: USER COOKIES {{{

global $proxy_variables;
$proxy_variables=array(COOK_PREF,COOK_PREF.'_pip',COOK_PREF.'_pport',COOK_PREF.'_useragent',COOK_PREF.'_useragenttext',COOK_PREF.'_url_form',COOK_PREF.'_remove_cookies',COOK_PREF.'_remove_referer',COOK_PREF.'_remove_scripts',COOK_PREF.'_remove_objects',COOK_PREF.'_encrypt_urls',COOK_PREF.'_encrypt_cooks');

if($postandget[COOK_PREF.'_set_values']){
	$proxy_varblacklist=array(COOK_PREF);
	if($postandget[COOK_PREF.'_useragent']!='1'){
		unset($postandget[COOK_PREF.'_useragenttext']);
		dosetcookie(COOK_PREF.'_useragenttext',false,0);
	}
	while(list($key,$val)=each($proxy_variables)){
		if(!in_array($val,$proxy_varblacklist)){
			dosetcookie($val,false,0);
			if(isset($postandget[$val]) && !empty($postandget[$val])) dosetcookie($val,$postandget[$val]);
		}
	}
	$theurl=framify_url(surrogafy_url(ORIG_URL),PAGETYPE_FRAME_TOP);
	#$theurl=surrogafy_url((ENCRYPT_URLS?proxdec($theurl):$theurl),null);
	header("Location: {$theurl}");
	finish();
}

# }}}

# PROXY EXECUTION: PROXY GET/POST/COOKIES {{{

define('PIP',(FORCE_DEFAULT_TUNNEL?$_COOKIE[COOK_PREF.'_pip']:DEFAULT_TUNNEL_PIP));
define('PPORT',intval(FORCE_DEFAULT_TUNNEL?$_COOKIE[COOK_PREF.'_pport']:DEFAULT_TUNNEL_PPORT));

define('ENCRYPT_COOKS',gethardattr('encrypt_cooks'));

global $referer;
if($_SERVER['HTTP_REFERER']==null){
	$refurlobj=new aurl($_SERVER['HTTP_REFERER']);
	$referer=proxdec(preg_replace('/^[\s\S]*'.COOK_PREF.'=([^&]*)[\s\S]*$/i','\1',$refurlobj->get_path())); #*
}
else $referer=null;

#$getkeys=array_keys($_GET);
#foreach($getkeys as $getvar){ if(!in_array($getvar,$proxy_variables)){ $curr_url.=(strpos($curr_url,'?')===false?'?':'&')."$getvar=".urlencode($_GET[$getvar]); } }

global $post_vars;
$post_vars=null;
$postkeys=array_keys($_POST);
foreach($postkeys as $postkey){
	if(!in_array($postkey,$proxy_variables)){
		if(!is_array($_POST[$postkey]))
			$post_vars.=($post_vars!=null?'&':null).httpclean($postkey).'='.httpclean($_POST[$postkey]);
		else{
			foreach($_POST[$postkey] as $postval)
				$post_vars.=($post_vars!=null?'&':null).httpclean($postkey).'%5B%5D='.httpclean($postval);
		}
	}
}
unset($postkeys);

# }}}

# PROXY EXECUTION: DNS CACHE {{{

if(!isset($_SESSION['DNS_CACHE_ARRAY'])) $dns_cache_array=array();
else $dns_cache_array=$_SESSION['DNS_CACHE_ARRAY'];

# purge old records from DNS cache
while(list($key,$entry)=each($dns_cache_array)){
	if($entry['time']<time()-(DNS_CACHE_EXPIRE*60)) unset($dns_cache_array[$key]);
}

# }}}

# PROXY EXECUTION: PAGE RETRIEVAL {{{

$pagestuff=getpage($curr_url);
$body=$pagestuff[0];

$tbody=trim($body);
if(($tbody{0}=='"' && substr($tbody,-1)=='"') || ($tbody{0}=='\'' && substr($tbody,-1)=='\'')){
	echo $body;
	finish();
}
unset($tbody);

$curr_url=$pagestuff[1];
define('PAGECOOK_PREFIX',$pagestuff[2]);
unset($pagestuff);
define('CONTENT_TYPE',preg_replace('/^([a-z0-9\-\/]+).*$/i','\1',$headers['content-type'][0])); #*

# }}}

# PROXY EXECUTION: PAGE PARSING {{{

if(strpos($body,'<base')){
	$base=preg_replace('/^.*'.BASE_REGEXP.'.*$/is','\1',$body);
	if(!empty($base) && $base!=$body && !empty($base{100})){
		$body=preg_replace('/'.BASE_REGEXP.'/i',null,$body);

		//preg_match('/^(["\']).*\1$/i',$base)>0
		if(($base{0}=='"' && substr($base,-1)=='"') || ($base{1}=='\'' && substr($base,-1)=='\''))
			$base=substr($base,1,strlen($base)-2); #*
		$curr_url=$base;
	}
	unset($base);
}

global $curr_urlobj;
$curr_urlobj=new aurl($curr_url);

# PROXY EXECUTION: PAGE PARSING: PARSING FUNCTIONS {{{

function parse_html($regexp,$partoparse,$html,$addproxy,$framify){
	global $curr_urlobj;
	$newhtml=null;
	while(preg_match($regexp,$html,$matcharr,PREG_OFFSET_CAPTURE)){
		$nurl=surrogafy_url($matcharr[$partoparse][0],$curr_urlobj,$addproxy);
		if($framify) $nurl=framify_url($nurl,$framify);
		$begin=$matcharr[$partoparse][1];
		$end=$matcharr[$partoparse][1]+strlen($matcharr[$partoparse][0]);
		$newhtml.=substr_replace($html,$nurl,$begin);
		$html=substr($html,$end,strlen($html)-$end);
	}
	$newhtml.=$html;
	return $newhtml;
}

function regular_express($regexp_array,$thevar){
	#$regexp_array[2].='S'; # in benchmarks, this 'optimization' appeared to not do anything at all, or possibly even slow things down
	if($regexp_array[0]==1) $newvar=preg_replace($regexp_array[2],$regexp_array[3],$thevar);
	elseif($regexp_array[0]==2){
		$addproxy=(isset($regexp_array[4])?$regexp_array[4]:true);
		$framify=(isset($regexp_array[5])?$regexp_array[5]:false);
		$newvar=parse_html($regexp_array[2],$regexp_array[3],$thevar,$addproxy,$framify);
	}
	return $newvar;
}

function parse_all_html($html){
	global $regexp_arrays;

	if(CONTENT_TYPE!='text/html'){
		for(reset($regexp_arrays);list($key,$arr)=each($regexp_arrays);){
			if($key==CONTENT_TYPE){
				foreach($arr as $regarr){
					if($regarr==null) continue;
					$html=regular_express($regarr,$html);
				}
			}
		}
		return $html;
	}

	#if(!empty($_COOKIE[COOK_PREF.'_remove_scripts'])) $splitarr=array($html);
	$splitarr=preg_split('/(<!--(?!\[if).*?-->|<style.*?<\/style>|<script.*?<\/script>)/is',$html,-1,PREG_SPLIT_DELIM_CAPTURE);
	unset($html);

	//define('REGEXP_SCRIPT_ONEVENT','( on[a-z]{3,20}=(?:"(?:[^"]+)"|\'(?:[^\']+)\'|[^"\' >][^ >]+[^"\' >]))');
	$firstrun=true;
	$firstjsrun=true;
	for(reset($regexp_arrays);list($key,$arr)=each($regexp_arrays);){
		if($key=='text/javascript') continue;

		// OPTION1: use ONLY if no Javascript REGEXPS affect HTML sections
		// and all HTML modifying Javascript REGEXPS are performed after HTML regexps
		// This gives a pretty significant speed boost
		// If used, make sure "OPTION2" lines are commented, and other "OPTION1" lines AREN'T
		if($firstjsrun && $key=='application/x-javascript'){
			if(!empty($_COOKIE[COOK_PREF.'_remove_scripts'])) break;
			$splitarr2=array();
			for($i=0;$i<count($splitarr);$i+=2){
				$splitarr2[$i]=preg_split('/'.REGEXP_SCRIPT_ONEVENT.'/is',$splitarr[$i],-1,PREG_SPLIT_DELIM_CAPTURE);
			}
		}
		// END OPTION1

		# firstrun remove scripts: on<event>s and noscript tags; also remove objects
		if($firstrun && (!empty($_COOKIE[COOK_PREF.'_remove_scripts']) || !empty($_COOKIE[COOK_PREF.'_remove_objects']))){
			for($i=0;$i<count($splitarr);$i+=2){
				if(!empty($_COOKIE[COOK_PREF.'_remove_scripts']))
					$splitarr[$i]=preg_replace('/(?:'.REGEXP_SCRIPT_ONEVENT.'|<.?noscript>)/is',null,$splitarr[$i]);
				if(!empty($_COOKIE[COOK_PREF.'_remove_objects']))
					$splitarr[$i]=preg_replace('/<(embed|object).*?<\/\1>/is',null,$splitarr[$i]);
			}
		}

		foreach($arr as $regexp_array){
			if($regexp_array==null) continue;
			for($i=0;$i<count($splitarr);$i++){

				# parse scripts for on<event>s
				if($i%2==0 && isset($splitarr2) && $regexp_array[1]==2){ // OPTION1

				// OPTION2
				//if($regexp_array[1]==2 && $i%2==0){ // OPTION2
					//$splitarr2[$i]=preg_split('/( on[a-z]{3,20}=(?:"(?:[^"]+)"|\'(?:[^\']+)\'|[^"\' >][^ >]+[^"\' >]))/is',$splitarr[$i],-1,PREG_SPLIT_DELIM_CAPTURE);
					// END OPTION2

					//if(count($splitarr2[$i])<2) $splitarr[$i]=regular_express($regexp_array,$splitarr[$i]); // UNRELATED TO OPTIONS
					if(count($splitarr2[$i])>1){
						for($j=1;$j<count($splitarr2[$i]);$j+=2){
							$begin=preg_replace('/^([^=]+=.).*$/i','\1',$splitarr2[$i][$j]);
							$quote=substr($begin,-1);
							if($quote!='"' && $quote!='\''){
								$quote=null;
								$begin=substr($begin,0,-1);
							}
							$code=preg_replace('/^[^=]+='.($quote==null?'(.*)$/i':'.(.*).$/i'),'\1',$splitarr2[$i][$j]);
							if(substr($code,0,11)=='javascript:'){
								$begin.='javascript:';
								$code=substr($code,11);
							}
							if($firstjsrun) $code=";{$code};";
							$splitarr2[$i][$j]=$begin.regular_express($regexp_array,$code).$quote;
						}
						//$splitarr[$i]=implode(null,$splitarr2[$i]); // OPTION2
					}
				}

				# remove scripts
				elseif($firstrun && !empty($_COOKIE[COOK_PREF.'_remove_scripts']) && strtolower(substr($splitarr[$i],0,7))=='<script')
					$splitarr[$i]=null;

				# parse valid stuff
				elseif(($i%2==0 && $regexp_array[1]==1) || ($regexp_array[1]==2 && strtolower(substr($splitarr[$i],0,7))=='<script') || ($key=='text/css' && strtolower(substr($splitarr[$i],0,6))=='<style')){
					$splitarr[$i]=regular_express($regexp_array,$splitarr[$i]);
				}

				# script purge cleanup
				if($firstrun && empty($_COOKIE[COOK_PREF.'_remove_scripts']) && strtolower(substr($splitarr[$i],-9))=='</script>' && !preg_match('/^[^>]*src/i',$splitarr[$i])){
					$splitarr[$i]=preg_replace('/'.END_OF_SCRIPT_TAG.'$/i',';'.COOK_PREF.'.purge();//--></script>',$splitarr[$i]);
				}

			}

			$firstrun=false;
			if($firstjsrun && $key=='application/x-javascript')
				$firstjsrun=false;
		}
	}

	// OPTION1

	if(empty($_COOKIE[COOK_PREF.'_remove_scripts'])){
		for($i=0;$i<count($splitarr);$i+=2){
			$splitarr[$i]=implode(null,$splitarr2[$i]);
		}
	}
	// END OPTION1

	return implode(null,$splitarr);
}

# }}}

//$starttime=microtime(true); # BENCHMARK
$body=parse_all_html($body);
//$parsetime=microtime(true)-$starttime; # BENCHMARK

# PROXY EXECUTION: PAGE PARSING: PROXY HEADERS/JAVASCRIPT {{{

if(CONTENT_TYPE=='text/html'){
	$big_headers='<meta name="robots" content="noindex, nofollow" />'.
			(PAGETYPE_ID===PAGETYPE_FRAMED_PAGE?'<base target="_top">':null).
			'<link rel="shortcut icon" href="'.surrogafy_url($curr_urlobj->get_proto().'://'.$curr_urlobj->get_servername().'/favicon.ico').'" />'.
			(empty($_COOKIE[COOK_PREF.'_remove_scripts'])?
				'<script type="text/javascript" src="'.THIS_SCRIPT.'?js_funcs'.(PAGE_FRAMED?'_framed':null).'"></script>'.
				'<script type="text/javascript" src="'.THIS_SCRIPT.'?js_regexps'.(PAGE_FRAMED?'_framed':null).'"></script>'.
				'<script language="javascript">'.
				//'<!--'.
				COOK_PREF.'.DOCUMENT_REFERER="'.(URL_FORM?str_replace('"','\\"',$referer):null).'";'.
				COOK_PREF.'.CURR_URL="'.str_replace('"','\\"',$curr_urlobj->get_url()).'"+location.hash;'.COOK_PREF.'.gen_curr_urlobj();'.
				COOK_PREF.'.LOCATION_SEARCH="'.($curr_urlobj->get_query()!=null?'?'.str_replace('"','\\"',$curr_urlobj->get_query()):null).'";'.
				COOK_PREF.'.LOCATION_HOSTNAME="'.str_replace('"','\\"',$curr_urlobj->get_servername()).'";'.
				COOK_PREF.'.LOCATION_PORT="'.str_replace('"','\\"',$curr_urlobj->get_portval()).'";'.
				COOK_PREF.'.ENCRYPT_URLS='.bool_to_js(ENCRYPT_URLS).';'.
				COOK_PREF.'.ENCRYPT_COOKS='.bool_to_js(ENCRYPT_COOKS).';'.
				COOK_PREF.'.URL_FORM='.bool_to_js(URL_FORM).';'.
				COOK_PREF.'.PAGE_FRAMED='.bool_to_js(PAGE_FRAMED).';'.
				COOK_PREF.".USERAGENT=\"{$useragent}\";".
				(URL_FORM && PAGETYPE_ID==PAGETYPE_FRAMED_PAGE?'if('.COOK_PREF.'.theparent=='.COOK_PREF.'.thetop) '.
					COOK_PREF.'.eventify("'.$curr_urlobj->get_proto().'","'.$curr_urlobj->get_servername().'");'
				:null).
				//'//-->'.
				'</script>'
			:null)
	;

	$body=preg_replace('/(?:(<(?:head|body)[^>]*>)|(<(?:\/head|meta|link|script)))/i',"\\1$big_headers\\2",$body,1);
	unset($big_headers);
}
elseif(CONTENT_TYPE=='application/x-javascript' || CONTENT_TYPE=='text/javascript') $body.=';'.COOK_PREF.'.purge();';

# }}}

# }}}

## Retrieved, Parsed, All Ready to Output ##
echo $body;
//echo 'total time: '.(microtime(true)-$totstarttime)."<br />parse time: {$parsetime} seconds".(isset($oparsetime)?"<br />other time 1: {$oparsetime} seconds":null).(isset($oparsetime2)?"<br />other time 2: {$oparsetime2} seconds":null); # BENCHMARK

# }}}

finish();

############
## THE END ##
##############
#
# VIM is the ideal way to edit this file.  Automatic folding occurs making the
# blocks of code easier to read and navigate
# vim:foldmethod=marker
#
################## ?>
