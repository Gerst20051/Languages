var crypto = require('crypto');
var util = require('util');
var net = require('net');
var events = require('events');
var stream = require('stream');

var assert = process.assert;

var debug;
if (process.env.NODE_DEBUG && /tls/.test(process.env.NODE_DEBUG)) {
  debug = function() { util.error.apply(this, arguments); };
} else {
  debug = function() { };
}


var Connection = null;
try {
  Connection = process.binding('crypto').Connection;
} catch (e) {
  throw new Error('node.js not compiled with openssl crypto support.');
}


// Base class of both CleartextStream and EncryptedStream
function CryptoStream(pair) {
  stream.Stream.call(this);

  this.pair = pair;

  this.readable = this.writable = true;

  this._writeState = true;
  this._pending = [];
  this._pendingCallbacks = [];
}
util.inherits(CryptoStream, stream.Stream);


CryptoStream.prototype.write = function(data /* , encoding, cb */) {
  var encoding, cb;

  // parse arguments
  if (typeof arguments[1] == 'string') {
    encoding = arguments[1];
    cb = arguments[2];
  } else {
    cb = arguments[1];
  }


  // Transform strings into buffers.
  if (typeof data == 'string') {
    data = new Buffer(data, encoding);
  }

  debug('clearIn data');

  this._pending.push(data);
  this._pendingCallbacks.push(cb);

  this.pair._cycle();
  return this._writeState;
};


CryptoStream.prototype.pause = function() {
  debug('paused cleartext');
  this._writeState = false;
};


CryptoStream.prototype.resume = function() {
  debug('resumed cleartext');
  this._writeState = true;
};


CryptoStream.prototype.setTimeout = function(n) {
  if (this.socket) this.socket.setTimeout(n);
};


CryptoStream.prototype.setNoDelay = function() {
  if (this.socket) this.socket.setNoDelay();
};


CryptoStream.prototype.setEncoding = function(encoding) {
  var StringDecoder = require('string_decoder').StringDecoder; // lazy load
  this._decoder = new StringDecoder(encoding);
};


// EG '/C=US/ST=CA/L=SF/O=Joyent/OU=Node.js/CN=ca1/emailAddress=ry@clouds.org'
function parseCertString(s) {
  var out = {};
  var parts = s.split('/');
  // Note: can always skip the first one.
  for (var i = 1; i < parts.length; i++) {
    var sepIndex = parts[i].indexOf('=');
    if (sepIndex > 0) {
      var key = parts[i].slice(0, sepIndex);
      var value = parts[i].slice(sepIndex + 1);
      out[key] = value;
    }
  }
  return out;
}


CryptoStream.prototype.getPeerCertificate = function() {
  if (this.pair._ssl) {
    var c = this.pair._ssl.getPeerCertificate();

    if (c) {
      if (c.issuer) c.issuer = parseCertString(c.issuer);
      if (c.subject) c.subject = parseCertString(c.subject);
      return c;
    }
  }

  return null;
};


CryptoStream.prototype.getCipher = function(err) {
  if (this.pair._ssl) {
    return this.pair._ssl.getCurrentCipher();
  } else {
    return null;
  }
};


CryptoStream.prototype.end = function(d) {
  if (this.pair._done) return;

  if (d) {
    this.write(d);
    this.pair._cycle();
  }

  // Sending EOF
  debug('cleartext end');
  this.pair._ssl.shutdown();
  this.pair._cycle();
  this.pair._destroy();
};


CryptoStream.prototype.destroySoon = function(err) {
  if (this.pair._done) return;

  this.pair._cycle();

  if (this._pending.length) {
    this.__destroyOnDrain = true;
  } else {
    this.end();
  }
};


CryptoStream.prototype.destroy = function(err) {
  if (this.pair._done) return;
  this.pair._destroy();
};

CryptoStream.prototype.fd = -1;
CryptoStream.prototype.__defineGetter__('readyState',
  net.Socket.prototype.__lookupGetter__('readyState'));

