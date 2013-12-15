window.onload = function(){
var num = document.getElementById("num");
var best = document.getElementById("mybest");

document.getElementById("allbest").innerHTML = data.best;

var rates = [], pings = 0, mybest = 160;

for ( var i = 0; i < 4; i++ ) (function(i){
	rates[i] = 0;

	// Build a worker
	var worker = new Worker("worker.js");

	// Listen for incoming messages
	worker.onmessage = function(e){
		var parts = e.data.split(" ");

		// We're getting the rate at which computations are done
		if ( parts[0] === "rate" ) {
			rates[i] = parseInt(parts[1]);

			// Total the rates from all the workers
			var total = 0;
			for ( var j = 0; j < rates.length; j++ ) {
				total += rates[j];
			}
			num.innerHTML = total;

		// We've found a new best score, send it to the server
		} else if ( parts[0] === "found" ) {
			var img = document.createElement("img");
			img.src = "http://www.raycmorgan.com/new-best?phrase=" +
				escape(parts.slice(1).join(" "));
			document.body.appendChild( img );

		// A new personal best score was found
		} else if ( parts[0] === "mybest" ) {
			var tmp = parseInt(parts[1]);
			if ( tmp < mybest ) {
				mybest = tmp;
				best.innerHTML = mybest;
			}
		}
	};

	// Start the worker
	worker.postMessage( data.sha + " " + data.words.join(",") + " " + data.best );
})(i);
};
