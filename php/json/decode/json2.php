<?php

// For those of you wanting json_decode to be a little more lenient (more like Javascript), here is a wrapper:

function json_decode_nice($json, $assoc = FALSE){
    $json = str_replace(array("\n","\r"),"",$json);
    $json = preg_replace('/([{,])(\s*)([^"]+?)\s*:/','$1"$3":',$json);
    return json_decode($json,$assoc);
}

// Some examples of accepted syntax:

$json = '{a:{b:"c",d:["e","f",0]}}';
$json = 
'{
   a : {
      b : "c",
      "d.e.f": "g"
   }
}';

// If your content needs to have newlines, do this:

$string = "This
Text
Has
Newlines";
$json = '{withnewlines:'.json_encode($string).'}';

// Note: This does not fix trailing commas or single quotes.

?>