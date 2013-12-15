<?php

  /**
  * FR_Object
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net> 
  * @license http://www.opensource.org/licenses/gpl-license.php
  * @package Framework
  * @filesource
  */

  require_once('Log.php');

  /**
  * FR_Object
  *
  * The base object class for most of the classes that we use in our framework.
  * Provides basic logging and set/get functionality.
  *
  * @author Joe Stump <joe@joestump.net>
  * @package Framework
  */
  abstract class FR_Object
  {
      /**
      * $log
      *
      * @var mixed $log Instance of PEAR Log 
      */
      protected $log;

      /**
      * $me
      *
      * @var mixed $me Instance of ReflectionClass
      */
      protected $me;

      /**
      * __construct
      * 
      * @author Joe Stump <joe@joestump.net>
      * @access public 
      */
      public function __construct()
      {
          $this->log = Log::factory('file',FR_LOG_FILE);
          $this->me = new ReflectionClass($this);
      }

      /**
      * setFrom
      *
      * @author Joe Stump <joe@joestump.net>
      * @access public
      * @param mixed $data Array of variables to assign to instance
      * @return void
      */
      public function setFrom($data)
      {
          if (is_array($data) && count($data)) {
              $valid = get_class_vars(get_class($this));
              foreach ($valid as $var => $val) {
                  if (isset($data[$var])) {
                      $this->$var = $data[$var];
                  }
              }
          }
      }

      /**
      * toArray
      *
      * @author Joe Stump <joe@joestump.net>
      * @access public
      * @return mixed Array of member variables keyed by variable name
      */
      public function toArray()
      {
          $defaults = $this->me->getDefaultProperties();
          $return = array();
          foreach ($defaults as $var => $val) {
              if ($this->$var instanceof FR_Object) {
                  $return[$var] = $this->$var->toArray();
              } else {
                  $return[$var] = $this->$var;
              }
          }
  
          return $return;
      }

      /**
      * __destruct
      *
      * @author Joe Stump <joe@joestump.net>
      * @access public
      * @return void
      */
      public function __destruct()
      {
          if ($this->log instanceof Log) {
              $this->log->close();
          }
      }
  }

?>
