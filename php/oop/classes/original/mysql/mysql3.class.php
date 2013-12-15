<?php 

/*************************************************************************** 
* 
*    Author   : Eric Sizemore ( www.secondversion.com & www.phpsociety.com ) 
*    Package  : Simple MySQL DB Class 
*    Version  : 1.0.1 
*    Copyright: (C) 2007 Eric Sizemore 
*    Site     : www.secondversion.com & www.phpsociety.com 
*    Email    : esizemore05@gmail.com 
*    File     : mysql.class.php 
* 
*    This program is free software; you can redistribute it and/or modify 
*    it under the terms of the GNU General Public License as published by 
*    the Free Software Foundation; either version 2 of the License, or 
*    (at your option) any later version. 
* 
*    This program is distributed in the hope that it will be useful, 
*    but WITHOUT ANY WARRANTY; without even the implied warranty of 
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
*    GNU General Public License for more details. 
* 
***************************************************************************/ 

/** 
* Class to interact with a mysql database 
*/ 
class db_mysql 
{ 
    /** 
    * Connection to MySQL. 
    * 
    * @var string 
    */ 
    var $link; 

    /** 
    * Holds the most recent connection. 
    * 
    * @var string 
    */ 
    var $recent_link = null; 

    /** 
    * Holds the contents of the most recent SQL query. 
    * 
    * @var string 
    */ 
    var $sql = ''; 

    /** 
    * Holds the number of queries executed. 
    * 
    * @var integer 
    */ 
    var $query_count = 0; 

    /** 
    * The text of the most recent database error message. 
    * 
    * @var string 
    */ 
    var $error = ''; 

    /** 
    * The error number of the most recent database error message. 
    * 
    * @var integer 
    */ 
    var $errno = ''; 

    /** 
    * Do we currently have a lock in place? 
    * 
    * @var boolean 
    */ 
    var $is_locked = false; 

    /** 
    * Show errors? If set to true, the error message/sql is displayed. 
    * 
    * @var boolean 
    */ 
    var $show_errors = false; 

    /** 
    * Constructor. Initializes a database connection and selects our database. 
    * 
    * @param  string  Database host 
    * @param  string  Database username 
    * @param  string  Database password 
    * @param  string  Database name 
    * @return boolean 
    */ 
    function db_mysql($db_host, $db_user, $db_pass, $db_name) 
    { 
        $this->link = @mysql_connect($db_host, $db_user, $db_pass); 

        if ($this->link) 
        { 
            if (@mysql_select_db($db_name, $this->link)) 
            { 
                $this->recent_link =& $this->link; 
                return $this->link; 
            } 
        } 
        // If we couldn't connect or select the db... 
        $this->raise_error("Could not select and/or connect to database: $db_name"); 
    } 

    /** 
    * Executes a sql query. If optional $only_first is set to true, it will 
    * return the first row of the result as an array. 
    * 
    * @param  string  Query to run 
    * @param  bool    Return only the first row, as an array? 
    * @return mixed 
    */ 
    function query($sql, $only_first = false) 
    { 
        $this->recent_link =& $this->link; 
        $this->sql =& $sql; 
        $result = @mysql_query($sql, $this->link); 

        $this->query_count++; 

        if ($only_first) 
        { 
            $return = $this->fetch_array($result); 
            $this->free_result($result); 
            return $return; 
        } 
        return $result; 
    } 

    /** 
    * Fetches a row from a query result and returns the values from that row as an array. 
    * 
    * @param  string  The query result we are dealing with. 
    * @return array 
    */ 
    function fetch_array($result) 
    { 
        return @mysql_fetch_assoc($result); 
    } 

    /** 
    * Returns the number of rows in a result set. 
    * 
    * @param  string  The query result we are dealing with. 
    * @return integer 
    */ 
    function num_rows($result) 
    { 
        return @mysql_num_rows($result); 
    } 

    /** 
    * Retuns the number of rows affected by the most recent query 
    * 
    * @return integer 
    */ 
    function affected_rows() 
    { 
        return @mysql_affected_rows($this->recent_link); 
    } 

    /** 
    * Returns the number of queries executed. 
    * 
    * @param  none 
    * @return integer 
    */ 
    function num_queries() 
    { 
        return $this->query_count; 
    } 

