// jQuery

function handleClick(){
	$("li", this).click(loadMenu);
}

function loadMenu(){
	$(this).load("menu.html", handleClick);
}

$(document).ready(function(){
	$(document).each(handleClick);
});


// Pure DOM (Ol' Fashion)

function handleClick(elem){
	var li = elem.getElementsByTagName("li");
	for (var i = 0; i < li.length; i++)
		li[i].addEventListener("click", loadMenu, false);
		// ^ doesn't exist in IE (attachEvent)
}

function loadMenu(){ // IE sets the wrong context
	var elem = this;
	var xhr = new XMLHttpRequest();
	// ^ doesn't exist in IE (ActiveXObject("Microsoft.XMLHTTP"))
	xhr.open("GET", "menu.html", false);
	xhr.onreadystatechange = function(){ // leaks in IE
		if (xhr.readyState == 4) {
			elem.innerHTML = xhr.responseText;
			handleClick(elem);
		}
	};
	xhr.send(null);
}

window.onload = function(){ // doesn't happen immediately
	var ul = document.getElementsByTagName("ul");
	for (var i = 0; i < ul.length; i++)
		handleClick(ul[i]);
}