<?php
class Template {
	const OPEN_BRACKET = "{";
	const CLOSE_BRACKET = "}";
	public static function inject(array $source, $template) {
		$ob_size = strlen(self::OPEN_BRACKET);
		$cb_size = strlen(self::CLOSE_BRACKET);
		$pos = 0;
		$end = strlen($template);
		while ($pos <= $end) {
			if ($pos_1 = strpos($template, self::OPEN_BRACKET, $pos)) {
				if ($pos_1) {
					$pos_2 = strpos($template, self::CLOSE_BRACKET, $pos_1);
					if ($pos_2) {
						$return_length = ($pos_2-$cb_size) - $pos_1;
						$var = substr($template, $pos_1+$ob_size, $return_length);
						$template = str_replace(self::OPEN_BRACKET.$var.self::CLOSE_BRACKET, $source[$var], $template);
						$pos = $pos_2 + $cb_size;
					} else {
						throw new exception("Incorrectly formed template - missing closing bracket. Please check your syntax.");
						break;
					}
				}
			} else break;
		}
		return $template;
	}
}

$array = array("NAME"=>"John Doe","DOB"=>"12/21/1986","ACL"=>"Super Administrator");
$template = "This is your template, {NAME}. You were born on {DOB} and you are a {ACL} on this system.";
echo Template::inject($array, $template);
?>