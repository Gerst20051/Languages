<? 
$length = 6; // Must be a multiple of 2
$conso = array("b","c","d","f","g","h","j","k","l","m","n","p","r","s","t","v","w","x","y","z");
$vocal = array("a","e","i","o","u");
$password = "";
srand((double) microtime() *1 000000);
$max = $length / 2;
for($i=1; $i <= $max; $i++) {
$password .= $conso[rand(0,19)];
$password .= $vocal[rand(0,4)];
}
$newpass = $password;
echo $newpass . "<p>";
?>