<?php /** db class : Database abstraction class **/
class db { function db() {}
	var $hostName = "";
	var $hostport = "";
	var $dbName = "";
	var $dbUser = "";
	var $dbPwd = "";
	var $reserved_words = array("ADD","ANALYZE","ASC","BDB","BETWEEN","BLOB","CALL","CHANGE","CHECK","COLUMNS","CONSTRAINT","CROSS","CURRENT_TIMESTAMP","DATABASES","DAY_MINUTE","DECIMAL","DELAYED","DESCRIBE","DISTINCTROW","DROP","ENCLOSED",
		"EXIT","FETCH","FOR","FOUND","FULLTEXT","HAVING","HOUR_MINUTE","IGNORE","INFILE","INOUT","INT","INTO","ITERATE","KEYS","LEAVE","LIMIT","LOCALTIME","LONG","LOOP","MATCH","MEDIUMTEXT",
		"MINUTE_SECOND","NOT","NUMERIC","OPTION","ORDER","OUTFILE","PRIVILEGES","READ","REGEXP","REPLACE","RETURN","RLIKE","SENSITIVE","SHOW","SONAME","SQL","SQLWARNING","SQL_SMALL_RESULT","SQL_TSI_HOUR","SQL_TSI_QUARTER","SQL_TSI_YEAR",
		"STRAIGHT_JOIN","TABLES","TIMESTAMPADD","TINYINT","TRAILING","UNION","UNSIGNED","USE","UTC_DATE","VALUES","VARCHARACTER","WHERE","WRITE","ZEROFILL","ALL","AND","ASENSITIVE","BEFORE","BIGINT","BOTH","CASCADE",
		"CHAR","COLLATE","CONDITION","CONTINUE","CURRENT_DATE","CURSOR","DAY_HOUR","DAY_SECOND","DECLARE","DELETE","DETERMINISTIC","DIV","ELSE","ESCAPED","EXPLAIN","FIELDS","FORCE","FRAC_SECOND","GRANT","HIGH_PRIORITY","HOUR_SECOND",
		"IN","INNER","INSENSITIVE","INTEGER","IO_THREAD","JOIN","KILL","LEFT","LINES","LOCALTIMESTAMP","LONGBLOB","LOW_PRIORITY","MEDIUMBLOB","MIDDLEINT","MOD","NO_WRITE_TO_BINLOG","ON","OPTIONALLY","OUT","PRECISION","PROCEDURE",
		"REAL","RENAME","REQUIRE","REVOKE","SECOND_MICROSECOND","SEPARATOR","SMALLINT","SPATIAL","SQLEXCEPTION","SQL_BIG_RESULT","SQL_TSI_DAY","SQL_TSI_MINUTE","SQL_TSI_SECOND","SSL","STRIPED","TERMINATED","TIMESTAMPDIFF","TINYTEXT","TRUE","UNIQUE","UPDATE",
		"USER_RESOURCES","UTC_TIME","VARBINARY","VARYING","WHILE","XOR","ALTER","AS","AUTO_INCREMENT","BERKELEYDB","BINARY","BY","CASE","CHARACTER","COLUMN","CONNECTION","CREATE","CURRENT_TIME","DATABASE","DAY_MICROSECOND","DEC",
		"DEFAULT","DESC","DISTINCT","DOUBLE","ELSEIF","EXISTS","FALSE","FLOAT","FOREIGN","FROM","GROUP","HOUR_MICROSECOND","IF","INDEX","INNODB","INSERT","INTERVAL","IS","KEY","LEADING","LIKE",
		"LOAD","LOCK","LONGTEXT","MASTER_SERVER_ID","MEDIUMINT","MINUTE_MICROSECOND","NATURAL","NULL","OPTIMIZE","OR","OUTER","PRIMARY","PURGE","REFERENCES","REPEAT","RESTRICT","RIGHT","SELECT","SET","SOME","SPECIFIC",
		"SQLSTATE","SQL_CALC_FOUND_ROWS","SQL_TSI_FRAC_SECOND","SQL_TSI_MONTH","SQL_TSI_WEEK","STARTING","TABLE","THEN","TINYBLOB","TO","UNDO","UNLOCK","USAGE","USING","UTC_TIMESTAMP","VARCHAR","WHEN","WITH","YEAR_MONTH");

