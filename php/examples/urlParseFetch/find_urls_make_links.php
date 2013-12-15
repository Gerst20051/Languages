<?php
// The Regular Expression filter
$reg_exUrl = "/(http|https|ftp|ftps)\:\/\/[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,3}(\/\S*)?/";

// The Text you want to filter for urls
$text = "The text you want to filter goes here. http://google.com";

// Check if there is a url in the text
if (preg_match($reg_exUrl, $text, $url)) {// make the urls hyper links
       // echo preg_replace($reg_exUrl, "<a href="{$url[0]}">{$url[0]}</a> ", $text);
       echo preg_replace($reg_exUrl, "<a href=" . $url[0] . ">{$url[0]}</a> ", $text);
} else {// if no urls in the text just return the text
       echo $text;
}
?>