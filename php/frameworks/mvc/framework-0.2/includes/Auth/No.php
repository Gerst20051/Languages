<?php

  /**
  * FR_Auth_No
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net>
  * @license http://www.opensource.org/licenses/gpl-license.php
  * @package Framework
  * @filesource
  */

  /**
  * FR_Auth_No
  *
  * If your module class does not require any authentication then it should
  * extend from this authentication module.
  *
  * @author Joe Stump <joe@joestump.net>
  * @package Framework
  */
  abstract class FR_Auth_No extends FR_Auth
  {
      function __construct()
      {
          parent::__construct();
      }

      function authenticate()
      {
          return true;
      }

      function __destruct()
      {
          parent::__destruct();
      }
  }

?>
