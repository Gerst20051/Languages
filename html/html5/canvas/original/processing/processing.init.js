if (window.addEventListener) {
	window.addEventListener("load", function() {	
	var scripts = document.getElementsByTagName("script");

		for (var i = 0; i < scripts.length; i++) {
			if (scripts[i].type == "application/processing") {
				var src = scripts[i].src, canvas = scripts[i].nextSibling;

				if (src && src.indexOf("#")) {
					canvas = document.getElementById(src.substr(src.indexOf("#") + 1));
				} else {
					while (canvas&&canvas.nodeName.toUpperCase() != "CANVAS")
						canvas = canvas.nextSibling;
				}

				if (canvas) {
					Processing(canvas,scripts[i].text);
				}

			}
		}
	}, false);
}