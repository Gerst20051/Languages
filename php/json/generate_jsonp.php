<?php
function generate_jsonp($data) {
  if (preg_match('/\W/', $_GET['callback'])) {
    // if $_GET['callback'] contains a non-word character,
    // this could be an XSS attack.
    header('HTTP/1.1 400 Bad Request');
    exit();
  }
  header('Content-type: application/javascript; charset=utf-8');
  print sprintf('%s(%s);', $_GET['callback'], json_encode($data));
}
?>