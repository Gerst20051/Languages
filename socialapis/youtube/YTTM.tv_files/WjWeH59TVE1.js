/*1299044032,169776320*/

if (window.CavalryLogger) { CavalryLogger.start_js(["b11Ej"]); }

var PlatformOptInPopup=function(){};copy_properties(PlatformOptInPopup,{DIALOG_URL:'/connect/uiserver.php',DIALOG_WIDTH:420,DIALOG_HEIGHT:450,APP_ID:127760087237610,open:function(d,c,a){if(!d)d='generic';if(!c)c='opt.in';var b=new URI(PlatformOptInPopup.DIALOG_URL);b.addQueryData({social_plugin:d,method:c,display:'popup',app_id:PlatformOptInPopup.APP_ID});if(a)b.addQueryData(a);return PopupResizer.open(b.toString(),PlatformOptInPopup.DIALOG_WIDTH,PlatformOptInPopup.DIALOG_HEIGHT);}});

if (window.Bootloader) { Bootloader.done(["b11Ej"]); }