// Move decrypted, clear data out into the application.
// From the user's perspective this occurs as a 'data' event
// on the pair.cleartext.
// also
// Move encrypted data to the stream. From the user's perspective this
// occurs as a 'data' event on the pair.encrypted. Usually the application
// will have some code which pipes the stream to a socket:
//
//   pair.encrypted.on('data', function (d) {
//     socket.write(d);
//   });
//
CryptoStream.prototype._blow = function() {
  var bytesRead;
  var pool;
  var chunkBytes;
  var chunk;

  do {
    bytesRead = 0;
    chunkBytes = 0;
    pool = new Buffer(4096); // alloc every time?
    pool.used = 0;

    do {
      try {
        chunkBytes = this._blower(pool,
                                  pool.used + bytesRead,
                                  pool.length - pool.used - bytesRead);
      } catch (e) {
        if (this.pair._secureEstablished) {
          this.pair._error(e);
        } else {
          this.pair._destroy();
        }
        return;
      }
      if (chunkBytes >= 0) {
        bytesRead += chunkBytes;
      }
    } while ((chunkBytes > 0) && (pool.used + bytesRead < pool.length));

    if (bytesRead > 0) {
      chunk = pool.slice(0, bytesRead);
      if (this._decoder) {
        var string = this._decoder.write(chunk);
        if (string.length) this.emit('data', string);
      } else {
        if (this._events && this._events['data']) {
          this.emit('data', chunk);
        }
      }

      // Optimization: emit the original buffer with end points
      if (this.ondata) this.ondata(pool, 0, bytesRead);
    }
  } while (bytesRead > 0 && this._writeState === true);
};


// Push in any clear data coming from the application.
// This arrives via some code like this:
//
//   pair.cleartext.write("hello world");
//
// also
//
// Push in incoming encrypted data from the socket.
// This arrives via some code like this:
//
//   socket.on('data', function (d) {
//     pair.encrypted.write(d)
//   });
//
CryptoStream.prototype._suck = function() {
  var tmp, cb, rv;
  var havePending = this._pending.length > 0;
  while (this._pending.length > 0) {
    tmp = this._pending.shift();
    cb = this._pendingCallbacks.shift();

    assert(this._pending.length === this._pendingCallbacks.length);

    try {
      rv = this._sucker(tmp);
    } catch (e) {
      if (this.pair._secureEstablished) {
        this.pair._error(e);
      } else {
        this.pair._destroy();
      }
      return;
    }

    if (rv === 0) {
      this._pending.unshift(tmp);
      this._pendingCallbacks.unshift(cb);
      break;
    }

    if (cb) cb();

    assert(rv === tmp.length);
  }

  // If we've cleared all of incoming encrypted data, emit drain.
  if (havePending && this._pending && this._pending.length === 0) {
    debug('drain');
    this.emit('drain');
    if (this.__destroyOnDrain) this.end();
  }
};


function CleartextStream(pair) {
  CryptoStream.call(this, pair);
}
util.inherits(CleartextStream, CryptoStream);


CleartextStream.prototype._sucker = function(b) {
  debug('writng from clearIn');
  return this.pair._ssl.clearIn(b, 0, b.length);
};


CleartextStream.prototype._blower = function(pool, offset, length) {
  debug('reading from clearOut');
  if (!this.pair._ssl) return -1;
  return this.pair._ssl.clearOut(pool, offset, length);
};


function EncryptedStream(pair) {
  CryptoStream.call(this, pair);
}
util.inherits(EncryptedStream, CryptoStream);


EncryptedStream.prototype._sucker = function(b) {
  debug('writing from encIn');
  return this.pair._ssl.encIn(b, 0, b.length);
};


EncryptedStream.prototype._blower = function(pool, offset, length) {
  debug('reading from encOut');
  if (!this.pair._ssl) return -1;
  return this.pair._ssl.encOut(pool, offset, length);
};


/**
 * Provides a pair of streams to do encrypted communication.
 */

