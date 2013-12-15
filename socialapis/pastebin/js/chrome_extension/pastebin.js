function savePaste(paste) {
	localStorage["paste"] = paste;
}
function clearPaste() {
	localStorage["paste"] = "";
}
function loadPaste() {
	return localStorage["paste"];
}
function clearResponse() {
	localStorage["API_RESPONSE"] = "";
}
function getUsername() {
	return localStorage["USER_NAME"];
}
function setUsername(username) {
	localStorage["USER_NAME"] = username;
}
function submitPaste(paste, private, name, expire, format) {
	console.log('submit paste?');
	var http = new XMLHttpRequest();
	console.log(localStorage["API_DEV_KEY"]);
	var params = "api_option=paste&api_user_key=" +localStorage["API_USER_KEY"]+ "&api_dev_key=" +localStorage["API_DEV_KEY"]+ "&api_paste_private=" +private+ "&api_paste_name=" +escape(name)+ "&api_paste_expire_date=" +expire+ "&api_paste_format=" +format+ "&api_paste_code=" +encodeURIComponent(paste);
	http.open("POST", localStorage["API_URL_POST"], true);

	http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	http.setRequestHeader("Content-length", params.length);
	http.setRequestHeader("Connection", "close");
	
	http.send(params);
	
	http.onreadystatechange = function() {//Call a function when the state changes.
		if(http.readyState == 4 && http.status == 200) {
			localStorage["API_RESPONSE"] = http.responseText;
			console.log("got a response! " +http.responseText);
		}
		else {
			localStorage["API_RESPONSE"] = "error";
			console.log("got a response!" +http.responseText);
		}
	}
	console.log('outside readystate');
}
function login(name, password) {
	var http = new XMLHttpRequest();
	var params = "api_dev_key=" +localStorage["API_DEV_KEY"]+ "&api_user_name=" +escape(name)+ "&api_user_password=" +escape(password);
	http.open("POST", localStorage["API_URL_LOGIN"], true);

	http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	http.setRequestHeader("Content-length", params.length);
	http.setRequestHeader("Connection", "close");
	
	http.send(params);
	
	http.onreadystatechange = function() {//Call a function when the state changes.
		if(http.readyState == 4 && http.status == 200) {
			if (http.responseText.substring(0,15) == "Bad API request") {
				//error handling
				logout();
			}
			else {
				localStorage["API_USER_KEY"] = http.responseText;
			}
		}
	}
}
function logout() {
	localStorage["API_USER_KEY"] = "";
	localStorage["USER_NAME"] = "";
}
