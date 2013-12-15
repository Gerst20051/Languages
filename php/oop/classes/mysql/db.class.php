<?php 
/**************************************
 seesaw associates | http://seesaw.net

 client: 		mysql
 file: 			class.mysql.php
 description: 	handles mysql paging

 Copyright (C) 2008 Matt Kenefick(.com)
**************************************/

class DB{
	var $host;
	var $user_name;
	var $password;
	var $db_name;
	
	var $link_id;
	var $result;
	var $col;
	var $query;
	var $fields;
	var $records;
	var $setting;
	
	var $debug = false;
	var $query_count = 0;
	var $debug_file = "debug.sql";
	
	function settings($key,$value){
		$this->setting[$key] = $value;
	}

	function init($_host, $_user, $_password, $_db_name){
		$this->host = $_host;
		$this->user_name = $_user;
		$this->password = $_password;
		$this->db_name = $_db_name;
		$this->fields = array();
		
		$this->link_id = @mysql_connect($_host, $_user, $_password) or die("Your website is not properly installed.");
		@mysql_select_db($_db_name, $this->link_id);
	}
	
	function assign($field, $value){
		$this->fields[$field] = ($value)==""?("'".$value."'"):$value;
	}
	function assign_str($field, $value){
		$this->fields[$field] = "'".addslashes($value)."'";
		//$this->fields[$field] = "'".($value)."'";
	}
	
	function reset(){
		$this->fields = array();
	}
	function insert($table){
		$f = "";
		$v = "";
		reset($this->fields);
		foreach($this->fields as $field=>$value){
			$f.= ($f!=""?", ":"").$field;
			$v.= ($v!=""?", ":"").$value;
		}
		$sql = "INSERT INTO ".$table." (".$f.") VALUES (".$v.")";
		$this->query($sql);
		return $this->insert_id();
	}
	
	function update($table, $where){
		$f = "";
		reset($this->fields);
		foreach($this->fields as $field=>$value){
			$f.= ($f!=""?", ":"").$field." = ".$value;
		}
		$sql = "UPDATE ".$table." SET ".$f." ".$where;
		$this->query($sql);
	}
	
	function timestampFormat($unixNumber){
		return date('Y-m-d H:i:s',$unixNumber);
		///      xxxx-xx-xx xx-xx-xx
	}
	
	function query($_query){
		list($usec, $sec) = explode(" ",microtime());
		$time_start  = ((float)$usec + (float)$sec);
		
		$this->query = $_query;
		$this->result = @mysql_query($_query, $this->link_id) or die( $_query."<p>".mysql_error($this->link_id) );
		
		list($usec, $sec) = explode(" ",microtime());
		$time_end  =  ((float)$usec + (float)$sec);
		$time = $time_end - $time_start;

		if($this->debug){
			$this->query_count ++;
			$f = fopen($this->debug_file, "a");
			$sss = "# ".$this->query_count."\n ".$time." sec \n\n".$_query."\n#-------------------------------------------------------------------------\n\n";
			fputs($f, $sss, strlen($sss));
			fclose($f);
		}
		
		return $this->result;
	}
	
	function get_records(){
		$this->records = array();
		while($row = @mysql_fetch_array($this->result, MYSQL_BOTH)){
			$this->records[count($this->records)] = $row;
		}
		reset($this->records);
		return $this->records;
	}
	
	function get_tables_status(){
		$this->query("SHOW TABLE STATUS FROM `".$this->db_name."`");
		if($this->num_rows() > 0){
			$tables = array();
			while($this->movenext()){
				$tables[$this->col["Name"]] = $this->col;
			}
			return $tables;
		}
		return false;
	}
	
	function fetch_array(){
		$this->col = @mysql_fetch_array($this->result, MYSQL_BOTH);
	}
	
	function num_rows(){
		return (int)@mysql_num_rows($this->result);
	}
	
	function fixSlashes(){
	 	if($this->col){
			foreach($this->col as $key => $value)
				$this->col[$key] = stripslashes($value);
			return $this->col;
		}
	}
		
	function movenext(){
		$this->col=@mysql_fetch_array($this->result, MYSQL_ASSOC);
		if($this->setting['fixSlashes'])
			return $this->fixSlashes();
		else
			return $this->col;
	}
	
	function done(){
		@mysql_close($this->link_id);
	}
	
	function insert_id(){
		return @mysql_insert_id($this->link_id);
	}
	
	function affected_rows(){
		return @mysql_affected_rows($this->link_id);
	}
}
?>