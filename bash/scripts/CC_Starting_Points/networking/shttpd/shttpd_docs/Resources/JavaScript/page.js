/*
    page.js
    TechPubPerlModules
    
    JavaScript library for use by Developer Publications.
    
    Copyright (c) 2009 Apple Inc. All Rights Reserved.

*/


// JSLint Configuration
/*jslint plusplus: false, browser: true */
/*global window location escape unescape localStorage self */

// prototype.js
/*global $, $$, Template, Prototype, Hash, Ajax */

// scriptaculous.js
/*global Effect */

// pedia.js
/*global Pedia */

Prototype.Browser.Xcode = navigator.userAgent.indexOf('Xcode/') > -1;

var Book = {
    root: $('toc-file') ? $('toc-file').content.replace(/toc\.html/, "") : "",
    language: "en"
};

var DevPubs = {
    hideInXcode: function () {
        if (Prototype.Browser.Xcode) {
            var hideInXcode = $$('.hideInXcode'),
                showInXcode = $$('.showInXcode'),
                i;
                
            for (i = 0; i < hideInXcode.length; i++) {
                hideInXcode[i].hide();
            }
            for (i = 0; i < showInXcode.length; i++) {
                showInXcode[i].show();
            }
        }
        return Prototype.Browser.Xcode;
    },
    reassignRelativeLinks: function (linklist) {
        for (var i = 0; i < linklist.length; i++) {
            linklist[i].href = Book.root + linklist[i].getAttribute("href");
        }
    },
    Window: function (id) {
        var box = document.createElement('div'),
            header = document.createElement('div'),
            contents = document.createElement('div'),
            footer = document.createElement('div');
        box.setAttribute('id', id + "Window");
        header.setAttribute('id', id + "Header");
        contents.setAttribute('id', id + "Body");
        footer.setAttribute('id', id + "Footer");
        box.appendChild(header);
        box.appendChild(contents);
        box.appendChild(footer);
        box.style.display = "none";
        document.body.appendChild(box);
        this.box = $(id + "Window");
        this.header = $(id + "Header");
        this.contents = $(id + "Body");
        this.footer = $(id + "Footer");
    }
};

////////////////// JS DOCSET PROTOCOL

function ContentFrameName() {
    return "content";
}

/////////////////// COOKIE HANDLING

function get_absolute_path(url) {
    var loc = window.location.pathname; 
    loc = loc.substring(0, loc.lastIndexOf('/'));
    while (/^\.\./.test(url)) {
        loc = loc.substring(0, loc.lastIndexOf('/'));
        url = url.substring(3);
    }
    if (url) {
        return loc + "/" + url;
    } else {
        return loc;
    }
}

function toc_cookie_path() {
    var cookiePath,
        tocMetaElt = document.getElementById("toc-file");
    if (!tocMetaElt) {
        tocMetaElt = parent.document.getElementById("toc-file");
    }
    if (tocMetaElt) {
        cookiePath = get_absolute_path(tocMetaElt.content);
    }
    return cookiePath;
}

function set_toc_cookie(cookieName, cookieValue) {
    var cookiePath = toc_cookie_path(),
        expiry;
    if (cookiePath) {
        expiry = new Date();
        expiry.setFullYear(expiry.getFullYear() + 1);
        document.cookie = cookieName + "=" + escape(cookieValue) +
            "; expires=" + expiry.toGMTString() + 
            "; path=" + window.location.pathname;
    }
}

function get_toc_cookie(cookie_name) {
    var results = document.cookie.match('(^|;) ?' + cookie_name + '=([^;]*)(;|$)');
    if (results) {
        return (unescape(results[2]));
    } else {
        return null;
    }
}

function delete_toc_cookie(cookie_name) {
    var cookie_date = new Date();
    cookie_date.setTime(cookie_date.getTime() - 1);
    document.cookie = cookie_name += "=;path=" + window.location.pathname + ";expires=" + cookie_date.toGMTString();
}

///////////////////////////////////////

function initialize_index() {

    var frameURL = document.getElementById("refresh").content.split("URL=")[1],
        xcodeVersion,
        versionRegexp = new RegExp("Xcode\/([0-9.]+)"),
        xcodeVersionArray = versionRegexp.exec(navigator.appVersion);
    
    if (xcodeVersionArray) {
        xcodeVersion = parseFloat(xcodeVersionArray[1]);
    }
    
    if (xcodeVersion && xcodeVersion < 1000.0 && !parent.frames.content) {
        window.webKitWorkaround = document.getElementsByName(location.hash.substr(1))[0];
        document.write('<iframe width="100%" height="100%" align="left" frameborder="0" id="content" name="content" src="' + frameURL + '">This document set is best viewed in a browser that supports iFrames.</iframe>');
        document.body.style.margin = "0";
        return;
    }
}

// cross-browser function to get an object's style object given its
function getStyleObject(objectId) {
    if (document.getElementById && document.getElementById(objectId)) {
        // W3C DOM
        return document.getElementById(objectId).style;
    } else if (document.all && document.all(objectId)) {
        // MSIE 4 DOM
        return document.all(objectId).style;
    } else if (document.layers && document.layers[objectId]) {
        // NN 4 DOM.. note: this won't find nested layers
        return document.layers[objectId];
    } else {
        return false;
    }
}

