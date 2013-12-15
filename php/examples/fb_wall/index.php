<html>
<head>
<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.6/jquery.min.js"></script>
</head>
<body>
<form id="submit_wall">
<label for="message_wall">Share your message on the Wall</label>
<input type="text" id="message_wall" />
<button type="submit">Post to wall</button>
</form>
<ul id="wall">
</ul>
<script type="text/javascript">
$(document).ready(function(){
$("form#submit_wall").submit(function() {
	var message_wall = $('#message_wall').attr('value');
	$.ajax({
		type: "POST",
		url: "insert.php",
		data:"message_wall="+message_wall,
		success: function(){
			$("ul#wall").prepend("<li style='display: none;'>"+message_wall+"</li>");
			$("ul#wall li:first").fadeIn();
		}
	});
	return false;
});
});
</script>
</body>
</html>