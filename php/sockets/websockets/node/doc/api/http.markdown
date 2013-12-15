## HTTP

To use the HTTP server and client one must `require('http')`.

The HTTP interfaces in Node are designed to support many features
of the protocol which have been traditionally difficult to use.
In particular, large, possibly chunk-encoded, messages. The interface is
careful to never buffer entire requests or responses--the
user is able to stream data.

HTTP message headers are represented by an object like this:

    { 'content-length': '123',
      'content-type': 'text/plain',
      'connection': 'keep-alive',
      'accept': '*/*' }

Keys are lowercased. Values are not modified.

In order to support the full spectrum of possible HTTP applications, Node's
HTTP API is very low-level. It deals with stream handling and message
parsing only. It parses a message into headers and body but it does not
parse the actual headers or the body.


## http.Server

This is an `EventEmitter` with the following events:

### Event: 'request'

`function (request, response) { }`

 `request` is an instance of `http.ServerRequest` and `response` is
 an instance of `http.ServerResponse`

### Event: 'connection'

`function (stream) { }`

 When a new TCP stream is established. `stream` is an object of type
 `net.Stream`. Usually users will not want to access this event. The
 `stream` can also be accessed at `request.connection`.

### Event: 'close'

`function (errno) { }`

 Emitted when the server closes.

### Event: 'request'

`function (request, response) {}`

Emitted each time there is request. Note that there may be multiple requests
per connection (in the case of keep-alive connections).

### Event: 'checkContinue'

`function (request, response) {}`

Emitted each time a request with an http Expect: 100-continue is received.
If this event isn't listened for, the server will automatically respond
with a 100 Continue as appropriate.

Handling this event involves calling `response.writeContinue` if the client
should continue to send the request body, or generating an appropriate HTTP
response (e.g., 400 Bad Request) if the client should not continue to send the
request body.

Note that when this event is emitted and handled, the `request` event will
not be emitted.

### Event: 'upgrade'

`function (request, socket, head)`

Emitted each time a client requests a http upgrade. If this event isn't
listened for, then clients requesting an upgrade will have their connections
closed.

* `request` is the arguments for the http request, as it is in the request event.
* `socket` is the network socket between the server and client.
* `head` is an instance of Buffer, the first packet of the upgraded stream, this may be empty.

After this event is emitted, the request's socket will not have a `data`
event listener, meaning you will need to bind to it in order to handle data
sent to the server on that socket.

### Event: 'clientError'

`function (exception) {}`

If a client connection emits an 'error' event - it will forwarded here.

### http.createServer(requestListener)

Returns a new web server object.

The `requestListener` is a function which is automatically
added to the `'request'` event.

### server.listen(port, [hostname], [callback])

Begin accepting connections on the specified port and hostname.  If the
hostname is omitted, the server will accept connections directed to any
IPv4 address (`INADDR_ANY`).

To listen to a unix socket, supply a filename instead of port and hostname.

This function is asynchronous. The last parameter `callback` will be called
when the server has been bound to the port.


### server.listen(path, [callback])

Start a UNIX socket server listening for connections on the given `path`.

This function is asynchronous. The last parameter `callback` will be called
when the server has been bound.


### server.close()

Stops the server from accepting new connections.


## http.ServerRequest

This object is created internally by a HTTP server -- not by
the user -- and passed as the first argument to a `'request'` listener.

This is an `EventEmitter` with the following events:

### Event: 'data'

`function (chunk) { }`

Emitted when a piece of the message body is received.

Example: A chunk of the body is given as the single
argument. The transfer-encoding has been decoded.  The
body chunk is a string.  The body encoding is set with
`request.setBodyEncoding()`.

### Event: 'end'

`function () { }`

Emitted exactly once for each message. No arguments.  After
emitted no other events will be emitted on the request.


### request.method

The request method as a string. Read only. Example:
`'GET'`, `'DELETE'`.


### request.url

Request URL string. This contains only the URL that is
present in the actual HTTP request. If the request is:

    GET /status?name=ryan HTTP/1.1\r\n
    Accept: text/plain\r\n
    \r\n

Then `request.url` will be:

    '/status?name=ryan'

If you would like to parse the URL into its parts, you can use
`require('url').parse(request.url)`.  Example:

    node> require('url').parse('/status?name=ryan')
    { href: '/status?name=ryan',
      search: '?name=ryan',
      query: 'name=ryan',
      pathname: '/status' }

If you would like to extract the params from the query string,
you can use the `require('querystring').parse` function, or pass
`true` as the second argument to `require('url').parse`.  Example:

    node> require('url').parse('/status?name=ryan', true)
    { href: '/status?name=ryan',
      search: '?name=ryan',
      query: { name: 'ryan' },
      pathname: '/status' }



### request.headers

