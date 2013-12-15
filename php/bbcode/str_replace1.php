<?php
$content = $_POST['content']; 

$bbcode = array(":)", ":(" , ":D", ":p"); // separate with commas. 

$newbbcode = array("<img src=\"./smile.gif\">", "<img src=\"./frown.gif\">", "<img src=\"./big-smile.gif\">", "<img src=\"./tongue.gif\">"); 

$content =  str_replace($bbcode, $newbbcode, $content); 

echo $content; 
?>