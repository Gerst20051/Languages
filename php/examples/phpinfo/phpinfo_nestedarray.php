<?php
ob_start();
phpinfo();
$phpinfo = array('phpinfo' => array());
if(preg_match_all('#(?:<h2>(?:<a name=".*?">)?(.*?)(?:</a>)?</h2>)|(?:<tr(?: class=".*?")?><t[hd](?: class=".*?")?>(.*?)\s*</t[hd]>(?:<t[hd](?: class=".*?")?>(.*?)\s*</t[hd]>(?:<t[hd](?: class=".*?")?>(.*?)\s*</t[hd]>)?)?</tr>)#s', ob_get_clean(), $matches, PREG_SET_ORDER))
    foreach($matches as $match)
        if(strlen($match[1]))
            $phpinfo[$match[1]] = array();
        elseif(isset($match[3]))
            $phpinfo[end(array_keys($phpinfo))][$match[2]] = isset($match[4]) ? array($match[3], $match[4]) : $match[3];
        else
            $phpinfo[end(array_keys($phpinfo))][] = $match[2];
?>

Some examples of using individual values from the array:

<?php
    echo "System: {$phpinfo['phpinfo']['System']}<br />\n";
    echo "Safe Mode: {$phpinfo['PHP Core']['safe_mode'][0]}<br />\n";
    echo "License: {$phpinfo['PHP License'][0]}<br />\n";
?>

To display everything:

<?php
    foreach($phpinfo as $name => $section) {
        echo "<h3>$name</h3>\n<table>\n";
        foreach($section as $key => $val) {
            if(is_array($val))
                echo "<tr><td>$key</td><td>$val[0]</td><td>$val[1]</td></tr>\n";
            elseif(is_string($key))
                echo "<tr><td>$key</td><td>$val</td></tr>\n";
            else
                echo "<tr><td>$val</td></tr>\n";
        }
        echo "</table>\n";
    }
?>