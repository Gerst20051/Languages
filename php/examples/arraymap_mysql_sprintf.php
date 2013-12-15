<?php
function mressf() {
    $args = func_get_args();
    if (count($args) < 2) return false;
    $query = array_shift($args);
    $args = array_map('mysql_real_escape_string', $args);
    array_unshift($args, $query);
    $query = call_user_func_array('sprintf', $args);
    return $query;
}
?>