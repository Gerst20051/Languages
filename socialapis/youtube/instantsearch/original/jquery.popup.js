function LoadPopup( ){
	// get screen height/width
	var maskHeight = $(document).height( );
	var maskWidth = $(window).width( );

	// set mask to height/width of screen
	$('#mask').css({'width': maskWidth, 'height': maskHeight});
	
	// transition effect		
	$('#mask').hide().fadeIn(1500).fadeTo("fast", 0.75);	

	// get window height/width
	var windowHeight = $(window).height();
	var windowWidth = $(window).width();
	
	// center the popup
	$('.window').css('top',  (windowHeight/2-$('.window').height()/2)+$(window).scrollTop());
	$('.window').css('left', windowWidth/2-$('.window').width()/2);

	// transition effect
	$('.window').hide().fadeIn(1000);
}

function DoPopup( video ){// create Popup window
	var source = "http://www.youtube.com/v/" + video;
	var video_code =
				"<object width=\"480\" height=\"385\">"
			+	"<param name=\"movie\" value=\"" + source + "&amp;hl=en_US\"><\/param>"
			+	"<param name=\"allowFullScreen\" value=\"true\"><\/param>"
			+	"<param name=\"allowscriptaccess\" value=\"always\"><\/param>"
			+	"<embed src=\"" + source + "&amp;hl=en_US\" type=\"application/x-shockwave-flash\" "
				+	"allowscriptaccess=\"always\" "
				+	"allowfullscreen=\"true\" " 
				+	"width=\"480\" height=\"385\">"
			+	"<\/embed>"
			+	"<\/object>";

	$("#dialog").html( video_code );
	LoadPopup( );
}