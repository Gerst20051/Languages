<?php 
class person {
	var $name;
	function __construct($persons_name) { $this->name = $persons_name; }
	public function get_name() { return $this->name; }
	protected function set_name($new_name) {
		if (name != "Jimmy Two Guns") $this->name = strtoupper($new_name);
	}
}

class employee extends person {
	function __construct($employee_name) { $this->set_name($employee_name); }
	protected function set_name($new_name) {
		if ($new_name == "Stefan Sucks") $this->name = $new_name;
		else if ($new_name == "Johnny Fingers") parent::set_name($new_name);
	}
}
?>