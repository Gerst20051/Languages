<?php
/*-----------------------------------------------------------------------
Created By:   Sathish Kumar.R
Date:         30 July 2010
E-mail:       smart2raise[at]gmail[dot]com
Purpose:      Database Manipulation


Functions Available:
db:           Constructor (for mysql connect)
query:        Executing query(mysql_query)
num_rows:     Finding total rows(mysql_num_rows)
fetch_object: Fetch results in Object (mysql_fetch_object)
fetch_array:  Fetch results in Array (mysql_fetch_array)
fetch_assoc:  Fetch results in Array (mysql_fetch_assoc)
insert:       Insert query (Insert into...)
insert_id:    Gets last inserted ID (mysql_insert_id)
update        Update query (Update ...)
delete        Delete query (Delete ...)
countof:      Count function in mysql
maxof:        Max function in mysql
sumof:        Sum function in mysql
avgof:        AVG function in mysql
last_query:   Displays the query which you executed last
throw_error:  If any mysql error occurs & you set $debug = true then it will show the query & the mysql error
-------------------------------------------------------------------------*/
class db{
var $debug = false;
var $query = array();
var $prefix = "";
function db($server,$username,$pwd,$db){
 mysql_connect($server,$username,$pwd) or die('Please check your database connection');
 mysql_select_db($db);
}
function query($qry){
 $this->query[] = $qry;
 $res = mysql_query($qry);
 if(mysql_error()){
  $this->throw_error();
 }
 return $res;
}
function num_rows($res){
 return mysql_num_rows($res);
}
function fetch_object($res){
 $fet = mysql_fetch_object($res);
 return $fet;
}
function fetch_array($res){
 $fet = mysql_fetch_array($res);
 return $fet;
}
function fetch_assoc($res){
 $fet = mysql_fetch_assoc($res);
 return $fet;
}
function insert($val,$table){
 $query = 'INSERT INTO '.$table.' (';
  foreach ($val AS $key => $value)
   $query .= '`'.$key.'`,';
 $query = rtrim($query, ',').') VALUES (';
  foreach ($val AS $key => $value){
   if(get_magic_quotes_gpc())
    $query .= '\''.$value.'\',';
   else
   $query .= '\''.mysql_real_escape_string($value).'\',';
  }
 $query = rtrim($query, ',').')';
 return $this->query($query);
}
function insert_id(){
 return mysql_insert_id();
}
function update($val,$table,$con){
 if($con!=""){
   $where = "where ";
   $lastitem = end($con);
  foreach ($con AS $key => $value){
   if($value!=$lastitem){
    if(get_magic_quotes_gpc())
     $where .= $key."='".$value."' && ";
    else
    $where .= $key."='".mysql_real_escape_string($value)."' && ";
   }
   else{
    if(get_magic_quotes_gpc())
     $where .= $key."='".$value."'";
    else
     $where .= $key."='".mysql_real_escape_string($value)."'";
   }
  }
 }
 else
 {
  $where = "";
 }
 $query = 'update '.$table.' set ';
 foreach ($val AS $key => $value){
  if(get_magic_quotes_gpc())
   $query .= $key."=".'\''.$value.'\',';
  else
  $query .= '\''.mysql_real_escape_string($value).'\',';
 }
 $query = rtrim($query, ',')." ".$where;
 return $this->query($query);
}
function delete($table,$con){
 if($con!=""){
   $where = "where ";
   $lastitem = end($con);
  foreach ($con AS $key => $value){
   if($value!=$lastitem){
    if(get_magic_quotes_gpc())
     $where .= $key."='".$value."' && ";
    else
    $where .= $key."='".mysql_real_escape_string($value)."' && ";
   }
   else{
    if(get_magic_quotes_gpc())
     $where .= $key."='".$value."'";
    else
     $where .= $key."='".mysql_real_escape_string($value)."'";
   }
  }
 }
 else {
  $where = "";
 }
 return $this->query("delete from {$table} {$where}");
}
function countof($col,$table,$con="",$group=""){
 if($con!=""){
   $where = "where ";
   $lastitem = end($con);
  foreach ($con AS $key => $value){
   if($value!=$lastitem){
    if(get_magic_quotes_gpc())
     $where .= $key."='".$value."' && ";
    else
    $where .= $key."='".mysql_real_escape_string($value)."' && ";
   }
   else{
    if(get_magic_quotes_gpc())
     $where .= $key."='".$value."'";
    else
     $where .= $key."='".mysql_real_escape_string($value)."'";
   }
  }
 }
 else {
  $where = "";
 }
 if($group!="")
  $groupby = "group by ".$group;
 else
  $groupby = "";
 $query = $this->query("select count({$col}) from {$table} {$where} {$groupby}");
 $fet = $this->fetch_array($query);
 return $fet[0];
}
function maxof($col,$table,$con="",$group=""){
 if($con!=""){
   $where = "where ";
   $lastitem = end($con);
  foreach ($con AS $key => $value){
   if($value!=$lastitem){
    if(get_magic_quotes_gpc())
     $where .= $key."='".$value."' && ";
    else
    $where .= $key."='".mysql_real_escape_string($value)."' && ";
   }
   else{
    if(get_magic_quotes_gpc())
     $where .= $key."='".$value."'";
    else
     $where .= $key."='".mysql_real_escape_string($value)."'";
   }
  }
 }
 else {
  $where = "";
 }
 if($group!="")
  $groupby = "group by ".$group;
 else
  $groupby = "";
 $query = $this->query("select max({$col}) from {$table} {$where} {$groupby}");
 $fet = $this->fetch_array($query);
 return $fet[0];
}
function sumof($col,$table,$con="",$group=""){
 if($con!=""){
   $where = "where ";
   $lastitem = end($con);
  foreach ($con AS $key => $value){
   if($value!=$lastitem){
    if(get_magic_quotes_gpc())
     $where .= $key."='".$value."' && ";
    else
    $where .= $key."='".mysql_real_escape_string($value)."' && ";
   }
   else{
    if(get_magic_quotes_gpc())
     $where .= $key."='".$value."'";
    else
     $where .= $key."='".mysql_real_escape_string($value)."'";
   }
  }
 }
 else {
  $where = "";
 }
 if($group!="")
  $groupby = "group by ".$group;
 else
  $groupby = "";
 $query = $this->query("select sum({$col}) from {$table} {$where} {$groupby}");
 $fet = $this->fetch_array($query);
 return $fet[0];
}
function avgof($col,$table,$con="",$group=""){
 if($con!=""){
   $where = "where ";
   $lastitem = end($con);
  foreach ($con AS $key => $value){
   if($value!=$lastitem){
    if(get_magic_quotes_gpc())
     $where .= $key."='".$value."' && ";
    else
    $where .= $key."='".mysql_real_escape_string($value)."' && ";
   }
   else{
    if(get_magic_quotes_gpc())
     $where .= $key."='".$value."'";
    else
     $where .= $key."='".mysql_real_escape_string($value)."'";
   }
  }
 }
 else {
  $where = "";
 }
 if($group!="")
  $groupby = "group by ".$group;
 else
  $groupby = "";
 $query = $this->query("select avg({$col}) from {$table} {$where} {$groupby}");
 $fet = $this->fetch_array($query);
 return $fet[0];
}
function last_query(){
 return end($this->query);
}
function throw_error(){
 if($this->debug==true){
 $qry = "<span style="color: rgb(0, 0, 0);">".end($this->query)."</span>
";
 }
 else{
 $qry = "";
 }
 die("<div style='width:500px; margin:auto; text-align:left; color:red; font-size:12px;border:2px solid #FFD700;vertical-align:middle; line-height:19px;background:#FFFFDD;font-family:verdana;padding:3px;'>".$qry."Mysql Error: ".mysql_error());
}
}
?>

