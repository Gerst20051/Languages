<?php
function insert_tabs($count = 1) {
	for ($x = 1; $x <= $count; $x++) $output .= "\t";
	return $output;
}
?>