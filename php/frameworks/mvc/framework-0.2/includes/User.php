<?php

  /**
  * FR_User
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net>
  * @license http://www.opensource.org/licenses/gpl-license.php
  * @package Framework
  * @filesource
  */

  /**
  * FR_User
  *
  * Base user object.
  *
  * @author Joe Stump <joe@joestump.net>
  * @package Framework
  */
  class FR_User extends FR_Object_DB
  {
      public $userID;
      public $email;
      public $password;
      public $fname;
      public $lname;
      public $posted;

      public function __construct($userID=null)
      {
          parent::__construct();

          if ($userID === null) {
              $session = FR_Session::singleton(); 
              if (!is_numeric($session->userID)) {
                  $userID = 0; 
              } else {
                  $userID = $session->userID;
              }
          }

          $sql = "SELECT *
                  FROM fr_users
                  WHERE userID=".$userID." AND 
                        status='active'";

          $result = $this->db->getRow($sql);
          if (!PEAR::isError($result) && is_array($result)) {
              $this->setFrom($result);
          }
      }

      public function __destruct()
      {
          parent::__destruct();
      }
  }

?>