<?php
//Usage of Class:


$hostname = "hostname";
$username = "username";
$password = "";
$database = "dbname";
$db = new db($hostname,$username,$password,$database);
$db->debug = true;
//If it is set to true, then mysql error will shown the query which throws the error.

$qry = $db->query("select columnname from tablename"); //Executing query
$db->num_rows($qry); //Finding total rows

$val = array("a"=>123,"b"=>"sathish","c"=>"kumar");

$a = $db->insert($val,"tablename");
/*Note: $val should be an array, else it will throw error Key in an array should be the column name in the table & value can be any value */

$db->insert_id(); //returns the last inserted ID in the database

$val1 = array("a"=>123,"b"=>"Sathish","c"=>"Kumar");

$db->update($val1,"tablename",array("id"=>2));
/*
Update query same as insert query, you can pass multiple conditions in the 3rd argument i.e array("id"=>2,"a"=>'123')
*/

$db->delete("tablename",array("id"=>4));
//You can pass multiple conditions in 2nd argument


$db->countof("id","tablename",array("a"=>123123));


$db->maxof("id","tablename",array("a"=>123123));


$db->avgof("id","tablename",array("a"=>123123));

/*
 for countof(),maxof(),avgof() you can pass additional argument group by


$db->countof("id","tablename",array("a"=>123123),"c");

this will result as "select count(id) where a = '123123' group by c"
*/

$db->last_query() //This will return last executed query
?>