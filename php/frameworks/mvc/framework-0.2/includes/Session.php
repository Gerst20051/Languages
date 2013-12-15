<?php

  /**
  * FR_Session
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net>
  * @license http://www.opensource.org/licenses/gpl-license.php
  * @package Framework
  * @filesource
  */

  /**
  * FR_Session
  *
  * Our base session class as a singleton. Handles creating the session, 
  * writing to the session variable (via overloading) and destroying the
  * session.
  *
  * @author Joe Stump <joe@joestump.net>
  * @package Framework
  */
  class FR_Session 
  {
      /**
      * $instance
      *
      * Instance variable used for singleton pattern. Stores a single instance
      * of FR_Session.
      *
      * @author Joe Stump <joe@joestump.net>
      * @var mixed $instance
      */
      private static $instance;

      /**
      * $sessionID
      *
      * The session ID assigned by PHP (usually a 32 character alpha-numeric
      * string).
      *
      * @author Joe Stump <joe@joestump.net>
      * @var string $sessionID
      */
      public static $sessionID;

      // {{{ __construct()
      /**
      * __construct
      *
      * Starts the session and sets the sessionID for the class.
      *
      * @author Joe Stump <joe@joestump.net>
      */
      private function __construct()
      {
          session_start();
          self::$sessionID = session_id();
      }
      // }}}
      // {{{ singleton()
      /**
      * singleton
      * 
      * Implementation of the singleton pattern. Returns a sincle instance
      * of the session class.
      * 
      * @author Joe Stump <joe@joestump.net>
      * @return mixed Instance of session
      */
      public static function singleton()
      {
          if (!isset(self::$instance)) {
              $className = __CLASS__;
              self::$instance = new $className;
          }

          return self::$instance;
      }
      // }}}
      // {{{ destroy()
      public function destroy()
      {
          foreach ($_SESSION as $var => $val) {
              $_SESSION[$var] = null;
          }

          session_destroy();
      }
      // }}}
      // {{{ __clone()
      /**
      * __clone
      *
      * Disable PHP5's cloning method for session so people can't make copies
      * of the session instance.
      * 
      * @author Joe Stump <joe@joestump.net>
      */
      public function __clone()
      {
          trigger_error('Clone is not allowed for '.__CLASS__,E_USER_ERROR);
      }
      // }}}
      // {{{ __get($var)
      /**
      * __get($var)
      *
      * Returns the requested session variable.
      *
      * @author Joe Stump <joe@joestump.net>
      * @return mixed
      * @see FR_Session::__get()
      */
      public function __get($var)
      {
          return $_SESSION[$var];
      }
      // }}}
      // {{{ __set($var,$val)
      /**
      * __set 
      * 
      * Using PHP5's overloading for setting and getting variables we can
      * use $session->var = $val and have it stored in the $_SESSION 
      * variable. To set an email address, for instance you would do the
      * following:
      *
      * <code>
      * $session->email = 'user@example.com';
      * </code>
      *
      * This doesn't actually store 'user@example.com' into $session->email,
      * rather it is stored in $_SESSION['email'].
      * 
      * @author Joe Stump <joe@joestump.net>
      * @param string $var
      * @param mixed $val
      * @see FR_Session::__get()
      * @link http://us3.php.net/manual/en/language.oop5.overloading.php
      */
      public function __set($var,$val)
      {
          return ($_SESSION[$var] = $val);
      }
      // }}}
      // {{{ __destruct()
      /**
      * __destruct()
      *
      * Writes the current session.
      *
      * @author Joe Stump <joe@joestump.net> 
      */
      public function __destruct()
      {
          session_write_close();
      }
      // }}}
  }

?>
