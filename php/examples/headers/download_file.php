<?php
$mm_type="application/octet-stream";

header("Cache-Control: public, must-revalidate");
header("Pragma: hack");
header("Content-Type: " . $mm_type);
header("Content-Length: " .(string)(filesize($fullpath)) );
header('Content-Disposition: attachment; filename="'.$filename.'"');
header("Content-Transfer-Encoding: binary\n");
                  
readfile($fullpath);
?>