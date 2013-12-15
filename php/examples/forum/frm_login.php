<?php require_once 'frm_header.inc.php'; ?>
<form name="theForm" method="post" action="frm_transact_user.php">
<h3>Member Login</h3>
<p>
Email Address:<br />
<input type="text" name="email" maxlength="255" value="<?php echo $_GET['e']; ?>" />
</p>
<p>
Password:<br />
<input type="password" name="passwd" maxlength="50" />
</p>
<p>
<input type="submit" class="submit" name="action" value="Login" />
</p>
<p>
Not a member yet? <a href="frm_useraccount.php">Create a new account!</a>
</p>
<p>
<a href="frm_forgotpass.php">Forgot your password?</a>
</p>
</form>
<?php require_once 'frm_footer.inc.php'; ?>