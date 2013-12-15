var TID = 0;
var longer = 0;
var thirdwrite = "";
var alphabet = new Array("0@","1#","2$","3%","4^","5&","6*","7 = ","8-","9+","Aa","Bb","Cc","Dd","Ee","Ff","Gg","Hh","Ii","Jj","Kk","Ll","Mm","Nn","Oo","Pp","Qq","Rr","Ss","Tt","Uu","Vv","Ww","Xx","Yy","Zz");

function preTime(coco, uinput, question) {
longer = coco.length - 1;
placement = 0;
ref = 0 ;
secondWrite = "";
firstWrite = "";
thirdWrite = "";
code1(coco, uinput, longer, question);
}

function findshift(letter) {
for (i = 0; i < 36; i++) {
if ((alphabet[i].substring(0,1) == letter) || (alphabet[i].substring(1,2) == letter)) {
if (alphabet[i].substring(0,1) == letter) {
x = 0;
}
if (alphabet[i].substring(1,2) == letter) {
x = 1;
}
return i;
      }
   }
}

function check(letter) {
bill = "no";
for (i = 0; i < 36; i++) {
if ((alphabet[i].substring(0,1) == letter) || (alphabet[i].substring(1,2) == letter)) {
bill = "yes";
   }
}
if (bill == "yes") {
return true;
}
else {
return false;
   }
}

function fixoffset(takin) {
if (takin > 35) {
return (takin - 36);
}
else {
return takin;
   }
}

function code1(inval1,inval2,inval3,sage) {
if (check(inval2.charAt(placement))) {
if (check(inval1.charAt(ref))) {
if (sage == 0) { 
shift = findshift(inval1.charAt(ref));
}
if (sage == 1) { 
shift = 36 - findshift(inval1.charAt(ref));
   }
}
else {
shift = 0;
}
orig = findshift(inval2.charAt(placement));
firstWrite = firstWrite+alphabet[fixoffset(orig+shift)].charAt(x);
}
else {
firstWrite = firstWrite+inval2.charAt(placement);
}
secondWrite = inval2.substring(placement+1,inval2.length+1);
thirdWrite = firstWrite+secondWrite;
document.inputo.written.value = thirdWrite;
placement++;
if (ref == inval3) {
ref = 0
}
else {
ref++;
}
if (placement == (inval2.length)) {
window.clearTimeout(TID);
return false;
}
one = inval1;
two = inval2;
three = inval3;
goer = sage;
TID = window.setTimeout("code1(one, two, three, goer)", 0);
}

// settings
var codeWord = "code phrase here";
var written = "Type message to encode here";

// encode
preTime(codeWord,written, 0);

// decode
preTime(codeWord,written, 1);