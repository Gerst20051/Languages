var common = require('../common');
var http = require('http');
var assert = require('assert');

// first 204 or 304 works, subsequent anything fails
var codes = [204, 200];

// Methods don't really matter, but we put in something realistic.
var methods = ['DELETE', 'DELETE'];

var server = http.createServer(function(req, res) {
  var code = codes.shift();
  assert.equal('number', typeof code);
  assert.ok(code > 0);
  console.error('writing %d response', code);
  res.writeHead(code, {});
  res.end();
});

var client = http.createClient(common.PORT);

function nextRequest() {
  var method = methods.shift();
  console.error('writing request: %s', method);

  var request = client.request(method, '/');
  request.on('response', function(response) {
    response.on('end', function() {
      if (methods.length == 0) {
        console.error('close server');
        server.close();
      } else {
        // throws error:
        nextRequest();
        // works just fine:
        //process.nextTick(nextRequest);
      }
    });
  });
  request.end();
}

server.listen(common.PORT, nextRequest);
