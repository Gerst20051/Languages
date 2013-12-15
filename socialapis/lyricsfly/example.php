<?php
$user_ID = "[user ID key]";
$artist = "[Artist]";
$title = "[Title]";
$song_number = 1;
$artist = urlencode(filterChar($artist));
$title = urlencode(filterChar($title));
$url = "http://lyricsfly.com/api/api.php?i=" . $user_ID . "&a=" . $artist . "&t=" . $title;
$html = file_get_contents($url);
$found_check = GetTag($html,"sg",$song_number);

if ($found_check == "Not found") {
	echo "Sorry, lyrics for this title are not available in lyricsfly database.<br>";
	echo "Please <a href='http://lyricsfly.com/submit/'>submit</a>";
} elseif ($found_check == "Wrong site ID!") {
	echo "Sorry, access to lyricsfly API has been terminated please contact the administrator";
} elseif ($found_check == "Missing site ID!" or $found_check==FALSE) {
	echo "Sorry, the API code was not properly implemented";
} else {
	$found_checksum = GetTag($html,"cs",$song_number);
	$found_ID = GetTag($html,"id",$song_number);
	$found_title = GetTag($html,"tt",$song_number);
	$found_artist = GetTag($html,"ar",$song_number);
	$found_album = GetTag($html,"al",$song_number);
	$found_lyrics = GetTag($html,"tx",$song_number);
	$found_lyrics = str_replace("[br]","<br>",$found_lyrics);
	$found_lyrics = str_replace("lyricsfly.com","<a href='http://lyricsfly.com'>lyricsfly.com</a>",$found_lyrics);
	echo "<b>Title: $found_title</b><br>";
	echo "<b>Artist: $found_artist</b><br>";
	echo "<b>Album: $found_album</b><br><br>";
	echo "$found_lyrics";
	echo "<a href='http://lyricsfly.com/search/correction.php?".$found_checksum."&id=".$found_ID."'>Fix above lyrics here</a><br><br>";
}

function GetTag($p,$t,$l) {
	if ($l == 0) { $l = 1; }
	$sp = 0;
	while ($l > 0) {
		$sp = strpos($p, "<".$t.">", $sp);
		$sp = $sp + strlen($t) + 2;
		$l--;
	}
	if ($sp-strlen($t)-2 == 0) {
		$GetTag = FALSE;
	} else {
		$ep = strpos($p, "</".$t.">", $sp);
		$ep = $ep-$sp;
		$GetTag = substr($p,$sp,$ep);
	}
	return $GetTag;
}
 
function filterChar($str) {
	$accept = "/[^a-z0-9\\)\\(\\$\\^\\*\\=\\:\\;\\,\\|\\#\\@\\}\\{\\]\\[\\!\\040\\.\\-\\_\\\\]/i";
	$str = preg_replace($accept,"%",$str);
	return $str;
}
?>