<?php

  /**
  * FR_Presenter
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net>  
  * @license http://www.opensource.org/licenses/gpl-license.php
  * @package Framework
  * @filesource
  */

  /**
  * FR_Presenter
  *
  * Presenter factory class. This is used by the controller file, in 
  * conjunction with the FR_Module::$presenter variable to produced the
  * desired presenter class.
  *
  * @author Joe Stump <joe@joestump.net>
  * @package Framework
  * @see FR_Module::$presenter, FR_Presenter_common
  */
  class FR_Presenter
  {
      // {{{ factory($type,FR_Module $module)
      /**
      * factory
      *
      * @author Joe Stump <joe@joestump.net>
      * @access public
      * @param string $type Presentation type (our view)
      * @param mixed $module Our module, which the presenter will display
      * @return mixed PEAR_Error on failure or a valid presenter
      * @static
      */
      static public function factory($type,FR_Module $module)
      {
          $file = FR_BASE_PATH.'/includes/Presenter/'.$type.'.php';
          if (include($file)) {
              $class = 'FR_Presenter_'.$type;
              if (class_exists($class)) {
                  $presenter = new $class($module);
                  if ($presenter instanceof FR_Presenter_common) {
                      return $presenter;
                  }

                  return PEAR::raiseError('Invalid presentation class: '.$type);
              }

              return PEAR::raiseError('Presentation class not found: '.$type);
          }

          return PEAR::raiseError('Presenter file not found: '.$type);
      }
      // }}}
  }

?>