var state = 'block';
var margin = 210;
// shows or hides the TOC as well as toggles the show/hide text (2 per page)
function showHideTOC(forceState) {
    var tocDiv     = getStyleObject("tocMenu"),
        bodyDiv    = getStyleObject("bodyText"),
        showHideTOCUpperSpan = document.getElementById("showHideTOCUpperSpan"),
        showHideTOCLowerSpan = document.getElementById("showHideTOCLowerSpan"),
        upperAnchor = showHideTOCUpperSpan.getElementsByTagName('a').item(1),
        lowerAnchor = showHideTOCLowerSpan.getElementsByTagName('a').item(1),
        showText = showHideTOCUpperSpan.getElementsByTagName('img').item(0).getAttribute('showText'),
        hideText = showHideTOCUpperSpan.getElementsByTagName('img').item(0).getAttribute('hideText');

    if (state === 'block' || forceState === 'hide') {
        //hide
        state = 'none'; 
        margin = 10;
        upperAnchor.innerHTML = showText;
        lowerAnchor.innerHTML = showText;
        set_toc_cookie("tocHideState", "hide");
    } else {
        //show
        state = 'block'; 
        margin = 210;
        upperAnchor.innerHTML = hideText;
        lowerAnchor.innerHTML = hideText;
        delete_toc_cookie("tocHideState");
    }
    
    tocDiv.display = state;
    if (document.layers) {
        bodyDiv.marginLeft = margin;
    } else {
        bodyDiv.marginLeft = margin + "px";
    }
    
    return false;
}

function initialize_page() {
    
    var xcodeVersion,
        versionRegexp = new RegExp("Xcode\/([0-9.]+)"),
        xcodeVersionArray = versionRegexp.exec(navigator.appVersion),
        tocFileElt,
        filePath,
        tocElt,
        newDiv;

    if (xcodeVersionArray) {
        xcodeVersion = parseFloat(xcodeVersionArray[1]);
    }
       
    if (xcodeVersion && xcodeVersion < 1000.0 && !parent.frames.content) {
        window.webKitWorkaround = document.getElementsByName(location.hash.substr(1))[0];
        document.body.innerHTML = '<iframe width="100%" height="100%" align="left" frameborder="0" id="content" name="content" src="' + self.location + '">This document set is best viewed in a browser that supports iFrames.</iframe>';
        return;
    }
    
    // dynamically load TOC
    tocFileElt = document.getElementById("toc-file");
    if (tocFileElt) {        
        filePath = tocFileElt.content;
        tocElt = document.createElement("div");
        tocElt.id = "tocMenu";
        tocElt.innerHTML = '<iframe id="toc_content" name="toc_content" SRC="' + filePath + '" width="210" height="100%" align="left" frameborder="0">This document set is best viewed in a browser that supports iFrames.</iframe>';
        document.body.appendChild(tocElt);
    }
    
    // create tooltip
    newDiv = document.createElement('div');
    newDiv.setAttribute('id', 'tooltip');
    document.body.appendChild(newDiv);
    
    // determine TOC cookie state and set TOC appropriately
    if (get_toc_cookie("tocHideState") && get_toc_cookie("tocHideState") === 'hide') {
        showHideTOC('hide');
    }

    //Tell pedia it is ok to load
    Pedia.init();
}

function announce_page_loaded() {

    // If we're in a frameset, tell the TOC frame this page was loaded, so it can track it.
    if (document.getElementById && document.getElementById('toc_content')) {
        document.getElementById('toc_content').contentWindow.page_loaded(document.location);
    } else if (frames.length) {
        frames.toc_content.page_loaded(document.location);
    }
}

function showtip(hovered, event) {
// Makes the "tooltip" element visible and moves it to the 
    // (x,y) of the mouse event (plus some buffer zone)
    
    var agent = navigator.userAgent,
        abstract_text,
        mouse_event,
        tooltip,
        xcoord = 0,
        ycoord = 0;
        
    if (agent.indexOf("MSIE") > 0 && agent.indexOf("Mac") > 0) { 
        // IE-Mac no longer supported, and the CSS functionality is not up to the par needed for this
        return;
    }
    
    abstract_text = hovered.getElementsByTagName('img').item(0).getAttribute('abstract');
    if (!abstract_text) { 
        return; 
    } 
    
    // Event-handling code for cross-browser support
    if (!event) { 
        mouse_event = window.event;
    } else { 
        mouse_event = event;
    }
    
    tooltip = document.getElementById("tooltip");
    tooltip.innerHTML = abstract_text;
    
    tooltip.style.backgroundColor = "#FDFEC8";
    

    
    if (mouse_event.pageX || mouse_event.pageY) {
        xcoord = event.pageX;
        ycoord = event.pageY;
    } else if (mouse_event.clientX || mouse_event.clientY) {
        xcoord = mouse_event.clientX + (document.documentElement.scrollLeft ?  document.documentElement.scrollLeft : document.body.scrollLeft);
        ycoord = mouse_event.clientY;
    }
    
    tooltip.style.left = xcoord + 4 + "px";
    tooltip.style.top = ycoord + 10 + "px";
    tooltip.style.visibility = "visible";
}

function hidetip() {
    var tooltip = getStyleObject("tooltip");
    tooltip.visibility = "hidden";
}

function placeWatermark() {
    if (document.layers) {
        document.watermark.pageX = (window.innerWidth - document.watermark.document.myImage.width) / 2;
        document.watermark.pageY = (window.innerHeight - document.watermark.document.myImage.height) / 2;
        document.watermark.visibility = 'visible';
    }
}

function closeWatermark() {
    var watermark = getStyleObject("watermark");
    watermark.visibility = "hidden";
}

document.observe("dom:loaded", function () {
    DevPubs.hideInXcode();
});
