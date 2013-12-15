<?php
//******** setings ***************************************************************
$user_ID=			"[user ID key]";																										//Set your lyricsfly.com User ID key
$artist=			"[Artist]";																													//Set the artist you want to find lyrics for
$title=				"[Title]";																													//Set the title for the song you want to find lyrics to
$song_number=	1;																																	//Set to 1 if you want the first result or 2 if you want 2nd result and so on... 
//******** setings ***************************************************************

$artist=	urlencode(filterChar($artist));																					//filter and encode for url string
$title=		urlencode(filterChar($title));                                          //filter and encode for url string

$url="http://lyricsfly.com/api/api.php?i=".$user_ID."&amp;a=".$artist."&amp;t=".$title;   //url construction string with pluged in from above values
$html=file_get_contents($url);																										//retrieve the entire xml page into string

$found_check=			GetTag($html,"sg",$song_number);																//get song data
if($found_check=="Not found")																											//check if song was found
	{
	echo "Sorry, lyrics for this title are not available in lyricsfly database.&lt;br&gt;";	//... if not print a message
	echo "Please &lt;a href='http://lyricsfly.com/submit/'&gt;submit&lt;/a&gt;";								//and print link to submit form
	}
	elseif($found_check=="Wrong site ID!")																					//check if site ID is good
		{
		echo "Sorry, access to lyricsfly API has been terminated please contact the administrator";	//check if user ID access key is valid. Incase we terminate your site ID for any reason your users will see this message. You may optionally put a contact email so that you get notified by someone.
		}
		elseif($found_check=="Missing site ID!" or $found_check==FALSE)								//check if user ID access key is missing and if the number of song titles is not passed the amount which was returned
			{
			echo "Sorry, the API code was not properly implemented";										//this is mostly for you. You should not see this message once your code is functioning properly.
			}
			else																																				//finally, if everything check on we retrieve and print the data returned by lyricsfly database
				{		
				$found_checksum=	GetTag($html,"cs",$song_number);												//get song checksum
				$found_ID=				GetTag($html,"id",$song_number);												//get song ID
				$found_title=			GetTag($html,"tt",$song_number);												//get song title
				$found_artist=		GetTag($html,"ar",$song_number);												//get song artist
				$found_album=			GetTag($html,"al",$song_number);												//get song album
				$found_lyrics=		GetTag($html,"tx",$song_number);												//get song lyrics
				$found_lyrics=str_replace("[br]","&lt;br&gt;",$found_lyrics);										//replace all "[br]" with "&lt;br&gt;" for html page																																									
				$found_lyrics=str_replace("lyricsfly.com","&lt;a href='http://lyricsfly.com'&gt;lyricsfly.com&lt;/a&gt;",$found_lyrics);	//add html for lyricsfly.com link back								
	
				echo "&lt;b&gt;Title: $found_title&lt;/b&gt;&lt;br&gt;";																		//print the title
				echo "&lt;b&gt;Artist: $found_artist&lt;/b&gt;&lt;br&gt;";																	//print artist name
				echo "&lt;b&gt;Album: $found_album&lt;/b&gt;&lt;br&gt;&lt;br&gt;";																//print album name																																									
				echo "$found_lyrics";																											//print lyrics
				echo "&lt;a href='http://lyricsfly.com/search/correction.php?".$found_checksum."&amp;id=".$found_ID."'&gt;Fix above lyrics here&lt;/a&gt;&lt;br&gt;&lt;br&gt;";	//create a link to correction form for users to be able to correct lyrics
	    	}
																														

function GetTag($p,$t,$l)
	//GetTag function:
	//p=page,t=tag,l=line,sp=StartPos,ep=EndPos
	//Usage: 
	//$result=GetTag([link to page], [tag to get], [tag number to get]);
	{
	if($l==0)
		{
		$l=1;
		}
	$sp=0;
	while($l&gt;0)
		{
		$sp=strpos($p, "&lt;".$t."&gt;", $sp);
		$sp=$sp+strlen($t)+2;
		$l--;
		}
	if($sp-strlen($t)-2==0)
		{
		$GetTag=FALSE;
		}
		else
			{
			$ep=strpos($p, "&lt;/".$t."&gt;", $sp);
			$ep=$ep-$sp;
			$GetTag=substr($p,$sp,$ep);
			}
	return $GetTag;
	}

function filterChar($str)
	{
	//accept: letters, digits, spaces and )($^*=:;|#@}{][!.,-_\
	//replace all others with %
	$accept="/[^a-z0-9\\)\\(\\$\\^\\*\\=\\:\\;\\,\\|\\#\\@\\}\\{\\]\\[\\!\\040\\.\\-\\_\\\\]/i";
	$str=preg_replace($accept,"%",$str);
	return $str;
	};
?>