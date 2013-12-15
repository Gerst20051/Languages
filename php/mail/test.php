<?php
$from_name = "Justrealized";
$from_email = "outgoing@justrealized.com";
$headers = "From: $from_name <$from_email>";
$body = "Hi, \nThis is a test mail from $from_name <$from_email>.";
$subject = "Test mail from Justrealized";
$to = "hnsalerts@gmail.com";
if (mail($to, $subject, $body, $headers)) {
	echo "success!";
} else {
	echo "fail…";
}
?>