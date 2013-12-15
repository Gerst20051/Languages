<?php
function redirect($url) {
if (!headers_sent()) {
header('location: http://' . $_SERVER['HTTP_HOST'] . dirname($_SERVER['PHP_SELF']) . '/' . $url);
} else {
die('Could not redirect; Headers already sent (output).');
}
}
?>