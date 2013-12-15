<?php

  /**
  * FR_Presenter_common
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net> 
  * @license http://www.opensource.org/licenses/gpl-license.php
  * @package Framework
  * @filesource
  */

  /**
  * FR_Presenter_common
  *
  * A common base class for our presenters (views). All of our presenters must
  * extend from this class. If they do not then Presenter::factory() will 
  * return an error.
  *
  * @author Joe Stump <joe@joestump.net> 
  * @package Framework
  */
  abstract class FR_Presenter_common extends FR_Object_Web
  {
      protected $module;

      public function __construct(FR_Module $module)
      {
          parent::__construct();
          $this->module = $module;
      }

      abstract public function display();

      public function __destruct()
      {
          parent::__destruct();
      }
  }

?>
