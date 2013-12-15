<?php
require('config.php');
require('func.php');
IsAdminLogin();
SiteMenu();

$GLOBALS['page_title'] = "Admin - " . $GLOBALS['page_title'];
if (!$GLOBALS['admin']) {
	if ($_POST['submit'] == '') {
		ShowForm();
	}
	elseif ($_POST['submit'] == 1) {
		if (CheckForm()) Redirect('admin.php');
		else ShowForm();
	}
} else {
	if ($_GET['action'] == 'edit') {
		EditWord($_GET['id']+0);
	} elseif ($_GET['action'] == 'delete') {
		DeleteWord($_GET['id']+0);
	} elseif ($_GET['action'] == 'logout') {
		Logout();
	} elseif ($_GET['action'] == 'rewrite') {
		ModRewrite();
	} elseif ($_GET['action'] == 'change')
		  if ($_POST['submit'] == 2) {
				if (ModPass()) Redirect('admin.php');
				else ChangePass();
			}
			else ChangePass();
}




include('_header.php');
echo $GLOBALS['adminContent'];
include('_footer.php');

mysql_close();


function ModRewrite() { 
	global $turn;

$handle = fopen(".htaccess", "r+");
$contents = fread($handle, filesize(".htaccess"));
fclose($handle);

$chandle = fopen("config.php", "r+");
$cc = fread($chandle, filesize("config.php"));
fclose($chandle);


if ($turn=="on") { 

	$handle = fopen(".htaccess", "w+");
	$contents=str_replace("RewriteEngine off", "RewriteEngine on", $contents);
	fwrite($handle, $contents);
	fclose($handle);

	$handle = fopen("config.php", "w+");
	$cc=str_replace("rewrite='off';", "rewrite='on';", $cc);
	fwrite($handle, $cc);
	fclose($handle);



}

elseif ($turn=="off") { 
	
	$handle = fopen(".htaccess", "w+");
	$contents=str_replace("RewriteEngine on", "RewriteEngine off", $contents);
	fwrite($handle, $contents);
	fclose($handle);

	$handle = fopen("config.php", "w+");
	$cc=str_replace("rewrite='on';", "rewrite='off';", $cc);
	fwrite($handle, $cc);
	fclose($handle);

}  


if(eregi("RewriteEngine on", $contents)) { $GLOBALS['adminContent'].="Rewrite MOD is <b>ON</b><br><br><a href='admin.php?action=rewrite&turn=off'><u>Turn off MOD Rewrite</u></a>"; }
elseif(eregi("RewriteEngine off", $contents)) { $GLOBALS['adminContent'].="Rewrite MOD is <b>OFF</b><br><br><a href='admin.php?action=rewrite&turn=on'><u>Turn on MOD Rewrite</u></a>"; }

}


function ChangePass() {
	$c = '
		<br /><br />
		<table align="center" width="300" cellpadding="5" cellspacing="0" border="0" class="table01">
			<tr>
				<td>
					<form name="form" method="post" action="">
						<table align="center" width="100%" cellpadding="0" cellspacing="0" border="0">
							<tr>
								<td colspan="2" align="center"><b>Change Admin password</b></td>
							</tr>
							<tr><td colspan="2" height="5"></td></tr>
							<tr><td colspan="2" class="error">' . $GLOBALS['error'] . '</td></tr>
							<tr><td colspan="2" height="5"></td></tr>
							<tr>
								<td>Old Password : </td>
								<td><input type="password" name="oldpass" style="width: 150px" /></td>
							</tr>
							<tr>
								<td>New Password : </td>
								<td><input type="password" name="newpass" style="width: 150px" /></td>
							</tr>
							<tr>
								<td>Confirm : </td>
								<td><input type="password" name="conpass" style="width: 150px" /></td>
							</tr>
							<td><td colspan="2" height="5"></td></tr>
							<tr>
								<td></td>
								<td align="right"><input type="submit" value="Submit" /></td>
							</tr>
						</table>
						<input type="hidden" name="submit" value="2" />
					</form>
				</tr>
			</table>
	';
	
	$GLOBALS['adminContent'].= $c;	
}

function ModPass() {
	if ($_POST['newpass'] != $_POST['conpass']) {
		$GLOBALS['error'] = 'Invalid confirm password';
		return FALSE;
	}
	$res = mysql_query("SELECT `id` FROM `admin` WHERE `name`= '".addslashes($_SESSION['user'])."' AND `password`= '".md5($_POST['oldpass'])."' LIMIT 1;");
	if (!($row=mysql_fetch_assoc($res)))
		$GLOBALS['error'] = 'Invalid old password';
	else {
		mysql_query("UPDATE `admin` SET `password` = '".md5($_POST['newpass'])."' WHERE `id` = '".$row['id']."' LIMIT 1 ;");
		$_SESSION['pass'] = $_POST['newpass'];
		Redirect('admin.php');
	} 
	
	if ($GLOBALS['error'] == '') return TRUE;
	else return FALSE;
}

