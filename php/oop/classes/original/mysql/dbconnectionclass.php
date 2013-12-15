<?php
class Connection
      {
        var $db_connection = null;        // Database connection string
        var $db_server = null;            // Database server
        var $db_database = null;          // The database being connected to
        var $db_username = null;          // The database username
        var $db_password = null;          // The database password
        var $CONNECTED = false;           // Determines if connection is established

        /** NewConnection Method
         * This method establishes a new connection to the database. */
        public function NewConnection($server, $database, $username, $password)
        {
            // Assign variables
            global $db_connection, $db_server, $db_database, $db_username, $db_password;
            $db_server = $server;
            $db_database = $database;
            $db_username = $username;
            $db_password = $password;

            // Attempt connection
            try
            {
                // Create connection to MYSQL database
                // Fourth true parameter will allow for multiple connections to be made
                $db_connection = mysql_connect ($server, $username, $password, true);
                mysql_select_db ($database);
                if (!$db_connection)
                {
                    throw new Exception('MySQL Connection Database Error: ' . mysql_error());
                }
                else
                {
                    $CONNECTED = true;
                }
            }
            catch (Exception $e)
            {
                echo $e->getMessage();
            }
        }

        /** Open Method
        * This method opens the database connection (only call if closed!) */
        public function Open()
        {
            global $db_connection, $db_server, $db_database, $db_username, $db_password, $CONNECTED;
            if (!$CONNECTED)
            {
                try
                {
                    $db_connection = mysql_connect ($db_server, $db_username, $db_password);
                    mysql_select_db ($db_database);
                    if (!$db_connection)
                    {
                        throw new Exception('MySQL Connection Database Error: ' . mysql_error());
                    }
                    else
                    {
                        $CONNECTED = true;
                    }
                }
                catch (Exception $e)
                {
                    echo $e->GetMessage();
                }
            }
            else
            {
                return "Error: No connection has been established to the database. Cannot open connection.";
            }
        }

        /** Close Method
         * This method closes the connection to the MySQL Database */
         public function Close()
         {
            global $db_connection, $CONNECTED;
            if ($CONNECTED)
            {
                mysql_close($db_connection);
                $CONNECTED = false;
            }
            else
            {
                return "Error: No connection has been established to the database. Cannot close connection.";
            }
         }

      }
?>