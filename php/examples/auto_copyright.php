<?php
/**
 * Contents: Auto Updating Copyright
 * Author: InertiaInMotion.com.au
 * Function Usage:
 *  <?php auto_copyright(); ?>
 *  <?php auto_copyright('date'); ?>
 *  Call the function by one of the usage methods
 *  You have 2 choices here auto_copyright() or auto_copyright('date')
 *
 *  You can call the function as auto_copyright()
 *  And the date will be selected and auto updated by the server your hosted on.
 *
 *  Or call the function as auto_copyright('2010') or what ever year you want the copyright to start from
 *  If you use this method, the copyright dates will be printed as "2010 - currentyear"
 *  This method also has a fallback, if you enter a date greater then the system date
 *  We will fall back and just call auto_copyright()
 */
?>
<?php function auto_copyright($year = 'auto'){ ?>
<?php if(intval($year) == 'auto'){ $year = date('Y'); } ?>
<?php if(intval($year) == date('Y')){ echo intval($year); } ?>
<?php if(intval($year) < date('Y')){ echo intval($year) . ' - ' . date('Y'); } ?>
<?php if(intval($year) > date('Y')){ echo date('Y'); } ?>
<?php } ?>