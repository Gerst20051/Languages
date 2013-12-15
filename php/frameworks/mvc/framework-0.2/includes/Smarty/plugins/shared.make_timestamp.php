<?php
/**
 * Smarty shared plugin
 * @package Smarty
 * @subpackage plugins
 */


/**
 * Function: smarty_make_timestamp<br>
 * Purpose:  used by other smarty functions to make a timestamp
 *           from a string.
 * @param string
 * @return string
 */
function smarty_make_timestamp($string)
{
    if(empty($string)) {
        $string = "now";
    }

    static $endOfTime;
    if (!isset($endOfTime)) {
        $endOfTime = mktime(3,14,6,1,19,2038); // End of 32-bit UNIX time - 1
                                               // 19 Jan 2038 at 03:14 and 7
    }

    $time = strtotime($string);
    if (is_numeric($time) && $time != -1 && $time < $endOfTime) {
        return $time;
    }

    // is mysql timestamp format of YYYYMMDDHHMMSS?
    if (preg_match('/^\d{14}$/', $string)) {
        $time = mktime(substr($string,8,2),substr($string,10,2),substr($string,12,2),
               substr($string,4,2),substr($string,6,2),substr($string,0,4));

        echo 'MySQL timestamp (1): '.$string.'<br />';
        echo 'MySQL timestamp (2): '.$time.'<br />';
        return $time;
    }

    // couldn't recognize it, try to return a time
    $time = (int) $string;
    if ($time > 0)
        return $time;
    else
        return time();
}

/* vim: set expandtab: */

?>
