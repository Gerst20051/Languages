<?php


$s = 5;
$password = generatePassword($s);


function generatePassword($strength = null ) {
/**********************************************************************************************************************************************************
* Author: Jason Christman
* Program: generatePassword()
* Date: 07/06/09
* Description: PHP Password Generator 
*         : This script utilizes a password strength building process
*       : Password Strength levels   :
*           :Minimum  : 1 : lenth of 4 characters being numbers and lower case
*           :Base     : 2 : length of 5 characters with a reqirement of 2 Upper case and the reast being numbers and lower case
*           :Strong   : 3 : length of 7 characters with a requirement of 1 Special Characters + minimum
*           :Stronger : 4 : length of 12 characters with a requirement of 2 Special Charcters + minimum
*           :Maximum  : 5 : length of 23 with a requirement of 3 Special Characters and 3 Upper Case + minimum
*
*
**************************************************************************************************************************************************************
*                           *** (Please give credit and send any updates back to jason@nurv-os.com ) Thank you ***
**************************************************************************************************************************************************************/
    
    
    /*************************************************************
    * Password Strenth Levels
    *************************************************************/
    
    $base = 2;
    $strong = 3;
    $stronger = 4;
    $maximum = 5;
    
    /*******************************************************
    * Set default strength level
    *******************************************************/
    
    if($strength == null){ $strength = $strong; }
    if(!is_numeric($strength)){ $strength = $strong; }
    if($strength > 5){ $strength = $maximum; }
    
    /******************************************
    * Initialize variables
    ******************************************/
    
    $length = 4;
    $slength = 0;
    $choice = 0;
    $sMin = 0;
    $uMin = 0;
    $uCnt = 0;
    $sCnt = 0;
    $symbol = false;
    $upper = false;
    $numbers = '0123456789';
    $lower_case = 'asdfghjklzxcvbnmqwertyuiop';
    $upper_case = 'ASDFGHJKLZXCVBNMQWERTYUIOP';
    $symbol_chr = '/[|!@#$%&*\/(=?;).:\-_+~^\\]';
    $password = '';
    
    /************************************************************
    * Set variables for building password based on strength
    ************************************************************/

    if($strength >= $base){ $upper = true; $uMin = 2; $length = 5;}
    if($strength >= $strong){ $symbol = true; $sMin = 1; $length = 7;}
    if($strength >= $stronger){ $sMin = 2; $length = 12; }
    if($strength >= $maximum){ $sMin = 3; $uMin = 3; $length = 23;}
    
    /***********************************************************
    * Build Password Based on strenth level requirements
    ***********************************************************/
    while($slength < $length){
        $choice = rand(1,4);
            
        if($choice == 1){ // Check to add upper case characters
            if($upper == true){
                preg_match_all('/[A-Z]/', $password, $uppercase_characters);
                 if (!empty($uppercase_characters)){
                    $uCnt= count($uppercase_characters[0]);
                    if($uCnt < $uMin){
                        $password .= $upper_case[(rand() % strlen($upper_case))];    
                    }
                }
                    
            }
        }
        if($choice == 2){ // add lower case characters
            if(($length - $slength) > (($uMin - $uCnt) + ($sMin - $sCnt))){    
                $password .= $lower_case[(rand() % strlen($lower_case))];
            }
        }
        if($choice == 3){ // add numbers
            if(($slength > 0) && (($length - $slength) > (($uMin - $uCnt) + ($sMin - $sCnt)))){        
                $password .= $numbers[(rand() % strlen($numbers))];
            }
        }
        if($choice == 4){ // check to add special characters
            if($symbol == true){
                preg_match_all('/[|!@#$%&*\/(=?;).:\-_+~^\\\]/', $password, $symbols);
                if (!empty($symbols)){
                    $sCnt = count($symbols[0]);
                    if (($sCnt < $sMin) && $slength > 0){
                        $password .= $symbol_chr[(rand() % strlen($symbol_chr))];
                    }
                }         
            }
        }
        $slength = strlen($password);

    }
    
    
    return $password;
}
?>