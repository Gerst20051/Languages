<?php
   include 'config.php'; // The file we made just now
 
   $echo = 'Hello there ' . $user; // $user is the logged in user. This will be displayed on the main application page
   $fbml = <<<EndHereDoc
   Hello world!
   EndHereDoc; // This is what will be displayed on users profile pages.
 
   $facebook->api_client->profile_setFBML($fbml, $user);
   // Set the profile box content for the logged in user
?>