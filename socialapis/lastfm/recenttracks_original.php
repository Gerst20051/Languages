<?php
//PHP Last Fm Recent Tracks Script 1.0
//Author: SoulSmasher ©2009
//web: http://www.soulsmasher.net
 
//Enter your last.fm username
$username="your_lastfm_user";
//Amount of data to be shown (not more than 10)
$amount=5;
 
 
// Do not edit anything below unless you know what you're doing
function lastfm_recenttracks($lastfm_username,$amount=5) {
	$list=file("http://ws.audioscrobbler.com/1.0/user/$lastfm_username/recenttracks.txt");
	$i=0;
	$pieces=array();
	$temp=array();
	while ($i<$amount) {
		//each piece's pieces array structure: 0=>$timestamp, 1=>$artist, 2=>$title
		$pieces[$i]=explode(",",$list[$i]);
		//now $pieces[$i][0] is our timestamp of current song, $pieces[$i][1] are other parts of the song info
		$temp[$i][0]=$pieces[$i][1];
		//I saved the other part of the track info (track, artist) to $temp[$i][0] for later to use substr. Because some songs may have " - " someting like "Oh God - Please Help me!"
		$temp[$i][1]=explode("---",preg_replace('/[^0-9a-z\s\`\~\!\@\#\$\%\^\*\(\)\; \,\.\'\/\_\-]/i', '-',$pieces[$i][1])); //file() messes up with the " - ", don't know. Tried on 2 different servers, both same. it changes it to some special char which hardens the job. Instead, I changed them to - char and join them to use with explode
		$pieces[$i][1]=trim($temp[$i][1][0]);
		//now $pieces[$i][1] is artist
		$pieces[$i][2]=substr($temp[$i][0],(strlen($pieces[$i][1])+4)); //4 , because " - " this makes 3 chars, and a file() shows - as a false character, and it makes one extra character, that's why it's 3+1=4
		//$pieces[$i][2] is now title
 
		//echo "timestamp: ".$pieces[$i][0]."<br>";
		//echo "artist: ".$pieces[$i][1]."<br>";
		//echo "title: ".$pieces[$i][2]."<hr>";
		$i++;
	}
 
for($j=0;$j<$i;$j++) { unset($temp[$j][0],$temp[$j][1]); } //for memory cleanup
 
return $pieces; //either returns the array, or uncomment echo statements and change this to return true; for echoing. I prefer returning the array
}
 
$lastsongs=lastfm_recenttracks($username,$amount);
foreach ($lastsongs as $thesongs) {
	echo "Timestamp: ".$thesongs[0]."<br>";
	echo "Artist: ".$thesongs[1]."<br>";
	echo "Title: ".$thesongs[2]."<hr>";
}
?>
