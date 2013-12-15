<?php
/*
Just few lines of code to convert in a clickable link with tag <a> 
the pattern "http://......." in a string: 
*/
$stringa = " bla bla bla http://www.example.com bla bla http://www.example.net bla bla bla"; 

$m = preg_match_all('/http:\/\/[a-z0-9A-Z.]+(?(?=[\/])(.*))/', $stringa, $match); 

if ($m) { 
    $links=$match[0]; 
    for ($j=0;$j<$m;$j++) { 
        $stringa=str_replace($links[$j],'<a href="'.$links[$j].'">'.$links[$j].'</a>',$stringa); 
    } 
} 

echo $stringa; 

?>