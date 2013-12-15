<?php
class User{
private $firstName;
private $lastName;
private $email;
private $table='users';
public function __construct(){}
// set first name
public function setFirstName($firstName){
$this->firstName=$firstName;
}
// set last name
public function setLastName($lastName){
$this->lastName=$lastName;
}
// set email
public function setEmail($email){
$this->email=$email;
}
// fetch row
public function fetch($id){
if(!$row=mysql_query("SELECT * FROM $this->table WHERE id='$id'")){
throw new Exception('Error fetching row');
}
return $row;
}
// insert row
public function insert(){
if(!mysql_query("INSERT INTO $this->table (firstname,lastname,email) VALUES ($this->firstName,$this->lastName,$this->email)")){
throw new Exception('Error inserting row');
}
}
// update row
public function update($id){
if(!mysql_query("UPDATE $this->table SET firstname='$this->firstName,lastname=$this->lastName,email=$this->email WHERE id='$id'")){
throw new Exception('Error updating row');
}
}
// delete row
public function delete($id){
if(!mysql_query("DELETE FROM $this->table WHERE id='$id'")){
throw new Exception('Error deleting row');
}
}
}


// SPACE


try{
$user=new User();
// set first name
$user->setFirstName('Alejandro');
// set last name
$user->setlastName('Gervasio');
// set email
$user->setEmail('alejandro@domain.com');
// insert row
$user->insert();

// set first name
$user->setFirstName('Alejandro');
// set last name
$user->lastName('Gervasio');
// set email
$user->setEmail('alex@domain.com');
// update row
$user->update(1);
// delete row
$user->delete(1);
}
catch(Exception $e){
echo $e->getMessage();
exit();
}


// SPACE


// define 'MySQL' class
class MySQL{
private $result;
public function __construct($host='localhost',$user='user',$password='password',$database='database'){
// connect to MySQL and select database
if(!$conId=mysql_connect($host,$user,$password)){
throw new Exception('Error connecting to the server');
}
if(!mysql_select_db($database,$conId)){
throw new Exception('Error selecting database');
}
}
// run SQL query
public function query($query){
if(!$this->result=mysql_query($query)){
throw new Exception('Error performing query '.$query);
}
}
// fetch one row
public function fetchRow(){
while($row=mysql_fetch_array($this->result)){
return $row;
}
return false;
}
// fetch all rows
public function fetchAll($table='default_table'){
$this->query('SELECT * FROM '.$table);
$rows=array();
while($row=$this->fetchRow()){
$rows[]=$row;
}
return $rows;
}
}


// SPACE


try{
// connect to MySQL and select a database
$db=new MySQL('host','user','password','mydatabase');
$result=$db->fetchAll('users');
foreach($result as $row){
echo $row['firstname'].' '.$row['lastname'].' '.$row['email'].'<br />';
}

/* displays the following

Alejandro Gervasio alejandro@domain.com
John Doe john@domain.com
Susan Norton susan@domain.com
Marian Wilson marian@domain.com
Mary Smith mary@domain.com
Amanda Bears amanda@domain.com
Jodie Foster jodie@domain.com
Laura Linney laura@domain.com
Alice Dern alice@domain.com
Jennifer Aniston jennifer@domain.com

*/
}
catch(Exception $e){
echo $e->getMessage();
exit();
}


// SPACE


class MySQL{
private $result;
public function __construct($host='localhost',$user='user',$password='password',$database='database'){
// connect to MySQL and select database
if(!$conId=mysql_connect($host,$user,$password)){
throw new Exception('Error connecting to the server');
}
if(!mysql_select_db($database,$conId)){
throw new Exception('Error selecting database');
}
}
// run SQL query
public function query($query){
if(!$this->result=mysql_query($query)){
throw new Exception('Error performing query '.$query);
}
}
// fetch one row
public function fetchRow(){
while($row=mysql_fetch_array($this->result)){
return $row;
}
return false;
}
// fetch all rows
public function fetchAll($table='default_table'){
$this->query('SELECT * FROM '.$table);
$rows=array();
while($row=$this->fetchRow()){
$rows[]=$row;
}
return $rows;
}
// insert row
public function insert($params=array(),$table='default_table'){
$sql='INSERT INTO '.$table.' ('.implode(',',array_keys($params)).') VALUES (''.implode("','",array_values($params)).'')';
$this->query($sql);
}
}


// SPACE


try{
// connect to MySQL and select a database
$db=new MySQL('host','user','password','mydatabase');
// insert new row into selected MySQL table
$db->insert(array('firstname'=>'Kate','lastname'=>'Johanson','email'=>'kate@domain.com'),'users');
}
catch(Exception $e){
echo $e->getMessage();
exit();
}
?>