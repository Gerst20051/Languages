<?php
    function tryGetHost($ip)
    {
        $string = '';
        exec("dig +short -x $ip 2>&1", $output, $retval);
        if ($retval != 0)
        { 
            // there was an error performing the command
        } 
        else 
        {
            $x=0;
            while ($x < (sizeof($output)))
            {
                $string.= $output[$x];
                $x++;
            }
        }
        
        if (empty($string))
            $string = $ip;
        else //remove the trailing dot
            $string = substr($string, 0, -1);
        
        return $string;
    }
?>