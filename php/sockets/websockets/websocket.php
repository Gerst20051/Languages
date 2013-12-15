<!DOCTYPE html>
<meta charset="utf-8" />
<title>WebSocket Test</title>
<script language="javascript" type="text/javascript">
var output, wsuri = "ws://echo.websocket.org/";
function init() { output = document.getElementById("output"); testWebSocket(); }

function testWebSocket() {
	websocket = new WebSocket(wsuri);
	websocket.onopen = function(e) { onOpen(e) };
	websocket.onclose = function(e) { onClose(e) };
	websocket.onmessage = function(e) { onMessage(e) };
	websocket.onerror = function(e) { onError(e) };
}

function onOpen(e) { writeToScreen("CONNECTED"); doSend("WebSocket Rocks"); }
function onClose(e) { writeToScreen("DISCONNECTED"); }
function onMessage(e) { writeToScreen('<span style="color: blue;">RESPONSE: ' + e.data + '</span>'); websocket.close(); }
function onError(e) { writeToScreen('<span style="color: red;">ERROR:</span> ' + e.data); }
function doSend(message) { writeToScreen("SENT: " + message); websocket.send(message); }

function writeToScreen(message) {
	var pre = document.createElement("p");
	pre.style.wordWrap = "break-word";
	pre.innerHTML = message;
	output.appendChild(pre);
}

window.addEventListener("load", init, false);
</script>
<h2>WebSocket Test</h2>
<div id="output"></div>
</html>