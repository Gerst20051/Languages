<?php 

/**
 * MySQL database class
 * @author gabe@fijiwebdesign.com
 * 
 * Invocation example
 * $config = array('host' => $host, 'user' => $user, 'password' => $password, 'database' => $database);
 * $DB = Database_MySQL::getInstance($config);
 * $rows = $DB->getRowList("select * from table1 where col1 = %d and col2 = '%s' LIMIT 1", 5, '6');
 * 
 */
class Database_MySQL {
	
	protected $result;
	protected $resource;
	
	/**
	 * Construct and optionally connect to database
	 */
	public function __construct($config) {
		extract($config);
		if (isset($host) && isset($user) && isset($password) && isset($database)) {
			return $this->connect($config);
		}
	}
	
	/**
	 * Retrieve a Singleton Instance of this class
	 */
	public static function getInstance($config) {
		extract($config);
		static $Instances = array();
		$key = "$host:$user:$password:$database";
		if (!isset($Instances[$key])) {
			$Instances[$key] = new Database_MySQL($config);
		}
		return $Instances[$key];
	}
	
	/**
	 * Connect to the MySQL Database
	 * @param $host String Host 
	 * @param $user String Username
	 * @param $password Password
	 * @param $database Database name
	 * @return Bool
	 */
	public function connect($config) {
		extract($config);
		if ($this->resource = mysql_connect($host, $user, $password)) {
			return mysql_select_db($database);
		}
		return false;
	}
	
	protected function setQuery($args) {
		call_user_func_array(array($this, 'query'), $args);
	}
	
	/**
	 * Execute a Query 
	 * @param $query SQL
	 * @param Int|String optional parameters to interpolate with SQL
	 * @return Bool
	 */
	public function query($query) {
		$args = func_get_args();
		if (count($args) > 1) {
			array_shift($args);
			$args = array_map('mysql_real_escape_string', $args);
			array_unshift($args, $query);
			$query = call_user_func_array('sprintf', $args);
		}
		//echo "SEND QUERY: $query\n";
		if (!$this->result = mysql_query($query)) {
			echo "QUERY ERROR: $query\n";
			throw new Exception('Query failed: '.mysql_error());
		}
		return $this->result;
	}

	/**
	 * Retrieve a an Array of Objects from query resultset
	 * @param $query SQL
	 * @param Int|String optional parameters to interpolate with SQL
	 * @return Array
	 */
	public function getRowList($query = null) {
		if ($query) {
			$args = func_get_args();
			$this->setQuery($args);
		}
		if ($this->result) {
			$rows = array();
			while($row = mysql_fetch_assoc($this->result)) {
				$rows[] = (object) $row;
			}
			return $rows;
		} else {
			return false;
		}
	}
	
	/**
	 * Retrieve a single row in query resultset as Object
	 * @param $query SQL
	 * @param Int|String optional parameters to interpolate with SQL
	 * @return Object
	 */
	public function getRow($query = null) {
		if ($query) {
			$args = func_get_args();
			$this->setQuery($args);
		}
		if ($this->result) {
			if ($row = mysql_fetch_assoc($this->result)) {
				return (object) $row;
			}
		}
		return false;
	}
	
	/**
	 * Retrieve a single result
	 * @param $query String
	 * @param Int|String optional parameters to interpolate with SQL
	 * @return String
	 */
	public function getResult($query = null) {
		if ($query) {
			$args = func_get_args();
			$this->setQuery($args);
		}
		if ($this->result) {
			if ($row = mysql_fetch_row($this->result)) {
				return $row[0];
			}
		}
		return false;
	}
	
	/**
	 * Retrieve number of rows affected in last insert|update query
	 * @return Int
	 */
	public function getAffectedRows() {
		return mysql_affected_rows($this->resource);
	}
	
	/**
	 * Get the auto-increment column of the last insert
	 * @return Int|false
	 */
	public function getInsertId() {
		return mysql_insert_id($this->resource);
	}
	
}

/*
// connect
$config = array(
  'host' => $host, 'user' => $user, 'password' => $password, 'database' => $database);
$DB = Database_MySQL::getInstance($config);
 
// set query
$DB->query("select * from table1 where col1 = %d and col2 = '%s' LIMIT 1", 5, 'a');
 
// get rows
$rows = $DB->getRowList();
 
// display rows
foreach($rows as $row) {
  echo $row->col1 . ' : ' . $row->col2 . '<br />';
}
*/
?>