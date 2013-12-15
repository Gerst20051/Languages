<!--
function getCookie(name){
var cname = name + "=";               
var dc = document.cookie;             
    if (dc.length > 0) {              
    	begin = dc.indexOf(cname);       
        if (begin != -1) {           
        	begin += cname.length;       
        	end = dc.indexOf(";", begin);
            if (end == -1) end = dc.length;
            return unescape(dc.substring(begin, end));
        } 
    }
return null;
}
function setCookie(name, value, expires) {
	document.cookie = name + "=" + escape(value) + "; path=/" + ((expires == null) ? "" : "; expires=" + expires.toGMTString());
	window.location.reload(true);
}

function delCookie(name) {
	document.cookie = name + "=; expires=Thu, 01-Jan-70 00:00:01 GMT" +  "; path=/";
	window.location.reload(true);
}
// Open new windows external links
function externalLinks() {
	var foo = getCookie("externalWin");
	if (foo == null) {
 		if (!document.getElementsByTagName) return;
 		var anchors = document.getElementsByTagName("a");
 		for (var i=0; i<anchors.length; i++) {
   			var anchor = anchors[i];
   			if (anchor.getAttribute("href") &&
      	 		anchor.getAttribute("rel") == "external")
     			anchor.target = "_blank";
 		}
	}
}
window.onload = externalLinks;
// Save client new window settings
function setExternal(x) {
	var exp = new Date();
	exp.setTime(exp.getTime() + (1000 * 60 * 60 * 24 * 365));
	if (x.checked != false) {
		delCookie("externalWin");
	} else {
		setCookie("externalWin","no",exp);
	}
}

// Open new window
function openBrWindow(theURL,winName,features) {
	var ranWinName = Math.round(Math.random()*1000000);
  	window.open(theURL,winName+ranWinName,features);
}
function showemail(email) { 
  	var rstring = ''; 
  	for (i = 0; i <= email.length; i++) { 
    	rstring = email.charAt (i) + rstring; 
  	} 
  return rstring;
} 
var sendto = showemail('troppus') + '@' + showemail('etisnoitaminahsalftseb') + showemail('moc.');

// submit.php
function isSubmitReady(form) {
	if (form.LinkType.selectedIndex == 0) {
    	alert("Please select a submit option.");
    	form.LinkType.focus();
    	return false;
	}
}
// Submit validate CGI
function isReady(form) {
	var thisURL = form.URL.value;
	if (thisURL == "") {
    		alert("Please enter the sites URL.");
    		form.URL.focus();
    		return false;
	}
	if (thisURL == "http://") {
    		alert("Please enter the sites URL.");
    		form.URL.focus();
    		return false;
	}
	var startURL = thisURL.substring(0, 7);
	if (startURL != "http://") {
    		alert("Your sites URL appears to be malformed. Please include starting portion, eg. http://");
    		form.URL.focus();
    		return false;
	}

}
// Contact form
function isEmail(email) {
    	if (email.value.indexOf("@") + "" != "-1" &&
        email.value.indexOf(".") + "" != "-1" &&
        email.value != "") 
    	return true;
    	else return false;
}
function isContactReady(form) {
	if (form.Subject.selectedIndex == 0) {
    		alert("Please select a subject.");
    		form.Subject.focus();
    		return false;
	}
	if (form.Name.value == "") {
    		alert("Please enter your name.");
    		form.Name.focus();
    		return false;
	}
    	if (isEmail(form.Email) == false) { 
    		alert("Please enter your email address.");
    		form.Email.focus();
    		return false;
    	}
	if (form.Email.value != form.Email2.value) {
    		alert("Your email addresses don\'t match, please correct before submitting.");
    		form.Email.focus();
    		return false;
	}
	if (form.Comment.value == "") {
    		alert("Please enter a comment.");
    		form.Comment.focus();
    		return false;
	}
}
function checkEditBookmarkFolder(form) {
	if (form.my_folder_name.value == " ") {
    		alert("Please enter a folder name.");
    		form.my_folder_name.focus();
    		return false;
	}	
}
function isSignupReady(form) {
	if (form.Username.value == "") {
    		alert("Please enter a Username.");
    		form.Username.focus();
    		return false;
	}
	var whitespace = /\s/i;				//
	if (whitespace.test(form.Password.value) == true) {
		alert('Your password can not contain spaces. Please correct before submitting.'); 
		form.Password.focus();
		return false;
	}
	if (form.Password.value == "") {
    		alert("Please enter your account password.");
    		form.Password.focus();
    		return false;
	}
    	if (isEmail(form.Email) == false) { 
    		alert("Please enter your email address.");
    		form.Email.focus();
    		return false;
    	}
	if (form.Email.value != form.Email2.value) {
    		alert("Your email addresses don\'t match, please correct before submitting.");
    		form.Email.focus();
    		return false;
	}
	if (form.Name.value == "") {
    		alert("Please enter your full name, alias or company name.");
    		form.Name.focus();
    		return false;
	}
	var thisLink = form.Link.value;
	var startURL = thisLink.substring(0, 7);
	if (thisLink != "http://" && startURL != "http://") {
    		alert("Your sites URL appears to be malformed. Please include starting portion, eg. http://");
    		form.Link.focus();
    		return false;
	}
}
function isReviewReady(form) {
	if (form.Review_Subject.value == "") {
    	alert("Please enter a title for this post.");
    	form.Review_Subject.focus();
    	return false;
	}
	if (form.Review_Contents.value == "") {
    	alert("Please enter your comments.");
    	form.Review_Contents.focus();
    	return false;
	}
}

function print_r(array) {
	var output = "", pad_char = " ", pad_val = 4, d = this.window.document;
	var getFuncName = function (fn) {
		var name = (/\W*function\s+([\w\$]+)\s*\(/).exec(fn);
		if (!name) return '(Anonymous)';
		return name[1];
	};
	var repeat_char = function (len, pad_char) {
		var str = "";
		for (var i=0; i < len; i++) str += pad_char;
		return str;
	};
	var formatArray = function (obj, cur_depth, pad_val, pad_char) {
		if (cur_depth > 0) cur_depth++;
		var base_pad = repeat_char(pad_val*cur_depth, pad_char);
		var thick_pad = repeat_char(pad_val*(cur_depth+1), pad_char);
		var str = "";

		if (typeof obj === 'object' && obj !== null && obj.constructor && getFuncName(obj.constructor) !== 'PHPJS_Resource') {
			str += "Array\n" + base_pad + "(\n";
			for (var key in obj) {
				if (obj[key] instanceof Array) str += thick_pad + "[" + key + "] => " + formatArray(obj[key], (cur_depth + 1), pad_val, pad_char);
				else str += thick_pad + "[" + key + "] => " + obj[key] + "\n";
			}
			str += base_pad + ")\n";
		} else if (obj === null || obj === undefined) str = ''; else str = obj.toString();
		return str;
	};
	return formatArray(array, 0, pad_val, pad_char);
}

-->