function SecurePair(credentials, isServer, requestCert, rejectUnauthorized) {
  if (!(this instanceof SecurePair)) {
    return new SecurePair(credentials,
                          isServer,
                          requestCert,
                          rejectUnauthorized);
  }

  var self = this;

  events.EventEmitter.call(this);

  this._secureEstablished = false;
  this._isServer = isServer ? true : false;
  this._encWriteState = true;
  this._clearWriteState = true;
  this._done = false;

  var crypto = require('crypto');

  if (!credentials) {
    this.credentials = crypto.createCredentials();
  } else {
    this.credentials = credentials;
  }

  if (!this._isServer) {
    // For clients, we will always have either a given ca list or be using
    // default one
    requestCert = true;
  }

  this._secureEstablished = false;
  this._rejectUnauthorized = rejectUnauthorized ? true : false;
  this._requestCert = requestCert ? true : false;

  this._ssl = new Connection(this.credentials.context,
                             this._isServer ? true : false,
                             this._requestCert,
                             this._rejectUnauthorized);


  /* Acts as a r/w stream to the cleartext side of the stream. */
  this.cleartext = new CleartextStream(this);

  /* Acts as a r/w stream to the encrypted side of the stream. */
  this.encrypted = new EncryptedStream(this);

  process.nextTick(function() {
    self._ssl.start();
    self._cycle();
  });
}

util.inherits(SecurePair, events.EventEmitter);


exports.createSecurePair = function(credentials,
                                    isServer,
                                    requestCert,
                                    rejectUnauthorized) {
  var pair = new SecurePair(credentials,
                            isServer,
                            requestCert,
                            rejectUnauthorized);
  return pair;
};




/* Attempt to cycle OpenSSLs buffers in various directions.
 *
 * An SSL Connection can be viewed as four separate piplines,
 * interacting with one has no connection to the behavoir of
 * any of the other 3 -- This might not sound reasonable,
 * but consider things like mid-stream renegotiation of
 * the ciphers.
 *
 * The four pipelines, using terminology of the client (server is just
 * reversed):
 *  (1) Encrypted Output stream (Writing encrypted data to peer)
 *  (2) Encrypted Input stream (Reading encrypted data from peer)
 *  (3) Cleartext Output stream (Decrypted content from the peer)
 *  (4) Cleartext Input stream (Cleartext content to send to the peer)
 *
 * This function attempts to pull any available data out of the Cleartext
 * input stream (4), and the Encrypted input stream (2).  Then it pushes any
 * data available from the cleartext output stream (3), and finally from the
 * Encrypted output stream (1)
 *
 * It is called whenever we do something with OpenSSL -- post reciving
 * content, trying to flush, trying to change ciphers, or shutting down the
 * connection.
 *
 * Because it is also called everywhere, we also check if the connection has
 * completed negotiation and emit 'secure' from here if it has.
 */
SecurePair.prototype._cycle = function() {
  if (this._done) {
    return;
  }

  this.encrypted._suck();
  this.cleartext._suck();
  this.cleartext._blow();
  this.encrypted._blow();

  if (this._ssl && !this._secureEstablished && this._ssl.isInitFinished()) {
    this._secureEstablished = true;
    debug('secure established');
    this.emit('secure');
    this._cycle();
  }
};


SecurePair.prototype._destroy = function(err) {
  if (!this._done) {
    this._done = true;
    this._ssl.close();
    this._ssl = null;

    this.encrypted.emit('end');
    this.encrypted.emit('close');

    this.cleartext.emit('end');
    this.cleartext.emit('close');
  }
  this._cycle();
};


SecurePair.prototype._error = function(err) {
  if (this._isServer &&
      this._rejectUnauthorized &&
      /peer did not return a certificate/.test(err.message)) {
    // Not really an error.
    this._destroy();
  } else {
    this.cleartext.emit('error', err);
  }
};

// TODO: support anonymous (nocert) and PSK


