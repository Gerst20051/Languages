<?php 
$data = file_get_contents('http://search.msn.com/results.aspx?q=site%3Afroogle.com');
$regex = '/Page 1 of (.+?) results/';
preg_match($regex,$data,$match);
var_dump($match);
echo $match[1];
?>