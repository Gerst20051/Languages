<?php 
class MySQLDB 
{ 
   private $connection;          // The MySQL database connection 

   /* Class constructor */ 
   function MySQLDB(){ 
      /* Make connection to database */ 
      $this->connection = mysql_connect(DB_SERVER, DB_USER, DB_PASS) or die(mysql_error()); 
      mysql_select_db(DB_NAME, $this->connection) or die(mysql_error()); 
   } 

   /* Transactions functions */ 

   function begin(){ 
         $null = mysql_query("START TRANSACTION", $this->connection); 
      return mysql_query("BEGIN", $this->connection); 
   } 

   function commit(){ 
      return mysql_query("COMMIT", $this->connection); 
   } 
   
   function rollback(){ 
      return mysql_query("ROLLBACK", $this->connection); 
   } 

   function transaction($q_array){ 
         $retval = 1; 

      $this->begin(); 

         foreach($q_array as $qa){ 
            $result = mysql_query($qa['query'], $this->connection); 
            if(mysql_affected_rows() == 0){ $retval = 0; } 
         } 

      if($retval == 0){ 
         $this->rollback(); 
         return false; 
      }else{ 
         $this->commit(); 
         return true; 
      } 
   } 

}; 

/* Create database connection object */ 
$database = new MySQLDB; 

// then from anywhere else simply put the transaction queries in an array or arrays like this: 

   function function(){ 
      global $database; 

      $q = array ( 
         array("query" => "UPDATE table WHERE something = 'something'"), 
         array("query" => "UPDATE table WHERE something_else = 'something_else'"), 
         array("query" => "DELETE FROM table WHERE something_else2 = 'something_else2'"), 
      ); 

      $database->transaction($q); 

   } 
?>