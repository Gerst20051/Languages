<?php 
ob_start();                                                                                                        
phpinfo();                                                                                                         
$info = ob_get_contents();                                                                                         
ob_end_clean();                                                                                                    

$info = preg_replace('%^.*<body>(.*)</body>.*$%ms', '$1', $info); 
?> 