<?php 
    /* 
    * phpBBCode 
    * 
    * @website   www.swaziboy.com 
    * @author    Duncan Mundell 
    * @updated   03/2003 
    * @version   1.0a 
    */ 
     
    function BBCode($Text) { 
         
        // Replace any html brackets with HTML Entities to prevent executing HTML or script 
        // Don't use strip_tags here because it breaks [url] search by replacing & with amp 
        $content = str_replace("<", "&lt;", $content); 
        $content = str_replace(">", "&gt;", $content); 
         
        // Convert new line chars to html <br /> tags 
        $content = nl2br($content); 
         
        // Set up the parameters for a URL search string 
        $URLSearchString = " a-zA-Z0-9\:\/\-\?\&\.\=\_\~\#\'"; 
        // Set up the parameters for a MAIL search string 
        $MAILSearchString = $URLSearchString . " a-zA-Z0-9\.@"; 
         
        // Perform URL Search 
        $content = preg_replace("/\[url\]([$URLSearchString]*)\[\/url\]/", '<a href="$1" target="_blank">$1</a>', $content); 
        $content = preg_replace("(\[url\=([$URLSearchString]*)\](.+?)\[/url\])", '<a href="$1" target="_blank">$2</a>', $content); 
         
        // Perform MAIL Search 
        $content = preg_replace("(\[mail\]([$MAILSearchString]*)\[/mail\])", '<a href="mailto:$1">$1</a>', $content); 
        $content = preg_replace("/\[mail\=([$MAILSearchString]*)\](.+?)\[\/mail\]/", '<a href="mailto:$1">$2</a>', $content); 
         
        // Check for bold text 
        $content = preg_replace("(\[b\](.+?)\[\/b])is",'<span class="bold">$1</span>',$content); 
         
        // Check for Italics text 
        $content = preg_replace("(\[i\](.+?)\[\/i\])is",'<span class="italics">$1</span>',$content); 
         
        // Check for Underline text 
        $content = preg_replace("(\[u\](.+?)\[\/u\])is",'<span class="underline">$1</span>',$content); 
         
        // Check for strike-through text 
        $content = preg_replace("(\[s\](.+?)\[\/s\])is",'<span class="strikethrough">$1</span>',$content); 
         
        // Check for over-line text 
        $content = preg_replace("(\[o\](.+?)\[\/o\])is",'<span class="overline">$1</span>',$content); 
         
        // Check for colored text 
        $content = preg_replace("(\[color=(.+?)\](.+?)\[\/color\])is","<span style=\"color: $1\">$2</span>",$content); 
         
        // Check for sized text 
        $content = preg_replace("(\[size=(.+?)\](.+?)\[\/size\])is","<span style=\"font-size: $1px\">$2</span>",$content); 
         
        // Check for list text 
        $content = preg_replace("/\[list\](.+?)\[\/list\]/is", '<ul class="listbullet">$1</ul>' ,$content); 
        $content = preg_replace("/\[list=1\](.+?)\[\/list\]/is", '<ul class="listdecimal">$1</ul>' ,$content); 
        $content = preg_replace("/\[list=i\](.+?)\[\/list\]/s", '<ul class="listlowerroman">$1</ul>' ,$content); 
        $content = preg_replace("/\[list=I\](.+?)\[\/list\]/s", '<ul class="listupperroman">$1</ul>' ,$content); 
        $content = preg_replace("/\[list=a\](.+?)\[\/list\]/s", '<ul class="listloweralpha">$1</ul>' ,$content); 
        $content = preg_replace("/\[list=A\](.+?)\[\/list\]/s", '<ul class="listupperalpha">$1</ul>' ,$content); 
        $content = str_replace("[*]", "<li>", $Text); 
         
        // Check for font change text 
        $content = preg_replace("(\[font=(.+?)\](.+?)\[\/font\])","<span style=\"font-family: $1;\">$2</span>",$content); 
         
        // Declare the format for [code] layout 
        $CodeLayout = '<table width="90%" border="0" align="center" cellpadding="0" cellspacing="0"> 
                            <tr> 
                                <td class="quotecodeheader"> Code:</td> 
                            </tr> 
                            <tr> 
                                <td class="codebody">$1</td> 
                            </tr> 
                       </table>'; 
        // Check for [code] text 
        $content = preg_replace("/\[code\](.+?)\[\/code\]/is","$CodeLayout", $content); 
         
        // Declare the format for [quote] layout 
        $QuoteLayout = '<table width="90%" border="0" align="center" cellpadding="0" cellspacing="0"> 
                            <tr> 
                                <td class="quotecodeheader"> Quote:</td> 
                            </tr> 
                            <tr> 
                                <td class="quotebody">$1</td> 
                            </tr> 
                       </table>'; 
                 
        // Check for [code] text 
        $content = preg_replace("/\[quote\](.+?)\[\/quote\]/is","$QuoteLayout", $content); 
         
        // Images 
        // [img]pathtoimage[/img] 
        $content = preg_replace("/\[img\](.+?)\[\/img\]/", '<img src="$1">', $content); 
         
        // [img=widthxheight]image source[/img] 
        $content = preg_replace("/\[img\=([0-9]*)x([0-9]*)\](.+?)\[\/img\]/", '<img src="$3" height="$2" width="$1">', $content); 
         
        return $Text; 
    } 
?>