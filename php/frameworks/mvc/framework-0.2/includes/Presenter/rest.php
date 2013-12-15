<?php

  /**
  * FR_Presenter_rest
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net>
  * @license http://www.opensource.org/licenses/gpl-license.php
  * @package Framework
  * @filesource
  */

  require_once('XML/Serializer.php');

  /**
  * FR_Presenter_rest
  *
  * Want to display your module's data in valid XML rather than HTML? This
  * presenter will automatically take your data and output it in valid XML.
  *
  * @author Joe Stump <joe@joestump.net>
  * @package Framework
  */
  class FR_Presenter_rest extends FR_Presenter_common
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
      * Output our data array using the PEAR package XML_Serializer. This may
      * not be the optimal output you want for your REST API, but it should
      * display valid XML that can be easily consumed by anyone.
      *
      * @author Joe Stump <joe@joestump.net>
      * @return void
      * @link http://pear.php.net/package/XML_Serializer
      */
      public function display()
      {
          $xml = new XML_Serializer();
          $xml->serialize($this->module->getData());

          header("Content-Type: text/xml");
          echo '<?xml version="1.0" encoding="UTF-8" ?>'."\n";
          echo $xml->getSerializedData();
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
