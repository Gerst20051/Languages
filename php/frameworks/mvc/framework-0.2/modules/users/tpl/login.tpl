{if is_array($errors) && count($errors)}
<div id="errors">
<p>
The following errors were reported:
</p>
<ol>
{foreach item=error from=$errors}
  <li> {$error} </li>
{/foreach}
</ol>
</div>
{/if}
<form method="post">
<label id="email">Email</label> <br />
<input type="text" id="email" name="email" />
<br /><br />
<label id="password">Password</label> <br />
<input type="password" id="password" name="password" />
<br /><br />
<input type="submit" id="submit" value="Login" />
</form>