	function connect($hostName = "", $dbUser = "", $dbPwd = "", $dbName = "", $port = "") {
		if ($hostName != "") $this -> hostName = $hostName;
		if ($dbUser != "") $this -> dbUser = $dbUser;
		if ($dbPwd != "") $this -> dbPwd = $dbPwd;
		if ($dbName != "") $this -> dbName = $dbName;
		if ($port != "") $this -> hostport = $port;
		$this -> Link_ID = @mysql_connect($this -> hostName, $this -> dbUser, $this -> dbPwd) or die (_CONNECTION_ERROR_);
		@mysql_select_db($this -> dbName) or die (_CONNECTION_ERROR_ . $this -> error() . "<br /><br />");
		return $this -> Link_ID;
	}

	function disconnect() {
		@mysql_close($this -> Link_ID);
	}

	function query($query) {
		$this -> dbQryResult = @mysql_query($query) or die (_QUERY_ERROR_ . mysql_error() . "<br /><br />");
		return $this -> dbQryResult;
	}

	function free_results() {
		return mysql_free_result ($this -> dbQryResult);
	}

	function fetch_row($result = "") {
		$this -> dbResultLine = @mysql_fetch_row($result);
		return $this -> dbResultLine;
	}

	function get_data($result = "") {
		if ($result != "") return $this -> fetch_row($result);
		else return $this -> fetch_row($this -> dbQryResult);
	}

	function fetch_array($result = "") {
		$this -> dbResultLine = @mysql_fetch_array($result);
		return $this -> dbResultLine;
	}

	function num_rows() {
		return @mysql_num_rows($this -> dbQryResult);
	}

	function list_tables($dbname) {
		$this -> dbResultLine = @mysql_list_tables($dbname);
		return $this -> dbResultLine;
	}

	function get_db_tables() {
		global $GonxAdmin;
		$result = @$this -> list_tables($this -> dbName);
		if (!$result) {
			print "Error : impossible de lister les tables\n";
			print 'Error ' . $GonxAdmin["dbtype"] . ' : ' . $this -> error();
			exit;
		}
		while ($row = $this -> fetch_row($result)) $Tables[] = $row[0];
		return $Tables;		
	}

	function list_dbs() {
		$this -> dbResultLine = @mysql_list_dbs($this -> Link_ID);
		return $this -> dbResultLine;
	}

	function get_dbs() {
		global $GonxAdmin;
		$result = $this -> list_dbs();
		if (!$result) {
			print "Error : impossible de lister les bases de donn&eacute;es\n";
			print 'Error ' . $GonxAdmin["dbtype"] . ' : ' . $this -> error();
			exit;
		}
		while ($row = $this -> fetch_row($result)) $DBS[] = $row[0];
		return $DBS;
	}

	function error() {
		return @mysql_error();
	}

	function escape_string($string = "") {
		return mysql_escape_string($string);
	}

	function valid_table_name($tbl_name) {
		if (in_array(strtoupper($tbl_name), $this -> reserved_words)) return FALSE;
		return TRUE;
	}

	function signature() {
		global $GonxAdmin;
		if ($GonxAdmin["dbtype"] == "mysql") {
		    $q1 = "SHOW VARIABLES LIKE 'version'";
			$q2 = "SHOW STATUS LIKE 'Questions'";
			$q3 = "SHOW STATUS LIKE 'Uptime'";
			if (!is_resource($this -> Link_ID)) return FALSE;
			$qres = $this -> query($q3);
			list($n,$uptime) = $this -> fetch_array($qres);
			$qres = $this -> query($q2);
			list($n,$queries) = $this -> fetch_array($qres);
			$qres = $this -> query($q1);
			list($n,$version) = $this -> fetch_array($qres);
			$qpersecond = number_format(($queries / $uptime), 4);
			$uptime = (integer)($uptime / (60 * 60 * 24));
			$Message = "\n<br /><br />MySQL Version <b>$version</b>, Uptime = <b>$uptime day(s)</b> and running <b>$queries queries</b> (<b>$qpersecond q/s</b>)<br />\n";
			return $Message;
		}
	}
}
?>