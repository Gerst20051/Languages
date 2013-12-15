<?php
// eval(gzinflate(base64_decode('encoded text')));
echo "1. Reading coded.txt\n";
$fp1 = fopen("coded.txt", "r");
$contents = fread($fp1, filesize("coded.txt"));
fclose($fp1);
echo "2. Decoding\n";
while(preg_match("/eval\(gzinflate/",$contents)) {
	$contents=preg_replace("/< \?|\?>/","",$contents);
	eval(preg_replace("/eval/","\$contents=",$contents));
}
echo "3. Writing decoded.txt\n";
$fp2 = fopen("decoded.txt","w");
fwrite($fp2, trim($contents));
fclose($fp2);
/*
1.) Save the code you wish to decode in coded.txt
2.) Save the above PHP snippet in file like decrypt.php
3.) Create an empty text file called decoded.txt and it CHMOD it to 0666.
4.) Run decrypt.php and open up decoded.txt to view the source code.
*/
?>