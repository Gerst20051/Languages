<?
function gethostbyaddr_timeout($ip, $dns, $timeout=1000)
{
    // random transaction number (for routers etc to get the reply back)
    $data = rand(0, 99);
    // trim it to 2 bytes
    $data = substr($data, 0, 2);
    // request header
    $data .= "\1\0\0\1\0\0\0\0\0\0";
    // split IP up
    $bits = explode(".", $ip);
    // error checking
    if (count($bits) != 4) return "ERROR";
    // there is probably a better way to do this bit...
    // loop through each segment
    for ($x=3; $x>=0; $x--)
    {
        // needs a byte to indicate the length of each segment of the request
        switch (strlen($bits[$x]))
        {
            case 1: // 1 byte long segment
                $data .= "\1"; break;
            case 2: // 2 byte long segment
                $data .= "\2"; break;
            case 3: // 3 byte long segment
                $data .= "\3"; break;
            default: // segment is too big, invalid IP
                return "INVALID";
        }
        // and the segment itself
        $data .= $bits[$x];
    }
    // and the final bit of the request
    $data .= "\7in-addr\4arpa\0\0\x0C\0\1";
    // create UDP socket
    $handle = @fsockopen("udp://$dns", 53);
    // send our request (and store request size so we can cheat later)
    $requestsize=@fwrite($handle, $data);

    @socket_set_timeout($handle, $timeout - $timeout%1000, $timeout%1000);
    // hope we get a reply
    $response = @fread($handle, 1000);
    @fclose($handle);
    if ($response == "")
        return $ip;
    // find the response type
    $type = @unpack("s", substr($response, $requestsize+2));
    if ($type[1] == 0x0C00)  // answer
    {
        // set up our variables
        $host="";
        $len = 0;
        // set our pointer at the beginning of the hostname
        // uses the request size from earlier rather than work it out
        $position=$requestsize+12;
        // reconstruct hostname
        do
        {
            // get segment size
            $len = unpack("c", substr($response, $position));
            // null terminated string, so length 0 = finished
            if ($len[1] == 0)
                // return the hostname, without the trailing .
                return substr($host, 0, strlen($host) -1);
            // add segment to our host
            $host .= substr($response, $position+1, $len[1]) . ".";
            // move pointer on to the next segment
            $position += $len[1] + 1;
        }
        while ($len != 0);
        // error - return the hostname we constructed (without the . on the end)
        return $ip;
    }
    return $ip;
}
?>