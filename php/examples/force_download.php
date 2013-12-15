
<!-- saved from url=(0061)http://www.hashbangcode.com/examples/forcedownload/force.phps -->
<html><head><meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1"></head><body><pre style="word-wrap: break-word; white-space: pre-wrap;">&lt;?php
$file = 'hashbangcode.png';
if ( file_exists($file) ) {
	$ext = substr($file, -3);
	if ( $ext=='jpg' || $ext=='gif' || $ext=='png' || $ext=='pdf' ) {
		header('Content-type: octet/stream');
		header('Content-disposition: attachment; filename='.$file.';');
		header('Content-Length: '.filesize($file));
		readfile($file);
		exit;
	}
} else {
	echo 'File not found!';
}
?&gt;</pre></body></html>