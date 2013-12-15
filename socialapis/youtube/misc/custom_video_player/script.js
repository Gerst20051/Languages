$(document).ready(function(){

	$('#player').youTubeEmbed("http://www.youtube.com/watch?v=0NcJ_63z-mA")
				.youTubeEmbed('http://www.youtube.com/watch?v=quwebVjAEJA');

	$('form').submit(function(){
		$('#player').youTubeEmbed($('#url').val());
		$('#url').val('');
		return false;
	});

});
