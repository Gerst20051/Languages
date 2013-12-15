<?php
// Example 1
echo ini_get('display_errors');

if (!ini_get('display_errors')) {
    ini_set('display_errors', 1);
}

echo ini_get('display_errors');

// Example 2
$setting = 'y2k_compliance';

echo 'Current value for \'' . $setting . '\': ' . ini_get($setting), PHP_EOL;

ini_set($setting, ini_get($setting) ? 0 : 1);
echo 'New value for \'' . $setting . '\': ' . ini_get($setting), PHP_EOL;

ini_restore($setting);
echo 'Original value for \'' . $setting . '\': ' . ini_get($setting), PHP_EOL;
?>