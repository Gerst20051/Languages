<?php

  /**
  * index.php
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net>
  * @license http://www.opensource.org/licenses/gpl-license.php
  * @package Framework
  */

  require_once('config.php');

  // {{{ __autoload($class)
  /**
  * __autoload
  *
  * Autoload is ran by PHP when it can't find a class it is trying to load.
  * By naming our classes intelligently we should be able to load most classes
  * dynamically.
  *
  * @author Joe Stump <joe@joestump.net>
  * @param string $class Class name we're trying to load
  * @return void
  * @package Framework
  */
  function __autoload($class)
  {
      $file = str_replace('_','/',substr($class,2)).'.php';     
      require_once(FR_BASE_PATH.'/includes/'.$file);
  }
  // }}}

  if (isset($_GET['module'])) {
      $module = $_GET['module'];
      if (isset($_GET['event'])) {
          $event = $_GET['event'];
      } else {
          $event = '__default';
      }

      if (isset($_GET['class'])) {
          $class = $_GET['class'];
      } else {
          $class = $module;
      }

      $classFile = FR_BASE_PATH.'/modules/'.$module.'/'.$class.'.php';
      if (file_exists($classFile)) {
          require_once($classFile);
          if (class_exists($class)) {
              try {
                  $instance = new $class();
                  if (!FR_Module::isValid($instance)) {
                      die("Requested module is not a valid framework module!");
                  }

                  $instance->moduleName = $module;
                  if ($instance->authenticate()) {
                      try {
                          $result = $instance->$event();
                          if (!PEAR::isError($result)) {
                              $presenter = FR_Presenter::factory($instance->presenter,$instance);

                              if (!PEAR::isError($presenter)) {
                                  $presenter->display();
                              } else {
                                  die($presenter->getMessage());
                              }
                          }
                      } catch (Exception $error) {
                          die($error->getMessage());
                      }
                  } else {
                      die("You do not have access to the requested page!");
                  }
              } catch (Exception $error) {
                  die($error->getMessage());   
              }
          } else {
              die("An valid module for your request was not found");        
          }
      } else {
          die("Could not find: $classFile");        
      }
  } else {
      die("A valid module was not specified");
  }

?>
