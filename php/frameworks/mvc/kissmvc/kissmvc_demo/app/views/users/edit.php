<form method="post" action="<?php echo myUrl('users/ops_update')?>">
<input type="hidden" name="uid" value="<?php echo $user->get('uid')?>" />
	<table>
		<tr><th colspan="2"><?php echo $form_heading?></th></tr>
		<tr>
			<td>Username</td>
			<td><input type="text" name="username" style="width:150px" value="<?php echo $user->get('username')?>" /></td>
		</tr>
		<tr>
			<td>Password</td>
			<td><input type="text" name="password" style="width:150px" value="<?php echo $user->get('password')?>" /></td>
		</tr>
		<tr>
			<td>Fullname</td>
			<td><input type="text" name="fullname" style="width:150px" value="<?php echo $user->get('fullname')?>" /></td>
		</tr>
		<tr>
			<td colspan="2" style="text-align:right"><input type="button" value="Submit" onclick="validateForm(this.form);return false;" /></td>
		</tr>
	</table>
</form>