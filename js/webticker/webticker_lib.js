
// WebTicker by Mioplanet
// www.mioplanet.com

TICKER_CONTENT = document.getElementById("TICKER").innerHTML;
 
TICKER_RIGHTTOLEFT = false;
TICKER_SPEED = 2;
TICKER_STYLE = "font-family:Arial; font-size:12px; color:#444444";
TICKER_PAUSED = false;

ticker_start();

function ticker_start() {
	var tickerSupported = false;
	TICKER_WIDTH = document.getElementById("TICKER").style.width;
	var img = "<img src=ticker_space.gif width="+TICKER_WIDTH+" height=0>";

	// Firefox
	if (navigator.userAgent.indexOf("Firefox")!=-1 || navigator.userAgent.indexOf("Safari")!=-1) {
		document.getElementById("TICKER").innerHTML = "<TABLE  cellspacing='0' cellpadding='0' width='100%'><TR><TD nowrap='nowrap'>"+img+"<SPAN style='"+TICKER_STYLE+"' ID='TICKER_BODY' width='100%'>&nbsp;</SPAN>"+img+"</TD></TR></TABLE>";
		tickerSupported = true;
	}
	// IE
	if (navigator.userAgent.indexOf("MSIE")!=-1 && navigator.userAgent.indexOf("Opera")==-1) {
		document.getElementById("TICKER").innerHTML = "<DIV nowrap='nowrap' style='width:100%;'>"+img+"<SPAN style='"+TICKER_STYLE+"' ID='TICKER_BODY' width='100%'></SPAN>"+img+"</DIV>";
		tickerSupported = true;
	}
	if(!tickerSupported) document.getElementById("TICKER").outerHTML = ""; else {
		document.getElementById("TICKER").scrollLeft = TICKER_RIGHTTOLEFT ? document.getElementById("TICKER").scrollWidth - document.getElementById("TICKER").offsetWidth : 0;
		document.getElementById("TICKER_BODY").innerHTML = TICKER_CONTENT;
		document.getElementById("TICKER").style.display="block";
		TICKER_tick();
	}
}

function TICKER_tick() {
	if(!TICKER_PAUSED) document.getElementById("TICKER").scrollLeft += TICKER_SPEED * (TICKER_RIGHTTOLEFT ? -1 : 1);
	if(TICKER_RIGHTTOLEFT && document.getElementById("TICKER").scrollLeft <= 0) document.getElementById("TICKER").scrollLeft = document.getElementById("TICKER").scrollWidth - document.getElementById("TICKER").offsetWidth;
	if(!TICKER_RIGHTTOLEFT && document.getElementById("TICKER").scrollLeft >= document.getElementById("TICKER").scrollWidth - document.getElementById("TICKER").offsetWidth) document.getElementById("TICKER").scrollLeft = 0;
	window.setTimeout("TICKER_tick()", 30);
}
