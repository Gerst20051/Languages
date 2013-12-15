<?php

$content = "[url]http://www.google.com[/url] - [img]http://www.brwpages.com/images/logo.png[/img] = [url=http://www.brwpages.com]BRWPages[/url]";

$bbcode = array('/\[url\=(.*?)\](.*?)\[\/url\]/is', '/\[img](.*?)\[\/img\]/is', '/\[url](.*?)\[\/url\]/is');

$newbbcode = array('<a href="$1">$2</a>', '<img src="$1">', '<a href="$1">$1</a>');

$content = preg_replace($bbcode, $newbbcode, $content);

echo $content;

?>