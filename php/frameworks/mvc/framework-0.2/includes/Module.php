<?php

  /**
  * FR_Module
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net>
  * @license http://www.opensource.org/licenses/gpl-license.php
  * @package Framework
  * @filesource
  */

  /**
  * FR_Module
  * 
  * The base module class. All applications will extends from this class. This
  * means each module will, by default, have an open DB connection and an
  * open log file to write to. Also, it's a good place to put functions, 
  * variables, etc. that all modules need.
  *
  * @author Joe Stump <joe@joestump.net>
  * @package Framework
  */
  abstract class FR_Module extends FR_Object_Web
  {
      // {{{ properties
      /**
      * $presenter
      *
      * Used in FR_Presenter::factory() to determine which presentation (view)
      * class should be used for the module.
      *
      * @author Joe Stump <joe@joestump.net>
      * @var string $presenter 
      * @see FR_Presenter, FR_Presenter_common, FR_Presenter_smarty
      */
      public $presenter = 'smarty';

      /**
      * $data
      *
      * Data set by the module that will eventually be passed to the view.
      *
      * @author Joe Stump <joe@joestump.net>
      * @var mixed $data Module data
      * @see FR_Module::set(), FR_Module::getData()
      */
      protected $data = array();

      /**
      * $name
      *
      * @author Joe Stump <joe@joestump.net>
      * @var string $name Name of module class
      */
      public $name;

      /**
      * $tplFile
      *
      * @author Joe Stump <joe@joestump.net>
      * @var string $tplFile Name of template file
      * @see FR_Presenter_smarty
      */
      public $tplFile;

      /**
      * $moduleName
      *
      * @author Joe Stump <joe@joestump.net>
      * @var string $moduleName Name of requested module
      * @see FR_Presenter_smarty
      */
      public $moduleName = null;

      /**
      * $pageTemplateFile
      *
      * @author Joe Stump <joe@joestump.net>
      * @var string $pageTemplateFile Name of outer page template
      */
      public $pageTemplateFile = null;
      // }}}
      // {{{ __construct()
      /**
      * __construct
      * 
      * @author Joe Stump <joe@joestump.net>
      */
      public function __construct()
      {
          parent::__construct();
          $this->name = $this->me->getName();
          $this->tplFile = $this->name.'.tpl';
      }
      // }}}
      // {{{ __default()
      /**
      * __default
      *
      * This function is ran by the controller if an event is not specified
      * in the user's request.
      *
      * @author Joe Stump <joe@joestump.net>
      */
      abstract public function __default();
      // }}}
      // {{{ set($var,$val)
      /**
      * set
      *
      * Set data for your module. This will eventually be passed toe the
      * presenter class via FR_Module::getData().
      *
      * @author Joe Stump <joe@joestump.net>
      * @param string $var Name of variable
      * @param mixed $val Value of variable
      * @return void
      * @see FR_Module::getData()
      */
      protected function set($var,$val) {
          $this->data[$var] = $val; 
      }
      // }}}
      // {{{ getData()
      /**
      * getData
      *
      * Returns module's data. 
      *
      * @author Joe Stump <joe@joestump.net>
      * @return mixed
      * @see FR_Presenter_common
      */
      public function getData()
      {
          return $this->data;
      }
      // }}}
      // {{{ isValid($module)
      /**
      * isValid
      *
      * Determines if $module is a valid framework module. This is used by
      * the controller to determine if the module fits into our framework's
      * mold. If it extends from both FR_Module and FR_Auth then it should be
      * good to run.
      *
      * @author Joe Stump <joe@joestump.net>
      * @static
      * @param mixed $module
      * @return bool
      */
      public static function isValid($module)
      {
          return (is_object($module) && 
                  $module instanceof FR_Module && 
                  $module instanceof FR_Auth);
      }
      // }}}
      // {{{ __destruct()
      public function __destruct()
      {
          parent::__destruct();
      }
      // }}}
  }

?>
