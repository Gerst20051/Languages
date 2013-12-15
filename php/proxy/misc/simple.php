<?php 
$returned=URLopen("http://facebook.com/"); 
function URLopen($url) {
        // Fake the browser type 
        ini_set('user_agent','Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US) AppleWebKit/525.13 (KHTML, like Gecko) Chrome/0.A.B.C Safari/525.13');
        $dh = fopen("$url",'r');
        $result = fread($dh,8192);                                                                                                                             
        return $result;
}
echo $returned;
?>