// AUTHENTICATION MODES
//
// There are several levels of authentication that TLS/SSL supports.
// Read more about this in "man SSL_set_verify".
//
// 1. The server sends a certificate to the client but does not request a
// cert from the client. This is common for most HTTPS servers. The browser
// can verify the identity of the server, but the server does not know who
// the client is. Authenticating the client is usually done over HTTP using
// login boxes and cookies and stuff.
//
// 2. The server sends a cert to the client and requests that the client
// also send it a cert. The client knows who the server is and the server is
// requesting the client also identify themselves. There are several
// outcomes:
//
//   A) verifyError returns null meaning the client's certificate is signed
//   by one of the server's CAs. The server know's the client idenity now
//   and the client is authorized.
//
//   B) For some reason the client's certificate is not acceptable -
//   verifyError returns a string indicating the problem. The server can
//   either (i) reject the client or (ii) allow the client to connect as an
//   unauthorized connection.
//
// The mode is controlled by two boolean variables.
//
// requestCert
//   If true the server requests a certificate from client connections. For
//   the common HTTPS case, users will want this to be false, which is what
//   it defaults to.
//
// rejectUnauthorized
//   If true clients whose certificates are invalid for any reason will not
//   be allowed to make connections. If false, they will simply be marked as
//   unauthorized but secure communication will continue. By default this is
//   false.
//
//
//
// Options:
// - requestCert. Send verify request. Default to false.
// - rejectUnauthorized. Boolean, default to false.
// - key. string.
// - cert: string.
// - ca: string or array of strings.
//
// emit 'secureConnection'
//   function (cleartextStream, encryptedStream) { }
//
//   'cleartextStream' has the boolean property 'authorized' to determine if
//   it was verified by the CA. If 'authorized' is false, a property
//   'authorizationError' is set on cleartextStream and has the possible
//   values:
//
//   "UNABLE_TO_GET_ISSUER_CERT", "UNABLE_TO_GET_CRL",
//   "UNABLE_TO_DECRYPT_CERT_SIGNATURE", "UNABLE_TO_DECRYPT_CRL_SIGNATURE",
//   "UNABLE_TO_DECODE_ISSUER_PUBLIC_KEY", "CERT_SIGNATURE_FAILURE",
//   "CRL_SIGNATURE_FAILURE", "CERT_NOT_YET_VALID" "CERT_HAS_EXPIRED",
//   "CRL_NOT_YET_VALID", "CRL_HAS_EXPIRED" "ERROR_IN_CERT_NOT_BEFORE_FIELD",
//   "ERROR_IN_CERT_NOT_AFTER_FIELD", "ERROR_IN_CRL_LAST_UPDATE_FIELD",
//   "ERROR_IN_CRL_NEXT_UPDATE_FIELD", "OUT_OF_MEM",
//   "DEPTH_ZERO_SELF_SIGNED_CERT", "SELF_SIGNED_CERT_IN_CHAIN",
//   "UNABLE_TO_GET_ISSUER_CERT_LOCALLY", "UNABLE_TO_VERIFY_LEAF_SIGNATURE",
//   "CERT_CHAIN_TOO_LONG", "CERT_REVOKED" "INVALID_CA",
//   "PATH_LENGTH_EXCEEDED", "INVALID_PURPOSE" "CERT_UNTRUSTED",
//   "CERT_REJECTED"
//
//
// TODO:
// cleartext.credentials (by mirroring from pair object)
// cleartext.getCertificate() (by mirroring from pair.credentials.context)
function Server(/* [options], listener */) {
  var options, listener;
  if (typeof arguments[0] == 'object') {
    options = arguments[0];
    listener = arguments[1];
  } else if (typeof arguments[0] == 'function') {
    options = {};
    listener = arguments[0];
  }

  if (!(this instanceof Server)) return new Server(options, listener);

  var self = this;

  // constructor call
  net.Server.call(this, function(socket) {
    var creds = crypto.createCredentials(
        { key: self.key, cert: self.cert, ca: self.ca });
    //creds.context.setCiphers('RC4-SHA:AES128-SHA:AES256-SHA');

    var pair = new SecurePair(creds,
                              true,
                              self.requestCert,
                              self.rejectUnauthorized);

    var cleartext = pipe(pair, socket);
    cleartext._controlReleased = false;

    pair.on('secure', function() {
      pair.cleartext.authorized = false;
      if (!self.requestCert) {
        cleartext._controlReleased = true;
        self.emit('secureConnection', pair.cleartext, pair.encrypted);
      } else {
        var verifyError = pair._ssl.verifyError();
        if (verifyError) {
          pair.cleartext.authorizationError = verifyError;

          if (self.rejectUnauthorized) {
            socket.destroy();
            pair._destroy();
          } else {
            cleartext._controlReleased = true;
            self.emit('secureConnection', pair.cleartext, pair.encrypted);
          }
        } else {
          pair.cleartext.authorized = true;
          cleartext._controlReleased = true;
          self.emit('secureConnection', pair.cleartext, pair.encrypted);
        }
      }
    });
  });

  if (listener) {
    this.on('secureConnection', listener);
  }

  // Handle option defaults:
  this.setOptions(options);
}

