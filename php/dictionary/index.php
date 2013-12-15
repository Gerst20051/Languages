<?php
require('config.php');
require('func.php');
IsAdminLogin();
SiteMenu();








include('_header.php');
echo $GLOBALS['content'];
include('_footer.php');

mysql_close();
?>