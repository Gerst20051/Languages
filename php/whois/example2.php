<?php
include("whois/main.whois");
$whois = new Whois($_GET['query']);
$result = $whois->Lookup();
echo "<pre>";
for($i=0;$i<count($result[rawdata]);$i++)
{
$row = each($result[rawdata]);
echo $row["value"]."\n";
}
echo "</pre>";
?>