Read only.

### request.trailers

Read only; HTTP trailers (if present). Only populated after the 'end' event.

### request.httpVersion

The HTTP protocol version as a string. Read only. Examples:
`'1.1'`, `'1.0'`.
Also `request.httpVersionMajor` is the first integer and
`request.httpVersionMinor` is the second.


### request.setEncoding(encoding=null)

Set the encoding for the request body. Either `'utf8'` or `'binary'`. Defaults
to `null`, which means that the `'data'` event will emit a `Buffer` object..


### request.pause()

Pauses request from emitting events.  Useful to throttle back an upload.


### request.resume()

Resumes a paused request.

### request.connection

The `net.Stream` object associated with the connection.


With HTTPS support, use request.connection.verifyPeer() and
request.connection.getPeerCertificate() to obtain the client's
authentication details.



## http.ServerResponse

This object is created internally by a HTTP server--not by the user. It is
passed as the second parameter to the `'request'` event. It is a `Writable Stream`.

### response.writeContinue()

Sends a HTTP/1.1 100 Continue message to the client, indicating that
the request body should be sent. See the the `checkContinue` event on
`Server`.

### response.writeHead(statusCode, [reasonPhrase], [headers])

Sends a response header to the request. The status code is a 3-digit HTTP
status code, like `404`. The last argument, `headers`, are the response headers.
Optionally one can give a human-readable `reasonPhrase` as the second
argument.

Example:

    var body = 'hello world';
    response.writeHead(200, {
      'Content-Length': body.length,
      'Content-Type': 'text/plain' });

This method must only be called once on a message and it must
be called before `response.end()` is called.

### response.write(chunk, encoding='utf8')

This method must be called after `writeHead` was
called. It sends a chunk of the response body. This method may
be called multiple times to provide successive parts of the body.

`chunk` can be a string or a buffer. If `chunk` is a string,
the second parameter specifies how to encode it into a byte stream.
By default the `encoding` is `'utf8'`.

**Note**: This is the raw HTTP body and has nothing to do with
higher-level multi-part body encodings that may be used.

The first time `response.write()` is called, it will send the buffered
header information and the first body to the client. The second time
`response.write()` is called, Node assumes you're going to be streaming
data, and sends that separately. That is, the response is buffered up to the
first chunk of body.

### response.addTrailers(headers)

This method adds HTTP trailing headers (a header but at the end of the
message) to the response.

Trailers will **only** be emitted if chunked encoding is used for the
response; if it is not (e.g., if the request was HTTP/1.0), they will
be silently discarded.

Note that HTTP requires the `Trailer` header to be sent if you intend to
emit trailers, with a list of the header fields in its value. E.g.,

    response.writeHead(200, { 'Content-Type': 'text/plain',
                              'Trailer': 'TraceInfo' });
    response.write(fileData);
    response.addTrailers({'Content-MD5': "7895bf4b8828b55ceaf47747b4bca667"});
    response.end();


### response.end([data], [encoding])

This method signals to the server that all of the response headers and body
has been sent; that server should consider this message complete.
The method, `response.end()`, MUST be called on each
response.

If `data` is specified, it is equivalent to calling `response.write(data, encoding)`
followed by `response.end()`.


## http.request(options, callback)

Node maintains several connections per server to make HTTP requests.
This function allows one to transparently issue requests.

Options:

- `host`: A domain name or IP address of the server to issue the request to.
- `port`: Port of remote server.
- `method`: A string specifing the HTTP request method. Possible values:
  `'GET'` (default), `'POST'`, `'PUT'`, and `'DELETE'`.
- `path`: Request path. Should include query string and fragments if any.
   E.G. `'/index.html?page=12'`
- `headers`: An object containing request headers.

`http.request()` returns an instance of the `http.ClientRequest`
class. The `ClientRequest` instance is a writable stream. If one needs to
upload a file with a POST request, then write to the `ClientRequest` object.

Example:

    var options = {
      host: 'www.google.com',
      port: 80,
      path: '/upload',
      method: 'POST'
    };

    var req = http.request(options, function(res) {
      console.log('STATUS: ' + res.statusCode);
      console.log('HEADERS: ' + JSON.stringify(res.headers));
      res.setEncoding('utf8');
      res.on('data', function (chunk) {
        console.log('BODY: ' + chunk);
      });
    });

    // write data to request body
    req.write('data\n');
    req.write('data\n');
    req.end();

Note that in the example `req.end()` was called. With `http.request()` one
must always call `req.end()` to signify that you're done with the request -
even if there is no data being written to the request body.

If any error is encountered during the request (be that with DNS resolution,
TCP level errors, or actual HTTP parse errors) an `'error'` event is emitted
on the returned request object.

There are a few special headers that should be noted.

