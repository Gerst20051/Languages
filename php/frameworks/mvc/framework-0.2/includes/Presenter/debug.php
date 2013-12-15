<?php

  /**
  * FR_Presenter_debug
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net>
  * @license http://www.opensource.org/licenses/gpl-license.php
  * @package Framework
  * @filesource
  */

  /**
  * FR_Presenter_debug
  *
  * Having problems with your application? Why not switch your module's 
  * presenter to 'debug' to view a bunch of useful output. 
  *
  * @author Joe Stump <joe@joestump.net>
  * @package Framework
  */
  class FR_Presenter_debug extends FR_Presenter_common
  {
      // {{{ __construct(FR_Module $module)
      /**
      * __construct
      *
      * @author Joe Stump <joe@joestump.net>
      * @access public
      * @param mixed $module Instance of FR_Module
      * @return void
      */
      public function __construct(FR_Module $module)
      {
          parent::__construct($module);
      }
      // }}}
      // {{{ display()
      /**
      * display
      *
      * This function displays debugging information for our module. It's a 
      * good place to start when things go bonkers with your application.
      *
      * @author Joe Stump <joe@joestump.net>
      * @return void
      * @todo Add calls to display module, reflection as well.
      */
      public function display()
      {
          $vars = array('_POST','_GET','_COOKIE','_SESSION','_SERVER');
          foreach ($vars as $var) {
              $array = $$var;
              if (is_array($array) && count($array)) {
                    echo '<h2>$'.$var.'</h2>'."\n";
                    echo '<pre>';
                    var_dump($array);
                    echo '</pre>';
              }
          }
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
