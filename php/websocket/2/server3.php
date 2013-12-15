#!/usr/bin/php -q 
<?php 
error_reporting(E_ALL); 

/* Allow the script to hang around waiting for connections. */ 
set_time_limit(0); 

/* Turn on implicit output flushing so we see what we're getting 
 * as it comes in. */ 
ob_implicit_flush(); 

$address = 'localhost'; 
$port = 10000; 

if (($sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) < 0) { 
   echo "socket_create() failed: reason: " . socket_strerror($sock) . "\n"; 
} 

if (($ret = socket_bind($sock, $address, $port)) < 0) { 
   echo "socket_bind() failed: reason: " . socket_strerror($ret) . "\n"; 
} 

if (($ret = socket_listen($sock, 5)) < 0) { 
   echo "socket_listen() failed: reason: " . socket_strerror($ret) . "\n"; 
} 

do { 
   if (($msgsock = socket_accept($sock)) < 0) { 
       echo "socket_accept() failed: reason: " . socket_strerror($msgsock) . "\n"; 
       break; 
   } 
   /* Send instructions. */ 
   $msg = "\nWelcome to the PHP Test Server. \n" . 
       "To quit, type 'quit'. To shut down the server type 'shutdown'.\n"; 
   socket_write($msgsock, $msg, strlen($msg)); 

   do { 
       if (false === ($buf = socket_read($msgsock, 2048, PHP_NORMAL_READ))) { 
           echo "socket_read() failed: reason: " . socket_strerror($ret) . "\n"; 
           break 2; 
       } 
       if (!$buf = trim($buf)) { 
           continue; 
       } 
       if ($buf == 'quit') { 
           break; 
       } 
       if ($buf == 'shutdown') { 
           socket_close($msgsock); 
           break 2; 
       } 
       $talkback = "PHP: You said '$buf'.\n"; 
       socket_write($msgsock, $talkback, strlen($talkback)); 
       echo "$buf\n"; 
   } while (true); 
   socket_close($msgsock); 
} while (true); 

socket_close($sock); 
?>