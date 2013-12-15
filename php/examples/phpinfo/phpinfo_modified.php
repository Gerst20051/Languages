<?php

// Get PHP INFO
ob_start();
phpinfo();
$phpinfo = ob_get_contents();
ob_end_clean();
$phpinfo = preg_replace ('/<\/div><\/body><\/html>/', '', $phpinfo);

//HR
$hr = '<div style="width:100%;background:#000; height:10px;margin-bottom:1em;"></div>'.PHP_EOL;

//GET EXT INFO
ob_start();
echo '<table border="0" cellpadding="3" width="600">'.PHP_EOL;
echo '<tr class="h"><td><a href="http://www.php.net/">';
echo '<img border="0" src="http://static.php.net/www.php.net/images/php.gif" alt="PHP Logo" />';
echo '</a><h1 class="p">PHP Extensions</h1>'.PHP_EOL;
echo '</td></tr>'.PHP_EOL;
echo '</table><br />'.PHP_EOL;
echo '<h2>Overview</h2>'.PHP_EOL;
echo '<table border="0" cellpadding="3" width="600">'.PHP_EOL;
echo '<tr><td class="e">Extensions</td><td class="v">'.PHP_EOL;
$exts = array();
foreach (get_loaded_extensions() as $ext) {
    $exts[] = $ext;
}
echo implode(', ', $exts).PHP_EOL;
echo '</td></tr></table><br />'.PHP_EOL;
echo '<h2>Details</h2>'.PHP_EOL;
echo '<table border="0" cellpadding="3" width="600">'.PHP_EOL;
foreach ($exts as $ext) {
    echo '<tr><td class="e">'.$ext.'</td><td class="v">';
        $funcs = array();
        foreach (get_extension_funcs($ext) as $func) {
            $funcs[] = $func;
        }
        echo implode(', ', $funcs).PHP_EOL;
    echo '</td></tr>'.PHP_EOL;
}
echo '</table><br />'.PHP_EOL;
echo '</div></body></html>'.PHP_EOL;
$extinfo = ob_get_contents();
ob_end_clean();

//OUTPUT
echo $phpinfo.$hr.$extinfo;

?>