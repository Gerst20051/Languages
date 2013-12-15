<?php
// Here's a small function to decode JSON. It might not work on all data, but it works fine on something like this: 
/*
$json_data = '{"response": { 
    "Text":"Hello there" 
 }, 
 "Details": null, "Status": 200} 
 '; 
*/

if ( !function_exists('json_decode') ){ 
function json_decode($json) 
{  
    $comment = false; 
    $out = '$x='; 
    
    for ($i=0; $i<strlen($json); $i++) 
    { 
        if (!$comment) 
        { 
            if ($json[$i] == '{')        $out .= ' array('; 
            else if ($json[$i] == '}')    $out .= ')'; 
            else if ($json[$i] == ':')    $out .= '=>'; 
            else                         $out .= $json[$i];            
        } 
        else $out .= $json[$i]; 
        if ($json[$i] == '"')    $comment = !$comment; 
    } 
    eval($out . ';'); 
    return $x; 
}  
}
?>