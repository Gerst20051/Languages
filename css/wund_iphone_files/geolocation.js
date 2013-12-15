/*
getGeolocation
Mobile Safari on iPhone can access the GPS location. Lets go to that city forecast page
Weather Underground, Inc.
*/ 
function getGeolocation() {
	// Get the current location
	if (navigator && navigator.geolocation) {
		navigator.geolocation.getCurrentPosition(showPosition);
	}

	function showPosition(position) {
		var lat = position.coords.latitude;
		var lon = position.coords.longitude;
		var url = "/cgi-bin/findweather/getForecast?brand=iphone&query="+lat+","+lon;
		window.location = url;
	}
}
