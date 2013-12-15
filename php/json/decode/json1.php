<?php 
// If you have a json encoded array that contains non UTF8 chars, this function will do the trick:

$array = json_decode(safeJSON_chars($iso_8859_1_data)); 

function safeJSON_chars($data) { 

    $aux = str_split($data); 

    foreach($aux as $a) { 

        $a1 = urlencode($a); 

        $aa = explode("%", $a1); 

        foreach($aa as $v) { 

            if($v!="") { 

                if(hexdec($v)>127) { 

                $data = str_replace($a,"&#".hexdec($v).";",$data); 

                } 

            } 

        } 

    } 

    return $data; 

} 
?>