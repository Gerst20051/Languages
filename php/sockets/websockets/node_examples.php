An example of a web server written in Node which responds with "Hello World" for every request.

<script>
var http = require('http');
http.createServer(function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/plain'});
  res.end('Hello World\n');
}).listen(8124, "127.0.0.1");
console.log('Server running at http://127.0.0.1:8124/');
</script>

To run the server, put the code into a file example.js and execute it with the node program:

% node example.js
Server running at http://127.0.0.1:8124/
Here is an example of a simple TCP server which listens on port 8124 and echoes whatever you send it:

<script>
var net = require('net');
net.createServer(function (socket) {
  socket.write("Echo server\r\n");
  socket.on("data", function (data) {
    socket.write(data);
  });
}).listen(8124, "127.0.0.1");
</script>

<script>
// old example
var ws = require("websocket-server");
var server = ws.createServer();
server.addListener("connection", function(connection){
  connection.addListener("message", function(msg){
    server.send(msg);
  });
});
server.listen(8080);
</script>