<?php
$fp = stream_socket_client("tcp://127.0.0.1:8000", $errno, $errstr, 30);
if (!$fp) {
    echo "$errstr ($errno)<br />\n";
} else {
    fwrite($fp, "Aloha");
    while (!feof($fp)) {
        var_dump(fgets($fp, 1024));
    }
    fclose($fp);
}
?>
