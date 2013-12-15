<?php

class Person {
	public $first;
	public $last;

	public function __construct($f, $l = '') {
		$this -> first = $f;
		$this -> last = $l;
	}

	public function greeting() {
		return "Hello, my name is " . $this -> first . " " . $this -> last . ".";
	}

	static public function staticGreeting($first, $last) {
		return "Hello, my name is " . $first . " " . $last . ".";
	}
}

$him = new Person('John', 'Smith');
$her = new Person('Sally', 'Davis');
$other = new Person('Joe');

echo $him -> greeting(); // prints "Hello, my name is John Smith."
echo '<br />';
echo $her -> greeting(); // prints "Hello, my name is Sally Davis."
echo '<br />';
echo $other -> greeting(); // prints "Hello, my name is Joe ."
echo '<br />';
echo Person::staticGreeting('Jane', 'Doe'); // prints "Hello, my name is Jane Doe."

?>