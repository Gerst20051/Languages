<?php
// PAGE TITLE

$html = file_get_contents("http://www.nashruddin.com");
 
preg_match("/<title>(.+)<\/title>/siU", $html, $t);
$title = $t[1];
?>
<?php
// META TAGS

$re = "<meta\s+name=['\"]??(.+)['\"]??\s+content=['\"]??(.+)['\"]??\s*\/?>";
preg_match_all("/$re/siU", $html, $m);
$meta = array_combine($m[1], $m[2]);
 
print_r($meta);
/*
outputs something like this:
Array
(
    [keywords] => PHP scripts, PHP classes, PHP programming, code snippets
    [description] => Free PHP scripts, classes & code snippets
    [robots] => index, follow
    [author] => Nashruddin Amin
)
*/
?>
<?php
// LINKS

$re = "<a\s[^>]*href\s*=\s*(['\"]??)([^'\" >]*?)\\1[^>]*>(.*)<\/a>";
preg_match_all("/$re/siU", $html, $m);
$links = $m[2];
 
print_r($links);    
?>