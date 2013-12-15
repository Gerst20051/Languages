<!DOCTYPE html>
<meta charset="utf-8" />
<title>WebSocket Test</title>
<script language="javascript" type="text/javascript">
var socket = new WebSocket('ws://echo.websocket.org/');
socket.onopen = function () {
	writeToScreen("CONNECTED");
	setInterval(function() {
		if (socket.bufferedAmount == 0) doSend();
	}, 50);
};

socket.onclose = function (e) { writeToScreen("DISCONNECTED"); };
socket.onmessage = function (e) { writeToScreen('<span style="color: blue;">RESPONSE: ' + e.data + '</span>'); };
socket.onerror = function (e) { writeToScreen('<span style="color: red;">ERROR:</span> ' + e.data); };

function getUpdateData() {
	return "WebSocket!";
}

function doSend() {
	writeToScreen("SENT: " + getUpdateData());
	socket.send(getUpdateData());
}

function writeToScreen(message) {
	var pre = document.createElement("p");
	pre.style.wordWrap = "break-word";
	pre.innerHTML = message;
	output.appendChild(pre);
}
</script>
<h2>WebSocket Test</h2>
<div id="output"></div>
</html>