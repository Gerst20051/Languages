if (typeof addEvent != 'function') {
var addEvent = function(o,t,f,l) {
var d = 'addEventListener',n = 'on' + t,rO = o,rT = t,rF = f,rL = l;
if (o[d]&&!l) return o[d](t,f,false);
if (!o._evts)o._evts = {};
if (!o._evts[t]) {
o._evts[t] = o[n] ? {b:o[n]} : {};
o[n] = new Function('e','var r = true,o = this,a = o._evts["' + t + '"],i;for (i in a) {o._f = a[i];r = o._f(e||window.event)!= false&&r;o._f = null}return r');
if (t != 'unload') addEvent(window,'unload',function() {
removeEvent(rO,rT,rF,rL)
})}

if (!f._i)f._i = addEvent._i++;
o._evts[t][f._i] = f
};

addEvent._i = 1;

var removeEvent = function(o,t,f,l) {
var d = 'removeEventListener';
if (o[d] && !l) return o[d](t,f,false);
if (o._evts && o._evts[t] && f._i) delete o._evts[t][f._i]}}

function cancelEvent(e,c) {
e.returnValue = false;

if (e.preventDefault)e.preventDefault();
if (c) {
e.cancelBubble = true;

if (e.stopPropagation)e.stopPropagation()}};
function DragResize(myName,config) {
var props = {
myName:myName,
enabled:true,
handles:['tl','tm','tr','ml','mr','bl','bm','br'],
isElement:null,
isHandle:null,
element:null,
handle:null,
minWidth:10,
minHeight:10,
minLeft:0,
maxLeft:9999,
minTop:0,
maxTop:9999,
zIndex:1,
mouseX:0,
mouseY:0,
lastMouseX:0,
lastMouseY:0,
mOffX:0,
mOffY:0,
elmX:0,
elmY:0,
elmW:0,
elmH:0,
allowBlur:true,
ondragfocus:null,
ondragstart:null,
ondragmove:null,
ondragend:null,
ondragblur:null
};

for (var p in props)
this[p] = (typeof config[p] == 'undefined') ? props[p] : config[p]
};

DragResize.prototype.apply = function(node) {
var obj = this;

addEvent(node,'mousedown',function(e) {
obj.mouseDown(e)
});

addEvent(node,'mousemove',function(e) {
obj.mouseMove(e)
});

addEvent(node,'mouseup',function(e) {
obj.mouseUp(e)
})
};

DragResize.prototype.select = function(newElement) {
with(this) {
if (!document.getElementById || !enabled) return;

if (newElement && (newElement != element) && enabled) {
element = newElement;
element.style.zIndex = ++zIndex;

if (this.resizeHandleSet) this.resizeHandleSet(element,true);
elmX = parseInt(element.style.left);
elmY = parseInt(element.style.top);
elmW = element.offsetWidth;
elmH = element.offsetHeight;
if (ondragfocus) this.ondragfocus()
}}};

DragResize.prototype.deselect = function(delHandles) {
with(this) {
if (!document.getElementById || !enabled) return;
if (delHandles) {
if (ondragblur) this.ondragblur();
if (this.resizeHandleSet) this.resizeHandleSet(element,false);
element = null
}

handle = null;
mOffX = 0;
mOffY = 0
}};

DragResize.prototype.mouseDown = function(e) {
with(this) {
if (!document.getElementById || !enabled) return true;
var elm = e.target || e.srcElement,newElement = null,newHandle = null,hRE = new RegExp(myName + '-([trmbl]{2})','');
while(elm) {
if (elm.className) {
if (!newHandle && (hRE.test(elm.className) || isHandle(elm))) newHandle = elm;
if (isElement(elm)) {
newElement = elm;
break
}}
elm = elm.parentNode
}

if (element && (element != newElement) && allowBlur)deselect(true);
if (newElement && (!element || (newElement == element))) {
if (newHandle)cancelEvent(e);
select(newElement,newHandle);
handle = newHandle;

if (handle && ondragstart) this.ondragstart(hRE.test(handle.className))
}}};

DragResize.prototype.mouseMove = function(e) {
with(this) {
if (!document.getElementById || !enabled) return true;
mouseX = e.pageX || (e.clientX + document.documentElement.scrollLeft);
mouseY = e.pageY || (e.clientY + document.documentElement.scrollTop);
var diffX = (mouseX - lastMouseX + mOffX);
var diffY = (mouseY - lastMouseY + mOffY);
mOffX = mOffY = 0;
lastMouseX = mouseX;
lastMouseY = mouseY;
if (!handle) return true;
var isResize = false;
if (this.resizeHandleDrag && this.resizeHandleDrag(diffX,diffY)) {
isResize = true
} else {
var dX = diffX,dY = diffY;
if ((elmX + dX) < minLeft) mOffX = (dX - (diffX = minLeft-elmX));
else if ((elmX + elmW + dX) > maxLeft) mOffX = (dX - (diffX = (maxLeft - elmX - elmW)));
if ((elmY + dY) < minTop) mOffY = (dY - (diffY = (minTop - elmY)));
else if ((elmY + elmH + dY) > maxTop) mOffY = (dY - (diffY = (maxTop - elmY - elmH)));
elmX += diffX;
elmY += diffY
}

with(element.style) {
left = elmX + 'px';
width = elmW + 'px';
top = elmY + 'px';
height = elmH + 'px'
}

$('div.panel-bwrap').height($('div.panel').height() - 34);

if (window.opera && document.documentElement) {
var oDF = document.getElementById('op-drag-fix');

if (!oDF) {
var oDF = document.createElement('input');
oDF.id = 'op-drag-fix';
oDF.style.display = 'none';
document.body.appendChild(oDF)
}
oDF.focus()
}

if (ondragmove) this.ondragmove(isResize);
cancelEvent(e)
}};

