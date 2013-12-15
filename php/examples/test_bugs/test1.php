<?php
/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 2011 The PHP Group                                     |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Johannes Schlueter <johannes@php.net>                        |
  +----------------------------------------------------------------------+
*/

if (PHP_SAPI != 'cli') {
    die("Please run this test from CLI!\n");
}

ini_set('display_errors', 1);
ini_set('output_buffering', 0);
error_reporting(-1);
if (!ini_get('safe_mode')) {
    set_time_limit(1);
}

echo "Testing float behaviour. If this script hangs or terminates with an error ".
     "message due to maximum execution time limit being reached, you should ".
     "update your PHP installation asap!\n";
echo "For more information refer to <http://bugs.php.net/53632>.\n";
$d = (double)"2.2250738585072011e-308";
echo "Your system seems to be safe.\n";

/* Code Example
$sapi_type = php_sapi_name();
if (substr($sapi_type, 0, 3) == 'cgi') {
    echo "You are using CGI PHP\n";
} else {
    echo "You are not using CGI PHP\n";
}

Possible Values
aolserver, apache, apache2filter, apache2handler, caudium, cgi (until PHP 5.3), cgi-fcgi, cli, continuity, embed, isapi, litespeed, milter, nsapi, phttpd, pi3web, roxen, thttpd, tux, and webjames.
*/
?>