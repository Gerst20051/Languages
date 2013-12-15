var now = new Date();
var offset = now.getTimezoneOffset();
document.forms[0].elements["txtClientUTC"].value = offset;