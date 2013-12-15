$(document).ready( function() { 
	$('a.social_links').hover(
		function() {
			$(this).find('span.hover').show();            
			$(this).find('span.normal').hide();            
		},
		function() {
			$(this).find('span.hover').hide();            
			$(this).find('span.normal').show();            
		}
    );

});
