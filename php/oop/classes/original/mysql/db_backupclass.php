<?php
   class Backup
       {
               /**
                * @var stores the options
                */
               var $config;

               /**
                * @var stores the final sql dump
                */
               var $dump;

               /**
                * @var stores the table structure + inserts for every table
                */
               var $struktur = array();

               /**
                * @var zip file name
                */
               var $datei;

               /**
                * this function is the constructor and phrase the options
                * and connect to the database
                * @return
                */
               public function Backup($options)
               {
                       // write options
                       foreach($options AS $name => $value)
                       {
                               $this->config[$name] = $value;
                       }

                       // check mysql connection
                       mysql_connect($this->config['mysql'][0], $this->config['mysql'][1], $this->config['mysql'][2]) or die(mysql_error());
                       mysql_select_db($this->config['mysql'][3]) or die(mysql_error());
               }

               /**
                * this function start the backup progress its the core function
                * @return
                */
               public function backupDB()
               {
                       // start backup
                       if(isset($_POST['backup']))
                       {
                               // check if tables are selected
                               if(empty($_POST['table']))
                               {
                                       die("Please select a table.");
                               }

                               /** start backup **/
                               $tables = array();
                               $insert = array();
                               $sql_statement = '';

                               // lock tables
                               foreach($_POST['table'] AS $table)
                               {
                                        mysql_query("LOCK TABLE $table WRITE");

                                       // Read table structure
                                       $res = mysql_query('SHOW CREATE TABLE '.$table.'');
                                       $createtable = mysql_result($res, 0, 1);
                                       $str = "\n\n".$createtable."\n\n";

                                       array_push($tables, $str);

                                       // Read table "inserts"
                           $sql = 'SELECT * FROM '.$table;
                           $query = mysql_query($sql) or die(mysql_error());
                           $feld_anzahl = mysql_num_fields($query);

                                       $sql_statement = '--
-- Data Table `$table`
--

                                               ';

                                       // start reading progress
                           while($ds = mysql_fetch_object($query)){
                               $sql_statement .= 'INSERT INTO `'.$table.'` (';

                               for ($i = 0;$i <$feld_anzahl;$i++){
                                   if ($i ==$feld_anzahl-1){
                                       $sql_statement .= mysql_field_name($query,$i);
                                   } else {
                                       $sql_statement .= mysql_field_name($query,$i).', ';
                                   }
                               }

                               $sql_statement .= ') VALUES (';

                               for ($i = 0;$i <$feld_anzahl;$i++){
                                   $name = mysql_field_name($query,$i);
                                   if (empty($ds->$name)){
                                       $ds->$name = 'NULL';
                                   }
                                   if ($i ==$feld_anzahl-1){
                                       $sql_statement .= '"'.$ds->$name.'"';
                                   } else {
                                       $sql_statement .= '"'.$ds->$name.'", ';
                                   }
                               }
                               $sql_statement .= ");\n";
                           }

                                       // insert "Inserts" into an array if not exists
                                       if(!in_array($sql_statement, $insert))
                                       {
                                               array_push($insert, $sql_statement);
                                               unset($sql_statement);
                                       }

                                       unset($sql_statement);

                               }

                               // put table structure and inserts together in one var
                               $this->struktur = array_combine($tables, $insert);

                               // create full dump
                               $this->createDUMP($this->struktur);

                               // create zip file
                               $this->createZIP();

                               /** end backup **/

                               // send an email with the sql dump
                               if(isset($this->config['email']) && !empty($this->config['email']))
                               {
                                       $this->sendEmail();
                               }

                               // output
                               echo '<h3 style="color:green;">Backup war erfolgreich</h3><a href="'.$this->datei.'">Download Backup</a>
                               <br />
                               <br />';
                       }
               }

               /**
                * this function generate an email with attachment
                * @return
                */
               protected function sendEmail()
               {
                               // start sending emails
                               foreach($this->config['email'] AS $email)
                               {
                                       $to = $email;

                                       $from = $this->config['email'][0];

                                       $message_body = "This email contains the database backup as a zip file.";

                                       $msep = strtoupper (md5 (uniqid (time ())));

                                       // set email header (only text)
                                       $header =
                                                 "From: $from\r\n" .
                                                 "MIME-Version: 1.0\r\n" .
                                                 "Content-Type: multipart/mixed; boundary="$msep"\r\n\r\n" .
                                                 "--$msep\r\n" .
                                                 "Content-Type: text/plain\r\n" .
                                                 "Content-Transfer-Encoding: 8bit\r\n\r\n" .
                                                 $message_body . "\r\n";

                                       // file name
                                       $dateiname = $this->datei;

                                       // get filesize of zip file
                                       $dateigroesse = filesize ($dateiname);

                                       // open file to read
                                       $f = fopen ($dateiname, "r");
                                       // save content
                                       $attached_file = fread ($f, $dateigroesse);
                                       // close file
                                       fclose ($f);

                                       // create attachment
                                       $attachment = chunk_split (base64_encode ($attached_file));

                                       // set attachment header
                                       $header .=
                                                  "--" . $msep . "\r\n" .
                                                  "Content-Type: application/zip; name='Backup'\r\n" .
                                                  "Content-Transfer-Encoding: base64\r\n" .
                                                  "Content-Disposition: attachment; filename='Backup.zip'\r\n" .
                                                  "Content-Description: Mysql Datenbank Backup im Anhang\r\n\r\n" .
                                                  $attachment . "\r\n";

                                       // mark end of attachment
                                       $header .= "--$msep--";

                                       // eMail Subject
                                       $subject = "Database Backup";

                                       // send email to emails^^
                                       if(mail($to, $subject, '', $header) == FALSE)
                                       {
                                               die("The email could not be sent. Please check the email address.");
                                       }

                                       echo "<p><small>Email was successfully sent.</small></p>";
                               }
               }

               /**
                * this function create the zip file with the database dump and save it on the ftp server
                * @return
                */
               protected function createZIP()
               {

                       // Set permissions to 777
                       chmod($this->config['folder'], 0777);

                       // create zip file
                       $zip = new ZipArchive();
                       // Create file name
                       $this->datei = $this->config['folder'].$this->config['mysql'][3]."_".date("j_F_Y_g:i_a").".zip";

                       // Checking if file could be created
                       if ($zip->open($this->datei, ZIPARCHIVE::CREATE)!==TRUE) {
                               exit("cannot open <".$this->datei.">\n");
                       }

                       // add mysql dump to zip file
                       $zip->addFromString("dump.sql", $this->dump);
                       // close file
                       $zip->close();

                       // Check whether file has been created
                       if(!file_exists($this->datei))
                       {
                               die("The ZIP file could not be created.");
                       }

                       echo "<p><small>The zip was created.</small></p>";
               }

               /**
                * this function create the full sql dump
                * @param object $dump
                * @return
                */
               protected function createDUMP($dump)
               {
                       $date = date("F j, Y, g:i a");

                       $header = <<<HEADER
-- SQL Dump
--
-- Host: {$_SERVER['HTTP_HOST']}
-- Erstellungszeit: {$date}

--
-- Datenbank: `{$this->config['mysql'][3]}`
--

-- --------------------------------------------------------

HEADER;
                       foreach($dump AS $name => $value)
                       {
                               $sql .= $name.$value;
                       }
                       $this->dump = $header.$sql;
               }

               /**
                * this function displays the output form to select tables
                * @return
                */
               public function outputForm()
               {
                       // select all tables from database
                       $result = mysql_list_tables($this->config['mysql'][3]);

                       $buffer = '
                       <fieldset>
                               <legend>Select some tables</legend>
                               <form method="post" action="">
                       <select name="table[]" multiple="multiple" size="30">';
                       while($row = mysql_fetch_row($result))
                       {
                               $buffer .= '<option value="'.$row[0].'">'.$row[0].'</option>';
                       }
                       $buffer .= '</select>
                       <br /><br />
                       <input type="submit" name="backup" value="Backup Tables" />
                       </form>
                       </fieldset>';

                       echo $buffer;
               }
       }
?>
<?php

       //You can add as many email addresses as you like
       $options = array('email' => array('email1', 'email2'),
                                  'folder' => './backup/',
                                  'mysql' => array('localhost', 'root', '****', 'database'));

       $b = new Backup($options);

       // if submit form start backup
       if(isset($_POST['backup']))
       {
               // start backup
               $b->backupDB();
       }

       // display tables
       $b->outputForm();

?>