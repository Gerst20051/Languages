<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>OOP in PHP</title>
<?php include("class_lib.php"); ?>
</head>
<body>
<?php
$stefan = new person("Stefan Mischook");
echo "Stefan's full name: " . $stefan->get_name();
$james2 = new employee("Johnny Fingers");
echo " ---> " . $james2->get_name();
/*
$andrew = new person();
$andrew->set_name("Andrew");
echo "Full name: " . $andrew->get_name();
$gerst = new person;
$gerst->set_name("Gerst");
echo "Full name: " . $gerst->get_name();
$stefan2 = new employee("Stefan Mischook2");
echo " ---> " . $stefan2->get_name();
$james = new employee("Jimmy Two Guns");
echo " ---> " . $james->get_name();
$james2 = new employee("Johnny Fingers");
echo " ---> " . $james2->get_name();
$jimmy = new employee("Stefan Mischook");
echo " ---> " . $jimmy->get_name();
$jimmy2 = new employee("Jimmy Two Guns");
echo " ---> " . $jimmy2->get_name();
*/
?>
</body>
</html>