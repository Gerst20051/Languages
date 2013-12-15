var panel = function() {
this.id = id;
this.border = border;
this.html = html;
this.region = region;
};

var window = function (title,id,closable,draggable,resizable,minHeight,height,minWidth,width,items) {
this.title = title;
this.id = id;
this.closable = closable;
this.draggable = draggable;
this.resizable = resizable;
this.minHeight = minHeight;
this.height = height;
this.minWidth = minWidth;
this.width = width;
this.items = items;
};

/*
function title() {
alert(title);
alert(this.title);
return this.title;
}

var wind = new window(hey);
*/

var login = function() {

function createBox(t, s) {
return ['<div class="msg">',
'<div class="x-box-tl"><div class="x-box-tr"><div class="x-box-tc"></div></div></div>',
'<div class="x-box-ml"><div class="x-box-mr"><div class="x-box-mc"><h3>', t, '</h3>', s, '</div></div></div>',
'<div class="x-box-bl"><div class="x-box-br"><div class="x-box-bc"></div></div></div>',
'</div>'].join('');
};

var panel = new panel({
id: 'login-logo',
border: false,
html: '<center><h2>HnS Desktop</h2></center><br />',
region: 'center'
});

var panel2 = new panel({
id: 'login-logo',
border: false,
html: '<center><h2>HnS Desktop</h2></center><br />',
region: 'center'
});

var win = new window({
title: 'Login',
id: 'login-win',
closable: false,
draggable: true,
resizable: false,
minHeight: 250,
height: 250,
minWidth: 430,
width: 430,
items: [
panel,
panel2
]
});

document.write(win.title);
}

// ---------------------------------------------------------------------------


function Window(w,h,t,l,p,bg) {
// define object properties
this.div = document.createElement('div');
this.w = w + 'px';
this.h = h + 'px';
this.pos = 'absolute';
this.top = t + 'px';
this.left = l + 'px';
this.pad = p + 'px';
this.bord = '1px solid #000';
this.bg = '#' + bg;

// define 'display()' method
this.display = function() {
this.div.style.width = this.w;
this.div.style.height = this.h;
this.div.style.position = this.pos;
this.div.style.top = this.top;
this.div.style.left = this.left;
this.div.style.padding = this.pad;
this.div.style.border = this.bord;
this.div.style.background = this.bg;
document.getElementsByTagName('body')[0].appendChild(this.div);
}
}

Window.prototype.margin = '2px';

var win1 = new Window(300,250,100,100,5,'00f');
win1.display();
alert(win.margin); // returns ‘2px’

var win2 = new Window(200,150,50,50,5,'0ff');
win2.display();
alert(div2.margin); // returns ‘2px’