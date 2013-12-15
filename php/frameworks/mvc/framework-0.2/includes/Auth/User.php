<?php

  /**
  * FR_Auth_User
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net>
  * @license http://www.opensource.org/licenses/gpl-license.php 
  * @package Framework
  * @filesource
  */

  /**
  * FR_Auth_User
  *
  * If your module class requires that a user be logged in in order to access
  * it then extend it from this Auth class. 
  *
  * @author Joe Stump <joe@joestump.net>
  * @package Framework
  */
  abstract class FR_Auth_User extends FR_Auth
  {
      function __construct()
      {
          parent::__construct();
      }

      function authenticate()
      {
          return ($this->session->userID > 0);
      }

      function __destruct()
      {
          parent::__destruct();
      }
  }

?>
