<?php

  /**
  * whoami
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net>
  * @license http://www.opensource.org/licenses/gpl-license.php
  * @package Modules
  * @filesource
  */

  /**
  * whoami
  *
  * @author Joe Stump <joe@joestump.net>
  * @package Modules
  */
  class whoami extends FR_Auth_User
  {
      public function __construct()
      {
          parent::__construct();
      }

      public function __default()
      {
          $this->set('user',$this->user);
          if ($_GET['output'] == 'rest') {
              $this->presenter = 'rest';
          }         
      }

      public function __destruct()
      {
          parent::__destruct();
      }
  }

?>
