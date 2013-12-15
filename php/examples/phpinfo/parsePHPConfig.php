<?php 
function parsePHPConfig() { 
    ob_start(); 
    phpinfo(-1); 
    $s = ob_get_contents(); 
    ob_end_clean(); 
    $a = $mtc = array(); 
    if (preg_match_all('/<tr><td class="e">(.*?)<\/td><td class="v">(.*?)<\/td>(:?<td class="v">(.*?)<\/td>)?<\/tr>/',$s,$mtc,PREG_SET_ORDER)) 
        foreach($mtc as $v){ 
            if($v[2] == '<i>no value</i>') continue; 
            $a[$v[1]] = $v[2]; 
        } 
    } 
    return $a; 
} 
?>