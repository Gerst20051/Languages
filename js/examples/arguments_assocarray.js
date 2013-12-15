function person(options) {
	var default_args = {
		'name'	:	"Binny V A",
		'me'		:	true,
		'site'		:	"http://www.bin-co.com/",
		'species'	:	"Homo Sapien"
	}

	for (var index in default_args) {
		if (typeof options[index] == "undefined") options[index] = default_args[index];
	}
	
	/* options[] has all the data - user provided and optional */
	for(var i in options) {
		alert(i + " = " + options[i]);
	}
}

person({
	'name'	:	"George W. Bush",
	'me'		:	false,
	'site'		:	"miserable failure"
});