* Sending a 'Connection: keep-alive' will notify Node that the connection to
  the server should be persisted until the next request.

* Sending a 'Content-length' header will disable the default chunked encoding.

* Sending an 'Expect' header will immediately send the request headers.
  Usually, when sending 'Expect: 100-continue', you should both set a timeout
  and listen for the `continue` event. See RFC2616 Section 8.2.3 for more
  information.

## http.get(options, callback)

Since most requests are GET requests without bodies, Node provides this
convience method. The only difference between this method and `http.request()` is
that it sets the method to GET and calls `req.end()` automatically.

Example:

    var options = {
      host: 'www.google.com',
      port: 80,
      path: '/index.html'
    };

    http.get(options, function(res) {
      console.log("Got response: " + res.statusCode);
    }).on('error', function(e) {
      console.log("Got error: " + e.message);
    });


## http.Agent
## http.getAgent(host, port)

`http.request()` uses a special `Agent` for managing multiple connections to
an HTTP server. Normally `Agent` instances should not be exposed to user
code, however in certain situations it's useful to check the status of the
agent. The `http.getAgent()` function allows you to access the agents.

### Event: 'upgrade'

`function (request, socket, head)`

Emitted each time a server responds to a request with an upgrade. If this event
isn't being listened for, clients receiving an upgrade header will have their
connections closed.

See the description of the `upgrade` event for `http.Server` for further details.

### Event: 'continue'

`function ()`

Emitted when the server sends a '100 Continue' HTTP response, usually because
the request contained 'Expect: 100-continue'. This is an instruction that
the client should send the request body.

### agent.maxSockets

By default set to 5. Determines how many concurrent sockets the agent can have open.

### agent.sockets

An array of sockets currently inuse by the Agent. Do not modify.

### agent.queue

A queue of requests waiting to be sent to sockets.



## http.ClientRequest

This object is created internally and returned from `http.request()`.  It
represents an _in-progress_ request whose header has already been sent.

To get the response, add a listener for `'response'` to the request object.
`'response'` will be emitted from the request object when the response
headers have been received.  The `'response'` event is executed with one
argument which is an instance of `http.ClientResponse`.

During the `'response'` event, one can add listeners to the
response object; particularly to listen for the `'data'` event. Note that
the `'response'` event is called before any part of the response body is received,
so there is no need to worry about racing to catch the first part of the
body. As long as a listener for `'data'` is added during the `'response'`
event, the entire body will be caught.


    // Good
    request.on('response', function (response) {
      response.on('data', function (chunk) {
        console.log('BODY: ' + chunk);
      });
    });

    // Bad - misses all or part of the body
    request.on('response', function (response) {
      setTimeout(function () {
        response.on('data', function (chunk) {
          console.log('BODY: ' + chunk);
        });
      }, 10);
    });

This is a `Writable Stream`.

This is an `EventEmitter` with the following events:

### Event 'response'

`function (response) { }`

Emitted when a response is received to this request. This event is emitted only once. The
`response` argument will be an instance of `http.ClientResponse`.


### request.write(chunk, encoding='utf8')

Sends a chunk of the body.  By calling this method
many times, the user can stream a request body to a
server--in that case it is suggested to use the
`['Transfer-Encoding', 'chunked']` header line when
creating the request.

The `chunk` argument should be an array of integers
or a string.

The `encoding` argument is optional and only
applies when `chunk` is a string.


### request.end([data], [encoding])

Finishes sending the request. If any parts of the body are
unsent, it will flush them to the stream. If the request is
chunked, this will send the terminating `'0\r\n\r\n'`.

If `data` is specified, it is equivalent to calling `request.write(data, encoding)`
followed by `request.end()`.


## http.ClientResponse

This object is created when making a request with `http.request()`. It is
passed to the `'response'` event of the request object.

The response implements the `Readable Stream` interface.

### Event: 'data'

`function (chunk) {}`

Emitted when a piece of the message body is received.


### Event: 'end'

`function () {}`

Emitted exactly once for each message. No arguments. After
emitted no other events will be emitted on the response.

### response.statusCode

The 3-digit HTTP response status code. E.G. `404`.

### response.httpVersion

The HTTP version of the connected-to server. Probably either
`'1.1'` or `'1.0'`.
Also `response.httpVersionMajor` is the first integer and
`response.httpVersionMinor` is the second.

### response.headers

The response headers object.

### response.trailers

The response trailers object. Only populated after the 'end' event.

### response.setEncoding(encoding=null)

Set the encoding for the response body. Either `'utf8'`, `'ascii'`, or `'base64'`.
Defaults to `null`, which means that the `'data'` event will emit a `Buffer` object..

### response.pause()

Pauses response from emitting events.  Useful to throttle back a download.

### response.resume()

Resumes a paused response.
