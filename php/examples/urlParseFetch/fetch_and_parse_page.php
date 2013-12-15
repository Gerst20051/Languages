<?php
function fetch_and_parse_page($url) {
	$html = file_get_contents($url);
 
	/* get page's title */
	preg_match("/<title>(.+)<\/title>/siU", $html, $matches);
	$title = $matches[1];
 
	/* get page's keywords */
	$re="<meta\s+name=['\"]??keywords['\"]??\s+content=['\"]??(.+)['\"]??\s*\/?>";
	preg_match("/$re/siU", $html, $matches);
	$keywords = $matches[1];
 
	/* get page's description */
	$re="<meta\s+name=['\"]??description['\"]??\s+content=['\"]??(.+)['\"]??\s*\/?>";
	preg_match("/$re/siU", $html, $matches);
	$desc = $matches[1];
 
	/* parse links */
	$re="<a\s[^>]*href\s*=\s*(['\"]??)([^'\">]*?)\\1[^>]*>(.*)<\/a>";
	preg_match_all("/$re/siU", $html, $matches);
	$links = $matches[2];
 
	$info = array(
		"url" => $url,
		"title" => $title,
		"keywords" => $keywords,
		"description" => $desc,
		"md5" => md5($html),
		"links" => array_unique($links)
	);
	return($info);
}
?>