function EditWord($id) {
	echo $_POST['submit']; 
	if ($_POST['submit'] == -1) Redirect('admin.php?' . $GLOBALS['tofrom']);
	
	if ($_POST['submit'] == 1) {
		if ($id != 0) $q = "UPDATE words SET {x}, word_mdate = NOW() WHERE word_id = $id ";
		else $q = "INSERT INTO words SET {x}, word_mdate = NOW(), word_cdate = NOW() ";
		
		$x = Array('x' => "word_title = '" . sqlesc($_POST['word_title']) . "', word_desc = '" . sqlesc($_POST['word_desc']) . "', word_comments = '" . sqlesc($_POST['word_comments']) . "'");
		
		$q = __($q, $x);
		mysql_query($q);
		Redirect('admin.php?' . $GLOBALS['tofrom']);
	}
	
	$title = 'Insert new word definition';
	if ($id != 0) {
		$q = "SELECT * FROM words WHERE word_id = $id LIMIT 1";
		$res= mysql_query($q);
		$_POST+= mysql_fetch_assoc($res);
		$title = 'Edit word definition';
	}
	
	
	$c = '
		<br /><br />
		<table align="center" width="400" cellpadding="5" cellspacing="0" border="0" class="table01">
			<tr>
				<td>
					<form name="form" method="post" action="">
						<table align="center" width="100%" cellpadding="0" cellspacing="0" border="0">
							<tr>
								<td colspan="2" align="center"><b>' . $title . '</b></td>
							</tr>
							<tr><td colspan="2" height="5"></td></tr>
							<tr><td colspan="2" class="error">' . $GLOBALS['error'] . '</td></tr>
							<tr><td colspan="2" height="5"></td></tr>
							<tr>
								<td>Word : </td>
								<td width="1%"><input type="text" name="word_title" value="' . htmlesc($_POST['word_title']) . '" style="width: 250px" /></td>
							</tr>
							<tr>
								<td valign="top">Description : </td>
								<td><textarea name="word_desc" style="width: 250px; height: 150px">' . htmlesc($_POST['word_desc']) . '</textarea></td>
							</tr>
							<tr>
								<td valign="top">Comments : </td>
								<td><textarea name="word_comments" style="width: 250px; height: 150px">' . htmlesc($_POST['word_comments']) . '</textarea></td>
							</tr>
							<td><td colspan="2" height="5"></td></tr>
							<tr>
								<td></td>
								<td valign="bottom" align="right" rowspan="2"><input type="submit" value="Submit" /></td>
							</tr>
							<tr>
								<td valign="bottom" align="left"><input type="submit" value="Back" OnClick="form.submit.value = -1"/></td>
							</tr>
						</table>
						<input type="hidden" name="submit" value="1" />
					</form>
				</tr>
			</table>
	';
	
	$GLOBALS['adminContent'].= $c;
	
}



function DeleteWord($id) {
	$q = "DELETE FROM words WHERE word_id = $id ";
	mysql_query($q);
	Redirect('admin.php?' . $GLOBALS['tofrom']);
}

function Logout() {
	unset($_SESSION['user']);
	unset($_SESSION['pass']);
	Redirect('admin.php');
}



function CheckForm() {
	$res = mysql_query("SELECT `name` FROM `admin` WHERE `name`= '".addslashes($_POST['user'])."' AND `password`= '".md5($_POST['pass'])."' LIMIT 1;");
	if (!mysql_fetch_assoc($res))
	//if ((ADMIN_USER != $_POST['user']) || (ADMIN_PASS != $_POST['pass']))
		$GLOBALS['error'] = 'Invalid login';
	else {
		$_SESSION['user'] = $_POST['user'];
		$_SESSION['pass'] = $_POST['pass'];
		Redirect('admin.php');
	} 
	
	if ($GLOBALS['error'] == '') return TRUE;
	else return FALSE;
}


function ShowForm() {
	$c = '
		<br /><br />
		<table align="center" width="300" cellpadding="5" cellspacing="0" border="0" class="table01">
			<tr>
				<td>
					<form name="form" method="post" action="">
						<table align="center" width="100%" cellpadding="0" cellspacing="0" border="0">
							<tr>
								<td colspan="2" align="center"><b>Admin login</b></td>
							</tr>
							<tr><td colspan="2" height="5"></td></tr>
							<tr><td colspan="2" class="error">' . $GLOBALS['error'] . '</td></tr>
							<tr><td colspan="2" height="5"></td></tr>
							<tr>
								<td>Username : </td>
								<td width="1%"><input type="text" name="user" value="' . htmlesc($_POST['user']) . '" style="width: 150px" /></td>
							</tr>
							<tr>
								<td>Password : </td>
								<td><input type="password" name="pass" style="width: 150px" /></td>
							</tr>
							<td><td colspan="2" height="5"></td></tr>
							<tr>
								<td></td>
								<td align="right"><input type="submit" value="Submit" /></td>
							</tr>
						</table>
						<input type="hidden" name="submit" value="1" />
					</form>
				</tr>
			</table>
	';
	
	$GLOBALS['adminContent'].= $c;
}



?>