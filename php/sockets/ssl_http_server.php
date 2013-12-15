<?php
// Hello World! SSL HTTP Server.
// Tested on PHP 5.1.2-1+b1 (cli) (built: Mar 20 2006 04:17:24)

$context = stream_context_create();

// local_cert must be in PEM format
stream_context_set_option($context, 'ssl', 'local_cert', './server.pem');        
// Pass Phrase (password) of private key
stream_context_set_option($context, 'ssl', 'passphrase', 'comet');

stream_context_set_option($context, 'ssl', 'allow_self_signed', true);
stream_context_set_option($context, 'ssl', 'verify_peer', false);

// Create the server socket
$server = stream_socket_server('ssl://0.0.0.0:9001', $errno, $errstr, STREAM_SERVER_BIND|STREAM_SERVER_LISTEN, $context);

while(true)
{
    $buffer = '';
    print "waiting...";
    $client = stream_socket_accept($server);
    print "accepted " . stream_socket_get_name( $client, true) . "\n";
    if( $client )
    {
        // Read until double CRLF
        while( !preg_match('/\r?\n\r?\n/', $buffer) )
            $buffer .= fread($client, 2046); 
        // Respond to client
        fwrite($client,  "200 OK HTTP/1.1\r\n"
                         . "Connection: close\r\n"
                         . "Content-Type: text/html\r\n"
                         . "\r\n"
                         . "Hello World! " . microtime(true)
                         . "<pre>{$buffer}</pre>");
        fclose($client);
    } else {
        print "error.\n";
    }
}