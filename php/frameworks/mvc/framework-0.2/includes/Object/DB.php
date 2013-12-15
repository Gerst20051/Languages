<?php

  /**
  * FR_Object_DB
  *
  * @author Joe Stump <joe@joestump.net>
  * @copyright Joe Stump <joe@joestump.net> 
  * @license http://www.opensource.org/licenses/gpl-license.php
  * @package Framework
  * @filesource
  */

  require_once('DB.php');

  /**
  * FR_Object_DB
  *
  * Extends the base FR_Object class to include a database connection.
  *
  * @author Joe Stump <joe@joestump.net>
  * @package Framework
  */
  abstract class FR_Object_DB extends FR_Object
  {
      protected $db;

      public function __construct()
      {
          parent::__construct();
          static $connection = null;
          if ($connection === null) {
              $connection = DB::connect(FR_DSN);              
              if (!PEAR::isError($connection)) {
                  $connection->setFetchMode(DB_FETCHMODE_ASSOC);
              } else {
                  throw new Exception($connection->getMessage());
              }
          }

          $this->db = $connection;
      }

      function __destruct()
      {
          parent::__destruct();
          if (DB::isConnection($this->db)) {
              $this->db->disconnect();
          }
      }
  }

?>
