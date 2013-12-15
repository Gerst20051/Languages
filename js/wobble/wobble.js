$.fn.center = function() {
	var w = $(window);
	this.css("position","absolute");
	this.css("top",(w.height() - this.height()) / 2 + w.scrollTop() + "px");
	this.css("left",(w.width() - this.width()) / 2 + w.scrollLeft() + "px");
	return this;
};

/*
$.fn.center = function() {
	var element = this;
	$(element).load(function() {
		updateCSS();
		$(window).bind("resize", function() { changeCSS(); });
		function updateCSS() {
			var dH = $(element).height();
			var dW = $(element).width();
			var wH = $(window).height();
			var wW = $(window).width();
			$(element).css({
				"position" : "absolute",
				"left" : wW / 2 - dW / 2,
				"top" : wH /2 - dH / 2
			});
		}; 
	});
};
*/

$.fn.wobble = function(options) {
	var defaults = {
		delay:0,
		speed:0,
		magnitude:0,
		duration:2000
	};

	var options = $.extend(defaults, options);
	alert(options.duration);
/*
	this.sX = startX;
	this.de = delay;
	this.sp = speed;

	xL = endX - startX;

	with (Math) {

	}

	this.start = start;
*/
/*
	if (Math.floor(hS - 1) != 0) {
		hS--;
		curX += xS * speed;
		curY += yS * speed; 
		ref.left = Math.round(curX);
		ref.top = Math.round(curY);
		setTimeout("doDynamicMovement(" + curX + ", " + curY + ", " + eX + ", " + eY + ", " + sp + ", " + xS + ", " + yS + ", ref, " + speed + ", " + hS + ")", sp);
	} else {
		setPos(eX, ref);
	}
*/
	return $(this).each(function() {
		$(this).css("background-color",options.color);
	});
};
 
function wobble(obj) {
	if (this == window) {
		return new wobble(obj);
	} else {
		this.obj = obj;
		this.reset();
		this.queue = [];
	}
}

wobble.prototype.reset = function() {
	alert(this.state);
	alert("reset");
	this.state = {
		attrs: {},
		duration: 2000
	}
};

wobble.prototype.stop = function() {
	alert("stop");
	this.reset();
	this.queue = [];
	return this;
};

wobble.prototype.duration = function(duration) {
	this.state.duration = duration ? duration : 0;
	return this;
};

wobble.prototype.hide = function() {
	this.state.hide = true;
	return this;
};

wobble.prototype.show = function() {
	this.state.show = true;
	return this;
};

wobble.prototype.ondone = function(fn) {
	this.state.ondone = fn;
	return this;
};

wobble.magnitude = function(p, v1, v2, whole) {
	return (whole ? parseInt : parseFloat)((v2 - v1) * p + v1, 10);
};

wobble.position = function(obj) {
	var pleft = parseInt(wobble.getstyle(obj, 'left'), 10), ptop = parseInt(wobble.getstyle(obj, 'top'), 10);
	return ;
};

wobble.getstyle = function(obj, prop) {
	var temp;
	return (window.getComputedStyle && window.getComputedStyle(obj, null).getPropertyValue(prop.replace(/[A-Z]/g, function(match) { return '-' + match.toLowerCase() }))) ||
	(document.defaultView && document.defaultView.getComputedStyle && (temp = document.defaultView.getComputedStyle(obj, null)) && temp.getPropertyValue(prop.replace(/[A-Z]/g, function(match) { return '-' + match.toLowerCase() }))) ||
	(obj.currentStyle && obj.currentStyle[prop]) ||
	obj.style[prop];
};

$(document).ready(function() {
$("div#obj").center();
$("div#obj").click(function() {
	$("div#obj").wobble(0,0,0,0);
});
})();