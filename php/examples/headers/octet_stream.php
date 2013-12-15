<?php 
header("Content-Type: application/octet-stream"); 
header("Content-Disposition: attachment; filename=\"".$filename."\";"); 
?>
<?php 
header("Pragma: public"); 
      header("Expires: 0"); 
      header("Cache-Control: must-revalidate, post-check=0, pre-check=0"); 
      header("Cache-Control: private",false); 
      header ( "Content-Type: $filedatatype" ); 
      header("Content-Disposition: attachment; filename=\"".$FileObj->name."\";"); 
      header("Content-Transfer-Encoding:­ binary"); 
      header("Content-Length: ".$filesize); 
  readfile($file); 
exit; 
?> 