<?php 
    require_once 'Password.php'; 
    print '<?xml version="1.0" encoding="utf-8"?>' . "\n"; 
?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" 
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"> 
<html xmlns="http://www.w3.org/1999/xhtml"> 
<head> 
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
    <meta http-equiv="Content-Style-Type" content="text/css" /> 
    <title>PHP5 Class - Password</title> 
    <link href="Password.Example.css" rel="stylesheet" type="text/css" /> 
</head> 
<body> 
<h1 id="example-001">Example 001. Single password</h1> 
<pre class="input_code"> 
&lt;?php 
require_once 'Password.php'; 
$pwd = Password::getInstance()-&gt;generate(); 
<span>// This is equal to: 
// $pwd = Password::getInstance(); 
// $pwd-&gt;generate();</span> 
print $pwd; 
?&gt; 
</pre> 
<h2>Output:</h2> 
<pre class="output_code"> 
<?php print Password::getInstance() 
            //->setAlphabet() 
            //->setDictionary() 
            ->generate(); ?> 
</pre> 

<hr /> 

<h1 id="example-002">Example 002. Single password with dictionary file</h1> 
<pre class="input_code"> 
&lt;?php 
require_once 'Password.php'; 
$pwd = Password::getInstance()-&gt;setDictionary('passwords')-&gt;generate(); 
<span>// This is equal to: 
// $pwd = Password::getInstance(); 
// $pwd-&gt;setDictionary('passwords'); 
// $pwd-&gt;generate(); 
// or 
// $pwd = Password::getInstance(); 
// $pwd-&gt;setDictionary('passwords')-&gt;generate();</span> 
print $pwd; 
?&gt; 
</pre> 
<h2>Output:</h2> 
<pre class="output_code"> 
<?php print Password::getInstance() 
            ->setAlphabet() 
            ->setDictionary('passwords') 
            ->generate(); ?> 
</pre> 

<hr /> 

<h1 id="example-003">Example 003. Single password with dictionary array</h1> 
<pre class="input_code"> 
&lt;?php 
require_once 'Password.php'; 
$pwd_array = array('Sample', 'Example', 'Variant', 'Version'); 
$pwd = Password::getInstance() 
     -&gt;setDictionary($pwd_array) 
     -&gt;generate(); 
print $pwd; 
?&gt; 
</pre> 
<h2>Output:</h2> 
<pre class="output_code"> 
<?php print Password::getInstance() 
            ->setAlphabet() 
            ->setDictionary(array('Sample', 'Example', 'Variant', 'Version')) 
            ->generate(); ?> 
</pre> 

<hr /> 

<h1 id="example-004">Example 004. Single password with custom alphabet</h1> 
<pre class="input_code"> 
&lt;?php 
require_once 'Password.php'; 
$pwd = Password::getInstance() 
     -&gt;setAlphabet(array('a', 'e'), array('b', 'c', 'd')) 
     -&gt;generate(); 
print_r($pwd); 
?&gt; 
</pre> 
<h2>Output:</h2> 
<pre class="output_code"> 
<?php print Password::getInstance() 
            ->setDictionary() 
            ->setAlphabet(array('a', 'e'), array('b', 'c', 'd')) 
            ->generate(); ?> 
</pre> 

<hr /> 

<h1 id="example-005">Example 005. Multiple passwords</h1> 
<pre class="input_code"> 
&lt;?php 
require_once 'Password.php'; 
$pwd = Password::getInstance() 
       -&gt;generate(32, 3); 
print_r($pwd); 
?&gt; 
</pre> 
<h2>Output:</h2> 
<pre class="output_code"> 
<?php 
ob_start(); 
print_r(Password::getInstance() 
        ->setAlphabet() 
        ->setDictionary() 
        ->generate(32, 3)); 
$pwd = ob_get_clean(); 
print htmlentities($pwd); 
?> 
</pre> 

<hr /> 

<h1 id="example-006">Example 006. Multiple passwords with custom alphabet, dictionary, custom length and only in lowercase</h1> 
<pre class="input_code"> 
&lt;?php 
require_once 'Password.php'; 
$pwd = Password::getInstance() 
       -&gt;setAlphabet(array('a'), array('b')) 
       -&gt;setDictionary('passwords') 
       -&gt;generate(32, 3, Password::LOWER); 
print_r($pwd); 
?&gt; 
</pre> 
<h2>Output:</h2> 
<pre class="output_code"> 
<?php 
ob_start(); 
print_r(Password::getInstance() 
        ->setAlphabet(array('a'), array('b')) 
        ->setDictionary('passwords') 
        ->generate(16, 3, Password::LOWER)); 
$pwd = ob_get_clean(); 
print htmlentities($pwd); 
?> 
</pre> 

<hr /> 

<h1 id="example-007">Example 007. Silly random bitfields (contain only digits 0 and 1)</h1> 
<pre class="input_code"> 
&lt;?php 
require_once 'Password.php'; 
$pwd = Password::getInstance() 
       -&gt;setAlphabet(array(0, 1)) 
       -&gt;setDictionary() 
       -&gt;generate(array(32, 0), 3)); 
print_r($pwd); 
?&gt; 
</pre> 
<h2>Output:</h2> 
<pre class="output_code"> 
<?php 
ob_start(); 
print_r(Password::getInstance() 
        ->setAlphabet(array(0, 1)) 
        ->setDictionary() 
        ->generate(array(32, 0), 3)); 
$pwd = ob_get_clean(); 
print htmlentities($pwd); 
?> 
</pre> 
</body> 
</html> 