    /** 
    * Lock database tables 
    * 
    * @param   array  Array of table => lock type 
    * @return  void 
    */ 
    function lock($tables) 
    { 
        if (is_array($tables) AND count($tables)) 
        { 
            $sql = ''; 

            foreach ($tables AS $name => $type) 
            { 
                $sql .= (!empty($sql) ? ', ' : '') . "$name $type"; 
            } 

            $this->query("LOCK TABLES $sql"); 
            $this->is_locked = true; 
        } 
    } 

    /** 
    * Unlock tables 
    */ 
    function unlock() 
    { 
        if ($this->is_locked) 
        { 
            $this->query("UNLOCK TABLES"); 
            $this->is_locked = false; 
        } 
    } 

    /** 
    * Returns the ID of the most recently inserted item in an auto_increment field 
    * 
    * @return  integer 
    */ 
    function insert_id() 
    { 
        return @mysql_insert_id($this->link); 
    } 

    /** 
    * Escapes a value to make it safe for using in queries. 
    * 
    * @param  string  Value to be escaped 
    * @param  bool    Do we need to escape this string for a LIKE statement? 
    * @return string 
    */ 
    function prepare($value, $do_like = false) 
    { 
        $value = stripslashes($value); 

        if ($do_like) 
        { 
            $value = str_replace(array('%', '_'), array('\%', '\_'), $value); 
        } 

        if (function_exists('mysql_real_escape_string')) 
        { 
            return mysql_real_escape_string($value, $this->link); 
        } 
        else 
        { 
            return mysql_escape_string($value); 
        } 
    } 

    /** 
    * Frees memory associated with a query result. 
    * 
    * @param  string   The query result we are dealing with. 
    * @return boolean 
    */ 
    function free_result($result) 
    { 
        return @mysql_free_result($result); 
    } 

    /** 
    * Turns database error reporting on 
    */ 
    function show_errors() 
    { 
        $this->show_errors = true; 
    } 

    /** 
    * Turns database error reporting off 
    */ 
    function hide_errors() 
    { 
        $this->show_errors = false; 
    } 

    /** 
    * Closes our connection to MySQL. 
    * 
    * @param  none 
    * @return boolean 
    */ 
    function close() 
    { 
        $this->sql = ''; 
        return @mysql_close($this->link); 
    } 

    /** 
    * Returns the MySQL error message. 
    * 
    * @param  none 
    * @return string 
    */ 
    function error() 
    { 
        $this->error = (is_null($this->recent_link)) ? '' : mysql_error($this->recent_link); 
        return $this->error; 
    } 

    /** 
    * Returns the MySQL error number. 
    * 
    * @param  none 
    * @return string 
    */ 
    function errno() 
    { 
        $this->errno = (is_null($this->recent_link)) ? 0 : mysql_errno($this->recent_link); 
        return $this->errno; 
    } 

    /** 
    * Gets the url/path of where we are when a MySQL error occurs. 
    * 
    * @access private 
    * @param  none 
    * @return string 
    */ 
    function _get_error_path() 
    { 
        if ($_SERVER['REQUEST_URI']) 
        { 
            $errorpath = $_SERVER['REQUEST_URI']; 
        } 
        else 
        { 
            if ($_SERVER['PATH_INFO']) 
            { 
                $errorpath = $_SERVER['PATH_INFO']; 
            } 
            else 
            { 
                $errorpath = $_SERVER['PHP_SELF']; 
            } 

            if ($_SERVER['QUERY_STRING']) 
            { 
                $errorpath .= '?' . $_SERVER['QUERY_STRING']; 
            } 
        } 

        if (($pos = strpos($errorpath, '?')) !== false) 
        { 
            $errorpath = urldecode(substr($errorpath, 0, $pos)) . substr($errorpath, $pos); 
        } 
        else 
        { 
            $errorpath = urldecode($errorpath); 
        } 
        return $_SERVER['HTTP_HOST'] . $errorpath; 
    } 

