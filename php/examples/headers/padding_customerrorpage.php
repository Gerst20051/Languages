<?php 
// set your custom error header --- example --- header('HTTP/1.1 503 Service Unavailable'); 

function padding($html){ 
return ($padding=1024-ob_get_length()) > 0 ? str_replace('</body>','<!--'. ($padding>8?str_repeat(' ',$padding-8) :null ).'-->'."\n".'</body>',$html) : $html; 
} 

ob_start('padding'); 
?> 