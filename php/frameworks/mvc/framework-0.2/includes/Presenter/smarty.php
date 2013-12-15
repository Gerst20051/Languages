<?php

  /**
  * FR_Presenter_smarty
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net>
  * @license http://www.opensource.org/licenses/gpl-license.php
  * @package Framework
  * @filesource
  */

  require_once(SMARTY_DIR.'Smarty.class.php');

  /**
  * FR_Presenter_smarty
  *
  * By default we use Smarty as our websites presentation layer (view). Smarty
  * is a robust compiling template engine with an active community.
  *
  * @author Joe Stump <joe@joestump.net>
  * @package Framework
  * @link http://smarty.php.net
  */
  class FR_Presenter_smarty extends FR_Presenter_common
  {
      private $template = null;
      private $path = null;

      public function __construct(FR_Module $module)
      {
          parent::__construct($module);
          $this->path = FR_BASE_PATH.'/tpl/'.FR_TEMPLATE;
          $this->template = new Smarty();
          $this->template->template_dir = $this->path.'/'.'templates';
          $this->template->compile_dir = $this->path.'/'.'templates_c';
          $this->template->cache_dir = $this->path.'/'.'cache';
          $this->template->config_dir = $this->path.'/'.'config';
      }

      public function display()
      {
          $path = FR_BASE_PATH.'/modules/'.$this->module->moduleName.'/tpl';;
          $tplFile = $this->module->tplFile;

          $this->template->assign('modulePath',$path);
          $this->template->assign('tplFile',$tplFile);
          $this->template->assign('user',$this->user);
          $this->template->assign('session',$this->session);

          foreach ($this->module->getData() as $var => $val) {
              if (!in_array($var,array('path','tplFile'))) {
                  $this->template->assign($var,$val);
              }
          }

          if ($this->module->pageTemplateFile == null) {
              $pageTemplateFile = 'page.tpl';
          } else {
              $pageTemplateFile = $this->module->pageTemplateFile;
          }

          $this->template->display($pageTemplateFile);
      }

      public function __destruct()
      {
          parent::__destruct();
      }
  }

?>
