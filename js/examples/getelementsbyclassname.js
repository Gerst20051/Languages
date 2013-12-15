window.onload = function() {
	if (!document.getElementsByClassName) {
		document.getElementsByClassName = function(classname) {
			var elArray = [];
			var tmp = document.getElementsByTagName("*");
			var regex = new RegExp("(^|\\s)" + classname + "(\\s|$)");
			for (var i = 0; i < tmp.length; i++) { if (regex.test(tmp[i].className)) elArray.push(tmp[i]); }
			return elArray;
		};
	}

	var el = document.getElementsByClassName("para");
	for (var i = 0; i < el.length; i++) el[i].style.color = "red";
};