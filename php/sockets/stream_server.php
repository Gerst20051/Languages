<?php
$master = array();
$socket = stream_socket_server("tcp://0.0.0.0:8000", $errno, $errstr);
if (!$socket) {
    echo "$errstr ($errno)<br />\n";
} else {
    $master[] = $socket;
    $read = $master;
    while (1) {
        $read = $master;
        $mod_fd = stream_select($read, $_w = NULL, $_e = NULL, 5);
        if ($mod_fd === FALSE) {
            break;
        }
        for ($i = 0; $i < $mod_fd; ++$i) {
            if ($read[$i] === $socket) {
                $conn = stream_socket_accept($socket);
                fwrite($conn, "Hello! The time is ".date("n/j/Y g:i a")."\n");
                $master[] = $conn;
            } else {
                $sock_data = fread($read[$i], 1024);
                var_dump($sock_data);
                if (strlen($sock_data) === 0) { // connection closed
                    $key_to_del = array_search($read[$i], $master, TRUE);
                    fclose($read[$i]);
                    unset($master[$key_to_del]);
                } else if ($sock_data === FALSE) {
                    echo "Something bad happened";
                    $key_to_del = array_search($read[$i], $master, TRUE);
                    unset($master[$key_to_del]);
                } else {
                    echo "The client has sent :"; var_dump($sock_data);
                    fwrite($read[$i], "You have sent :[".$sock_data."]\n");
                    fclose($read[$i]);
                     unset($master[array_search($read[$i], $master)]);
                }
            }
        }
    }
}
?>