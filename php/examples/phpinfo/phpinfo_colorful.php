<?php

ob_start();
phpinfo();
$phpinfo = ob_get_contents();
ob_end_clean();

preg_match_all('/#[0-9a-fA-F]{6}/', $phpinfo, $rawmatches);
for ($i = 0; $i < count($rawmatches[0]); $i++)
   $matches[] = $rawmatches[0][$i];
$matches = array_unique($matches);

$hexvalue = '0123456789abcdef';

$j = 0;
foreach ($matches as $match)
{

   $r = '#';
   $searches[$j] = $match;
   for ($i = 0; $i < 6; $i++)
      $r .= substr($hexvalue, mt_rand(0, 15), 1);
   $replacements[$j++] = $r;
   unset($r);
}

for ($i = 0; $i < count($searches); $i++)
   $phpinfo = str_replace($searches, $replacements, $phpinfo);
echo $phpinfo;
?>