DragResize.prototype.mouseUp = function(e) {
with(this) {
if (!document.getElementById || !enabled) return;
var hRE = new RegExp(myName + '-([trmbl]{2})','');
if (handle && ondragend) this.ondragend(hRE.test(handle.className));
deselect(false)
}};

DragResize.prototype.resizeHandleSet = function(elm,show) {
with(this) {
if (!elm._handle_tr) {
for (var h = 0; h < handles.length; h++) {
var hDiv = document.createElement('div');
hDiv.className = myName + ' ' + myName + '-' + handles[h];
elm['_handle_' + handles[h]] = elm.appendChild(hDiv)
}}

for (var h = 0; h < handles.length; h++) {
elm['_handle_' + handles[h]].style.visibility = show ? 'inherit' : 'hidden'
}}};

DragResize.prototype.resizeHandleDrag = function(diffX,diffY) {
with(this) {
var hClass = handle && handle.className && handle.className.match(new RegExp(myName + '-([tmblr]{2})')) ? RegExp.$1 : '';
var dY = diffY,dX = diffX,processed = false;
if (hClass.indexOf('t') >= 0) {
rs = 1;

if ((elmH - dY) < minHeight) mOffY = (dY - (diffY = elmH - minHeight));
else if ((elmY + dY) < minTop) mOffY = (dY - (diffY = minTop - elmY));
elmY += diffY;
elmH -= diffY;
processed = true
}

if (hClass.indexOf('b') >= 0) {
rs = 1;
if ((elmH + dY) < minHeight) mOffY = (dY - (diffY = (minHeight - elmH)));
else if ((elmY + elmH + dY) > maxTop) mOffY = (dY - (diffY = (maxTop - elmY - elmH)));
elmH += diffY;
processed = true
}

if (hClass.indexOf('l') >= 0) {
rs = 1;

if ((elmW - dX) < minWidth) mOffX = (dX - (diffX = (elmW - minWidth)));
else if ((elmX + dX) < minLeft) mOffX = (dX - (diffX = (minLeft - elmX)));
elmX += diffX;
elmW -= diffX;
processed = true
}

if (hClass.indexOf('r')>= 0) {
rs = 1;
if (elmW + dX < minWidth) mOffX = (dX - (diffX = minWidth - elmW));
else if ((elmX + elmW + dX) > maxLeft) mOffX = (dX - (diffX = maxLeft - elmX - elmW));
elmW += diffX;
processed = true
}
return processed
}};

// Using DragResize is simple!
// You first declare a new DragResize() object, passing its own name and an object
// whose keys constitute optional parameters/settings:

var dragresize = new DragResize('dragresize', { minWidth: 150, minHeight: 150, minLeft: 0, minTop: 0, maxLeft: (window.innerWidth), maxTop: (window.innerHeight - 30) });

// Optional settings/properties of the DragResize object are:
//  enabled: Toggle whether the object is active.
//  handles[]: An array of drag handles to use (see the .JS file).
//  minWidth, minHeight: Minimum size to which elements are resized (in pixels).
//  minLeft, maxLeft, minTop, maxTop: Bounding box (in pixels).

// Next, you must define two functions, isElement and isHandle. These are passed
// a given DOM element, and must "return true" if the element in question is a
// draggable element or draggable handle. Here, I'm checking for the CSS classname
// of the elements, but you have have any combination of conditions you like:

dragresize.isElement = function(elm) {
if (elm.className && elm.className.indexOf('drsElement') > -1) return true;
};

dragresize.isHandle = function(elm) {
if (elm.className && elm.className.indexOf('drsMoveHandle') > -1) return true;
};

// You can define optional functions that are called as elements are dragged/resized.
// Some are passed true if the source event was a resize, or false if it's a drag.
// The focus/blur events are called as handles are added/removed from an object,
// and the others are called as users drag, move and release the object's handles.
// You might use these to examine the properties of the DragResize object to sync
// other page elements, etc.

dragresize.ondragfocus = function() { };
dragresize.ondragstart = function(isResize) { };
dragresize.ondragmove = function(isResize) { };
dragresize.ondragend = function(isResize) { };
dragresize.ondragblur = function() { };

// Finally, you must apply() your DragResize object to a DOM node; all children of this
// node will then be made draggable. Here, I'm applying to the entire document.
dragresize.apply(document);