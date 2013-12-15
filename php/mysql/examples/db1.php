<?php
require_once("../config.php");

$connect = mysql_connect("$database_host", "$database_username", "$database_password") or die(mysql_error());
mysql_select_db("$database_name") or die(mysql_error());
if (!$connect)
  {
  die('Could not connect: ' . mysql_error());
  }

// Create table
mysql_select_db("$database_name", $connect);
$sql = "CREATE TABLE users
(
  first_name VARCHAR(255) NOT NULL,
  last_name VARCHAR(255) NOT NULL,
  blog_name VARCHAR(255) NOT NULL,
  user_role VARCHAR(255) NOT NULL,
  blog_slogan VARCHAR(255) NOT NULL,
  user_email VARCHAR(255) NOT NULL
)";

// Execute query
mysql_query($sql,$connect);

// Create table
mysql_select_db("$database_name", $connect);
$sql = "CREATE TABLE categories
(
  id INT(6) NOT NULL auto_increment,
  cat_name VARCHAR(255) NOT NULL,
  date_added VARCHAR(255) NOT NULL,
  added_by VARCHAR(255) NOT NULL,
  PRIMARY KEY  (id),
  UNIQUE KEY id (id)
) TYPE = MyISAM AUTO_INCREMENT = 1;
";

// Execute query
mysql_query($sql,$connect);

// Create table
mysql_select_db("$database_name", $connect);
$sql = "CREATE TABLE members
(
  id INT(6) NOT NULL auto_increment,
  username VARCHAR(255) NOT NULL,
  password VARCHAR(255) NOT NULL,
  PRIMARY KEY  (id),
  UNIQUE KEY id (id)
) TYPE = MyISAM AUTO_INCREMENT = 1;
";

// Execute query
mysql_query($sql,$connect);

// Create table
mysql_select_db("$database_name", $connect);
$sql = "CREATE TABLE posts
(
  id INT(6) NOT NULL auto_increment,
  post_header VARCHAR(255) NOT NULL,
  post_category VARCHAR(255) NOT NULL,
  post_content BLOB(1000000) NOT NULL,
  user_added VARCHAR(255) NOT NULL,
  date_added VARCHAR(255) NOT NULL,
  PRIMARY KEY  (id),
  UNIQUE KEY id (id)
) TYPE = MyISAM AUTO_INCREMENT = 1;
";

// Execute query
mysql_query($sql,$connect);

// Create table
mysql_select_db("$database_name", $connect);
$sql = "CREATE TABLE links
(
  id INT(6) NOT NULL auto_increment,
  link_name VARCHAR(255) NOT NULL,
  link_url VARCHAR(255) NOT NULL,
  link_desc VARCHAR(255) NOT NULL,
  added_by VARCHAR(255) NOT NULL,
  date_added VARCHAR(255) NOT NULL,
  PRIMARY KEY  (id),
  UNIQUE KEY id (id)
) TYPE = MyISAM AUTO_INCREMENT = 1;
";

// Execute query
mysql_query($sql,$connect);

mysql_select_db("$database_name", $connect);

$sql="INSERT INTO members (username, password)
VALUES
('$_POST[user_login]','$_POST[user_pass]')";

if (!mysql_query($sql,$connect))
  {
  die('Error: ' . mysql_error());
  }

mysql_select_db("$database_name", $connect);

$sql="INSERT INTO users (first_name, last_name, blog_name, blog_slogan, user_email)
VALUES
('$_POST[first_name]','$_POST[last_name]','$_POST[site_name]','$_POST[site_slogan]','$_POST[user_email]')";

mysql_query("INSERT INTO posts (post_header, post_content, post_category)
VALUES ('Welcome to PrudalPress', 'This is your first post. You can either edit it or delete it in the admin panel.','Uncategorized')");

mysql_query("INSERT INTO categories (cat_name)
VALUES ('Uncategorized')");

mysql_query("INSERT INTO links (link_name, link_url)
VALUES ('PrudalPress','http://www.example.com/')");

if (!mysql_query($sql,$connect))
  {
  die('Error: ' . mysql_error());
  }

header( 'Location: done.php' );

mysql_close($connect)
?>