<?php
session_start();
chdir("/xampp/HomenetSpaces/hnsdesktop/");

include ("db.member.inc.php");
include ("login.inc.php");

if (isset($_SESSION['logged']) && ($_SESSION['logged'] == 1)) { // user is logged in
include ("users_online.loggedin.inc.php"); // include db.om.inc if this gets removed
include ("check_session.inc.php");
} else {
include ("users_online.notloggedin.inc.php"); // include db.om.inc if this gets removed
include ("check_session.inc.php");
}
?>
<!DOCTYPE html>
<html lang="en" dir="ltr">

<head>
<title>Homenet Spaces OS | HTML5 Canvas!</title>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7,chrome=1" /> 
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1, windows-1252" />
<meta name="author" content="Homenet Spaces Andrew Gerst" />
<meta name="copyright" content="© HnS Desktop" />
<meta name="keywords" content="Homenet, Spaces, HnS, OS, Web, Desktop, The, Place, To, Be, Creative, Andrew, Gerst, Free, Profile, Profiles, Apps, Applications" />
<meta name="description" content="Welcome to Homenet Spaces OS | This is the place to be creative! Feel free to add yourself to our wonderful community by registering! HnS Desktop" />
<script type="text/javascript" src="/hnsdesktop/jquery.js"></script>
<script type="text/javascript" src="/hnsdesktop/jquery.corners.js"></script>
<base target="_top" />
<!--[if IE]>
<style>
#fundo {
	filter:progid:DXImageTransform.Microsoft.Alpha(opacity=60);
}
</style>
<![endif]-->
<style type="text/css">
body {
background-color: #fff;
background-image: url("fundo.png");
background-repeat: repeat-x;
font-family: verdana, geneva, arial, helvetica, sans-serif;
margin: 0px;
}

#tela {
position: absolute;
left: 50%;
top: 50%;
display: none;
}

#contorno {
width: 742px;
height: 386px;
border: 2px solid gray;
background-color: transparent;
background-image: url("tela.png");
padding: 8px;
-moz-border-radius: 10px;
-webkit-border-radius: 10px;
border-radius: 10px;
}

#desenho {
border: 1px solid #666;
width: 560px;
height: 383px;
margin: 0px 8px 0px 0px;
background-color: #fff;
position: relative;
}

#logo {
border: 1px solid #666;
text-align: center;
padding: 5px;
width: 160px;
}

.borda {
width: 170px;
border: 1px solid #666;
font-size: 11px;
background-color: white;
margin-top: 8px;
}

#opcoes {
float: left;
background-color: #ddd;
width: 54px;
height: 330px;
padding: 2px;
border-right: 1px solid #ccc;
}

#opcoes2 {
float: left;
background-color: #ddd;
width: 26px;
height: 301px;
padding: 2px;
border-left: 1px solid #ccc;
}

#menu {
width: 382px;
margin: 0;
}

.op {
float: left;
width: 24px;
height: 24px;
background-color: #eee;
border: 1px solid #ccc;
cursor: pointer;
background-repeat: no-repeat;
background-position: center center;
}

.op:hover {
border-color: #0000ff;
}

.op_sel {
float: left;
width: 24px;
height: 24px;
background-color: #00bfff;
border: 1px solid #0000ff;
cursor: pointer;
background-repeat: no-repeat;
background-position: center center;
}

#canvas {
float: right;
background-color: #fff;
}

#canvasPrev {
position: relative;
float: right;
margin-top: -334px;
z-index: 2;
}

#cores {
clear: both;
background-color: #ddd;
height: 26px;
padding: 2px;
border-top: 1px solid #ccc;
text-align: center;
}

.titulo {
background-image: url("titulo.jpg");
height: 16px;
border-bottom: 1px solid #666;
font-size: 11px;
font-weight: bold;
color: #666;
padding: 1px 4px 0 4px;
text-align: center;
}

.conteudo  {
padding: 4px;
font-size: 11px;
font-weight: normal;
color: #333;
overflow: auto;
}

.cor {
float: left;
border: 1px solid black;
width: 10px;
height: 10px;
margin-left: 2px;
cursor: pointer;
overflow: hidden;
}

.envio {
font-weight: bolder;
font-size: 11px;
color: #666;
text-decoration: none;
}

ul.lista {
padding-left: 0;
padding-right: 0;
border: none;
margin: 0;
}

ul.lista li {
list-style: none;
}

ul.lista li a {
margin: 0px;
padding-left: 0px;
padding-right: 0px;
padding-top: 3px;
padding-bottom: 2px;
text-decoration: none;
color: blue;
display: block;
border-bottom: 1px solid #ddd;
}
ul.lista li a:hover {
background-color: #eee;
}

.link_off a {
color: gray;
}

.imgSalvar {
width: 125px;
height: 84px;
}

#texto_alerta {
position: absolute;
top: 50%;
left: 78px;
width: 282px;
text-align: center;
font: bolder 19px arial;
line-height: normal;
color: #999;
z-index: 1;
display: none;
}

hr {
margin: 0 0 3px 0;
padding: 0;
}

.divhr {
margin: 2px 0 2px 0;
border-top: 1px solid gray;
height: 1px;
overflow: hidden;
clear: both;
}

#janela {
border: 3px solid gray;
position: absolute;
width: 330px;
height: 380px;
background-color: white;
top: 50%;
left: 50%;
z-index: 11;
text-align: center;
display: none;
margin-left: -167px;
margin-top: -195px;
}

#fundo {
display: none;
position: absolute;
top: 0px;
left: 0px;
height: 100%;
width: 100%;
background-color: black;
opacity: .60;
z-index: 10;
}

.transparente {
-moz-opacity:0.75;
-khtml-opacity:.75;
opacity:.75;
}

#carregando {
font: bold 12px verdana;
background-color: white;
color: #333;
}

#canvas {
float: none;
position: absolute;
top: 0;
left: 0;
background-color: transparent;
background-image: url(http://www.gartic.com.br/imgs/b.gif);
z-index: 3;
}
 
#canvasPrev {
float: none;
position: absolute;
margin: 0;
top: 0;
left: 0;
z-index: 4;
}

#imagemBase {
position: absolute;
top: 0;
left: 0;
z-index: 1;
}
</style>
<script> 
var texto = {txtWidth:"Width",txtHeight:"Height",headerDimensions:"Dimensions",headerSave:"Save",txtTitle:"Title",opSaveOnGallery:"Sate to Gallery",errTitle:"Error: Your drawing must have a title.",txtSaving:"Saving drawing...",errAttributes1:"Width must be between 100px and 501px.",errAttributes2:"Height must be between 100px and 334px.",errInvalidAttr:"Invalid height and width!",errSaving:"Error: unable to save the drawing.",headerGallery:"Gallery",txtError:"Error",txtSaved:"Drawing successfully saved!",txtSave:"Save",errAuthor:"Error: this drawing hasn't been made by you.",alertClear:"Are you sure you want to erase the drawing?",fim:""};
</script> 
</head>

<body scroll="no">
<!-- Begin page content -->
<div id="main">
<span id="carregando">Loading...</span> 
<div id="fundo"></div> 
<div id="borracha"></div> 
<div id="janela"> 
	<div class="titulo"> 
		<div id="janela_titulo" style="float: left">Test</div> 
		<div style="float: right"><a href="javascript:fecharJanela()">Close</a></div> 
	</div> 
	<div id="janela_conteudo" style="padding: 4px; font-size: 10px; font-weight: normal; color: #666;"> 
	</div> 
</div> 
 
<div id="tela"> 
	<div id="contorno"> 
		<div style="float: left;"> 
			<div id="desenho" style="background-color: gray;"> 
				<div id="texto_alerta"></div> 
				<div id="tit_desenho" class="titulo">Drawing</div> 
				<div id="opcoes"> 
					<div id="opcoes_vez"> 
						<div> 
							<div id="op_0" class="op" style="margin-bottom: 2px; background-image: url('op_lapis.gif');" onclick="sel_opcao(0)" title="Pencil"> 
							</div> 
							<div id="op_1" class="op" style="margin-bottom: 2px; margin-left: 2px; background-image: url('op_borracha.gif');" onclick="sel_opcao(1)" title="Eraser"> 
							</div> 
						</div> 
						<div> 
							<div id="op_2" class="op" style="margin-bottom: 2px; background-image: url('op_quadrado.gif');" onclick="sel_opcao(2)" title="Solid rectangle"> 
							</div> 
							<div id="op_3" class="op" style="margin-bottom: 2px; margin-left: 2px; background-image: url('op_quadrado_borda.gif');" onclick="sel_opcao(3)" title="Empty rectangle"> 
							</div> 
						</div> 
						<div> 
							<div id="op_4" class="op" style="margin-bottom: 2px; background-image: url('op_circulo.gif');" onclick="sel_opcao(4)" title="Solid ellipse"> 
							</div> 
							<div id="op_5" class="op" style="margin-bottom: 2px; margin-left: 2px; background-image: url('op_circulo_borda.gif');" onclick="sel_opcao(5)" title="Empty ellipse"> 
							</div> 
						</div> 
						<div> 
							<div id="op_6" class="op" style="margin-bottom: 2px; background-image: url('op_linha.gif');" onclick="sel_opcao(6)" title="Line"> 
							</div> 
							<div id="op_7" class="op" style="margin-bottom: 2px; margin-left: 2px; background-image: url('op_balde.gif');" onclick="sel_opcao(7)" title="Flood fill"> 
							</div> 
						</div> 
						<div> 
							<div id="op_8" class="op" style="margin-bottom: 2px; background-image: url('op_piker.gif');" onclick="sel_opcao(8)" title="Color picker"> 
							</div> 
							<div id="novo" class="op" style="margin-bottom: 2px; margin-left: 2px; background-image: url('op_nova.gif');" onclick="sel_limpar()" title="Fresh screen"> 
							</div> 
						</div> 
						<div> 
							<div id="op_desfazer" class="op" style="margin-bottom: 2px; background-image: url('op_desfazer.gif'); background-color: #ccc;" onclick="sel_desfazer()" title="Undo"> 
							</div> 
							<div id="op_refazer" class="op" style="margin-bottom: 2px; margin-left: 2px; background-image: url('op_refazer.gif'); background-color: #ccc;" onclick="sel_refazer()" title="Redo"> 
							</div> 
						</div> 
					</div> 
				</div> 
				<div id="telaCanvas" style="float: right; line-height: 0; width: 501px; height: 334px; overflow: auto;"> 
					<div id="areaCanvas" style="width: 501px; position: relative;"> 
						<div style="position: absolute; width: 501px; height: 334px; z-index: 2;"></div> 
						<canvas id="canvas" width="501" height="334"></canvas> 
						<canvas id="canvasPrev" width="501" height="334"></canvas> 
					</div> 
				</div> 
				<div id="cores"> 
					<div id="dica_dada"></div> 
					<div id="cores_vez"> 
						<div id="cor_escolhida" class="op" style="border-color: black; float: left; background-color: #000;" title=""></div> 
						<div style="float: left;"> 
							<div> 
								<div class="cor" style="background-color: #000;" onclick="sel_cor('x000000',true)"></div> 
								<div class="cor" style="background-color: #666;" onclick="sel_cor('x666666',true)"></div> 
								<div class="cor" style="background-color: #8b0000;" onclick="sel_cor('x8b0000',true)"></div> 
								<div class="cor" style="background-color: #008b00;" onclick="sel_cor('x008b00',true)"></div> 
								<div class="cor" style="background-color: #00008b;" onclick="sel_cor('x00008b',true)"></div> 
								<div class="cor" style="background-color: #008b8b;" onclick="sel_cor('x008b8b',true)"></div> 
								<div class="cor" style="background-color: #8b8b00;" onclick="sel_cor('x8b8b00',true)"></div> 
								<div class="cor" style="background-color: #8b4500;" onclick="sel_cor('x8b4500',true)"></div> 
								<div class="cor" style="background-color: #8b0a50;" onclick="sel_cor('x8b0a50',true)"></div> 
								<div class="cor" style="background-color: #551a8b;" onclick="sel_cor('x551a8b',true)"></div> 
								<div class="cor" style="background-color: #548b54;" onclick="sel_cor('x548b54',true)"></div> 
								<div class="cor" style="background-color: #8b6969;" onclick="sel_cor('x8b6969',true)"></div> 
								<div class="cor" style="background-color: #8b7b8b;" onclick="sel_cor('x8b7b8b',true)"></div> 
							</div> 
							<div style="clear: both;"> 
								<div class="cor" style="margin-top: 2px; background-color: #fff;" onclick="sel_cor('xffffff',true)"></div> 
								<div class="cor" style="margin-top: 2px; background-color: #ccc;" onclick="sel_cor('xcccccc',true)"></div> 
								<div class="cor" style="margin-top: 2px; background-color: #ff0000;" onclick="sel_cor('xff0000',true)"></div> 
								<div class="cor" style="margin-top: 2px; background-color: #00ff00;" onclick="sel_cor('x00ff00',true)"></div> 
								<div class="cor" style="margin-top: 2px; background-color: #0000ff;" onclick="sel_cor('x0000ff',true)"></div> 
								<div class="cor" style="margin-top: 2px; background-color: #00ffff;" onclick="sel_cor('x00ffff',true)"></div> 
								<div class="cor" style="margin-top: 2px; background-color: #ffff00;" onclick="sel_cor('xffff00',true)"></div> 
								<div class="cor" style="margin-top: 2px; background-color: #ff7f00;" onclick="sel_cor('xff7f00',true)"></div> 
								<div class="cor" style="margin-top: 2px; background-color: #ff1493;" onclick="sel_cor('xff1493',true)"></div> 
								<div class="cor" style="margin-top: 2px; background-color: #9b30ff;" onclick="sel_cor('x9b30ff',true)"></div> 
								<div class="cor" style="margin-top: 2px; background-color: #9aff9a;" onclick="sel_cor('x9aff9a',true)"></div> 
								<div class="cor" style="margin-top: 2px; background-color: #ffc1c1;" onclick="sel_cor('xffc1c1',true)"></div> 
								<div class="cor" style="margin-top: 2px; background-color: #ffe1ff;" onclick="sel_cor('xffe1ff',true)"></div> 
							</div> 
						</div> 
						<div style="float: left"> 
							<div id="degrade" class="cor color" style="height: 24px; background-image: url('picker_degrade.png');" colorValue="ffffff"></div> 
						</div> 
						<div id="op_larg10" class="op" style="margin-left: 2px; float: right; background-image: url('op_larg10.gif');" onclick="sel_largura(10)" title=" 10"></div> 
						<div id="op_larg8" class="op" style="margin-left: 2px; float: right; background-image: url('op_larg8.gif');" onclick="sel_largura(8)" title=" 8"></div> 
						<div id="op_larg6" class="op" style="margin-left: 2px; float: right; background-image: url('op_larg6.gif');" onclick="sel_largura(6)" title=" 6"></div> 
						<div id="op_larg4" class="op" style="margin-left: 2px; float: right; background-image: url('op_larg4.gif');" onclick="sel_largura(4)" title=" 4"></div> 
						<div id="op_larg2" class="op" style="margin-left: 2px; float: right; background-image: url('op_larg2.gif');" onclick="sel_largura(2)" title=" 2"></div> 
					</div> 
				</div> 
			</div> 
		</div> 
		<div style="float: left;"> 
			<div id="logo"> 
				<a href="http://www.gartic.net/"><img src="logo_room.png" width="142" height="70" alt="Gartic" title="Gartic" style="border: none;" /></a> 
			</div> 
			<div class="borda"> 
				<div id="tit_desenho" class="titulo">Animation</div> 
				<div style="padding: 5px; height: 44px;"> 
					<ul id="iniciarAni" class="lista"> 
						<li><a href="#" onclick="ani.iniciar(1); return false;">Start animation</a></li> 
					</ul> 
 
					<ul id="pararAni" class="lista" style="display: none;"> 
						<li><a href="#" onclick="ani.pausar(); return false;">Pause animation</a></li> 
						<li><a href="#" onclick="ani.parar(); return false;">Stop animation</a></li> 
					</ul> 
 
					<ul id="continuarAni" class="lista" style="display: none;"> 
						<li><a href="#" onclick="ani.continuar(); return false;">Continue animation</a></li> 
						<li><a href="#" onclick="ani.parar(); return false;">Stop animation</a></li> 
					</ul> 
				</div> 
			</div> 
			<div class="borda"> 
				<div id="tit_desenho" class="titulo">Options</div> 
				<div style="padding: 5px; height: 180px; overflow: auto;"> 
					<ul class="lista"> 
						<li><a href="../">Exit</a></li> 
						<li><a href="#" onclick="dimensao(); return false;">Change dimensions</a></li> 
						<li><a href="#" onclick="salvaDesenho(0); return false;">Save as PNG</a></li> 
						<li><a href="#" onclick="salvaDesenho(1); return false;">Save as JPG</a></li> 
						<li><a href="#" onclick="salvaDesenho(2); return false;">Save as GIF</a></li> 
					</ul> 
				</div> 
			</div> 
		</div> 
	</div> 
