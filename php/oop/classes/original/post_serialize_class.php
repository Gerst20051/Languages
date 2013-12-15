In my specific situation, I wanted to be able to pass some data from page to page, but without relying on a session variable. The answer I came up with was to serialize() the object in question, pass it on to the next page as a hidden form field, then unserialize() it. When ALL class variables are public, this worked fine. However, if there was at least one private/protected variable, the code no longer worked as expected ("Fatal error: Call to a member function display() on a non-object in page2.php on line 4")

As others have already mentioned, private/protected class variables will not behave nicely (private variables are prefixed by class_name + &#65533;, while protected variables are only prefixed by &#65533; - when looking at the page source using Firefox). Internet Explorer does NOT display the extra character between the class name and variable name, but the code still doesn't work as one would expect.

Suppose you have a simple class:

testclass.php
=============
<?php
class TestClass {
  var $i = 1;

  function display() {
    echo "i=" . $this->i;
  }
?>

page1.php
=========
<?php
  require_once 'testclass.php';
  $tc = new TestClass;
  $tc->display();
?>
<form method = "post" action = "page2.php">
<input type = "hidden" name = "str" value = "<?php echo htmlspecialchars( serialize( $tc ) ); ?>">
<input type = "submit">
</form>

page2.php
=========
<?php
  require_once 'testclass.php';
  $tc = unserialize( stripslashes( htmlspecialchars_decode( $_POST["str"] ) ) );
  $tc->display();
?>

The fix, suggested by evulish on #php/irc.dal.net, is to replace htmlspecialchars()/htmlspecialchars_decode() by base64_encode()/base64_decode. The code becomes:

page1.php
=========
<input type = "hidden" name = "str" value = "<?php echo base64_encode( serialize( $tc ) ); ?>">

page2.php
=========
<?php
  $tc = unserialize( base64_decode( $_POST["str"] ) );
?>

Hope this will help someone...