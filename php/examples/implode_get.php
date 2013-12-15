<?php
function implode_get() {
    $first = true;
    $output = '';
    foreach($_GET as $key => $value) {
        if ($first) {
            $output = '?'.$key.'='.$value;
            $first = false;
        } else {
            $output .= '&'.$key.'='.$value;    
        }
    }
    return $output;
}

echo '<form action='.$_SERVER['PHP_SELF'].implode_get()' method=post>';
?>