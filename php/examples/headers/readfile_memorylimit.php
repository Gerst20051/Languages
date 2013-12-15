<?php
$filename = "theDownloadedFileIsCalledThis.mp3";
$myFile = "/absolute/path/to/my/file.mp3";

$mm_type="application/octet-stream";

header("Cache-Control: public, must-revalidate");
header("Pragma: hack"); // WTF? oh well, it works...
header("Content-Type: " . $mm_type);
header("Content-Length: " .(string)(filesize($myFile)) );
header('Content-Disposition: attachment; filename="'.$filename.'"');
header("Content-Transfer-Encoding: binary\n");

readfile($myFile);

?>