</div> 
<script> 
var matrizDesenho;
var imgBaseID = 0;
var desenhistaID = 0;

// ----------------------------------------------------------------------------------

/*
 * Desenvolvido por Henrique Moreira e Rafael Duarte
 * Proibida a cópia
 */

function Ajax()
{
	//Variaveis globais (privadas)
	var self = this;
	var vez = null;
	var fila = new Array();
	var method = "POST";
	var lastTime = 0;
	var assincrono = true;
	var inicialTime;
	var xmlhttp;
	var esperaMaxima = 5000;

	novoObjeto();
	setInterval(function() { rotina() },2000);

	//cria objeto ajax
	function novoObjeto(){
		try{
			xmlhttp = new XMLHttpRequest();
		}
		catch(ee){
			try{
				xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
			}
			catch(e){
				try{
					xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
				}
				catch(E){
					xmlhttp = false;
				}
			}
		}
	}

	//objeto de requisição
	function objRequest(pag,param,func,met) {
		this.pag = pag;
		this.param = param;
		this.func = func;
		this.method = met;
	}

	//calcula tempo atual
	function UTCTime() {
		var atual = new Date();
		return Date.UTC(atual.getFullYear(),atual.getMonth(),atual.getDate(),atual.getHours(),atual.getMinutes(),atual.getSeconds(),atual.getMilliseconds());
	}

	//funcao que realiza requisição
	function ajaxRequest() {
		inicialTime = UTCTime();

		if (vez.method == null)
			xmlhttp.open("POST", vez.pag + "?rand=" + UTCTime(), assincrono);
		else {
			var parametro = (vez.param.length > 0) ? '&'+vez.param : '';
			xmlhttp.open("GET", vez.pag + "?r=" + UTCTime() + parametro,assincrono);
		}

		//bug firefox
		try {
			xmlhttp.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
		}
		catch(e) {

		}

		xmlhttp.onreadystatechange = function(){
			if (xmlhttp.readyState == 4){
				var status = 0;

				try
				{
					status = xmlhttp.status;
				}
				catch(e)
				{
					novoObjeto();
					return;
				}

				//caso seja uma pagina valida
				if(status == 200)
				{
					if(vez.func != null)
					{
						var texto = xmlhttp.responseText;
						eval(vez.func+"(texto)");
					}

					lastTime = UTCTime();

					if(fila.length > 0)
					{
						vez = fila.shift();
						delay();
					}
					else
						vez = null;
				}
				// caso ocora algum erro...

				else if(status != 0)
				{
					inicialTime = UTCTime();
					setTimeout(function() { self.abortar() },10000);
				}

			}
		};
		//bug firefox
		try {
			if(method == "POST")
				xmlhttp.send(vez.param);
			else
				xmlhttp.send(null);
		}
		catch(e) {

		}
	}


	function delay()
	{
		var diferenca = UTCTime() - lastTime;

		if(diferenca > 700)
			ajaxRequest();
		else
		{
			diferenca = 700 - diferenca;
			setTimeout(function() { ajaxRequest() },diferenca);
		}
	}
	
	function rotina()
	{
		if(vez != null)
		{
			var diferenca = UTCTime() - inicialTime;

			if(diferenca > esperaMaxima)
			    self.abortar();
		}
	}

	this.chamada = function (pag,param,func,met){
		if(vez == null)
		{
			//adiciona para a chamada atual
			vez = new objRequest(pag,param,func,met);
			//delay, if need
			delay();
		}
		else
		{
			//adiciona chamada na fila
			fila.push(new objRequest(pag,param,func,met));
		}
	}

	//altera metodo
	this.setMethod = function(metodo) {
		method = metodo;
	}

	//obtem metodo
	this.getMethod = function() {
		return method;
	}

	//aborta chamada ajax
	this.abortar = function(){
		xmlhttp.abort();
		ajaxRequest();
	}

	//altera tipo do ajax
	this.setAssicrono = function(val) {
		assicrono = val;
	}
	
	//altera tipo do ajax
	this.setEsperaMaxima = function(val) {
		esperaMaxima = val;
	}

	//obtem tipo do ajax
	this.getAssicrono = function() {
		return assincrono;
	}

	this.isVezNull = function() {
		return (vez == null);
	}
}

function url_encode(str) {
    var hex_chars = "0123456789ABCDEF";
    var noEncode = /^([a-zA-Z0-9\_\-\.])$/;
    var n, strCode, hex1, hex2, strEncode = "";

    for(n = 0; n < str.length; n++) {
        if (noEncode.test(str.charAt(n))) {
            strEncode += str.charAt(n);
        } else {
		        strCode = str.charCodeAt(n);
		        hex1 = hex_chars.charAt(Math.floor(strCode / 16));
		        hex2 = hex_chars.charAt(strCode % 16);
		        strEncode += "%" + (hex1 + hex2);
		    }
		}
return strEncode;
}

// ----------------------------------------------------------------------------------

/**
 * jscolor, JavaScript Color Picker
 *
 * @version 1.2.3
 * @license http://www.gnu.org/copyleft/lesser.html  GNU Lesser General Public License
 * @author  Honza Odvarko <honza@odvarko.cz>
 * @created 2008-06-15
 * @updated 2009-02-25
 * @link    http://jscolor.com
 */


