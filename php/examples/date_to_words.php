<?php
/* Change the following constants to suit your language */
 
define('STRING_TODAY', "today");
define('STRING_YESTERDAY', "yesterday");
define('STRING_DAYS', "%d days ago");
define('STRING_WEEK', "1 week ago");
define('STRING_WEEKS', "%d weeks ago");
 
/* Change the following date format to your taste */
define('DATE_FORMAT', "m-d-Y");
 
/* The functions takes the date as a timestamp */        
function DateToWords($time)
{
 
    $_word = "";
 
    /* Get the difference between the current time 
       and the time given in days */
    $days = intval((time() - $time) / 86400);
 
    /* If some forward time is given return error */
    if($days < 0) {
        return -1;
    }
 
    switch($days) {
        case 0: $_word = STRING_TODAY;
                break;
        case 1: $_word = STRING_YESTERDAY;
                break;
        case ($days >= 2 && $days <= 6): 
              $_word =  sprintf(STRING_DAYS, $days);
              break;
        case ($days >= 7 && $days < 14): 
              $_word= STRING_WEEK;
              break;
        case ($days >= 14 && $days <= 365): 
              $_word =  sprintf(STRING_WEEKS, intval($days / 7));
              break;
        default : return date(DATE_FORMAT, $time);
 
    }
 
    return $_word;
}
?>