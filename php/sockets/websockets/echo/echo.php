<!DOCTYPE html>
<meta charset="utf-8" />
<title>WebSocket Test</title>
<link rel="stylesheet" href="style.css">
<script language="javascript" type="text/javascript">
var secureCb, secureCbLabel, wsUri, consoleLog, connectBut, disconnectBut, sendMessage, sendBut, clearLogBut;

function echoHandlePageLoad() {
	if (window.WebSocket) document.getElementById("webSocketSupp").style.display = "block";
	else document.getElementById("noWebSocketSupp").style.display = "block";
	secureCb = document.getElementById("secureCb");
	secureCb.checked = false;
	secureCb.onclick = toggleTls;
	secureCbLabel = document.getElementById("secureCbLabel")
	wsUri = document.getElementById("wsUri");
	toggleTls();
	connectBut = document.getElementById("connect");
	connectBut.onclick = doConnect;
	disconnectBut = document.getElementById("disconnect");
	disconnectBut.onclick = doDisconnect;
	sendMessage = document.getElementById("sendMessage");
	sendBut = document.getElementById("send");
	sendBut.onclick = doSend;
	consoleLog = document.getElementById("consoleLog");
	clearLogBut = document.getElementById("clearLogBut");
	clearLogBut.onclick = clearLog;
	setGuiConnected(false);
	document.getElementById("disconnect").onclick = doDisconnect;
	document.getElementById("send").onclick = doSend;
}

function toggleTls() {
	if (secureCb.checked) wsUri.value = "wss://echo.websocket.org";
	else wsUri.value = "ws://echo.websocket.org";
}

function doConnect() {
	if (!window.WebSocket) {
		logToConsole('<span style="color: red;"><strong>Error:</strong> This browser does not have support for WebSocket</span>');
		return;
	}

	websocket = new WebSocket(wsUri.value);
	websocket.onopen = function(evt) { onOpen(evt) };
	websocket.onclose = function(evt) { onClose(evt) };
	websocket.onmessage = function(evt) { onMessage(evt) };
	websocket.onerror = function(evt) { onError(evt) };
}

function doDisconnect() {
	websocket.close();
}

function doSend() {
	logToConsole("SENT: " + sendMessage.value);
	websocket.send(sendMessage.value);
}

function logToConsole(message) {
	var pre = document.createElement("p");
	pre.style.wordWrap = "break-word";
	pre.innerHTML = getSecureTag() + message;
	consoleLog.appendChild(pre);
	while (consoleLog.childNodes.length > 50) consoleLog.removeChild(consoleLog.firstChild);
	consoleLog.scrollTop = consoleLog.scrollHeight;
}

function onOpen(evt) {
	logToConsole("CONNECTED");
	setGuiConnected(true);
}

function onClose(evt) {
	logToConsole("DISCONNECTED");
	setGuiConnected(false);
}

function onMessage(evt) {
	logToConsole('<span style="color: blue;">RESPONSE: ' + evt.data+'</span>');
}

function onError(evt) {
	logToConsole('<span style="color: red;">ERROR:</span> ' + evt.data);
}

function setGuiConnected(isConnected) {
	wsUri.disabled = isConnected;
	connectBut.disabled = isConnected;
	disconnectBut.disabled = !isConnected;
	sendMessage.disabled = !isConnected;
	sendBut.disabled = !isConnected;
	secureCb.disabled = isConnected;
	var labelColor = "black";
	if (isConnected) labelColor = "#999999";
	secureCbLabel.style.color = labelColor;
}

function clearLog() {
	while (consoleLog.childNodes.length > 0) consoleLog.removeChild(consoleLog.lastChild);
}

function getSecureTag() {
	if (secureCb.checked) return '<img src="tls-lock.png" width="6px" height="9px"> ';
	else return '';
}

window.addEventListener("load", echoHandlePageLoad, false);

function browserDetect() {
	document.getElementById("browserType").innerHTML = BrowserDetect.browser + ' ' + BrowserDetect.version;
	document.getElementById("platform").innerHTML = BrowserDetect.OS;
	if (window.WebSocket) document.getElementById("wsSupportImg").src = "check.png"
	else document.getElementById("wsSupportImg").src = "cross.png"
}

if (window.attachEvent) window.attachEvent("load", browserDetect)
else window.addEventListener("load", browserDetect, false);
</script>
<div id="webSocketSupp">
<div style="float: left;">
<img src="check.png" width="36" height="36" align="absmiddle">
</div>
<div style="float: left; width: 600px; margin-left: 10px; position:relative; top: 8px;">
This browser supports WebSocket.
</div>
<div class="clearfix"></div>
</div>
<div id="noWebSocketSupp">
<div style="float: left;">
<img src="cross.png" width="36" height="36" align="absmiddle">
</div>
<div style="float: left; width: 600px; margin-left: 10px;">
<p>Uh-oh, the browser you're using doesn't have native support for WebSocket. That means you can't run this demo.</p>
<p>The following link lists the browsers that support WebSocket:</p>
<p><a href="http://caniuse.com/#feat=websockets">http://caniuse.com/#feat=websockets</a></p>
</div>
<div class="clearfix"></div>
</div>
<div id="echo" style="margin-left: 20px;">
<div id="echo-config" style="float: left;">
<strong>Location:</strong><br>
<input id="wsUri" size="35">
<br>
<input type="checkbox" id="secureCb" onClick="toggleTls();">
<span id="secureCbLabel" style="font-size: smaller; color: black;">Use secure WebSocket (TLS)</span><br>
<button id="connect">Connect</button>
<button id="disconnect">Disconnect</button>
<br>
<br>
<strong>Message:</strong><br>
<input id="sendMessage" size="35" value="Rock it with HTML5 WebSocket">
<br>
<button id="send" class="wsButton">Send</button>
</div>
<div id="echo-log" style="float: left; margin-left: 20px; padding-left: 20px; width: 350px; border-left: solid 1px #cccccc;"> <strong>Log:</strong>
<div id="consoleLog"></div>
<button id="clearLogBut" style="position: relative; top: 3px;">Clear log</button>
</div>
<div class="clearfix"></div>
</div>
</html>