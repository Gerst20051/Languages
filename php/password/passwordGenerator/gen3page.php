<form action="gen3.php" method="post">
<div align="center">
<input type="checkbox" name="sgen" />Show generated password progression
</div>
<div align="center">
<select name="level">
<option value="">Select Strength Level</option>
<option value=""></option>
<option value="1"<?php if(isset($_POST['level']) && $_POST['level'] == 1){ echo "selected='selected'";}?> >Minimum</option>
<option value="2"<?php if(isset($_POST['level']) && $_POST['level'] == 2){ echo "selected='selected'";}?>  >Base</option>
<option value="3"<?php if(isset($_POST['level']) && $_POST['level'] == 3){ echo "selected='selected'";}?>  >Strong</option>
<option value="4"<?php if(isset($_POST['level']) && $_POST['level'] == 4){ echo "selected='selected'";}?>  >Stronger</option>
<option value="5"<?php if(isset($_POST['level']) && $_POST['level'] == 5){ echo "selected='selected'";}?>  >Maximum</option>
</select>
  
<input type="submit" name="ssubmit" value="Generate Password" /><br />
<?php if(isset($_POST['ssubmit'])){ $spass = generatePassword($_POST['level']); echo "<br><b>" . $spass;}?>
</b>
</div>
</form>