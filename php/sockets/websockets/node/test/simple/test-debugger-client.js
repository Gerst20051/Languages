var common = require('../common');
var assert = require('assert');
var debug = require('_debugger');

var spawn = require('child_process').spawn;


var resCount = 0;
var p = new debug.Protocol();
p.onResponse = function (res) {
  resCount++;
};

p.execute("Type: connect\r\n" +
          "V8-Version: 3.0.4.1\r\n" +
          "Protocol-Version: 1\r\n" +
          "Embedding-Host: node v0.3.3-pre\r\n" +
          "Content-Length: 0\r\n\r\n");
assert.equal(1, resCount);

// Make sure split messages go in.

var parts = [];
parts.push('Content-Length: 336\r\n');
assert.equal(21, parts[0].length);
parts.push('\r\n');
assert.equal(2, parts[1].length);
var bodyLength = 0;

parts.push('{"seq":12,"type":"event","event":"break","body":' +
           '{"invocationText":"#<a Server>');
assert.equal(78, parts[2].length);
bodyLength += parts[2].length;

parts.push('.[anonymous](req=#<an IncomingMessage>, res=#<a ServerResponse>)",' +
           '"sourceLine"');
assert.equal(78, parts[3].length);
bodyLength += parts[3].length;

parts.push(':45,"sourceColumn":4,"sourceLineText":"    debugger;","script":' +
           '{"id":24,"name":"/home/ryan/projects/node/benchmark/http_simple.js",' +
           '"lineOffset":0,"columnOffset":0,"lineCount":98}}}');
assert.equal(180, parts[4].length);
bodyLength += parts[4].length;

assert.equal(336, bodyLength);

for (var i = 0; i < parts.length; i++) {
  p.execute(parts[i]);
}
assert.equal(2, resCount);


// Make sure that if we get backed up, we still manage to get all the
// messages
var d = 'Content-Length: 466\r\n\r\n' +
        '{"seq":10,"type":"event","event":"afterCompile","success":true,' +
        '"body":{"script":{"handle":1,"type":"script","name":"dns.js",' +
        '"id":34,"lineOffset":0,"columnOffset":0,"lineCount":241,"sourceStart":' +
        '"(function (module, exports, require) {var dns = process.binding(\'cares\')' +
        ';\\nvar ne","sourceLength":6137,"scriptType":2,"compilationType":0,' +
        '"context":{"ref":0},"text":"dns.js (lines: 241)"}},"refs":[{"handle":0' +
        ',"type":"context","text":"#<a ContextMirror>"}],"running":true}' +
        'Content-Length: 119\r\n\r\n' +
        '{"seq":11,"type":"event","event":"scriptCollected","success":true,' +
        '"body":{"script":{"id":26}},"refs":[],"running":true}';
p.execute(d);
assert.equal(4, resCount);

var expectedConnections = 0;
var tests = [];
function addTest (cb) {
  expectedConnections++;
  tests.push(cb);
}

addTest(function (client, done) {
  console.error("requesting version");
  client.reqVersion(function (v) {
    console.log("version: %s", v);
    assert.equal(process.versions.v8, v);
    done();
  });
});

addTest(function (client, done) {
  console.error("requesting scripts");
  client.reqScripts(function () {
    console.error("got %d scripts", Object.keys(client.scripts).length);

    var foundMainScript = false;
    for (var k in client.scripts) {
      var script = client.scripts[k];
      if (script && script.name === 'node.js') {
        foundMainScript = true;
        break;
      }
    }
    assert.ok(foundMainScript);
    done();
  });
});

addTest(function (client, done) {
  console.error("eval 2+2");
  client.reqEval("2+2", function (res) {
    assert.ok(res.success);
    console.error(res);
    assert.equal('4', res.body.text);
    assert.equal(4, res.body.value);
    done();
  });
});


var connectCount = 0;

function doTest(cb, done) {
  var nodeProcess = spawn(process.execPath,
      ['-e', 'setInterval(function () { console.log("blah"); }, 100);']);

  nodeProcess.stdout.once('data', function () {
    console.log(">>> new node process: %d", nodeProcess.pid);
    process.kill(nodeProcess.pid, "SIGUSR1");
    console.log(">>> signaling it with SIGUSR1");
  });

  var didTryConnect = false;
  nodeProcess.stderr.setEncoding('utf8');
  var b = ''
  nodeProcess.stderr.on('data', function (data) {
    b += data;
    if (didTryConnect == false && /debugger listening on port/.test(b)) {
      didTryConnect = true;

      setTimeout(function() {
        // Wait for some data before trying to connect
        var c = new debug.Client();
        process.stdout.write(">>> connecting...");
        c.connect(debug.port)
        c.on('ready', function () {
          connectCount++;
          console.log("ready!");
          cb(c, function () {
            console.error(">>> killing node process %d\n\n", nodeProcess.pid);
            nodeProcess.kill();
            done();
          });
        });
      }, 100);
    }
  });
}


function run () {
  var t = tests[0];
  if (!t) return;

  doTest(t, function () {
    tests.shift();
    run();
  });
}

run();

process.on('exit', function() {
  assert.equal(expectedConnections, connectCount);
});

