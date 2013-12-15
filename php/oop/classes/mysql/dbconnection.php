<?php
class Connection {
	var $db_connection = null;
	var $db_server = null;
	var $db_database = null;
	var $db_username = null;
	var $db_password = null;
	var $CONNECTED = false;

	public function NewConnection($server, $database, $username, $password) {
		global $db_connection, $db_server, $db_database, $db_username, $db_password;
		$db_server = $server;
		$db_database = $database;
		$db_username = $username;
		$db_password = $password;
		try {
			$db_connection = mysql_connect ($server, $username, $password, true);
			mysql_select_db ($database);
			if (!$db_connection) { throw new Exception('MySQL Connection Database Error: ' . mysql_error()); }
			else $CONNECTED = true;
		} catch (Exception $e) { echo $e->getMessage(); }
	}

	public function Open() {
		global $db_connection, $db_server, $db_database, $db_username, $db_password, $CONNECTED;
		if (!$CONNECTED) {
			try {
				$db_connection = mysql_connect ($db_server, $db_username, $db_password);
				mysql_select_db ($db_database);
				if (!$db_connection) { throw new Exception('MySQL Connection Database Error: ' . mysql_error()); }
				else $CONNECTED = true;
			} catch (Exception $e) { echo $e->GetMessage(); }
		} else return "Error: No connection has been established to the database. Cannot open connection.";
	}

	public function Close() {
		global $db_connection, $CONNECTED;
		if ($CONNECTED) {
			mysql_close($db_connection);
			$CONNECTED = false;
		} else return "Error: No connection has been established to the database. Cannot close connection.";
	}
}
?>