    /** 
    * If there is a database error, the script will be stopped and an error message displayed. 
    * 
    * @param  string  The error message. If empty, one will be built with $this->sql. 
    * @return string 
    */ 
    function raise_error($error_message = '') 
    { 
        if ($this->recent_link) 
        { 
            $this->error = $this->error($this->recent_link); 
            $this->errno = $this->errno($this->recent_link); 
        } 

        if ($error_message == '') 
        { 
            $this->sql = "Error in SQL query:\n\n" . rtrim($this->sql) . ';'; 
            $error_message =& $this->sql; 
        } 
        else 
        { 
            $error_message = $error_message . ($this->sql != '' ? "\n\nSQL:" . rtrim($this->sql) . ';' : ''); 
        } 

        $message = "<textarea rows=\"10\" cols=\"80\">MySQL Error:\n\n\n$error_message\n\nError: {$this->error}\nError #: {$this->errno}\nFilename: " . $this->_get_error_path() . "\n</textarea>"; 

        if (!$this->show_errors) 
        { 
            $message = "<!--\n\n$message\n\n-->"; 
        } 
        die("There seems to have been a slight problem with our database, please try again later.<br /><br />\n$message"); 
    } 
} 

/** 
* Instantiate our mysql class and connect to the database. 
* 
* database host/server 
* database username 
* database password 
* database name 
*/ 
$db =& new db_mysql('localhost', 'username', 'password', 'database_name'); 

/** 
* Example 1 - Simple query.. 
*/ 
$user = $db->query(" 
    SELECT * 
    FROM users 
    WHERE userid = 1 
") or $db->raise_error(); // Leaving 'raise_error()' blank will create an error message with the SQL 
$user = $db->fetch_array($user); 

// OR setting the second param to true, which will return the result set, effectively the same as above 
$user = $db->query(" 
    SELECT * 
    FROM users 
    WHERE userid = 1 
", true) or $db->raise_error(); // Leaving 'raise_error()' blank will create an error message with the SQL 

/** 
* Example 2 - Getting the number of rows 
*/ 
$users = $db->query(" 
    SELECT * 
    FROM users 
") or $db->raise_error(); // Leaving 'raise_error()' blank will create an error message with the SQL 
echo $db->num_rows($users); 

/** 
* Example 3 - Getting the number of affected rows 
*/ 
$users = $db->query(" 
    UPDATE users 
    SET is_active = 0 
") or $db->raise_error(); // Leaving 'raise_error()' blank will create an error message with the SQL 
echo $db->affected_rows($users); 

/** 
* Example 4 - Getting the number of executed queries 
*/ 
echo $db->num_queries(); 

/** 
* Example 5 - (un)locking a table/tables 
* 
* array of  tablename => locktype 
*/ 
// Single table 
$tables = array( 
    'users' => 'write' 
); 
$db->lock($tables); 

// Multiple tables 
$tables = array( 
    'users'  => 'write', 
    'config' => 'read', 
    'posts'  => 'write' 
); 
$db->lock($tables); 

// Unlock 
$db->unlock(); 

/** 
* Example 6 - Getting the last insert id of an auto_increment field 
*/ 
$db->query(" 
    INSERT INTO users (name, email, is_active) 
    VALUES ('Eric', 'esizemore05@gmail.com', 0) 
") or $db->raise_error('Failed adding new user'); // Will use the message we give it + the SQL 
echo $db->insert_id(); 

/** 
* Example 7 - Freeing a mysql result 
*/ 
$users = $db->query(" 
    SELECT * 
    FROM users 
") or $db->raise_error(); // Leaving 'raise_error()' blank will create an error message with the SQL 
$db->free_result($users); 

/** 
* Example 8 - Turning error reporting on 
*/ 
$db->show_errors(); 

/** 
* Example 9 - Turning error reporting off 
*/ 
$db->hide_errors(); 

/** 
* Example 10 - Preparing a value for database queries 
* 
* Will use mysql_real_escape_string or mysql_escape_string 
* depending on your PHP version. 
*/ 
$name = $db->prepare(trim(strip_tags($_POST['name']))); 
$email = $db->prepare(trim(strip_tags($_POST['email']))); 

$db->query(" 
    INSERT INTO users (name, email, is_active) 
    VALUES ('$name', '$email', 0) 
") or $db->raise_error('Failed adding new user'); // Will use the message we give it + the SQL 

/** 
* Example 11 - Preparing a value for database queries + escaping for LIKE queries 
* 
* Will use mysql_real_escape_string or mysql_escape_string 
* depending on your PHP version. 
*/ 
$email = $db->prepare(trim(strip_tags($_POST['email'])), true); 

$db->query(" 
    SELECT * 
    FROM users 
    WHERE email LIKE '%$email%' 
") or $db->raise_error(); // Leaving 'raise_error()' blank will create an error message with the SQL 

/** 
* Closing the database connection 
*/ 
$db->close(); 
?>