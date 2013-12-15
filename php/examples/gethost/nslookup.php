<?php
/*
 * This function returns the real hostname of an ip address.
 *
 * @param: $ip - the ip address in format x.x.x.x where x are 
 *         numbers (0-255) or the hostname you want to lookup
 * @return: returns the hostname as string. Something like 'user-id.isp-dialin.tld'
 *
 * Warning: $ip must be validated before calling this function. 
 */
function nslookup($ip) {

        // execute nslookup command
        exec('nslookup '.$ip, $op);

        // php is running on windows machine
        if (substr(php_uname(), 0, 7) == "Windows") {
                return substr($op[3], 6);
        }
        else {
                // on linux nslookup returns 2 diffrent line depending on
                // ip or hostname given for nslookup
                if (strpos($op[4], 'name = ') > 0)
                        return substr($op[4], strpos($op[4], 'name =') + 7, -1);
                else
                        return substr($op[4], strpos($op[4], 'Name:') + 6);
        }
}

// example function call to get hostname of user ip:
echo nslookup($_SERVER['REMOTE_ADDR']);
?>