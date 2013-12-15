<?php
/**
 * Smarty plugin
 * @package Smarty
 * @subpackage plugins
 */


/**
 * Strip "Section X:" out of a section title
 *
 * Type:     modifier<br>
 * Name:     tests_section<br>
 * Purpose:  Strip "Section X:" out of a section title
 * Example:  {$var|tests_section} {$var|tests_section:"&nbsp;"}
 * Date:     September 25th, 2002
 * @author   Joe Stump <joe@joestump.net> 
 * @version  1.0
 * @param string
 * @return string
 */
function smarty_modifier_tests_section($text)
{
    return eregi_replace('Section [0-9]*:','',$text);
}

/* vim: set expandtab: */

?>
