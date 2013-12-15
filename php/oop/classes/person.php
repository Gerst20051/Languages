<?php
class Person {
	public $first,$last;
	public function __construct($f, $l = '') { $this -> first = $f; $this -> last = $l; }
	public function greeting() { return "Hello, my name is " . $this -> first . " " . $this -> last . "."; }
	static public function staticGreeting($first, $last) {return "Hello, my name is " . $first . " " . $last . "."; }
}

$him = new Person('John', 'Smith');
$her = new Person('Sally', 'Davis');
$other = new Person('Joe');

echo $him->greeting().'<br />';
echo $her->greeting().'<br />';
echo $other->greeting().'<br />';
echo Person::staticGreeting('Jane','Doe');
?>