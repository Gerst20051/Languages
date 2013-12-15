<?php

header("content-type: text/xml");

$xmlData = "";
$xmlData .= "<store>\n";

$xmlData .= " <item>\n";
$xmlData .= "  <name>Book</name>\n";
$xmlData .= "  <section>Learning</section>\n";
$xmlData .= "  <price>19.95</price>\n";
$xmlData .= "  <inStock>yes</inStock>\n";
$xmlData .= " </item>\n";

$xmlData .= " <item>\n";
$xmlData .= "  <name>Football</name>\n";
$xmlData .= "  <section>Sports</section>\n";
$xmlData .= "  <price>4.99</price>\n";
$xmlData .= "  <inStock>no</inStock>\n";
$xmlData .= " </item>\n";

$xmlData .= " <item>\n";
$xmlData .= "  <name>Bike</name>\n";
$xmlData .= "  <section>Sports</section>\n";
$xmlData .= "  <price>89.95</price>\n";
$xmlData .= "  <inStock>yes</inStock>\n";
$xmlData .= " </item>\n";

// add more item's here

$xmlData .= "</store>\n";

print "response=" . $xmlData;

?>