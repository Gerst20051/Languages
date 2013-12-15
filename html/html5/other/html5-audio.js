/* 
 * html5-audio.js - JavaScript Kantan Library for Audio
 * Copyright (c) 2009 Yusuke Kawasaki http://www.kawa.net/
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

//  global

if ( ! window.HTML5 ) HTML5 = {};

if ( ! HTML5.Audio ) {
    HTML5.Audio = function ( url ) {
        this.proxy = HTML5.Audio.Proxy.getProxy();
        this.create( url );
    };
    HTML5.Audio.prototype = new JKL.JS2AS.Item();
    HTML5.Audio.prototype.play = function () {
        return this.call( 'play' );
    }
    HTML5.Audio.prototype.pause = function () {
        return this.call( 'pause' );
    }
}

// proxy

if ( ! HTML5.Audio.Proxy ) {
    HTML5.Audio.Proxy = function () {
        if ( HTML5.Audio.Proxy._instance && JKL.Console.error ) {
            JKL.Console.error( 'HTML5.Audio.Proxy is a singleton.' );
        }
        return this;
    };
    HTML5.Audio.Proxy.prototype = new JKL.JS2AS();
    HTML5.Audio.Proxy.prototype.swfPath     = '../swf/html5-audio.swf';
    HTML5.Audio.Proxy.prototype.installPath = '../swf/expressInstall.swf';
    HTML5.Audio.Proxy.prototype.className   = 'HTML5_Audio';

    // singleton

    HTML5.Audio.Proxy._instance;
    HTML5.Audio.Proxy.getProxy = function (param) {
        var proxy = HTML5.Audio.Proxy._instance;
        if ( ! proxy ) {
            proxy = new HTML5.Audio.Proxy();
            proxy.init(param);
            HTML5.Audio.Proxy._instance = proxy;
        }
        return proxy;
    }
}