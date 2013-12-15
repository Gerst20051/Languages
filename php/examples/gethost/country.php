<?php 

  $host =  gethostbyaddr($ipa); 

  $at        = $host 
  $hostdot   = "."; 
  $result    = strrchr($at, $hostdot); 

  echo "The country is $result"; 


?>