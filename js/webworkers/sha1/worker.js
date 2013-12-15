var CHUNKS = 120;
var CHUNK_TIMEOUT = 1;
var SERVER = "";

var again = 0;
var startTime;
var processed = 0;
var statInterval;
var stopped = false;
var myBest = 160;
var ratesHis = [];

function start() {
  var best = data.best;
  var key = data.sha;
  var wordList = data.words;
  startTime = (new Date()).getTime();
  
  function compute() {
    wordList.sort(randOrd);
    var append = [97, 97, 97, 97];
    var words = wordList.join(" ") + " ";
    
    function nextGroup() {
      while (append[0] < 123) {
        while (append[1] < 123) {
          while (append[2] < 123) {
            while (append[3] < 123) {
              var phrase = words + String.fromCharCode.apply(this, append);
              var sha1 = sha1Hash(phrase);
              var distance = hamming(sha1, key);
              
              // New Personal Best Found
              if (distance < myBest) {
                myBest = distance;
		postMessage("mybest " + myBest);
              }

              // New All-time Best Found
              if (distance < best) {
                best = distance;
		postMessage("found " + phrase);
              }
              
              processed++;
              
              append[3]++;

              if (processed % CHUNKS === 0) {
                if (!stopped) {
		  stats();
		}
              }
            }
            append[3] = 97;
            append[2]++;
          }
          append[2] = 97;
          append[1]++;
        }
        append[1] = 97;
        append[0]++;
      }

      compute();
    }
    
    nextGroup();
  }
  
  compute();
};


function stats() {
  var nowDiff = (new Date()).getTime() - startTime;
  var perSec = Math.floor(processed/nowDiff*1000);

  postMessage( "rate " + perSec );
  return;
}

function stop() {
  stopped = true;
  clearInterval(statInterval);
  stats();
}

//
//  Util Functions
//

function randOrd(){
  return (Math.round(Math.random())-0.5);
}

function sha1Hash(msg)
{
    var K = [0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6];

    msg += String.fromCharCode(0x80);

    var l = Math.ceil(msg.length/4) + 2;
    var N = Math.ceil(l/16);
    var M = new Array(N);
    for (var i=0; i<N; i++) {
        M[i] = new Array(16);
        for (var j=0; j<16; j++) {
            M[i][j] = (msg.charCodeAt(i*64+j*4)<<24) | (msg.charCodeAt(i*64+j*4+1)<<16) | 
                      (msg.charCodeAt(i*64+j*4+2)<<8) | (msg.charCodeAt(i*64+j*4+3));
        }
    }
    M[N-1][14] = ((msg.length-1)*8) / Math.pow(2, 32); M[N-1][14] = Math.floor(M[N-1][14])
    M[N-1][15] = ((msg.length-1)*8) & 0xffffffff;

    var H0 = 0x67452301;
    var H1 = 0xefcdab89;
    var H2 = 0x98badcfe;
    var H3 = 0x10325476;
    var H4 = 0xc3d2e1f0;

    var W = new Array(80); var a, b, c, d, e;
    for (var i=0; i<N; i++) {

        for (var t=0;  t<16; t++) W[t] = M[i][t];
        for (var t=16; t<80; t++) W[t] = ROTL(W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16], 1);

        a = H0; b = H1; c = H2; d = H3; e = H4;

        for (var t=0; t<80; t++) {
            var s = Math.floor(t/20);
            var T = (ROTL(a,5) + f(s,b,c,d) + e + K[s] + W[t]) & 0xffffffff;
            e = d;
            d = c;
            c = ROTL(b, 30);
            b = a;
            a = T;
        }

        H0 = (H0+a) & 0xffffffff;
        H1 = (H1+b) & 0xffffffff; 
        H2 = (H2+c) & 0xffffffff; 
        H3 = (H3+d) & 0xffffffff; 
        H4 = (H4+e) & 0xffffffff;
    }

    return H0.toHexStr() + H1.toHexStr() + H2.toHexStr() + H3.toHexStr() + H4.toHexStr();
}

function f(s, x, y, z) 
{
    switch (s) {
    case 0: return (x & y) ^ (~x & z);         
    case 1: return x ^ y ^ z;                  
    case 2: return (x & y) ^ (x & z) ^ (y & z);
    case 3: return x ^ y ^ z;                  
    }
}

function ROTL(x, n)
{
    return (x<<n) | (x>>>(32-n));
}

Number.prototype.toHexStr = function()
{
    var s="", v;
    for (var i=7; i>=0; i--) { v = (this>>>(i*4)) & 0xf; s += v.toString(16); }
    return s;
}

hexLookup = {
  "0": "0000",
  "1": "0001",
  "2": "0010",
  "3": "0011",
  "4": "0100",
  "5": "0101",
  "6": "0110",
  "7": "0111",
  "8": "1000",
  "9": "1001",
  "a": "1010",
  "b": "1011",
  "c": "1100",
  "d": "1101",
  "e": "1110",
  "f": "1111"
};


function hamming(sha1, sha2) {
  var dist = 0;
  for (var i = 0; i < 40; i++) {
    var b1 = hexLookup[sha1.charAt(i)];
    var b2 = hexLookup[sha2.charAt(i)];
    
    for (var ii = 0; ii < 4; ii++) {
      if (b1.charAt(ii) != b2.charAt(ii))
        dist++;
    }
  }
  
  return dist;
}

onmessage = function(e){
	var parts = e.data.split(" ");
	data = { sha: parts[0], words: parts[1].split(","), best: parts[2] };
	start();
};
