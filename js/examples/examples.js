
// ---------------------------------------------------------------------------------------

Login = function() {
var win, form, msgCt, submitUrl = 'login.pl';

function onSubmit() {
this.showMask();
form.submit({
reset: true
});
}

function createBox(t, s) {
return ['<div class="msg">',
'<div class="x-box-tl"><div class="x-box-tr"><div class="x-box-tc"></div></div></div>',
'<div class="x-box-ml"><div class="x-box-mr"><div class="x-box-mc"><h3>', t, '</h3>', s, '</div></div></div>',
'<div class="x-box-bl"><div class="x-box-br"><div class="x-box-bc"></div></div></div>',
'</div>'].join('');
}


return {
Init:function() {
Ext.QuickTips.init();

new Ext.KeyMap(document, [{
key: Ext.EventObject.BACKSPACE,
stopEvent: false,

fn: function(key, e) {
var t = e.target.tagName;

if ( !t )
return;
t = t.toLowerCase();

if (t != "input" && t != "textarea")
e.stopEvent();
}
}
]);

var logoPanel = new Ext.Panel({
// baseCls: 'x-plain',
id: 'login-logo',
border: false,
html: '<center><h2>Comet Desktop</h2>\
<br/>\
<table><tr>\
<td><div style="text-align:left;">\
Copyright (c) 2008 - <a href="http://xant.us/" target="_blank">David Davis</a><br/>\
&lt;<a href="mailto:xantus@xantus.org">xantus@xantus.org</a>&gt;\
<br/>\
<a href="http://code.google.com/p/cometdesktop/" target="_blank">Comet Desktop is Open Source!</a><br/>\
Join the <a href="http://groups.google.com/group/comet-desktop/" target="_blank">Comet Desktop Group</a>\
<br/>\
</div></td></tr></table></center>\
',
region: 'center'
});

var focusFirst = 'user';
var email = 'guest';
var pass = 'guest';
var lastUser = get_cookie('lastUsername');

if ( lastUser ) {
email = lastUser;

if ( lastUser != 'guest' ) {
pass = '';
focusFirst = 'pass';
}
}

var formPanel = new Ext.form.FormPanel({
baseCls: 'x-plain',

baseParams: {
module: 'login'
},

defaults: {
width: 250
},

defaultType: 'textfield',
frame: false,
height: 70,
id: 'login-form',

items: [{
fieldLabel: 'Username',
name: 'user',
value: email
},{
fieldLabel: 'Password',
inputType: 'password',
name: 'pass',
value: pass
},{
xtype: 'hidden',
name: 'sha1',
value: ''},{
xtype: 'hidden',
name: 'token',
value: ( window.loginToken ? window.loginToken : '' )
}],

labelWidth:65,

listeners: {
'actioncomplete': {
fn: this.onActionComplete,
scope: this
},

'actionfailed': {
fn: this.onActionFailed,
scope: this
}
},

region: 'south',
url: submitUrl
});

win = new Ext.Window({
iconCls: ( Ext.isSecure ? 'lock-icon' : '' ),
buttons: [{
handler: onSubmit,
scope: Login,
text: 'Login'
}],

buttonAlign: 'right',
closable: false,
draggable: true,
height: 250,
id: 'login-win',

keys: {
key: [13], // enter key
fn: onSubmit,
scope:this
},

layout: 'border',
minHeight: 250,
minWidth: 430,
plain: false,
resizable: false,

items: [
logoPanel,
formPanel
],

title: 'Login',
width: 430
});

form = formPanel.getForm();

form.on('beforeaction',function() {
var pass = form.findField('pass');
var token = form.findField('token').getValue().split( '~' )[ 1 ];
var sha = sha1( pass.getValue() );
form.findField('sha1').setValue( sha1( token + ':' + sha ) + ':' + sha );
/* send the hash, not the password */
pass.setValue('');
return true;
},this);

formPanel.on('render', function(){
var f = form.findField(focusFirst);

if (f)
f.focus.defer(100,f);
}, this, {delay: 200});

win.show();
},

hideMask : function(){
this.pMask.hide();
win.buttons[0].enable();
},

onActionComplete : function(f, a){
this.hideMask();
if(a && a.result){
// get the path
var path = window.location.pathname;
path = path.substring(0, path.lastIndexOf('/') + 1);

// delete older cookies without a domain
delete_cookie('sessionId', path );
delete_cookie('lastUsername', path );

set_cookie('lastUsername', a.result.userName || '', a.result.days || '', path, '.' + document.domain );

if ( a.result.success && a.result.success == 'true' ) {
// set the cookies
set_cookie('sessionId', a.result.sessionId, a.result.days || '', path, '.' + document.domain );
} else {
return this.onActionFailed(f,a);
}

win.destroy(true);

if ( a.result.nonSecure && Ext.isSecure ) {
// we're secure and the server requested non secure mode
path = window.location.toString();
path = path.substring(0, path.lastIndexOf('/') + 1);
window.location.href = path.replace( /^https:/, 'http:' );
} else
window.location = path;
}
},

onActionFailed : function(f, a){
this.hideMask();
if ( a && a.result && a.result.errors )
for ( var i = 0, len = a.result.errors.length; i < len; i++ )
if ( a.result.errors[i].msg )
this.msg('Error', a.result.errors[i].msg);

if ( a && a.result && a.result.reload )
window.location = window.location;

if ( a && a.result && a.result.redirect )
window.location = a.result.redirect;
},

showMask : function(msg) {
if (!this.pMask) {
// using this.pMask, seems that using this.mask caused conflict
// when this dialog is modal (uses this.mask also)
this.pMask = new Ext.LoadMask(win.body, {
msg: 'Please wait...'
});
}

if (msg) {
this.pMask.msg = msg;
}

this.pMask.show();
win.buttons[0].disable();
},

msg: function(title, format) {
if ( !msgCt )
msgCt = Ext.DomHelper.insertFirst(document.body, {id:'msg-div'}, true);
msgCt.alignTo(document, 'b-t');

var s = String.format.apply(String, Array.prototype.slice.call(arguments, 1));
var m = Ext.DomHelper.append(msgCt, {html:createBox(title, s)}, true);

m.slideIn('t').pause(2).ghost("t", {remove:true});
}
};
}();

Ext.onReady(Login.Init, Login, true);