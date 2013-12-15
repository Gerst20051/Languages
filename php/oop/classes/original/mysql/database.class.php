<?php

class database {

	private $hostname;
	private $username;
	private $password;
	private $database;
	private $connection;
	
	private $query;
	private $where;
	private $order;
	private $result = array();

	var $lastQuery;

	# the constructor initializes the mysql connection
	public function database() {
	
		$this->hostname = 'localhost';
		$this->username = 'root';
		$this->password = NULL;
		$this->database = 'dev';
		
		$connection = mysql_connect($this->hostname, $this->username, $this->password);
		mysql_select_db($this->database, $connection) or die(mysql_error());
		
		return true;
	
	}

	# selects the data from the table
	public function select($query, $where, $order) {
		
		$this->query = $query;
		$this->where = ($where == NULL) ? NULL : $where;
		$this->order = ($order == NULL) ? NULL : $order;
		
		$fullQuery = $this->query . $this->where . $this->order;
		$this->lastQuery = $fullQuery;
		$this->result = mysql_query($fullQuery);
		
		if ($this->result()) { return true; }
		
		return false;
		
	}
	
	# used to validate result from select()
	private function result() {
		
		if ($this->result) { return true; }
		
		echo mysql_error();
		return false;
	
	}

	# displays the data from the select() member
	public function display() {
		
		if ($this->select($this->query, $this->where, $this->order)) {
		
			echo '<h3>' . $this->lastQuery . '</h3><br />';
		
			while ($row = mysql_fetch_array($this->result, MYSQL_ASSOC)) {
				echo '<b>' . $row['id'] . ' </b>' . $row['file_name'] . '<br />';
			}
			
			return true;
			
		}
		
		return false;
	
	}

}

/*
include 'database.class.php';

$photos = new database();

$query = "SELECT * FROM photos ";
$where = NULL;
$order = NULL;

$photos->select($query, $where, $order);
$photos->display();
*/
?>