var jscolor = {
	dir : '', // location of jscolor directory (leave empty to autodetect)
	bindClass : 'color', // class name
	bindTag : 'div',
	binding : true, // automatic binding via <input class="...">
	preloading : true, // use image preloading?
	instancia : null,


	install : function() {
		jscolor.addEvent(window, 'load', jscolor.init)
	},


	init : function() {
		if(jscolor.binding) {
			jscolor.bind();
		}
		if(jscolor.preloading) {
			jscolor.preload();
		}
	},


	getDir : function() {
		if(!jscolor.dir) {
			var detected = jscolor.detectDir();
			jscolor.dir = detected!=false ? detected : '';
		}
		return jscolor.dir;
	},


	detectDir : function() {
		var base = location.href;

		var e = document.getElementsByTagName('base');
		for(var i=0; i<e.length; i++) {
			if(e[i].href) base = e[i].href;
		}

		var e = document.getElementsByTagName('script');
		for(var i=0; i<e.length; i++) {
			if(e[i].src && /(^|\/)jscolor\.js([?#].*)?$/i.test(e[i].src)) {
				var src = new jscolor.URI(e[i].src);
				var srcAbs = src.toAbsolute(base);
				srcAbs.path = srcAbs.path.replace(/[^\/]+$/, ''); // remove filename
				delete srcAbs.query;
				delete srcAbs.fragment;
				return srcAbs.toString();
			}
		}
		return false;
	},


	bind : function() {
		var matchClass = new RegExp('(^|\\s)('+jscolor.bindClass+')\\s*(\\{[^}]*\\})?', 'i');
		var e = document.getElementsByTagName(jscolor.bindTag);
		for(var i=0; i<e.length; i++) {
			var m;
			if(!e[i].color && e[i].className && (m = e[i].className.match(matchClass))) {
				var prop = {};
				if(m[3]) {
					try {
						eval('prop='+m[3]);
					} catch(eInvalidProp) {}
				}
				e[i].color = new jscolor.color(e[i], prop);
			}
		}
	},


	preload : function() {
		for(var fn in jscolor.imgRequire) {
			jscolor.loadImage(fn);
		}
	},


	images : {
		pad : [ 181, 101 ],
		sld : [ 16, 101 ],
		cross : [ 15, 15 ],
		arrow : [ 7, 11 ]
	},


	imgRequire : {},
	imgLoaded : {},


	requireImage : function(filename) {
		jscolor.imgRequire[filename] = true;
	},


	loadImage : function(filename) {
		if(!jscolor.imgLoaded[filename]) {
			jscolor.imgLoaded[filename] = new Image();
			jscolor.imgLoaded[filename].src = jscolor.getDir()+filename;
		}
	},


	fetchElement : function(mixed) {
		return typeof(mixed) == 'string' ? document.getElementById(mixed) : mixed;
	},


	addEvent : function(el, evnt, func) {
		if(el.addEventListener) {
			return el.addEventListener(evnt, func, false);
		} else if(el.attachEvent) {
			return el.attachEvent('on'+evnt, func);
		} else {
			return false;
		}
	},


	fireEvent : function(el, evnt) {
		if(!el) {
			return false;
		} else if(document.createEventObject) {
			var ev = document.createEventObject();
			return el.fireEvent('on'+evnt, ev);
		} else if(document.createEvent) {
			var ev = document.createEvent('HTMLEvents');
			ev.initEvent(evnt, true, true);
			return el.dispatchEvent(ev);
		} else if(el['on'+evnt]) { // alternatively use the traditional event model (IE5)
			return el['on'+evnt]();
		} else {
			return false;
		}
	},


	getElementPos : function(e) {
		var e1=e, e2=e;
		var x=0, y=0;
		if(e1.offsetParent) {
			do {
				x += e1.offsetLeft;
				y += e1.offsetTop;
			} while(e1 = e1.offsetParent)
		}
		while((e2 = e2.parentNode) && e2.nodeName != 'BODY') {
			x -= e2.scrollLeft;
			y -= e2.scrollTop;
		}
		return [x, y];
	},


	getElementSize : function(e) {
		return [e.offsetWidth, e.offsetHeight];
	},


	getMousePos : function(e) {
		if(!e) e = window.event;
		if(typeof e.pageX == 'number') {
			return [e.pageX, e.pageY];
		} else if(typeof e.clientX == 'number') {
			return [
				e.clientX + document.body.scrollLeft + document.documentElement.scrollLeft,
				e.clientY + document.body.scrollTop + document.documentElement.scrollTop
			];
		}
	},


	getViewPos : function() {
		if(typeof window.pageYOffset == 'number') {
			return [window.pageXOffset, window.pageYOffset];
		} else if(document.body && (document.body.scrollLeft || document.body.scrollTop)) {
			return [document.body.scrollLeft, document.body.scrollTop];
		} else if(document.documentElement && (document.documentElement.scrollLeft || document.documentElement.scrollTop)) {
			return [document.documentElement.scrollLeft, document.documentElement.scrollTop];
		} else {
			return [0, 0];
		}
	},


	getViewSize : function() {
		if(typeof window.innerWidth == 'number') {
			return [window.innerWidth, window.innerHeight];
		} else if(document.body && (document.body.clientWidth || document.body.clientHeight)) {
			return [document.body.clientWidth, document.body.clientHeight];
		} else if(document.documentElement && (document.documentElement.clientWidth || document.documentElement.clientHeight)) {
			return [document.documentElement.clientWidth, document.documentElement.clientHeight];
		} else {
			return [0, 0];
		}
	},

	//ATE AKI

	URI : function(uri) { // See RFC3986

		this.scheme = null;
		this.authority = null;
		this.path = '';
		this.query = null;
		this.fragment = null;

		this.parse = function(uri) {
			var m = uri.match(/^(([A-Za-z][0-9A-Za-z+.-]*)(:))?((\/\/)([^\/?#]*))?([^?#]*)((\?)([^#]*))?((#)(.*))?/)
			this.scheme = m[3] ? m[2] : null
			this.authority = m[5] ? m[6] : null
			this.path = m[7]
			this.query = m[9] ? m[10] : null
			this.fragment = m[12] ? m[13] : null
			return this
		}

		this.toString = function() {
			var result = ''
			if(this.scheme != null) result = result + this.scheme + ':'
			if(this.authority != null) result = result + '//' + this.authority
			if(this.path != null) result = result + this.path
			if(this.query != null) result = result + '?' + this.query
			if(this.fragment != null) result = result + '#' + this.fragment
			return result
		}

		this.toAbsolute = function(base) {
			var base = new jscolor.URI(base)
			var r = this
			var t = new jscolor.URI

			if(base.scheme == null) return false

			if(r.scheme != null && r.scheme.toLowerCase() == base.scheme.toLowerCase()) {
				r.scheme = null
			}

			if(r.scheme != null) {
				t.scheme = r.scheme
				t.authority = r.authority
				t.path = removeDotSegments(r.path)
				t.query = r.query
			} else {
				if(r.authority != null) {
					t.authority = r.authority
					t.path = removeDotSegments(r.path)
					t.query = r.query
				} else {
					if(r.path == '') {
						t.path = base.path
						if(r.query != null) {
							t.query = r.query
						} else {
							t.query = base.query
						}
					} else {
						if(r.path.substr(0,1) == '/') {
							t.path = removeDotSegments(r.path)
						} else {
							if(base.authority != null && base.path == '') {
								t.path = '/'+r.path
							} else {
								t.path = base.path.replace(/[^\/]+$/,'')+r.path
							}
							t.path = removeDotSegments(t.path)
						}
						t.query = r.query
					}
					t.authority = base.authority
				}
				t.scheme = base.scheme
			}
			t.fragment = r.fragment

			return t
		}

		function removeDotSegments(path) {
			var out = ''
			while(path) {
				if(path.substr(0,3)=='../' || path.substr(0,2)=='./') {
					path = path.replace(/^\.+/,'').substr(1)
				} else if(path.substr(0,3)=='/./' || path=='/.') {
					path = '/'+path.substr(3)
				} else if(path.substr(0,4)=='/../' || path=='/..') {
					path = '/'+path.substr(4)
					out = out.replace(/\/?[^\/]*$/, '')
				} else if(path=='.' || path=='..') {
					path = ''
				} else {
					var rm = path.match(/^\/?[^\/]*/)[0]
					path = path.substr(rm.length)
					out = out + rm
				}
			}
			return out
		}

		if(uri) {
			this.parse(uri)
		}

	},


	/*
	 * Usage example:
	 * var myColor = new jscolor.color(myInputElement)
	 */

	color : function(target, prop) {


		this.required = true // refuse empty values?
		this.adjust = true // adjust value to uniform notation?
		this.hash = false // prefix color with # symbol?
		this.caps = true // uppercase?
		this.valueElement = target // value holder
		this.styleElement = target // where to reflect current color
		this.hsv = [0, 0, 1] // read-only  0-6, 0-1, 0-1
		this.rgb = [1, 1, 1] // read-only  0-1, 0-1, 0-1

		this.pickerOnfocus = true // display picker on focus?
		this.pickerMode = 'HSV' // HSV | HVS
		this.pickerPosition = (document.all && !window.opera) ? 'bottom' : 'top'; // left | right | top | bottom
		this.pickerFace = 10 // px
		this.pickerFaceColor = 'ThreeDFace' // CSS color
		this.pickerBorder = 1 // px
		this.pickerBorderColor = 'ThreeDHighlight ThreeDShadow ThreeDShadow ThreeDHighlight' // CSS color
		this.pickerInset = 1 // px
		this.pickerInsetColor = 'ThreeDShadow ThreeDHighlight ThreeDHighlight ThreeDShadow' // CSS color
		this.pickerZIndex = 5


		for(var p in prop) this[p] = prop[p]


		this.hidePicker = function() {
			if(isPickerOwner()) {
				removePicker()
			}
		}


		this.showPicker = function() {
			if(!isPickerOwner()) {
				var tp = jscolor.getElementPos(target) // target pos
				var ts = jscolor.getElementSize(target) // target size
				var vp = jscolor.getViewPos() // view pos
				var vs = jscolor.getViewSize() // view size
				var ps = [ // picker size
					2*this.pickerBorder + 4*this.pickerInset + 2*this.pickerFace + jscolor.images.pad[0] + 2*jscolor.images.arrow[0] + jscolor.images.sld[0],
					2*this.pickerBorder + 2*this.pickerInset + 2*this.pickerFace + jscolor.images.pad[1]
				]
				var a, b, c
				switch(this.pickerPosition.toLowerCase()) {
					case 'left': a=1; b=0; c=-1; break
					case 'right':a=1; b=0; c=1; break
					case 'top':  a=0; b=1; c=-1; break
					default:     a=0; b=1; c=1; break
				}
				var l = (ts[b]+ps[b])/2
				var pp = [ // picker pos
					-vp[a]+tp[a]+ps[a] > vs[a] ?
						(-vp[a]+tp[a]+ts[a]/2 > vs[a]/2 && tp[a]+ts[a]-ps[a] >= 0 ? tp[a]+ts[a]-ps[a] : tp[a]) :
						tp[a],
					-vp[b]+tp[b]+ts[b]+ps[b]-l+l*c > vs[b] ?
						(-vp[b]+tp[b]+ts[b]/2 > vs[b]/2 && tp[b]+ts[b]-l-l*c >= 0 ? tp[b]+ts[b]-l-l*c : tp[b]+ts[b]-l+l*c) :
						(tp[b]+ts[b]-l+l*c >= 0 ? tp[b]+ts[b]-l+l*c : tp[b]+ts[b]-l-l*c)
				]
				drawPicker(pp[a], pp[b])
			}
		}

		this.importColor = function() {
			if(!valueElement) {
				this.exportColor()
			} else {
				if(!this.adjust) {
					if(!this.fromString(valueElement.getAttribute('colorValue') , leaveValue)) {
						styleElement.style.backgroundColor = styleElement.jscStyle.backgroundColor
						styleElement.style.color = styleElement.jscStyle.color
						this.exportColor(leaveValue | leaveStyle)
					}
				} else if(!this.required && /^\s*$/.test(valueElement.getAttribute('colorValue'))) {
					valueElement.setAttribute('colorValue','')
					styleElement.style.backgroundColor = styleElement.jscStyle.backgroundColor
					styleElement.style.color = styleElement.jscStyle.color
					this.exportColor(leaveValue | leaveStyle)

				} else if(this.fromString(valueElement.getAttribute('colorValue'))) {
					// OK
				} else {
					this.exportColor()
				}
			}
		}


		this.exportColor = function(flags) {
			if(!(flags & leaveValue) && valueElement) {
				var value = this.toString()
				var oldValue = valueElement.getAttribute('colorValue')
				if(this.caps) value = value.toUpperCase()
				if(this.hash) value = '#'+value
				
				valueElement.setAttribute('colorValue',value)
			}
			if(!(flags & leaveStyle) && styleElement) {
				styleElement.style.backgroundColor = '#'+this.toString()
				styleElement.style.color =
					0.213 * this.rgb[0] +
					0.715 * this.rgb[1] +
					0.072 * this.rgb[2]
					< 0.5 ? '#fff' : '#000'
			}
			
			if(!(flags & leavePad) && isPickerOwner()) {
				redrawPad()
				if(value != oldValue) sel_cor('x'+value)
			}
			if(!(flags & leaveSld) && isPickerOwner()) {
				redrawSld()
				if(value != oldValue) sel_cor('x'+value)
			}
		}


		this.fromHSV = function(h, s, v, flags) { // null = don't change
			h<0 && (h=0) || h>6 && (h=6)
			s<0 && (s=0) || s>1 && (s=1)
			v<0 && (v=0) || v>1 && (v=1)
			this.rgb = HSV_RGB(
				h==null ? this.hsv[0] : (this.hsv[0]=h),
				s==null ? this.hsv[1] : (this.hsv[1]=s),
				v==null ? this.hsv[2] : (this.hsv[2]=v)
			)
			this.exportColor(flags)
		}


		this.fromRGB = function(r, g, b, flags) { // null = don't change
			r<0 && (r=0) || r>1 && (r=1)
			g<0 && (g=0) || g>1 && (g=1)
			b<0 && (b=0) || b>1 && (b=1)
			var hsv = RGB_HSV(
				r==null ? this.rgb[0] : (this.rgb[0]=r),
				g==null ? this.rgb[1] : (this.rgb[1]=g),
				b==null ? this.rgb[2] : (this.rgb[2]=b)
			)
			if(hsv[0] != null) {
				this.hsv[0] = hsv[0]
			}
			if(hsv[2] != 0) {
				this.hsv[1] = hsv[1]
			}
			this.hsv[2] = hsv[2]
			this.exportColor(flags)
		}


		this.fromString = function(hex, flags) {
			var m = hex.match(/^\W*([0-9A-F]{3}([0-9A-F]{3})?)\W*$/i)
			if(!m) {
				return false
			} else {
				if(m[1].length == 6) { // 6-char notation
					this.fromRGB(
						parseInt(m[1].substr(0,2),16) / 255,
						parseInt(m[1].substr(2,2),16) / 255,
						parseInt(m[1].substr(4,2),16) / 255,
						flags
					)
				} else { // 3-char notation
					this.fromRGB(
						parseInt(m[1].charAt(0)+m[1].charAt(0),16) / 255,
						parseInt(m[1].charAt(1)+m[1].charAt(1),16) / 255,
						parseInt(m[1].charAt(2)+m[1].charAt(2),16) / 255,
						flags
					)
				}
				return true
			}
		}


		this.toString = function() {
			return (
				(0x100 | Math.round(255*this.rgb[0])).toString(16).substr(1) +
				(0x100 | Math.round(255*this.rgb[1])).toString(16).substr(1) +
				(0x100 | Math.round(255*this.rgb[2])).toString(16).substr(1)
			)
		}


		function RGB_HSV(r, g, b) {
			var n = Math.min(Math.min(r,g),b)
			var v = Math.max(Math.max(r,g),b)
			var m = v - n
			if(m == 0) return [ null, 0, v ]
			var h = r==n ? 3+(b-g)/m : (g==n ? 5+(r-b)/m : 1+(g-r)/m)
			return [ h==6?0:h, m/v, v ]
		}


		function HSV_RGB(h, s, v) {
			if(h == null) return [ v, v, v ]
			var i = Math.floor(h)
			var f = i%2 ? h-i : 1-(h-i)
			var m = v * (1 - s)
			var n = v * (1 - s*f)
			switch(i) {
				case 6:
				case 0: return [v,n,m]
				case 1: return [n,v,m]
				case 2: return [m,v,n]
				case 3: return [m,n,v]
				case 4: return [n,m,v]
				case 5: return [v,m,n]
			}
		}


		function removePicker() {
			delete jscolor.picker.owner
			document.getElementsByTagName('body')[0].removeChild(jscolor.picker.boxB)
		}
		
		var timer
		function blurFunction() {
			if(timer) clearTimeout(timer)
			
			if(!abortBlur) {
				timer = setTimeout(function(){ abortBlur || blurTarget(); abortBlur=false }, 800)
			} else {
				abortBlur = false
			}
		}

		function killEvent(e) {
			if(document.all && !window.opera) {
				event.cancelBubble = true;
				event.returnValue = false;
				event.stopped = true;
			}
			//OUTROS
			else {
				e.preventDefault();
				e.stopPropagation();
			}
		}

		function drawPicker(x, y) {
			if(!jscolor.picker) {
				jscolor.picker = {
					box : document.createElement('div'),
					boxB : document.createElement('div'),
					pad : document.createElement('div'),
					padB : document.createElement('div'),
					padM : document.createElement('div'),
					sld : document.createElement('div'),
					sldB : document.createElement('div'),
					sldM : document.createElement('div')
				}
				for(var i=0,segSize=4; i<jscolor.images.sld[1]; i+=segSize) {
					var seg = document.createElement('div')
					seg.style.height = segSize+'px'
					seg.style.fontSize = '1px'
					seg.style.lineHeight = '0'
					jscolor.picker.sld.appendChild(seg)
				}
				jscolor.picker.sldB.appendChild(jscolor.picker.sld)
				jscolor.picker.box.appendChild(jscolor.picker.sldB)
				jscolor.picker.box.appendChild(jscolor.picker.sldM)
				jscolor.picker.padB.appendChild(jscolor.picker.pad)
				jscolor.picker.box.appendChild(jscolor.picker.padB)
				jscolor.picker.box.appendChild(jscolor.picker.padM)
				jscolor.picker.boxB.appendChild(jscolor.picker.box)
			}

			var p = jscolor.picker

			// recompute controls positions
			posPad = [
				x+THIS.pickerBorder+THIS.pickerFace+THIS.pickerInset,
				y+THIS.pickerBorder+THIS.pickerFace+THIS.pickerInset ]
			posSld = [
				null,
				y+THIS.pickerBorder+THIS.pickerFace+THIS.pickerInset ]

			// controls interaction
			p.box.onmouseup = 
			p.box.onmouseout = function() { 
				target.focus()
				blurFunction()
			}
			p.box.onmouseover = function () { abortBlur = true }
			p.box.onmousedown = function() { abortBlur = true; }
			p.box.onmousemove = function(e) { abortBlur = true; holdPad && setPad(e); holdSld && setSld(e) }
			// ------
			p.padM.onmouseup =
			p.padM.onmouseout = function() { 
				if(holdPad) { holdPad=false; } 
				blurFunction()
			}
			p.padM.onmouseover = function () { abortBlur = true }
			p.padM.onmousedown = function(e) { abortBlur = true; holdPad=true; setPad(e) }
			p.padM.onmousemove = function(){ abortBlur = true }
			// ------
			p.sldM.onmouseup =
			p.sldM.onmouseout = function() { 
				if(holdSld) { holdSld=false; } 
				blurFunction()
			}
			p.sldM.onmouseover = function () { abortBlur = true }
			p.sldM.onmousedown = function(e) { abortBlur = true; holdSld=true; setSld(e) }
			p.sldM.onmousemove = function(){ abortBlur = true }

			// picker
			p.box.style.width = 4*THIS.pickerInset + 2*THIS.pickerFace + jscolor.images.pad[0] + 2*jscolor.images.arrow[0] + jscolor.images.sld[0] + 'px'
			p.box.style.height = 2*THIS.pickerInset + 2*THIS.pickerFace + jscolor.images.pad[1] + 'px'

			// picker border
			p.boxB.style.position = 'absolute'
			p.boxB.style.zIndex = 10;
			p.boxB.style.clear = 'both'
			p.boxB.style.left = x+'px'
			p.boxB.style.top = y+'px'
			p.boxB.style.zIndex = THIS.pickerZIndex
			p.boxB.style.border = THIS.pickerBorder+'px solid'
			p.boxB.style.borderColor = THIS.pickerBorderColor
			p.boxB.style.background = THIS.pickerFaceColor

			// pad image
			p.pad.style.width = jscolor.images.pad[0]+'px'
			p.pad.style.height = jscolor.images.pad[1]+'px'

			// pad border
			p.padB.style.position = 'absolute'
			p.padB.style.left = THIS.pickerFace+'px'
			p.padB.style.top = THIS.pickerFace+'px'
			p.padB.style.border = THIS.pickerInset+'px solid'
			p.padB.style.borderColor = THIS.pickerInsetColor

			// pad mouse area
			p.padM.style.position = 'absolute'
			p.padM.style.left = '0'
			p.padM.style.top = '0'
			p.padM.style.width = THIS.pickerFace + 2*THIS.pickerInset + jscolor.images.pad[0] + jscolor.images.arrow[0] + 'px'
			p.padM.style.height = p.box.style.height
			p.padM.style.cursor = 'crosshair'

			// slider image
			p.sld.style.overflow = 'hidden'
			p.sld.style.width = jscolor.images.sld[0]+'px'
			p.sld.style.height = jscolor.images.sld[1]+'px'

			// slider border
			p.sldB.style.position = 'absolute'
			p.sldB.style.right = THIS.pickerFace+'px'
			p.sldB.style.top = THIS.pickerFace+'px'
			p.sldB.style.border = THIS.pickerInset+'px solid'
			p.sldB.style.borderColor = THIS.pickerInsetColor

			// slider mouse area
			p.sldM.style.position = 'absolute'
			p.sldM.style.right = '0'
			p.sldM.style.top = '0'
			p.sldM.style.width = jscolor.images.sld[0] + jscolor.images.arrow[0] + THIS.pickerFace + 2*THIS.pickerInset + 'px'
			p.sldM.style.height = p.box.style.height
			try {
				p.sldM.style.cursor = 'pointer'
			} catch(eOldIE) {
				p.sldM.style.cursor = 'hand'
			}

			// load images in optimal order
			switch(modeID) {
				case 0: var padImg = 'hs.png'; break
				case 1: var padImg = 'hv.png'; break
			}
			p.padM.style.background = "url('"+jscolor.getDir()+"cross.gif') no-repeat"
			p.sldM.style.background = "url('"+jscolor.getDir()+"arrow.gif') no-repeat"
			p.pad.style.background = "url('"+padImg+"') 0 0 no-repeat"

			// place pointers
			redrawPad()
			redrawSld()

			jscolor.picker.owner = THIS
			document.getElementsByTagName('body')[0].appendChild(p.boxB)
		}


		function redrawPad() {
			// redraw the pad pointer
			switch(modeID) {
				case 0: var yComponent = 1; break
				case 1: var yComponent = 2; break
			}
			var x = Math.round((THIS.hsv[0]/6) * (jscolor.images.pad[0]-1))
			var y = Math.round((1-THIS.hsv[yComponent]) * (jscolor.images.pad[1]-1))
			jscolor.picker.padM.style.backgroundPosition =
				(THIS.pickerFace+THIS.pickerInset+x - Math.floor(jscolor.images.cross[0]/2)) + 'px ' +
				(THIS.pickerFace+THIS.pickerInset+y - Math.floor(jscolor.images.cross[1]/2)) + 'px'

			// redraw the slider image
			var seg = jscolor.picker.sld.childNodes

			switch(modeID) {
				case 0:
					var rgb = HSV_RGB(THIS.hsv[0], THIS.hsv[1], 1)
					for(var i=0; i<seg.length; i++) {
						seg[i].style.backgroundColor = 'rgb('+
							(rgb[0]*(1-i/seg.length)*100)+'%,'+
							(rgb[1]*(1-i/seg.length)*100)+'%,'+
							(rgb[2]*(1-i/seg.length)*100)+'%)'
					}
					break
				case 1:
					var rgb, s, c = [ THIS.hsv[2], 0, 0 ]
					var i = Math.floor(THIS.hsv[0])
					var f = i%2 ? THIS.hsv[0]-i : 1-(THIS.hsv[0]-i)
					switch(i) {
						case 6:
						case 0: rgb=[0,1,2]; break
						case 1: rgb=[1,0,2]; break
						case 2: rgb=[2,0,1]; break
						case 3: rgb=[2,1,0]; break
						case 4: rgb=[1,2,0]; break
						case 5: rgb=[0,2,1]; break
					}
					for(var i=0; i<seg.length; i++) {
						s = 1 - 1/(seg.length-1)*i
						c[1] = c[0] * (1 - s*f)
						c[2] = c[0] * (1 - s)
						seg[i].style.backgroundColor = 'rgb('+
							(c[rgb[0]]*100)+'%,'+
							(c[rgb[1]]*100)+'%,'+
							(c[rgb[2]]*100)+'%)'
					}
					break
			}
		}


		function redrawSld() {
			// redraw the slider pointer
			switch(modeID) {
				case 0: var yComponent = 2; break
				case 1: var yComponent = 1; break
			}
			var y = Math.round((1-THIS.hsv[yComponent]) * (jscolor.images.sld[1]-1))
			jscolor.picker.sldM.style.backgroundPosition =
				'0 ' + (THIS.pickerFace+THIS.pickerInset+y - Math.floor(jscolor.images.arrow[1]/2)) + 'px'
		}


		function isPickerOwner() {
			return jscolor.picker && jscolor.picker.owner == THIS
		}


		function blurTarget() {
			if(valueElement == target) THIS.importColor()
			if(THIS.pickerOnfocus) THIS.hidePicker()
		}


		function blurValue() {
			if(valueElement != target) THIS.importColor()
		}


		function setPad(e) {
			var posM = jscolor.getMousePos(e)
			var x = posM[0]-posPad[0]
			var y = posM[1]-posPad[1]
			switch(modeID) {
				case 0: THIS.fromHSV(x*(6/(jscolor.images.pad[0]-1)), 1 - y/(jscolor.images.pad[1]-1), null, leaveSld); break
				case 1: THIS.fromHSV(x*(6/(jscolor.images.pad[0]-1)), null, 1 - y/(jscolor.images.pad[1]-1), leaveSld); break
			}
		}


		function setSld(e) {
			var posM = jscolor.getMousePos(e)
			var y = posM[1]-posPad[1]
			switch(modeID) {
				case 0: THIS.fromHSV(null, null, 1 - y/(jscolor.images.sld[1]-1), leavePad); break
				case 1: THIS.fromHSV(null, 1 - y/(jscolor.images.sld[1]-1), null, leavePad); break
			}
		}


		var THIS = this
		var modeID = this.pickerMode.toLowerCase()=='hvs' ? 1 : 0
		var abortBlur = false
		var
			valueElement = jscolor.fetchElement(this.valueElement),
			styleElement = jscolor.fetchElement(this.styleElement)
		var
			holdPad = false,
			holdSld = false
		var
			posPad,
			posSld
		var
			leaveValue = 1<<0,
			leaveStyle = 1<<1,
			leavePad = 1<<2,
			leaveSld = 1<<3

		// target
		jscolor.addEvent(target, 'click', function() {
			if(THIS.pickerOnfocus) THIS.showPicker()
		})
		jscolor.addEvent(target, 'mouseout', function() {
			blurFunction()
		})

		// valueElement
		if(valueElement) {
			var updateField = function() {
				THIS.fromString(valueElement.getAttribute('colorValue'), leaveValue)
			}
			jscolor.addEvent(valueElement, 'keyup', updateField)
			jscolor.addEvent(valueElement, 'input', updateField)
			jscolor.addEvent(valueElement, 'blur', blurValue)
		}

		// styleElement
		if(styleElement) {
			styleElement.jscStyle = {
				backgroundColor : styleElement.style.backgroundColor,
				color : styleElement.style.color
			}
		}

		// require images
		switch(modeID) {
			case 0: jscolor.requireImage('hs.png'); break
			case 1: jscolor.requireImage('hv.png'); break
		}
		jscolor.requireImage('cross.gif');
		jscolor.requireImage('arrow.gif');

		this.importColor()
	}

}


jscolor.install()

// ----------------------------------------------------------------------------------

var ajax = new Ajax();
ajax.setEsperaMaxima(60000);

window.onload = startCanvas;

//rotina para obter o numero de jogadores em cada sala
function rotinaPing() {
	if(ajax.isVezNull) {
		ajax.chamada('ping.php','',null,true);
	}
}

//Variaveis globais
var iexplorer = (document.all && !window.opera) ? true : false;

var ani;
var desenho;
var des_cod = [];

var canvasElem;

//abre uma janela div
function abrirJanela(jWidth, jHeight, jTitle){
	var jTop, jLeft;
	
	$("fundo").style.display = "block";
	$("janela_titulo").innerHTML = jTitle;
	var janelaElem = $("janela");
	janelaElem.style.display = "block";
	janelaElem.style.width = jWidth+'px';
	janelaElem.style.height = jHeight+'px';
    janelaElem.style.marginLeft = (-1 * janelaElem.offsetWidth / 2) + "px";
    janelaElem.style.marginTop = (-1 * janelaElem.offsetHeight / 2) + "px";
}

//fecha a janela atual
function fecharJanela(){
	$("fundo").style.display = "none";
	$("janela").style.display = "none";
}

//salvar imagem no mural
function salvarMural() {
	$('janela_conteudo').innerHTML = '<form action="javascript:salvarMuralEnvio()">'+
									 '<table width="100%">'+
									 '<tr><td>'+texto.txtTitle+':</td><td><input type="text" size="7" id="tituloImagem" autocomplete="off" value="" /></td></tr>'+
									 '<tr><td colspan="2" align="center"><input type="submit" value="'+texto.txtSave+'" /></td></tr>'+
									 '</table>'+
									 '</form>';
	abrirJanela(180,130,texto.opSaveOnGallery);
}

function salvarMuralEnvio(tipo){
	desenho.comprimir();
	
	var titulo = document.getElementById('tituloImagem').value;
	if (titulo.replace(/\s/, '').length > 0) {
		$('janela_conteudo').innerHTML = texto.txtSaving;
		abrirJanela(200, 50, texto.headerSave);
		
		var dados = desenho.getSequencia();
		if(!imgBaseID)
			ajax.chamada('salvar_mural.php', 'titulo=' + url_encode(titulo) + '&dados=' + desenho.getSequencia() + '&width=' + desenho.getLargura() + '&height=' + desenho.getAltura()+'&desenhista='+desenhistaID, 'salvarMuralRetorno');
		else
			ajax.chamada('salvar_mural.php', 'titulo=' + url_encode(titulo) + '&dados=' + desenho.getSequencia() + '&width=' + desenho.getLargura() + '&height=' + desenho.getAltura()+'&desenhista='+desenhistaID+'&livre='+imgBaseID, 'salvarMuralRetorno');
	} else {
		alert(texto.errTitle);
	}
}

function salvarMuralRetorno(txt){
	var num = parseInt(txt);
	var msg;
	switch(num) {
		case 1:
			msg = texto.txtSaved; //Desenho gravado com sucesso!
			break;
		case 2:
			msg = texto.txtError;
			break;
		case 4:
			msg = texto.errAuthor; //Este desenho n&atilde;o foi produzido por v&ecirc;.
			break;
		default:
			msg = texto.txtError;
		
	}
	$('janela_conteudo').innerHTML = msg;
	abrirJanela(200, 50, texto.headerGallery);
}


//abre janela para alteração da dimenção
function dimensao() {
	$('janela_conteudo').innerHTML = '<form action="javascript:alterarDimencao()">'+
									 '<table width="100%">'+
									 '<tr><td>'+texto.txtWidth+':</td><td><input type="text" size="3" id="dimLargura" autocomplete="off" value="'+desenho.getLargura()+'" />px</td></tr>'+
									 '<tr><td>'+texto.txtHeight+':</td><td><input type="text" size="3" id="dimAltura" autocomplete="off" value="'+desenho.getAltura()+'" />px</td></tr>'+
									 '<tr><td colspan="2" align="center"><input type="submit" value="Ok" /></td></tr>'+
									 '</table>'+
									 '</form>';
	abrirJanela(180,130,texto.headerDimensions);
}

//altera a dimensão do desenho
function alterarDimencao() {
	var dimLargura = parseInt($('dimLargura').value);
	var dimAltura = parseInt($('dimAltura').value);
	
	if (!isNaN(dimLargura) && !isNaN(dimAltura)) {
		if (!(dimLargura > 501 || dimAltura > 334 || dimLargura < 100 || dimAltura < 100)) {
			$("canvas").width = $('dimLargura').value;
			$("canvas").height = $('dimAltura').value;
			$("canvasPrev").width = $('dimLargura').value;
			$("canvasPrev").height = $('dimAltura').value;
			$("areaCanvas").style.width = $('dimLargura').value + 'px';
			fecharJanela();
			
			//var sequencia = desenho.getSequencia();
			preparaCanvas();
			//desenho.setSequencia(sequencia);
		} else {
			alert(texto.errAttributes1+'\n'+texto.errAttributes2);
		}
	} else {
		alert(texto.errInvalidAttr);
	}
}

function salvaDesenho(tipo){
	desenho.comprimir();
	
	//verificando se não é o IE
	//if(!(document.all && !window.opera))
	//	var imagem = desenho.salvarPNG();

	$('janela_conteudo').innerHTML = 'Salvando desenho...';
	abrirJanela(200, 50, texto.headerSave);

	var dados = desenho.getSequencia();
	//document.getElementById("imagem").innerHTML = "Carregando...";
	if(!imgBaseID)
		ajax.chamada('criar_img.php','t='+tipo+'&dados='+desenho.getSequencia()+'&width='+desenho.getLargura()+'&height='+desenho.getAltura(),'desenhoSalvo');
	else
		ajax.chamada('criar_img.php','t='+tipo+'&dados='+desenho.getSequencia()+'&width='+desenho.getLargura()+'&height='+desenho.getAltura()+'&livre='+imgBaseID,'desenhoSalvo');
}

function desenhoSalvo(arq) {
	var partes = arq.split('-');
	
	if (partes[0] == '1') {
		//var titulo = "Clique com o bot&atilde;o direito e selecione 'Salvar Imagem'";
		location.href = 'download.php?nome='+partes[1];
		fecharJanela();
		//document.getElementById("imagem").innerHTML = "<img src=\"../imgs/praticar/"+partes[1]+"\" class=\"imgSalvar\" alt=\"" + titulo + "\" title=\"" + titulo + "\" />";
	} else {
		$('janela_conteudo').innerHTML = texto.errSaving; //Erro ao gravar desenho.
		abrirJanela(100, 50, texto.headerSave);
	}
}

//--------------------------------------------------------------------------------------
//EVENTOS DA PÁGINA

//funcao de nome pequeno pra acessar elemento
function $(id) {
	return document.getElementById(id);
}

//--------------------------------------------------------------------------------------

//Inicialização do canvas
function startCanvas() {
	var telaElem = $("tela");
	//mostrando tela do jogo
    telaElem.style.display = "block";
    //centralizando tela
    telaElem.style.marginLeft = (-1 * telaElem.offsetWidth / 2) + "px";
    telaElem.style.marginTop = (-1 * $("contorno").offsetHeight / 2) + "px";
    telaElem.style.top = "50%";
    telaElem.style.left = "50%";

	preparaCanvas();
	
	//ping para manter a sessão
	setInterval(rotinaPing,180000);
}

function preparaCanvas() {
	$("carregando").innerHTML = "";

	canvasElem = $("canvas");
	
	desenho = new Desenho(canvasElem,$("canvasPrev"),true,true);
	
	//verifica se esta abrindo um desenho
	if(imgBaseID) {
		desenho.setMatriz(matrizDesenho);
		desenho.transparencia();
	}
	
	//destativando recursos
	desenho.setMatriz = function() {};
	//desenho.setSequencia = function() {};
	
	ani = new Animacao(desenho);

	desenho.onSelCor = function() {
		sel_cor(this.getCorReal().replace('#','x'),true);
	}
	desenho.onStartDraw = function() {
		try {
			$("op_desfazer").style.backgroundColor = "";
			$("op_refazer").style.backgroundColor = "#CCC";
		} catch(e) {}
	}
	ani.onComecar = function () {
		$("iniciarAni").style.display = "none";
		$("pararAni").style.display = "block";
		if(imgBaseID)desenho.transparencia();
	}
	ani.onPausar = function () {
		$("pararAni").style.display = "none";
		$("continuarAni").style.display = "block";
	}
	ani.onContinuar = function () {
		$("continuarAni").style.display = "none";
		$("pararAni").style.display = "block";
	}
	ani.onTerminar = function () {
		$("continuarAni").style.display = "none";
		$("pararAni").style.display = "none";
		$("iniciarAni").style.display = "block";
	}

	desenho.liberar(true);
	confPadrao();
}

//configuração padão do desenho
function confPadrao() {
    var cor = 'x000000';
    var borda = 4;
    var tipo = 0;

    sel_opcao(tipo);
    sel_largura(borda);
    sel_cor(cor,true);
}

function sel_opcao(num) {
    var opcoes = [0,1,2,3,4,5,6,7,8];
    for(var cont = 0;cont < opcoes.length;cont++)
        $("op_"+opcoes[cont]).className = "op";

    $("op_"+num).className = "op_sel";
    desenho.mudaOpcao(num);
}

function sel_desfazer() {
	if (desenho.getLiberado() && desenho.desfazer()) {
		$("op_desfazer").style.backgroundColor = "#CCC";
		$("op_refazer").style.backgroundColor = "";
	}
}

function sel_refazer() {
	if (desenho.getLiberado() && desenho.refazer()) {
		$("op_refazer").style.backgroundColor = "#CCC";
		$("op_desfazer").style.backgroundColor = "";
	}
}

function sel_limpar() {
	var msgConfirma = texto.alertClear;
	if (desenho.getLiberado() && confirm(msgConfirma)) {
		$("op_desfazer").style.backgroundColor = "#CCC";
		$("op_refazer").style.backgroundColor = "#CCC";
		desenho.limparTela(true);
		imgBaseID = 0;
	}
}

function sel_largura(num) {
	if (desenho.getLiberado()) {
		var opcoes = [2, 4, 6, 8, 10];
		for (var cont = 0; cont < opcoes.length; cont++) 
			$("op_larg" + opcoes[cont]).className = "op";
		
		$("op_larg" + num).className = "op_sel";
		
		desenho.mudaBorda(num, true);
	}
}

function sel_cor(hex,semDegrade) {
    if (desenho.getLiberado()) {
		desenho.mudaCor(hex, true);
		$("cor_escolhida").style.backgroundColor = desenho.getCorReal();
		$("cor_escolhida").title = desenho.getCorReal();
		
		if (semDegrade) {
			try {
				$("degrade").setAttribute("colorvalue", hex.substr(1));
				$("degrade").color.importColor();
			} catch(e) {}
		}
	}
}


function UTCTime() {
    var atual = new Date();
    return parseInt(Date.UTC(atual.getFullYear(),atual.getMonth(),atual.getDate(),atual.getHours(),atual.getMinutes(),atual.getSeconds()) / 1000);
}


//--------------------------------------------------------------------------------------
// CONTROLE DO AJAX
//--------------------------------------------------------------------------------------

// url_encode version 1.0
function url_encode(str) {
    var hex_chars = "0123456789ABCDEF";
    var noEncode = /^([a-zA-Z0-9\_\-\.])$/;
    var n, strCode, hex1, hex2, strEncode = "";

    for(n = 0; n < str.length; n++) {
        if (noEncode.test(str.charAt(n))) {
            strEncode += str.charAt(n);
        } else {
	        strCode = str.charCodeAt(n);
	        hex1 = hex_chars.charAt(Math.floor(strCode / 16));
	        hex2 = hex_chars.charAt(strCode % 16);
	        strEncode += "%" + (hex1 + hex2);
	    }
	}
	return strEncode;
}

// ----------------------------------------------------------------------------------

/*
 * Desenvolvido por Henrique Moreira e Rafael Duarte
 * Proibida a cópia
 */

//----------------------------------------
// OBJETOS PARA MANUPULAR DESENHOS CANVAS
//----------------------------------------

function Desenho(canvasElem, canvasPrevElem, usoDesfazer, usoRefazer, imgElem) {
	var self = this;
	
	//verificando se o browser usa o excanvas
	var excanvas = (document.all && !window.opera);

	var arquivoBase = '';
	var arquivoBaseUndo = '';
	var arquivoBaseRedo = '';
	var arrImages = [];
	
	var liberado = false; //liberação para poder desenhar
	var draw = false; //indicador de estado de desenho
	var draw_off = false;
	var stack = ""; //pilha de posições
	var des_cod = [];
	var des_cod_undo = "";
	var des_cod_redo = "";
	
	var valorBranco = 16777215;

	//Elementos canvas
	var canvas = canvasElem.getContext("2d");
	var canvasPrev, canvasUndo, canvasUndoElem, canvasRedo, canvasRedoElem;
	var local = [canvas];

	var canvasLargura = canvasElem.offsetWidth;
	var canvasAltura = canvasElem.offsetHeight;

	//Dados do desenho
	var cor = 0;
	var borda = 4;
	var tipo = 0;

	//lista de cores padrão (compatibilidade com a versão antiga)
	var cores = ["rgba(0,0,0,0)","#000","#666", "#8b0000", "#008b00", "#00008b", "#008b8b", "#8b8b00", "#8b4500", "#8b0a50", "#551a8b", "#548b54", "#8b6969", "#8b7b8b", "#fff", "#ccc", "#ff0000", "#00ff00", "#0000ff", "#00ffff", "#ffff00", "#ff7f00", "#ff1493", "#9b30ff", "#9aff9a", "#ffc1c1", "#ffe1ff"];

	//Coordenadas Iniciais e Atuais
	var xFirst;
	var yFirst;
	var xCoord;
	var yCoord;

	//Matriz que armazena os pixels
	var matriz = new Array();
	var matrizUndo = new Array();
	var matrizRedo = new Array();

	//Inicializando pilha do balde
	var pilha = new Array();

	//eventos da classe
	this.onSelCor = null;
	this.onCodigo = null;
	this.onStartDraw = null;

	//funcao de nome pequeno pra acessar elemento
	function $(id) {
		return document.getElementById(id);
	}

	//--------------------------------------------------------------------------------------
	// ELEMENTOS DE DESENHO
	//--------------------------------------------------------------------------------------

	this.borracha = function(xini,yini,lugar,gravar){
	    xini -= borda * 3;
	    yini -= borda * 3;
		
		canvas.fillStyle = "#FFF";
		if(!excanvas)
		    local[lugar].clearRect(xini,yini,borda * 6 + 1,borda * 6 + 1);
		self.retanguloF(xini,yini,borda * 6 + 1,borda * 6 + 1,lugar);
		canvas.fillStyle = converterCor(cor,true);

	    for(var cont = xini;cont <= xini + borda * 6;cont++) {
	        for(var cont2 = yini;cont2 <= yini + borda * 6;cont2++) {
	            if(cont >= 0 && cont2 >= 0 && cont < canvasLargura && cont2 < canvasAltura)
	                matriz[cont][cont2] = valorBranco;
	        }
	    }
	}

	//--------------------------------------------------------------------------------------

	this.linha = function(x1,y1,x2,y2,lugar) {
	    local[lugar].beginPath();
	    local[lugar].lineWidth = borda;
	    local[lugar].moveTo(x1, y1);
	    local[lugar].lineTo(x2, y2);
	    local[lugar].stroke();
	    local[lugar].closePath();
	}

	//--------------------------------------------------------------------------------------

	this.gLinha = function(x1,y1,x2,y2) {
	    //bresenhams
	    var temp;
	    var steep = Math.abs(y2 - y1) > Math.abs(x2 - x1);
	    if(steep) {
	        temp = x1;
	        x1 = y1;
	        y1 = temp;

	        temp = x2;
	        x2 = y2;
	        y2 = temp;
	    }
	    if(x1 > x2) {
	        temp = x1;
	        x1 = x2;
	        x2 = temp;

	        temp = y1;
	        y1 = y2;
	        y2 = temp;
	    }
	    var deltax = x2 - x1;
	    var deltay = Math.abs(y2 - y1);
	    var error = -deltax / 2;
	    var ystep;
	    var y = y1;
	    if (y1 < y2) ystep = 1; else ystep = -1;
	    for (var x = x1;x <= x2;x++) {
	        if (steep) self.gElipseF(y,x,Math.floor(borda/2),Math.floor(borda/2));
	        else self.gElipseF(x,y,Math.floor(borda/2),Math.floor(borda/2));
	        error = error + deltay;
	        if (error >= 0) {
	            y = y + ystep;
	            error = error - deltax;
	        }
	    }
	}

	//--------------------------------------------------------------------------------------

	this.retanguloF = function(x1,y1,largura,altura,lugar) {
	    local[lugar].fillRect(x1, y1, largura, altura);
	}

	//--------------------------------------------------------------------------------------

	this.gRetanguloF = function(x1,y1,largura,altura) {
	    var x2 = x1 + largura;
	    var y2 = y1 + altura;

	    stack += "#" + x1 + "#" + y1 + "#" + largura + "#" + altura;

	    for(var cont = x1;cont < x2; cont++){
	        for(var cont2 = y1;cont2 < y2;cont2++) {
	            matriz[cont][cont2] = cor;
	        }
	    }
	}

	//--------------------------------------------------------------------------------------

	this.retanguloB = function(x1,y1,largura,altura,lugar) {
	    local[lugar].strokeRect(x1, y1, largura, altura);
	}

	//--------------------------------------------------------------------------------------

	this.gRetanguloB = function(x1,y1,largura,altura) {
	    var calc_borda = Math.ceil(borda / 2);

	    var x2 = x1 + largura + calc_borda - 1;
	    var y2 = y1 + altura + calc_borda - 1;

	    x1 -= calc_borda;
	    y1 -= calc_borda;

	    for(var cont = x1; cont <= x2; cont++ ){

	        for(var contG = 0; contG < borda;contG++){
	            matriz[cont][y1 + contG] = cor;
	            matriz[cont][y2 - contG] = cor;
	        }

	    }
	    for(var cont2 = y1; cont2 <= y2; cont2++){

	        for(var contG = 0; contG < borda; contG++){
	            matriz[x1 + contG][cont2] = cor;
	            matriz[x2 - contG][cont2] = cor;
	        }
	    }
	}

	//--------------------------------------------------------------------------------------

	this.elipseF = function(cx,cy,rx,ry,lugar) {
	    if(excanvas)
	        local[lugar].fillEllipse(cx - rx,cy - ry,rx*2,ry*2);
	    else {
	        var KAPPA = 4 * ((Math.sqrt(2) -1) / 3);
	        local[lugar].beginPath();
	        local[lugar].moveTo(cx, cy - ry);
	        local[lugar].bezierCurveTo(cx + (KAPPA * rx), cy - ry, cx + rx, cy - (KAPPA * ry), cx + rx, cy);
	        local[lugar].bezierCurveTo(cx + rx, cy + (KAPPA * ry), cx + (KAPPA * rx), cy + ry, cx, cy + ry);
	        local[lugar].bezierCurveTo(cx - (KAPPA * rx), cy + ry, cx - rx, cy + (KAPPA * ry), cx - rx, cy);
	        local[lugar].bezierCurveTo(cx - rx, cy - (KAPPA * ry), cx - (KAPPA * rx), cy - ry, cx, cy - ry);
	        local[lugar].fill();
	    }
	}

	//--------------------------------------------------------------------------------------

	this.gElipseF = function(cx, cy, XRadius, YRadius){
	    if (XRadius > 0 && YRadius > 0) {
	        var xe;
	        var ye;
	        var xChange, yChange;
	        var elipseError;
	        var twoAs, twoBs;
	        var stopX, stopY;
	        twoAs = 2 * XRadius * XRadius;
	        twoBs = 2 * YRadius * YRadius;
	        xe = XRadius;
	        ye = 0;
	        xChange = YRadius * YRadius * (1 - 2 * XRadius);
	        yChange = XRadius * XRadius;
	        elipseError = 0;
	        stopX = twoBs * XRadius;
	        stopY = 0;

	        while (stopX >= stopY) {
	            for (var cont = xe; cont >= 1; cont--) {
	                matriz[cx + cont - 1][cy + ye - 1] = cor;
	                matriz[cx - cont][cy + ye - 1] = cor;
	                matriz[cx - cont][cy - ye] = cor;
	                matriz[cx + cont - 1][cy - ye] = cor;
	            }

	            ye++;
	            stopY += twoAs;
	            elipseError += yChange;
	            yChange += twoAs;
	            if ((2 * elipseError + xChange) > 0) {
	                xe--;
	                stopX -= twoBs;
	                elipseError += xChange;
	                xChange += twoBs;
	            }

	        }

	        xe = 0;
	        ye = YRadius;
	        xChange = YRadius * YRadius;
	        yChange = XRadius * XRadius * (1 - 2 * YRadius);
	        elipseError = 0;
	        stopX = 0;
	        stopY = twoAs * YRadius;
	        while (stopX <= stopY) {
	            for (var cont = xe; cont >= 1; cont--) {
	                matriz[cx + cont - 1][cy + ye - 1] = cor;
	                matriz[cx - cont][cy + ye - 1] = cor;
	                matriz[cx - cont][cy - ye] = cor;
	                matriz[cx + cont - 1][cy - ye] = cor;
	            }

	            xe++;
	            stopX += twoBs;
	            elipseError += xChange;
	            xChange += twoBs;
	            if ((2 * elipseError + yChange) > 0) {
	                ye--;
	                stopY -= twoAs;
	                elipseError += yChange;
	                yChange += twoAs;
	            }
	        }
	    }
	}

	//--------------------------------------------------------------------------------------

	this.elipseB = function(cx,cy,rx,ry,lugar) {
	    var KAPPA = 4 * ((Math.sqrt(2) -1) / 3);
	    local[lugar].beginPath();
	    local[lugar].moveTo(cx, cy - ry);
	    local[lugar].bezierCurveTo(cx + (KAPPA * rx), cy - ry, cx + rx, cy - (KAPPA * ry), cx + rx, cy);
	    local[lugar].bezierCurveTo(cx + rx, cy + (KAPPA * ry), cx + (KAPPA * rx), cy + ry, cx, cy + ry);
	    local[lugar].bezierCurveTo(cx - (KAPPA * rx), cy + ry, cx - rx, cy + (KAPPA * ry), cx - rx, cy);
	    local[lugar].bezierCurveTo(cx - rx, cy - (KAPPA * ry), cx - (KAPPA * rx), cy - ry, cx, cy - ry);
	    local[lugar].stroke();
	}

	//--------------------------------------------------------------------------------------

	this.gElipseB = function(cx, cy, XRadius, YRadius){
	    if (XRadius > 0 && YRadius > 0) {

	        var xe;
	        var ye;
	        var xChange, yChange;
	        var elipseError;
	        var twoAs, twoBs;
	        var stopX, stopY;
	        twoAs = 2 * XRadius * XRadius;
	        twoBs = 2 * YRadius * YRadius;
	        xe = XRadius;
	        ye = 0;
	        xChange = YRadius * YRadius * (1 - 2 * XRadius);
	        yChange = XRadius * XRadius;
	        elipseError = 0;
	        stopX = twoBs * XRadius;
	        stopY = 0;

	        while (stopX > stopY) {
	    		self.gElipseF(cx + xe,cy + ye,Math.floor(borda/2),Math.floor(borda/2));
	            self.gElipseF(cx - xe,cy + ye,Math.floor(borda/2),Math.floor(borda/2));
	            self.gElipseF(cx - xe,cy - ye,Math.floor(borda/2),Math.floor(borda/2));
	            self.gElipseF(cx + xe,cy - ye,Math.floor(borda/2),Math.floor(borda/2));

	            ye++;
	            stopY += twoAs;
	            elipseError += yChange;
	            yChange += twoAs;
	            if ((2 * elipseError + xChange) > 0) {
	                xe--;
	                stopX -= twoBs;
	                elipseError += xChange;
	                xChange += twoBs;
	            }

	        }

	        xe = 0;
	        ye = YRadius;
	        xChange = YRadius * YRadius;
	        yChange = XRadius * XRadius * (1 - 2 * YRadius);
	        elipseError = 0;
	        stopX = 0;
	        stopY = twoAs * YRadius;
	        while (stopX <= stopY) {
	            self.gElipseF(cx + xe,cy + ye,Math.floor(borda/2),Math.floor(borda/2));
	            self.gElipseF(cx - xe,cy + ye,Math.floor(borda/2),Math.floor(borda/2));
	            self.gElipseF(cx - xe,cy - ye,Math.floor(borda/2),Math.floor(borda/2));
	            self.gElipseF(cx + xe,cy - ye,Math.floor(borda/2),Math.floor(borda/2));

	            xe++;
	            stopX += twoBs;
	            elipseError += xChange;
	            xChange += twoBs;
	            if ((2 * elipseError + yChange) > 0) {
	                ye--;
	                stopY -= twoAs;
	                elipseError += yChange;
	                yChange += twoAs;
	            }
	        }
	    }
	}

	//--------------------------------------------------------------------------------------
	// FUNÇÕES DE BALDE
	//--------------------------------------------------------------------------------------

	function maiorRetanguloSL(antigo, x1, y1) {
	    var xc, yc;
	    xc = x1;
	    yc = y1;

	    if(matriz[x1][y1] == antigo) {
	        while(matriz[xc + 1][yc] == antigo) xc++;

	        var xfinal = xc;
	        do {
	            xc = x1 - 1;
	            yc++;
	            while(matriz[xc + 1][yc] == antigo && xc + 1 <= xfinal) xc++;
	        } while(xc == xfinal);
	        yc--;

	        var x = xfinal - x1;
	        var y = yc - y1;
	        return { x:x1 , y:y1 , w:x , h:y };
	    } else
	    return { w:-1 , h:-1 };
	}

	//--------------------------------------------------------------------------------------

	function maiorRetanguloNO(antigo, x1, y1) {
	    var xc, yc;
	    xc = x1;
	    yc = y1;

	    if(matriz[x1][y1] == antigo) {
	        while(matriz[xc - 1][yc] == antigo) xc--;

	        var xfinal = xc;
	        do {
	            xc = x1 + 1;
	            yc--;
	            while(matriz[xc - 1][yc] == antigo && xc - 1 >= xfinal) xc--;
	        } while(xc == xfinal);
	        yc++;

	        var x = x1 - xfinal;
	        var y = y1 - yc;
	        return { x:xfinal , y:yc , w:x , h:y };
	    } else
	    return { w:-1 , h:-1 };
	}

	//--------------------------------------------------------------------------------------

	function maiorRetanguloSO(antigo, x1, y1) {
	    var xc, yc;
	    xc = x1;
	    yc = y1;

	    if(matriz[x1][y1] == antigo) {
	        while(matriz[xc][yc + 1] == antigo) yc++;

	        var yfinal = yc;
	        do {
	            yc = y1 - 1;
	            xc--;
	            while(matriz[xc][yc + 1] == antigo && yc + 1 <= yfinal) yc++;
	        } while(yc == yfinal);
	        xc++;

	        var x = x1 - xc;
	        var y = yfinal - y1;
	        return { x:xc , y:y1 , w:x , h:y };
	    } else
	    return { w:-1 , h:-1 };
	}

	//--------------------------------------------------------------------------------------

	function maiorRetanguloNL(antigo, x1, y1) {
	    var xc, yc;
	    xc = x1;
	    yc = y1;

	    if(matriz[x1][y1] == antigo) {
	        while(matriz[xc][yc - 1] == antigo) yc--;

	        var yfinal = yc;
	        do {
	            yc = y1 + 1;
	            xc++;
	            while(matriz[xc][yc - 1] == antigo && yc - 1 >= yfinal) yc--;
	        } while(yc == yfinal);
	        xc--;

	        var x = xc - x1;
	        var y = y1 - yfinal;
	        return { x:x1 , y:yfinal , w:x , h:y };
	    } else
	    return { w:-1 , h:-1 };
	}

	//--------------------------------------------------------------------------------------

	//objeto axiliar para balde, guarda dados de um retangulo encontrado
	function baldeObj(x, y, w, h, ref, andada)
	{
	    this.x = x;
	    this.y = y;
	    this.w = w;
	    this.h = h;
	    this.ref = ref;
	    this.andada = andada;
	}

	//--------------------------------------------------------------------------------------

	//algoritmo do balde (teoria dos retangulos)
	this.balde = function(x,y) {
	    var antigo = matriz[x][y];

	    if(cor != antigo)
	        {
	            while(matriz[x - 1][y] == antigo) x--;
				while(matriz[x][y - 1] == antigo) y--;

	            var retIni = maiorRetanguloSL(antigo, x, y);
	            var valor = new baldeObj(x, y, retIni.w, retIni.h,0,0);
	            var andada;
	            var nivel = retIni.w;
	            var ref;
	            stack = "";

	            canvas.fillRect(retIni.x,retIni.y,retIni.w + 1,retIni.h + 1);
	            self.gRetanguloF(retIni.x,retIni.y,retIni.w + 1,retIni.h + 1);

	            stack = stack.substr(1,stack.length);

	            do
	            {
	                //verificando direita
	                andada = 0;
	                if(valor.ref == 2)
	                    andada += valor.andada;

	                while(andada <= valor.h) {
	                    retIni = maiorRetanguloNL(antigo, valor.x + valor.w + 1, valor.y + valor.h + 1 - andada);
	                    ref = retIni.h;

	                    if(ref != -1) {
	                        pilha[ref].push(new baldeObj(retIni.x,retIni.y,retIni.w,retIni.h,1,valor.h + 1 - andada));

	                        canvas.fillRect(retIni.x,retIni.y,retIni.w + 1,retIni.h + 1);
	                        self.gRetanguloF(retIni.x,retIni.y,retIni.w + 1,retIni.h + 1);

	                        if(ref > nivel)
	                            nivel = ref;
	                        andada += ref;
	                    } else
	                    andada++;
	                }

	                //verificando esquerda
	                andada = 0;
	                if(valor.ref == 1)
	                    andada += valor.andada;
	                while(andada <= valor.h) {
	                    retIni = maiorRetanguloSO(antigo, valor.x - 1, valor.y + andada);
	                    ref = retIni.h;

	                    if(ref != -1) {
	                        pilha[ref].push(new baldeObj(retIni.x,retIni.y,retIni.w,retIni.h,2,valor.h + 1 - andada));

	                        canvas.fillRect(retIni.x,retIni.y,retIni.w + 1,retIni.h + 1);
	                        self.gRetanguloF(retIni.x,retIni.y,retIni.w + 1,retIni.h + 1);

	                        if(ref > nivel)
	                            nivel = ref;
	                        andada += ref;
	                    }else
	                    andada++;
	                }

	                //verificando baixo
	                andada = 0;
	                if(valor.ref == 4)
	                    andada += valor.andada;
	                while(andada <= valor.w) {
	                    retIni = maiorRetanguloSL(antigo, valor.x + andada, valor.y + valor.h + 1);
	                    ref = retIni.w;
	                    if(ref != -1) {
	                        pilha[ref].push(new baldeObj(retIni.x,retIni.y,retIni.w,retIni.h,3,valor.w + 1 - andada));

	                        canvas.fillRect(retIni.x,retIni.y,retIni.w + 1,retIni.h + 1);
	                        self.gRetanguloF(retIni.x,retIni.y,retIni.w + 1,retIni.h + 1);

	                        if(ref > nivel)
	                            nivel = ref;
	                        andada += ref;
	                    }else
	                    andada++;
	                }

	                //verificando cima
	                andada = 0;
	                if(valor.ref == 3)
	                    andada += valor.andada;
	                while(andada <= valor.w) {
	                    retIni = maiorRetanguloNO(antigo, valor.x + valor.w + 1 - andada, valor.y - 1);
	                    ref = retIni.w;
	                    if(ref != -1) {
	                        pilha[ref].push(new baldeObj(retIni.x,retIni.y,retIni.w,retIni.h,4,valor.w + 1 - andada));

	                        canvas.fillRect(retIni.x,retIni.y,retIni.w + 1,retIni.h + 1);
	                        self.gRetanguloF(retIni.x,retIni.y,retIni.w + 1,retIni.h + 1);

	                        if(ref > nivel)
	                            nivel = ref;
	                        andada += ref;
	                    }else
	                    andada++;
	                }

	                valor = pilha[nivel].pop();

	                while(valor == null && nivel > 0)
	                    valor = pilha[--nivel].pop();

	            } while(valor != null);

				var codigo = "3@"+stack;
				des_cod.push(codigo);
				if(self.onCodigo instanceof Function) self.onCodigo(codigo);
	        }
	    }

	    //--------------------------------------------------------------------------------------
		// CONTROLE DE EVENTOS
		//--------------------------------------------------------------------------------------

		//Inicio de um desenho
		function startDraw(e)
		{

			if(!draw && liberado)
		    {
		    	//direcionando eventos da página para o objeto
		    	document.onmousemove = coordenada;
		    	document.onmouseup = endDraw;

				//obtendo as coordenadas atuais do mouse
		    	coordenada(e);

		        draw = true;

		        xFirst = xCoord;
		        yFirst = yCoord;

		        //casos especiais
		        switch(tipo) {
		            case 0: //PINCEL
		            salvarEstadoUndo();
		            stack = xFirst+"#"+yFirst;
		            break;
		            case 7: //BALDE
		            salvarEstadoUndo();
		            self.balde(xFirst,yFirst);
		            draw = false;
		            break;
		            case 8: //COLOR PICKER
		            self.mudaCor(converterCor(matriz[xFirst][yFirst],false),true);
		            if(self.onSelCor instanceof Function) self.onSelCor();
		            draw = false;
		            break;
		            case 1: //BORRACHA
		            salvarEstadoUndo();
		            stack = xFirst+"#"+yFirst;
		            self.borracha(xCoord,yCoord,0,true);
		            break;
		        }

		        if(self.onStartDraw instanceof Function) self.onStartDraw();
		    }
		}

		//--------------------------------------------------------------------------------------

		//Pega a posicao de um elemento HTML
		function getPos(o) {
		    var bo, x, y, b; x = y = 0;
		    if (o.getBoundingClientRect) { //ie
		        bo = o.getBoundingClientRect();
		        x = Math.round(bo.left); y = Math.round(bo.top);
		        x += document.documentElement.scrollLeft;
		        y += document.documentElement.scrollTop;
			} else if(document.getBoxObjectFor) { //moz
		        bo = document.getBoxObjectFor(o);
		        x = bo.x; y = bo.y;
			} else {
		        while(o && o.nodeName != 'BODY') {
		            x += o.offsetLeft;
		            y += o.offsetTop;
		            if(window.opera) { //opera
		                x -= 1;
		                y -= 1;
		            } else { //resto
			            x += 1;
			            y += 1;
			        }
			        b = parseInt(document.defaultView.getComputedStyle(o,null).getPropertyValue('border-width'));
			        if(b > 0) { x += b; y += b; }
			        o = o.offsetParent;
				}
			}
			return { x:x, y:y }
		}

		//--------------------------------------------------------------------------------------

	    //Função para obter a coordenada relativa ao quadro
		function coordenada(e) {
			if(!liberado) return;
			
		    //obtendo dados de acordo com o navegador...
		    //IE
		    if (excanvas){
		        xCoord = event.clientX;
		        yCoord = event.clientY;
		    }
		    //Firefox
		    else if (document.layers){
		        xCoord = e.x;
		        yCoord = e.y;
		    }
		    //Outros
		    else if ($){
		        xCoord = e.clientX;
		        yCoord = e.clientY;
		    }

		    xCoord = xCoord + document.documentElement.scrollLeft - getPos(canvasPrevElem).x;
		    yCoord = yCoord + document.documentElement.scrollTop - getPos(canvasPrevElem).y;
			
			//marcacao da borracha
			if(canvasPrevElem && tipo == 1) {
				var xini = xCoord - borda * 3;
			    var yini = yCoord - borda * 3;
				var tamanho = borda * 6 + 1;
				canvasPrev.clearRect(0, 0, canvasLargura, canvasAltura);
				self.desenhar(xini,yini,xini+tamanho,yini+tamanho,2,1,false);
				self.desenhar(xini,yini,xini+tamanho,yini+tamanho,3,1,false);
			}

		    //tratando limites
		    var ref_borda = Math.floor(borda / 2);
		    if(xCoord >= canvasLargura - ref_borda)
		        xCoord = canvasLargura - ref_borda;
		    else if(xCoord <= ref_borda)
		        xCoord = ref_borda;

		    if(yCoord >= canvasAltura - ref_borda)
		        yCoord = canvasAltura - ref_borda;
		    else if(yCoord <= ref_borda)
		        yCoord = ref_borda;

		    //Verifica de não há bloqueio e se começou a desenhar
		    if(draw && liberado) {
		        //LAPIS
		        if(tipo == 0) {
					if(stack.length > 230)
	                {
						var codigo = "2@"+stack;
						des_cod.push(codigo);
						if(self.onCodigo instanceof Function) self.onCodigo(codigo);
	                    stack = xFirst + "#" + yFirst;
	                }

		            self.linha(xFirst, yFirst, xCoord, yCoord, 0);
		            self.gLinha(xFirst, yFirst, xCoord, yCoord);

		            xFirst = xCoord;
		            yFirst = yCoord;

		            stack += "#" + xFirst + "#" + yFirst;
		        }
		        //BORRACHA
		        else if(tipo == 1) {
		            if(stack.length > 230)
		                {
							var codigo = "21@"+stack;
							des_cod.push(codigo);
							if(self.onCodigo instanceof Function) self.onCodigo(codigo);
		                    stack = xCoord + "#" + yCoord;
		                }

		                self.borracha(xCoord,yCoord,0,true);
		                stack += "#" + xCoord + "#" + yCoord;
		            }
		            //OUTROS
		            else {
		                canvasPrev.clearRect(0, 0, canvasLargura, canvasAltura);
		                self.desenhar(xFirst,yFirst,xCoord,yCoord,tipo,1,false);
		            }
		        }

		        //Parar evento do mouse
		        //IE
	        	if(excanvas) {
					event.cancelBubble = true;
					event.returnValue = false;
					event.stopped = true;
				}
				//OUTROS
				else {
					e.preventDefault();
					e.stopPropagation();
				}
		    }

		//--------------------------------------------------------------------------------------

		//trata todo o metodo de desenho das figuras exceto balde e pincel
		this.desenhar = function(x1, y1, x2, y2, tipoo, lugar, gravar) {
			//coordenadas de inicio e fim iguais... invalidar
			if(x1 == x2 && y1 == y2) {
		        	return;
			}

		    //Verifica o tipo do desenho

		    var xIni, xFim, yIni, yFim;

		    if(x1 >= x2) {
		        xIni = x2;
		        xFim = x1;
		    } else {
		        xIni = x1;
		        xFim = x2;
		    }
		    if(y1 >= y2) {
		        yIni = y2;
		        yFim = y1;
		    } else {
			    yIni = y1;
			    yFim = y2;
			}

			if(gravar)
				salvarEstadoUndo();
			
		    switch(tipoo)
		    {
		        //LINHA
		        case 6:
		        self.linha(x1, y1, x2, y2, lugar);
		        if(gravar) self.gLinha(x1, y1, x2, y2);
		        break;
		        //RETANGULO FUNDO
		        case 2:
		        if(xIni == xFim || yIni == yFim) {
		        	gravar = false;
					break;
				}

				self.retanguloF(xIni, yIni, xFim - xIni, yFim - yIni, lugar);
		        if(gravar) self.gRetanguloF(xIni, yIni, xFim - xIni, yFim - yIni);
		        break;
		        //RETANGULO BORDA
		        case 3:
		        self.retanguloB(xIni, yIni, xFim - xIni, yFim - yIni, lugar);
		        if(gravar) self.gRetanguloB(xIni, yIni, xFim - xIni, yFim - yIni);
		        break;
		        //ELIPSE FUNDO
		        case 4:
		        if(xIni == xFim || yIni == yFim) {
		        	gravar = false;
					break;
				}

		        var rx = Math.floor((xFim-xIni)/2);
		        var ry = Math.floor((yFim-yIni)/2);
		        var cx = Math.round(xIni+rx);
		        var cy = Math.round(yIni+ry);

		        self.elipseF(cx,cy,rx,ry,lugar);
		        if(gravar) self.gElipseF(cx,cy,rx,ry);
		        break;
		        //ELIPSE BORDA
		        case 5:
		        var rx = Math.floor((xFim-xIni)/2);
		        var ry = Math.floor((yFim-yIni)/2);
		        var cx = Math.round(xIni+rx);
		        var cy = Math.round(yIni+ry);

		        self.elipseB(cx,cy,rx,ry,lugar);
		        if(gravar) self.gElipseB(cx,cy,rx,ry);
		        break;
		    }

			if(gravar) {
				var codigo = "1@" + tipoo + "#" + x1 + "#" + y1 + "#" + x2 + "#" + y2;
				des_cod.push(codigo);
				if(self.onCodigo instanceof Function) self.onCodigo(codigo);
			}
		}

		//--------------------------------------------------------------------------------------

		//Finaliza um desenho
		function endDraw(e)
		{
		    //Caso esteja desenhando
		    if(draw && liberado) {
		    	var codigo;
				if(tipo == 0) {
					codigo = "2@"+stack;
					des_cod.push(codigo);
					if(self.onCodigo instanceof Function) self.onCodigo(codigo);
		        } else if (tipo == 1) {
		        	codigo = "21@"+stack;
					des_cod.push(codigo);
					if(self.onCodigo instanceof Function) self.onCodigo(codigo);
		        } else {
		    		self.desenhar(xFirst, yFirst, xCoord, yCoord, tipo, 0, true);
		        	canvasPrev.clearRect(0, 0, canvasLargura, canvasAltura);
				}

		    	//termina o processo de desenho
		    	draw = false;
			}
		}

		//--------------------------------------------------------------------------------------
		// DETALHES E MENSAGENS
		//--------------------------------------------------------------------------------------

		this.mudaOpcao = function(valor) {
			//se for borracha, preenchimento sempre branco
		    if (valor == 1) {
				canvas.fillStyle = "#FFFFFF";
				if (canvasPrevElem && liberado) {
					canvasPrev.lineWidth = 2;
					canvasPrev.strokeStyle = "#000000";
					canvasPrev.fillStyle = "rgba(255,255,255,0.8)";
				}
			}
			else if (tipo == 1) {
				canvas.fillStyle = converterCor(cor, true);
				if (canvasPrevElem) {
					canvasPrev.clearRect(0, 0, canvasLargura, canvasAltura);
					canvasPrev.lineWidth = borda;
					canvasPrev.fillStyle = canvas.fillStyle;
					canvasPrev.strokeStyle = canvas.fillStyle; 
				}
			}
			
		    tipo = valor;
		}

		//--------------------------------------------------------------------------------------

		//altera valor da borda
		this.mudaBorda = function(valor, gravar)
		{
		    canvas.lineWidth = valor;
		    if(canvasPrevElem != null && tipo != 1)
		    	canvasPrev.lineWidth = valor;
		    borda = valor;

		    if(gravar) {
		    	var codigo = "6@"+valor;
		        des_cod.push(codigo);
		    	if(self.onCodigo instanceof Function) self.onCodigo(codigo);
			}
		}

		//--------------------------------------------------------------------------------------

		//altera cor de preenchimento
		this.mudaCor = function(valor, gravar)
		{
		    var valorInt = parseInt(valor);
			var corHTML;
			
			//verificando tipo de cor para manter compatbilidade com a versão antiga
			if(!isNaN(valorInt))
				corHTML = cores[valorInt];
			else if (valor.search(/^x[0-9A-F]{6}$/) != -1) {
				valorInt = parseInt(valor.replace('x',''),16);	
				corHTML = converterCor(valorInt,true);
			}
			else 
				return;
				
		    canvas.strokeStyle = corHTML;
		    if(canvasPrevElem != null && tipo != 1) {
			    canvasPrev.strokeStyle = corHTML;
			    canvasPrev.fillStyle = corHTML;
			}

		    //caso seja sua vez, nao mudar cor para borracha
		    if(tipo != 1)
		        canvas.fillStyle = corHTML;

		    cor = valorInt;

			if(gravar) {
				var codigo = "5@"+valor;
		        des_cod.push(codigo);
		    	if(self.onCodigo instanceof Function) self.onCodigo(codigo);
			}
		}

		//--------------------------------------------------------------------------------------

		this.limparTela = function(gravar,enviarCodigo)
		{
			//apagando tudo e setando o fundo padrao (branco)
			self.transparencia();
			canvas.fillStyle = "#FFF";
			self.retanguloF(0, 0, canvasLargura, canvasAltura, 0);
			canvas.fillStyle = converterCor(cor,true);
			
			if (gravar) {
				//iniciando matriz
				matriz = new Array(canvasLargura);
				for (var cont = 0; cont < canvasLargura; cont++) {
					matriz[cont] = new Array(canvasAltura);
					for (var cont2 = 0; cont2 < canvasAltura; cont2++) 
						matriz[cont][cont2] = valorBranco;
				}
				
				//Bordas de matriz, importante para o balde
				matriz[-1] = new Array();
				matriz[canvasLargura] = new Array();
				for (var cont = -1; cont <= canvasAltura; cont++) {
					matriz[-1][cont] = 16777216;
					matriz[canvasLargura][cont] = 16777216;
				}
				for (cont = 0; cont < canvasLargura; cont++) {
					matriz[cont][-1] = 16777216;
					matriz[cont][canvasAltura] = 16777216;
				}
				
				des_cod = ["5@" + converterCor(cor, true).replace('#', 'x'), "6@" + borda];
				if (!enviarCodigo) {
					if (self.onCodigo instanceof Function) 
						self.onCodigo("4@");
				}
				
				if (imgElem) {
					arquivoBase = '';
					imgElem.style.display = 'none';
				}
				
				canvas.fillStyle = "#FFF";
			    self.retanguloF(0, 0, canvasLargura, canvasAltura, 0);
			    canvas.fillStyle = converterCor(cor,true);
			}
		    draw = false;
			
			//Futuramente deve apagar todos os undos
			if (gravar) {
				des_cod_undo = '';
				des_cod_redo = '';
			}
		}
		
		//--------------------------------------------------------------------------------------
		
		this.transparencia = function() {
			canvas.clearRect(0, 0, canvasLargura, canvasAltura);
			if(canvasPrevElem != null) canvasPrev.clearRect(0, 0, canvasLargura, canvasAltura);
		}
		
		//--------------------------------------------------------------------------------------
		
		this.desfazer = function() {
			if (usoDesfazer && des_cod_undo) {
				if(usoRefazer)
					salvarEstadoRedo();
				
				//undo diferenciado para o ie
				var baseDiferente = (arquivoBase != arquivoBaseUndo);
				
				//imagem de fundo
				if(imgElem) {
					//verificando se havia imagem-base
					if (!arquivoBaseUndo) 
						imgElem.style.display = 'none';
					else 
						imgElem.src = arquivoBaseUndo;
					arquivoBase = arquivoBaseUndo;
				}
				
				//executando o desfazer
				if (excanvas) {
					if(!baseDiferente)
						canvas.undo();
					else
						canvas.undo2();
				}
				else {
					self.transparencia();
					//mostrando a imagem guardada
					canvas.drawImage(canvasUndoElem, 0, 0);
				}
				
				//copiando matriz anterior
				for (var cont = 0; cont < canvasLargura; cont++) {
					for (var cont2 = 0; cont2 < canvasAltura; cont2++) 
						matriz[cont][cont2] = matrizUndo[cont][cont2];
				}
				
				des_cod = des_cod_undo.split('|');
				des_cod_undo = '';
				
				return true;
			} else
				return false;
		}
		
		//--------------------------------------------------------------------------------------
		
		this.refazer = function() {
			if (usoRefazer && des_cod_redo) {
				//se estiver habilitado o refazer
				if (usoDesfazer)
					salvarEstadoUndo(true);
				
				
				//undo diferenciado para o ie
				var baseDiferente = (arquivoBase != arquivoBaseRedo);
				
				//imagem de fundo
				if(imgElem) {
					//verificando se havia imagem-base
					if (arquivoBaseRedo && arquivoBase != arquivoBaseRedo) {
						imgElem.style.display = 'block';
						imgElem.src = arquivoBaseRedo;
					}
					arquivoBase = arquivoBaseRedo;
				}
				
				//executando o desfazer
				if(!baseDiferente) {
					if (excanvas) 
						canvas.redo();
					else {		
						self.transparencia();			
						//mostrando a imagem guardada
						canvas.drawImage(canvasRedoElem, 0, 0);
					}
				} else {
					self.transparencia();
				}
				
				//copiando matriz anterior
				for (var cont = 0; cont < canvasLargura; cont++) {
					for (var cont2 = 0; cont2 < canvasAltura; cont2++) 
						matriz[cont][cont2] = matrizRedo[cont][cont2];
				}
				
				des_cod = des_cod_redo.split('|');
				des_cod_redo = '';
				
				return true;
			} else
				return false;
		}
		
		//--------------------------------------------------------------------------------------
		
		function salvarEstadoUndo(manterRedo) {
			if (usoDesfazer) {
				if (excanvas) 
					canvas.saveState();
				else {
					
					//apagando tudo e setando o fundo padrao (branco)
					canvasUndo.clearRect(0, 0, canvasLargura, canvasAltura);
					//canvasUndo.fillStyle = "#FFF";
				    //self.retanguloF(0, 0, canvasLargura, canvasAltura, 2);
					
					//copiando ultima imagem
					canvasUndo.drawImage(canvasElem, 0, 0);
				}
				
				//copiando arquivo base
				arquivoBaseUndo = arquivoBase;
				
				//iniciando matriz
				matrizUndo = new Array(canvasLargura);
				for (var cont = 0; cont < canvasLargura; cont++) {
					matrizUndo[cont] = new Array(canvasAltura);
					for (var cont2 = 0; cont2 < canvasAltura; cont2++) 
						matrizUndo[cont][cont2] = matriz[cont][cont2];
				}
				
				des_cod_undo = des_cod.join("|");
				if(!manterRedo)
					des_cod_redo = '';
			}
		}
		
		//--------------------------------------------------------------------------------------
		
		function salvarEstadoRedo() {
			//se estiver habilitado o refazer
			if (usoRefazer) {
				if (!excanvas) {
					//apagando tudo e setando o fundo padrao (branco)
					canvasRedo.clearRect(0, 0, canvasLargura, canvasAltura);
					//canvasRedo.fillStyle = "#FFF";
				    //self.retanguloF(0, 0, canvasLargura, canvasAltura, 3);
					
					//copiando ultima imagem
					canvasRedo.drawImage(canvasElem, 0, 0);
				}
				
				
				//copiando arquivo base
				arquivoBaseRedo = arquivoBase;
				
				//copiando matriz atual
				matrizRedo = new Array(canvasLargura);
				for (var cont = 0; cont < canvasLargura; cont++) {
					matrizRedo[cont] = new Array(canvasAltura);
					for (var cont2 = 0; cont2 < canvasAltura; cont2++) 
						matrizRedo[cont][cont2] = matriz[cont][cont2];
				}
				
				des_cod_redo = des_cod.join("|");
			}
		}
		
		//--------------------------------------------------------------------------------------
		
		//transforma o numero em cor hexadecial html
		function converterCor(val,hash) {
			var hex = val.toString(16).toUpperCase();
			while(hex.length < 6) hex = '0'+hex;
			return (hash) ? '#'+hex : 'x'+hex;
		}
		
		//--------------------------------------------------------------------------------------
		//Salvar Imagem
		//--------------------------------------------------------------------------------------
		this.salvarPNG = function() {
			if(canvasElem.toDataURL())
				return canvasElem.toDataURL();
			else
				return false;
		}

		this.salvarJPG = function() {
			if(canvasElem.toDataURL("image/jpeg") && !window.opera)
				return canvasElem.toDataURL("image/jpeg");
			else
				return false;
		}

		//--------------------------------------------------------------------------------------
		//Retornos
		//--------------------------------------------------------------------------------------
		this.getSequencia = function() {
			return des_cod.join("|"); //passando String para haver réplica
		}
		this.setSequencia = function(seq) {
			des_cod = seq.split("|");
		}
		
		this.getLiberado = function() {
			return liberado;
		}
		
		//calcula tempo atual
		function UTCTime() {
			var atual = new Date();
			return Date.UTC(atual.getFullYear(),atual.getMonth(),atual.getDate(),atual.getHours(),atual.getMinutes(),atual.getSeconds(),atual.getMilliseconds());
		}
		
		this.setMatriz = function(arr) {
			salvarEstadoUndo();
			
			var pos = 0, ultimo = -1, val;
		    for(var cont = 0;cont < canvasLargura;cont++)
		    {
		        for (var cont2 = 0; cont2 < canvasAltura; cont2++) {
					val = arr[pos++];
					//verificando se eh uma posicao repetida
					if (val != 'x') {
						ultimo = parseInt(val)
						matriz[cont][cont2] = ultimo;
					}
					//repetindo o ultimo pixel
					else {
						matriz[cont][cont2] = ultimo;
						//numX++;
					}
				}
		    }
		}
		
		this.setArquivoBase = function(arq, gravar) {
			if (imgElem) {
				if (gravar) {
					arquivoBase = arq;
					
					//gravando na lista de imagens para cache
					var imagem = new Image();
					imagem.src = arquivoBase;
					arrImages.push(imagem);
					
					//codigo correspondente
					var codigo = "26@" + arquivoBase;
					des_cod.push(codigo);
					if (self.onCodigo instanceof Function) 
						self.onCodigo(codigo);
				}
				
				//mostrando imagem
				imgElem.src = arq;
				imgElem.style.display = 'block';
			}
		}
		this.getArquivoBase = function() {
			return arquivoBase;
		}
		
		this.comprimir = function() {
			var tipoAtual;
			var ultimoTipo = -1;
			for(var cont = 0;cont < des_cod.length;cont++) {
				tipoAtual = parseInt(des_cod[cont]);
				if(ultimoTipo == tipoAtual &&
				  (tipoAtual == 5 || tipoAtual == 6)) {
					des_cod.splice(cont-1,1);
					cont--;  	
				} else
					ultimoTipo = tipoAtual;
			}
		}

		this.getCor = function() {
			return converterCor(cor,false);
		}
		this.getCorReal = function() {
			return converterCor(cor,true);
		}
		this.getLargura = function() {
			return canvasLargura;
		}
		this.getAltura = function() {
			return canvasAltura;
		}

		this.getUltimaAcao = function() {
			if(des_cod.length > 0)
				return des_cod[des_cod.length - 1];
			return null;
		}

		this.liberar = function(valor) {
			liberado = valor;
			if (valor) {
				canvasElem.style.cursor = "crosshair";
				if (canvasPrevElem != null) {
					canvasPrevElem.style.cursor = "crosshair";
					//direcionando eventos da página para o objeto
					canvasPrevElem.onmousedown = startDraw;
					document.onmousemove = coordenada;
					document.onmouseup = endDraw;
				}
			}
			else {
				canvasElem.style.cursor = "default";
				if(canvasPrevElem != null)
					canvasPrevElem.style.cursor = "default";
			}
		}

		//--------------------------------------------------------------------------------------
		//Construtor
		//--------------------------------------------------------------------------------------

		if(canvasPrevElem != null) {
			canvasPrev = canvasPrevElem.getContext("2d");
			local.push(canvasPrev);
			canvasPrevElem.onmousedown = startDraw;

			//eventos extras para o IE
			if(document.all && !window.opera) {
				canvasElem.onmousedown = startDraw;
				canvasElem.onselectstart = function(e) {
					startDraw(e);
					return false;
				}
				canvasPrevElem.onselectstart = function(e) {
					startDraw(e);
					return false;
				}
			}

			canvasPrev.lineCap = "round";
	    	canvasPrev.fillStyle = "#000000";
	    	canvasPrev.strokeStyle = "#000000";
	    	canvasPrev.lineWidth = borda;
		}

		//ponta redonda
	    canvas.lineCap = "round";
		//cor padrao
	    canvas.fillStyle = "#000000";
	    canvas.strokeStyle = "#000000";
	    //grossura padrao
	    canvas.lineWidth = borda;
		
		//criando canvas escondido (Caso nao IE)
		if(usoDesfazer && !excanvas) {
			canvasUndoElem = document.createElement('canvas');
			canvasUndoElem.width = canvasLargura;
			canvasUndoElem.height = canvasAltura;
			canvasUndoElem.style.display = 'none';
			document.body.appendChild(canvasUndoElem);
			
			canvasUndo = canvasUndoElem.getContext("2d");
			local.push(canvasUndo);
			canvasUndo.lineCap = "round";
	    	canvasUndo.fillStyle = "#000000";
	    	canvasUndo.strokeStyle = "#000000";
	    	canvasUndo.lineWidth = borda;
		}
		
		//criando canvas escondido (Caso nao IE)
		if(usoRefazer && !excanvas) {
			canvasRedoElem = document.createElement('canvas');
			canvasRedoElem.width = canvasLargura;
			canvasRedoElem.height = canvasAltura;
			canvasRedoElem.style.display = 'none';
			document.body.appendChild(canvasRedoElem);
			
			canvasRedo = canvasRedoElem.getContext("2d");
			local.push(canvasRedo);
			canvasRedo.lineCap = "round";
	    	canvasRedo.fillStyle = "#000000";
	    	canvasRedo.strokeStyle = "#000000";
	    	canvasRedo.lineWidth = borda;
		}

	    //iniciando pilha
	    for(var cont = 0;cont <= canvasLargura;cont++) {
	        pilha[cont] = new Array();
	    }
	    //-----------------------------------

		//preparando tela e matriz
    	self.limparTela(true);
}

if(window.top.location.href.search(/gartic/) == -1) window.location.href = "http://gartic.com/";

// ----------------------------------------------------------------------------------

/*
 * Desenvolvido por Henrique Moreira e Rafael Duarte
 * Proibida a cópia
 */

//----------------------------------------
// OBJETO PARA CRIAR ANIMAÇÕES DE DESENHO
// Depende de um objeto Desenho instanciado
//----------------------------------------

function Animacao(desenho) {
	var self = this;
	var passo;
	var rodando = false;
	var velocidade1 = 200;
	var velocidade2 = 20;
	var velocidadeDireto = 4;
	var velocidadeAtual = velocidade1;
	var direto = velocidadeDireto - 1;
	
	var tempRodar = "";
	var divisor = 1;
	var seq = "";
	var desenho = desenho;

	//Trata cada passo da animação
	function rodar(){
		if (rodando) {
			desenhoTela();

			passo++;
			if (passo < seq.length) {
				if (direto == velocidadeDireto - 1) {
					setTimeout(function(){
						rodar();
					}, velocidadeAtual);
				} else {
					rodar();
				}
			} else {
				rodando = false;
				//liberando o ato de desenhar
				desenho.liberar(true);
				if(self.onTerminar instanceof Function) self.onTerminar();
			}
		}
	}

	//executa o desenho na tela
	function desenhoTela(){
		var vez = seq[passo].split("@");
		var vals = vez[1].split("#");
		velocidadeAtual = velocidade1;

		switch (parseInt(vez[0])) {
			//retangulo, circulo, linha...
			case 1:
				desenho.desenhar(Math.round(parseInt(vals[1]) / divisor), Math.round(parseInt(vals[2]) / divisor), Math.round(parseInt(vals[3]) / divisor), Math.round(parseInt(vals[4]) / divisor), parseInt(vals[0]), 0, false);
				break;
			//lápis
			case 2:
				if(vals.length >= 4) {
					desenho.desenhar(Math.round(parseInt(vals[0]) / divisor), Math.round(parseInt(vals[1]) / divisor), Math.round(parseInt(vals[2]) / divisor), Math.round(parseInt(vals[3]) / divisor), 6, 0, false);

					if (vals.length > 4) {
						seq[passo] = "2@" + vals.slice(2).join("#");
						passo--;
						//acelerando
						velocidadeAtual = velocidade2;
						//passar com ou sem tempo alternado (para ficar mais rapido)
						direto = (direto + 1) % velocidadeDireto;
					}
					else {
						direto = velocidadeDireto - 1;
						//verificar se o proximo tambem é lapis, para manter a velocidade
						if (seq[passo + 1] != undefined && seq[passo].charAt(0) == seq[passo + 1].charAt(0)) {
							velocidadeAtual = velocidade2;
						}
					}
				} else
					direto = false;
				break;
			//balde
			case 3:
				var x1, y1, x2, y2;

				for (var cont = 0; cont < vals.length; cont = cont + 4) {
					x1 = Math.round(parseInt(vals[cont]) / divisor);
					y1 = Math.round(parseInt(vals[cont + 1]) / divisor);
					x2 = Math.round(parseInt(vals[cont + 2]) / divisor);
					y2 = Math.round(parseInt(vals[cont + 3]) / divisor);
					desenho.retanguloF(x1, y1, x2, y2, 0);
				}
				break;
			case 21:
				desenho.borracha(Math.round(parseInt(vals[0]) / divisor), Math.round(parseInt(vals[1]) / divisor), 0, false);

				if (vals.length >= 4) {
					seq[passo] = "21@" + vals.slice(2).join("#");
					passo--;
					//acelerando
					velocidadeAtual = velocidade2;
				}
				else
					if (seq[passo + 1] != undefined && seq[passo].substr(0, 2) == seq[passo + 1].substr(0, 2)) {
						velocidadeAtual = velocidade2;
					}
				break;
			//mudar cor
			case 5:
				desenho.mudaCor(vals[0], false);
				velocidadeAtual = velocidade2;
				break;
			//mudar borda
			case 6:
				var valor = Math.round(parseInt(vals[0]) / divisor);
				desenho.mudaBorda(valor, false);
				velocidadeAtual = velocidade2;
				break;
			//mudar imagem base
			case 26:
				desenho.transparencia();
				desenho.setArquivoBase(vals[0],false);
			break;
		}
	}

	//Iniciar a animação
	this.iniciar = function(div){
		if(!rodando) {
			rodando = true;
			divisor = div;

			//limpando tela
			desenho.limparTela(false);
			if(self.onComecar instanceof Function) self.onComecar();
			seq = desenho.getSequencia().split("|");

			//desabilitar novos desenhos
			desenho.liberar(false);

			passo = 0;
			direto = velocidadeDireto - 1;
			setTimeout(function() { rodar(); }, velocidade1);
		}
	}

	//Pausar a animação
	this.pausar = function(){
		if(rodando) {
			rodando = false;
			if(self.onPausar instanceof Function) self.onPausar();
		}
	}

	//cancela animacao e exibe desenho completo
	this.parar = function(){
		rodando = false;
		while(passo < seq.length) {
			desenhoTela();
			passo++;
		}
		//liberando o ato de desenhar
		desenho.liberar(true);
		if(self.onTerminar instanceof Function) self.onTerminar();
	}

	//continuar animação que foi paralizada
	this.continuar = function() {
		if(!rodando) {
			rodando = true;
			if(self.onContinuar instanceof Function) self.onContinuar();
			rodar();
		}
	}
	
	this.setVelocidades = function(vel1, vel2, veld) {
		velocidade1 = vel1;
		velocidade2 = vel2;
		velocidadeDireto = veld;
	}

	//Eventos da classe
	this.onTerminar = null;
	this.onComecar = null;
	this.onPausar = null;
	this.onContinuar = null;
}
</script> 
</div>
<!-- End page content -->
<?php
include ("tracking_scripts.inc.php");
?>
</body>

</html>
<?php
mysql_close($db);
?>