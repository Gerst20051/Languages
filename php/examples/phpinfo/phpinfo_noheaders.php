<?php
ob_start();
phpinfo();
$info = ob_get_clean ();

$matches = array ();
$i = preg_match ('%(<style type="text/css">.*</style>).*<body>(.*)</body>%s', $info, $matches);

print $matches [1]; # Style information
print $matches [2]; # Body
?>