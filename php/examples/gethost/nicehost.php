<?php
function nicehost($host) {
    if (ereg('^([0-9]{1,3}\.){3}[0-9]{1,3}$', $host)) {
        return(ereg_replace('\.[0-9]{1,3}$', '.*', $host));
    } else {
        return(ereg_replace('^.{' . strpos($host, '.') . '}', '*', $host));
    }
}
?>