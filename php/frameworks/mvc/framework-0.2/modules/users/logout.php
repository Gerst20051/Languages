<?php

  /**
  * logout
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net>
  * @license http://www.opensource.org/licenses/gpl-license.php
  * @package Modules
  * @filesource
  */

  /**
  * logout
  *
  * @author Joe Stump <joe@joestump.net>
  * @package Modules
  */
  class logout extends FR_Auth_User
  {
      function __construct()
      {
          parent::__construct();
      }

      function __default()
      {
          $this->session->destroy();
          if (isset($_GET['pg'])) {
              $go = urldecode($_GET['pg']);
          } else {
              $go = '/';
          }

          header("Location: $go");
          exit();
      }

      function __destruct()
      {
          parent::__destruct();
      }
  }

?>
