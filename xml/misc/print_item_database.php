<?php

header("content-type: text/xml");

$query = "SELECT * FROM store WHERE inStock=yes";

$link = mysql_connect("localhost", "user", "pass");
$result = mysql_query($query, $link);

$xmlData = "";
$xmlData .= "<store>\n";

while ($row = mysql_fetch_array($result))
{
	$xmlData .= " <item>\n";
	$xmlData .= "  <name>Book</name>\n";
	$xmlData .= "  <section>Learning</section>\n";
	$xmlData .= "  <price>19.95</price>\n";
	$xmlData .= "  <inStock>yes</inStock>\n";
	$xmlData .= " </item>\n";
}

$xmlData .= "</store>\n";

print "response=" . $xmlData;

?>