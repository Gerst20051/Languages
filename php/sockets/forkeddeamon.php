<?PHP

/*

    PHP forked daemon
    Standalone PHP binary must be compiled with --enable-sockets and --enable-pcntl
    Dave M. -2002
        Online Services USA
*/

function sig_handler($signo) {

     switch($signo) {
         case SIGTERM:
             // handle shutdown tasks
             exit;
             break;
         case SIGHUP:
             // handle restart tasks
             break;
         case SIGUSR1:
             print "Caught SIGUSR1...\n";
             break;
         case SIGCHLD:
             while( pcntl_waitpid(-1,$status,WNOHANG)>0 ) { 
             }
             break;
         case SIGINT:
        exit;
         default:
             // not implemented yet...
             break;
     }

}

function interact($sock) {

    // Custom code goes here... e.g: socket_read() socket_write()...

}

function become_daemon() {

    $child = pcntl_fork();
    if($child) {
        exit; // kill parent
    }
    posix_setsid(); // become session leader
    chdir("/");
    umask(0); // clear umask
    return posix_getpid();

}

function open_pid_file($file) {

    if(file_exists($file)) {
        $fp = fopen($file,"r");
        $pid = fgets($fp,1024);
        fclose($fp);
        if(posix_kill($pid,0)) {
            print "Server already running with PID: $pid\n";
            exit;
        }
        print "Removing PID file for defunct server process $pid\n";
        if(!unlink($file)) {
            print "Cannot unlink PID file $file\n";
            exit;
        }
    }
    if($fp = fopen($file,"w")) {
        return $fp;
    } else {
        print "Unable to open PID file $file for writing...\n";
        exit;
    }
}

function change_identity($uid,$gid) {
    global $pid_file;
    if(!posix_setgid($gid)) {
        print "Unable to setgid to $gid!\n";
        unlink($pid_file);
        exit;
    }    
    if(!posix_setuid($uid)) {
        print "Unable to setuid to $uid!\n";
        unlink($pid_file);
        exit;
    }
    
    
}

error_reporting (4);

set_time_limit (0);

ob_implicit_flush ();

$pid_file = '/tmp/php_daemon.pid';

$underpriv_uid = '99'; // uid 99 == user nobody, at least on my system.
$underpriv_gid = '99';

$port = 10000;
$address = 0; // 0 binds to all addresses, may not work on fbsd

$quit = 0;

pcntl_signal(SIGCHLD, "sig_handler");
pcntl_signal(SIGTERM, "sig_handler");
pcntl_signal(SIGINT, "sig_handler");

$fh = open_pid_file($pid_file);

if (($sock = socket_create (AF_INET, SOCK_STREAM, 0)) < 0) {
    print "socket_create() failed: reason: " . socket_strerror ($sock) . "\n";
}

if (($ret = socket_bind ($sock, $address, $port)) < 0) {
    print "socket_bind() failed: reason: " . socket_strerror ($ret) . "\n";
}

if (($ret = socket_listen ($sock, 0)) < 0) {
    print "socket_listen() failed: reason: " . socket_strerror ($ret) . "\n";
}

change_identity($underpriv_uid,$underpriv_gid);

print "Server ready. Waiting for connections.....\n";

$pid = become_daemon();
fputs($fh,$pid);
fclose($fh);

while(!$quit) {

    if (($connection = socket_accept($sock)) < 0) {
        next;
    }
    
    if( ($child = pcntl_fork()) == -1 ) {
        print  "Could not fork!!\n";
        print "Dying...\n";
        $quit++;
    }
    elseif($child == 0) {
        socket_close($sock);
        interact($connection);
        exit;
    }
    
    socket_close($connection);
    
}

if(posix_getpid() == $pid) {
    unlink($pid_file);
}
judeman at yahoo dot com 05-Jun-2001 05:49
After several hours of working with sockets in an attempt to do UDP broadcasting, I thought a little help was in order for anyone else looking to do something similar, since it uses a number of those "undocumented" functions.  Here's how I did it: 

<?php 
    // here is a basic opening of the a socket.  AF_INET specifies the internet domain.  SOCK_DGRAM 
// specifies the Datagram socket type the 0 specifies that I want to use the default protcol (which in this 
// case is UDP) 
    $sock = socket(AF_INET, SOCK_DGRAM, 0); 

    // if the file handle assigned to socket is less than 0 then opening the socket failed 
    if($sock < 0) 
    { 
        echo "socket() failed, error: " . strerror($sock) . "\n"; 
    } 

    // here's where I set the socket options, this is essential to allow broadcasting.  An earlier comment (as of 
// June 4th, 2001) explains what the parameters are.  For my purposes (UDP broadcasting) I need to set 
// the broadcast option at the socket level to true.  In C, this done using SOL_SOCKET as the level param 
// (2) and SO_BROADCAST as the type param (3).  These may exist in PHP but I couldn't reference them  
// so I used the values that referencing these variables in C returns (namely 1 and 6 respectively).  This 
// function is basically just a wrapper to the C  function so check out the C documentation for more info 
    $opt_ret = setsockopt($sock, 1, 6, TRUE); 

    // if the return value is less than one, an error occured setting the options 
    if($opt_ret < 0) 
    { 
        echo "setsockopt() failed, error: " . strerror($opt_ret) . "\n"; 
    } 

    // finally I am ready to broad cast something.  The sendto function allows this without any 
// connections (essential for broadcasting).  So, this function sends the contents of $broadcast_string to the 
// general broadcast address (255.255.255.255) on port 4096.  The 0 (param 4) specifies no special 
// options, you can read about the options with man sendto 
    $send_ret = sendto($sock, $broadcast_string, strlen($broadcast_string), 0, '255.255.255.255', 4096); 

// if the return value is less than 0, an error has occured 
    if($send_ret < 0) 
    { 
    echo "sendto() failed, error: " . strerror($send_ret) . "<BR>\n";            } 
// be sure to close your socket when you're done 
close($sock);

?>