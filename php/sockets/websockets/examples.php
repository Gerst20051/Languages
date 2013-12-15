<?php

// Example 1
var socket = new WebSocket('ws://game.example.com:12010/updates');
socket.onopen = function () {
	setInterval(function() {
		if (socket.bufferedAmount == 0) socket.send(getUpdateData());
	}, 50);
};

// Example 2
if ('WebSocket' in window) connect('ws://localhost:8181/service'); 
function connect(host) {
	var ws = new WebSocket(host);
	ws.onopen = function () { alert('connected'); };
 	ws.onmessage = function (evt) { alert('reveived data:' + evt.data); };
	ws.onclose = function () { alert('socket closed'); };
}

?>