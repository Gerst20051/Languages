var char_set = '$%^NOZ1&PQR(./~`"CDEFG!@STUVWZghij}:<pHB*#8uvwx>?[]\',ef34590qrklmnoIJ)_+{st67 abcdAyzKLM2Y';

function encrypt() {
	if (arguments.length > 0) {
		var i = arguments[0]; if (arguments[1]) a = arguments[1]; else a = mt_rand(1, 9); a++;
		var o = "", ccode, alen = (cset.length - a), s;
		for (l = 0; l < i.length; l++) {
			if (cset.indexOf(i.charAt(l)) == -1) alert("Program Error: Unknown Character!");
			ccode = cset.indexOf(i.charAt(l));
			if (ccode + a > cset.length) { s = (cset.length - ccode); ccode = (a - s); } else ccode += a;
			o += cset.charAt(ccode);
		}
		return o;
	}
}

function decrypt(i,a) {
	var o = "", ccode, alen = (cset.length - a), s; a++;
	for (l = 0; l < i.length; l++) {
		if (cset.indexOf(i.charAt(l)) == -1) alert("Program Error: Unknown Character!");
		ccode = cset.indexOf(i.charAt(l));
		if (ccode - a < 0) { s = (a - ccode); ccode = (cset.length - s); } else ccode -= a;
		o += cset.charAt(ccode);
	}
	return o;
}