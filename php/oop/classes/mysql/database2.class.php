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
	
	public $data = array(array());
	public $row = array();

	var $lastQuery;

	# the constructor initializes the mysql connection
	public function database() {
	
		$this->hostname = '127.0.0.1';
		$this->username = 'root';
		$this->password = '';
		$this->database = 'dev';
		
		$connection = @mysql_connect($this->hostname, $this->username, $this->password);
		@mysql_select_db($this->database, $connection) or die(mysql_error());
		
		return true;
	}

	# selects the data from the table
	public function select($query) {
		
		$this->query = $query;

		$this->lastQuery = $this->query;
		$this->result = @mysql_query($this->query);
		
		if ($this->result) {

			while ($this->row = mysql_fetch_array($this->result, MYSQL_ASSOC)) {
				$this->data[$this->row['id']]['file_name'] = $this->row['file_name'];
				$this->data[$this->row['id']]['date_added'] = $this->row['date_added'];
			}
			#var_dump ($this->data);
			return true;	
		}
		
		# if it makes it here there is a problem!
		echo mysql_error();
		return false;
	}
}

/*
include 'mysql2.class.php';

$photos = new database();
$query = "SELECT id, file_name, date_added FROM photos LIMIT 8";
$photos->select($query);
*/
?>
