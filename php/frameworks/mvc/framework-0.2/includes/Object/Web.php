<?php

  /**
  * FR_Object_Web
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net>
  * @license http://www.opensource.org/licenses/gpl-license.php 
  * @package Framework
  * @filesource
  */

  /**
  * FR_Object_Web
  *
  * This is the base class for web applications extended from FR_Module, which
  * includes all module classes. Sets up a current user and session.
  *
  * @author Joe Stump <joe@joestump.net>
  * @package Framework
  */
  abstract class FR_Object_Web extends FR_Object_DB
  {
      // {{{ properties
      /**
      * $user
      *
      * This is the current user. If the user is not logged in then the
      * information defaults to the special anonymous user (userID = 0).
      *
      * @author Joe Stump <joe@joestump.net>
      * @var mixed $user Instnace of FR_User of current user
      */
      protected $user;

      /**
      * $session
      *
      * A simple wrapper class around PHP's $_SESSION variable.
      *
      * @author Joe Stump <joe@joestump.net>
      * @var mixed $session Instance of FR_Session
      */
      protected $session;
      // }}} 
      public function __construct()
      {
          parent::__construct();
          $this->user = new FR_User();
          $this->session = FR_Session::singleton();
      }

      public function __destruct()
      {
          parent::__destruct();
      }
  }

?>
