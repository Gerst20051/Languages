<?php

  /**
  * login
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net>
  * @license http://www.opensource.org/licenses/gpl-license.php 
  * @package Modules
  * @filesource
  */

  require_once('Validate.php');

  /**
  * login
  *
  * @author Joe Stump <joe@joestump.net>
  * @package Modules
  */
  class login extends FR_Auth_No
  {
      public function __construct()
      {
          parent::__construct();
      }

      public function __default()
      {
          if (count($_POST)) {
              $errors = array();

              if (Validate::email($_POST['email'])) {
                  $sql = "SELECT *
                          FROM fr_users
                          WHERE email='".$_POST['email']."'";

                  $user = $this->db->getRow($sql);
                  if (!PEAR::isError($result) && is_array($user)) {
                      if ($user['status'] == 'active') {
                          if ($user['password'] == $_POST['password']) {
                              if (isset($_GET['pg'])) {
                                  $go = urldecode($_GET['pg']);
                              } elseif (isset($_POST['pg'])) {
                                  $go = urldecode($_POST['pg']);
                              } else {
                                  $go = '/';
                              }

                              $this->session->userID = $user['userID'];
                              header("Location: $go");
                              exit();
                          } else {
                              $errors[] = 'Your password is invalid';
                          }
                      } else {
                          $errors[] = 'Your account has been disabled';
                      }
                  } else {
                      $errors[] = 'Email does not exist in database';
                  }

                  $this->set('errors',$errors);
              }
          }
      }

      public function __destruct()
      {
          parent::__destruct();
      }
  }


?>
