//Made by Joshua Lückers < http://joshualuckers.nl >
var DEFAULT_SYNTAX = "text";
var DEFAULT_EXPIRATION = "N";
var DEFAULT_EXPOSURE = 0;
var DEFAULT_NAME = "";
var DEFAULT_POSTED_OPTION = "show";
var CURRENT_VERSION = 2.1;
var UPDATED_STORAGE = false;

function setDefaults() {
	setSyntax(DEFAULT_SYNTAX);
	setExpiration(DEFAULT_EXPIRATION);
	setExposure(DEFAULT_EXPOSURE);
	setName(DEFAULT_NAME);
	setPostedOption(DEFAULT_POSTED_OPTION);
}

function setNewOptions() {
	setEmail(DEFAULT_EMAIL);
	localStorage['version'] = CURRENT_VERSION;
}

function setSyntax(syntax) {
	localStorage["syntax"] = syntax;
}
function getSyntax() {
	return localStorage["syntax"];
}

function setExpiration(expiration) {
	localStorage["expiration"] = expiration;
}
function getExpiration() {
	return localStorage["expiration"];
}

function setExposure(exposure) {
	localStorage["exposure"] = exposure;
}
function getExposure() {
	return parseInt(localStorage["exposure"]);
}

function setName(name) {
	localStorage["name"] = name;
}
function getName() {
	return localStorage["name"];
}

function setPostedOption(option) {
	localStorage["posted_option"] = option;
}
function getPostedOption() {
	return localStorage["posted_option"];
}

function checkFirstRun() {
	if (!localStorage['firstrun']) {
		firstRun();
	}
	else if (!localStorage['version']) {
		setNewOptions();
	}
	else if (localStorage['version']  < CURRENT_VERSION && UPDATED_STORAGE == true) {
		setNewOptions();
	}
}
function firstRun() {
	setDefaults();
	localStorage['firstrun'] = true;
	localStorage['version'] = CURRENT_VERSION;
}