util.inherits(Server, net.Server);
exports.Server = Server;
exports.createServer = function(options, listener) {
  return new Server(options, listener);
};


Server.prototype.setOptions = function(options) {
  if (typeof options.requestCert == 'boolean') {
    this.requestCert = options.requestCert;
  } else {
    this.requestCert = false;
  }

  if (typeof options.rejectUnauthorized == 'boolean') {
    this.rejectUnauthorized = options.rejectUnauthorized;
  } else {
    this.rejectUnauthorized = false;
  }

  if (options.key) this.key = options.key;
  if (options.cert) this.cert = options.cert;
  if (options.ca) this.ca = options.ca;
};


// Target API:
//
//  var s = tls.connect(8000, "google.com", options, function() {
//    if (!s.authorized) {
//      s.destroy();
//      return;
//    }
//
//    // s.socket;
//
//    s.end("hello world\n");
//  });
//
exports.connect = function(port /* host, options, cb */) {
  // parse args
  var host, options = {}, cb;
  switch (typeof arguments[1]) {
    case 'string':
      host = arguments[1];
      if (typeof arguments[2] == 'object') {
        options = arguments[2];
        if (typeof arguments[3] == 'function') cb = arguments[3];
      } else if (typeof arguments[2] == 'function') {
        cb = arguments[2];
      }
      break;

    case 'object':
      options = arguments[1];
      if (typeof arguments[2] == 'function') cb = arguments[2];
      break;

    case 'function':
      cb = arguments[1];
      break;

    default:
      break;
  }


  var socket = new net.Stream();

  var sslcontext = crypto.createCredentials(options);
  //sslcontext.context.setCiphers('RC4-SHA:AES128-SHA:AES256-SHA');

  var pair = new SecurePair(sslcontext, false);

  var cleartext = pipe(pair, socket);

  socket.connect(port, host);

  pair.on('secure', function() {
    var verifyError = pair._ssl.verifyError();

    if (verifyError) {
      cleartext.authorized = false;
      cleartext.authorizationError = verifyError;
    } else {
      cleartext.authorized = true;
    }

    if (cb) cb();
  });

  cleartext._controlReleased = true;
  return cleartext;
};


function pipe(pair, socket) {
  pair.encrypted.pipe(socket);
  socket.pipe(pair.encrypted);

  var cleartext = pair.cleartext;
  cleartext.socket = socket;
  cleartext.encrypted = pair.encrypted;
  cleartext.authorized = false;

  function onerror(e) {
    if (cleartext._controlReleased) {
      cleartext.emit('error', e);
    }
  }

  function onclose() {
    socket.removeListener('error', onerror);
    socket.removeListener('close', onclose);
  }

  socket.on('error', onerror);
  socket.on('close', onclose);

  return cleartext;
}
