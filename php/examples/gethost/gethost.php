<?php
function gethost ($ip)
{
    //Make sure the input is not going to do anything unexpected
    //IPs must be in the form x.x.x.x with each x as a number
    
    if( preg_match('/^(?:25[0-5]|2[0-4]\d|1\d\d|[1-9]\d|\d)(?:[.]
(?:25[0-5]|2[0-4]\d|1\d\d|[1-9]\d|\d)){3}$/', $ip) )
    {
        $host = `host $ip`;
        return (($host ? end ( explode (' ', $host)) : $ip));
    }
    else
    {
        return false;
    }
}
?>

Though to be honest I would use:

<?php
function gethost ($ip)
{
    return ( preg_match('/^(?:25[0-5]|2[0-4]\d|1\d\d|[1-9]\d|\d)(?:[.]
(?:25[0-5]|2[0-4]\d|1\d\d|[1-9]\d|\d)){3}$/', $ip) ) ? gethostbyaddr($ip) : false;
}
?>