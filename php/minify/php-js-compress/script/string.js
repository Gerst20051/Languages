// JavaScript string handling library
var lib = lib || {};

lib.String = function() {

	var reTrim = /^\s*|\s*$/g;

	// string trim
	function Trim(str) {
		return String(str).replace(reTrim, "");
	}
	
	// public functions
	return {
		Trim: Trim
	};

}();