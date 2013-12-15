<?php 
$filename = $_SERVER['DOCUMENT_ROOT'] . "/path/to/file/my_file.pdf"; 
header("Cache-Control: public"); 
header("Content-Description: File Transfer"); 
header('Content-disposition: attachment; filename='.basename($filename)); 
header("Content-Type: application/pdf"); 
header("Content-Transfer-Encoding: binary"); 
header('Content-Length: '. filesize($filename)); 
readfile($filename); 
?>