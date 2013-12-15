<?php
$config_file= "../config.php";
if(!file_exists($config_file))
	die("Cannot find config file.");
if(!is_writable($config_file) && !@chmod($config_file, 0666))
		die("The config file is not writable.");

$handle= fopen($config_file, "r");
$contents= fread($handle, filesize($config_file));
fclose($handle);

preg_match_all("/'(.*)'/", $contents, $matches);
preg_match_all("/\"(.*)\"/", $contents, $dmatche);

if (!isset($_POST['Submit'])) {
	$f = '
		<form name="step1" method="post" action="">
		<table width="260" border="0">
		  <tr>
		    <td width="80"><div align="right">Site Name:</div></td>
		    <td width="15">&nbsp;</td>
		    <td width="165">
		    	<input name="title" type="text" maxlength="32" value="'.(empty($dmatche[1][6])?'':$dmatche[1][6]).'">
		    </td>
		  </tr>
		  <tr>
		  <tr>
		    <td>&nbsp;</td>
		    <td>&nbsp;</td>
		    <td>&nbsp;</td>
		  </tr>
		    <td><div align="right">Host:</div></td>
		    <td>&nbsp;</td>
		    <td>
		    	<input name="host" type="text" maxlength="32" value="'.(empty($matches[1][0])?'':$matches[1][0]).'">
		    </td>
		  </tr>
		  <tr>
		    <td><div align="right">Username:</div></td>
		    <td>&nbsp;</td>
		    <td>
		    	<input name="user" type="text" maxlength="32" value="'.(empty($matches[1][1])?'':$matches[1][1]).'">
		    </td>
		  </tr>
		  <tr>
		    <td><div align="right">Password:</div></td>
		    <td>&nbsp;</td>
		    <td>
		    	<input name="pass" type="password" maxlength="32" value="'.(empty($matches[1][2])?'':$matches[1][2]).'">
		    </td>
		  </tr>
		  <tr>
		    <td><div align="right">Database:</div></td>
		    <td>&nbsp;</td>
		    <td>
		    	<input name="db" type="text" maxlength="32" value="'.(empty($matches[1][3])?'':$matches[1][3]).'">
		    </td>
		  </tr>
		  <tr>
		    <td><div align="right"></div></td>
		    <td>&nbsp;</td>
		    <td>
		    	<input type="submit" name="Submit" value="Next">
		    </td>
		  </tr>
		</table>
		</form>';
	echo $f;
}
elseif ($_POST['Submit'] == "Next") {
	if(!isset($_POST['host']))
		die("Please enter a valid MySQL host.");
	if(!isset($_POST['user']))
		die("Please enter a valid MySQL username.");
	if(!isset($_POST['pass']))
		die("Please enter a valid MySQL password.");
	if(!isset($_POST['db']))
	 die("Please enter a valid MySQL database.");
	if(!isset($_POST['title']))
	 die("Please enter a valid title.");
	if(!@mysql_connect($_POST['host'], $_POST['user'], $_POST['pass']))
	 die(mysql_error());
	if(!@mysql_select_db($_POST['db'])) {
		echo mysql_error()."<br>\r\n";
		if (mysql_query('CREATE DATABASE '.$_POST['db'].';'))
			echo "Database '".$_POST['db']."' created successfully\n";
		else
			die(mysql_error());
	}
	mysql_close();

	$tag= "\"DB_HOST\", '";
	$contents= str_replace($tag.$matches[1][0], $tag.$_POST['host'], $contents);
	$tag= "\"DB_USER\", '";
	$contents= str_replace($tag.$matches[1][1], $tag.$_POST['user'], $contents);
	$tag= "\"DB_PWD\", '";
	$contents= str_replace($tag.$matches[1][2], $tag.$_POST['pass'], $contents);
	$tag= "\"DB_NAME\", '";
	$contents= str_replace($tag.$matches[1][3], $tag.$_POST['db'], $contents);
	$tag= "\$GLOBALS['page_title'] = \"";
	$contents= str_replace($tag.$matches[1][6], $tag.addslashes($_POST['title']), $contents);

	$handle= @fopen($config_file, "w") or die("The config file is not writable.");
	if (@fwrite($handle, $contents) === FALSE)
		die("Cannot write config file.");
	fclose($handle);

	$f='
		<form name="step2" method="post" action="">
		<table width="260" border="0">
		  <tr>
		    <td><div align="right">Admin:</div></td>
		    <td>&nbsp;</td>
		    <td><input name="admin" type="text" maxlength="32"></td>
		  </tr>
		  <tr>
		    <td><div align="right">Password:</div></td>
		    <td>&nbsp;</td>
		    <td><input name="pass" type="password" maxlength="32"></td>
		  </tr>
		  <tr>
		    <td><div align="right"></div></td>
		    <td>&nbsp;</td>
		    <td><input type="submit" name="Submit" value="Finish"></td>
		  </tr>
		</table>
		</form>';
		echo $f;
}
elseif ($_POST['Submit'] == "Finish") {
	@include($config_file);
	if (@mysql_errno()) die("Cannot use MySQL database");
	//mysql_query($GLOBALS['query']) or die(mysql_error());
	mysql_query("DROP TABLE IF EXISTS `words`;") or die(mysql_error());
	mysql_query("
		CREATE TABLE `words` (
		  `word_id` int(10) unsigned NOT NULL auto_increment,
		  `word_title` varchar(255) NOT NULL default '',
		  `word_desc` text NOT NULL,
		  `word_comments` text NOT NULL,
		  `word_mdate` datetime NOT NULL default '0000-00-00 00:00:00',
		  `word_cdate` datetime NOT NULL default '0000-00-00 00:00:00',
		  `found` tinyint(3) unsigned NOT NULL default '0',
		  PRIMARY KEY  (`word_id`),
		  KEY `word_title` (`word_title`(5)),
		  FULLTEXT KEY `word_desc` (`word_desc`)
		) TYPE=MyISAM ;
	") or die(mysql_error());
	mysql_query("DROP TABLE IF EXISTS `admin`;") or die(mysql_error());
	mysql_query("
		CREATE TABLE `admin` (
		  `id` tinyint(4) NOT NULL auto_increment,
		  `name` varchar(16) NOT NULL default '',
		  `password` varchar(64) NOT NULL default '',
		  PRIMARY KEY  (`id`),
		  UNIQUE KEY `name` (`name`)
		) TYPE=MyISAM AUTO_INCREMENT=2;
	") or die(mysql_error());
	mysql_query("INSERT INTO `admin` VALUES (1, '".addslashes($_POST['admin'])."', '".md5($_POST['pass'])."');") or die(mysql_error());
	mysql_close();
	echo "Setup complete, please make sure to delete the install/ folder.";
}
?>