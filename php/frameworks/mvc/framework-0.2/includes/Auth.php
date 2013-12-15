<?php

  /**
  * FR_Auth
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net> 
  * @license http://www.opensource.org/licenses/gpl-license.php
  * @package Framework
  * @filesource
  */

  /**
  * FR_Auth
  *
  * Base class of authentication classes. The controller will check to make
  * sure your module is an instance of FR_Auth.
  *
  * @author Joe Stump <joe@joestump.net>
  * @package Framework
  * @see FR_Module
  */
  abstract class FR_Auth extends FR_Module
  {
      // {{{ __construct()
      function __construct()
      {
          parent::__construct();
      }
      // }}}
      // {{{ authenticate()
      abstract function authenticate();
      // }}}
      // {{{ __destruct()
      function __destruct()
      {
          parent::__destruct();
      }
      // }}}
  }

?>
