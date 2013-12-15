<?

/*
This program is free software; you can redistribute it and/or modify
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/************************************************\
* URL proxy v1.3 Pre-Alpha                       *
* a co-authored script                           *
*  by Scott Atkins <atkinssc@engr.orst.edu>      *
*     Bob Matcuk <bmatcuk@users.sourceforge.net> *
* Copyright (C) 2002 All rights reserved         *
*  Released under the GPL, see the README        *
*                                                *
* Edit config.php instead of this file please    *
* If you change this file and break your proxy   *
* don't ask for help, as you were warned         *
\************************************************/

// User should not have to edit this file
include("config.php");
include("classWebPage.php");

// Constants
$STATIC = 1;
$DYNAMIC = 2;

// Mode of the script
$proxyMode = ((isset($server) AND isset($redirectIP)) ? $STATIC : $DYNAMIC);


// Version Number
$phase = "pre-alpha";
$version = "1.3 " . $phase;

/*Flags******************************************\
* $isHTML;  true if mimetype is html             *
* $isImage; true if mimetype is an image         *
* $isDown;  true if specified mimetype is to be  *
*           downloaded                           *
* $isAuth;  true if the page is protected by     *
*           .htaccess                            *
* $isForm;  true if the page contains a form     *
\************************************************/
$isHTML  = false;
$isImage = false;
$isDown  = false;
$isAuth  = false;
$isForm  = false;
$isError = false;

/*getContentType*********************************\
* Function for finding the mime type of the file *
* Returns the content type                       *
\************************************************/
function getContentType($headers){
	foreach($headers as $h){
		if(eregi("^Content-Type: (.*)$",$h,$A1))
			return $A1[1];
	}
}

/*processHeaders*********************************\
* Function for handling the headers that apache  *
* sends out.                                     *
* Returns an array with the headers that should  *
* be sent out                                    *
\************************************************/
function processHeaders($headers, $fileName, $mime_dl, &$type,
			  &$isDown, &$isHTML, &$isImage){
	array_shift($headers);
	$type = getContentType($headers);
	$isDown = (isset($mime_dl[$type]) ? $mime_dl[$type] : true);
	if(eregi("image",$type))
		$isImage = true;
	elseif(eregi("text/html",$type))
		$isHTML  = true;
	if($isDown)
		$headers[] = "Content-Disposition: attachment;" . 
				" filename=$fileName";
	return $headers;
}

/************************************************\
* This block of code gets the directory we are   *
* currently in, for rel links.                   *
\************************************************/
if(eregi("http://",$_GET[$fileVar])){
    $relDir = eregi_replace("^http://[^/]*", "", $_GET[$fileVar]);
}else{
    $relDir = $_GET[$fileVar];
}
$relDir = eregi_replace("/[^/]*$","/",$relDir);

/************************************************\
* We create a new object of type WebPage and     *
* pass it the url we are being a proxy for and   *
* other information about the current state.     *
\************************************************/
$page = new WebPage($redirectIP."/".$_GET[$fileVar],true,$server,"URLproxyServer.php",$fileVar,$relDir);

/************************************************\
* This tells the WebPage object to open up a     *
* connection to the URL.                         *
*                                                *
* Note:                                          *
* This does not actually get the web page, just  *
* opens the connection for the headers.          *
\************************************************/
$page->openLink();

/************************************************\
* Process the headers so we know what kind of    *
* data we have (html/other)                      *
\************************************************/
$head = processHeaders($page->getHeaders(),$file,$mime_dl,$type,$isDown,$isHtml,$isImage);

/************************************************\
* This code replicates the headers that were     *
* sent when the class connected to the url.      *
*                                                *
* FIXME: extra headers need to be sent if we are *
* downloading the file.                          *
*                                                *
* GOTCHA?: need to check if http 1.1 will work   *
* correctly                                      *
\************************************************/
foreach($head as $h) header($h);

/************************************************\
* This block of code displays the page to the    *
* user.                                          *
*                                                *
* Note: Both processPageData and getRawPageData  *
* close the connection to the URL when they      *
* return.  You must re-open a connection with    *
* openLink to use them again.                    *
\************************************************/
if($isHtml){
    $page->processPageData();
    if($encryptPage)
        $page->encryptPage();
    echo $page->getPageData();

}else{
    if($isImage)
    	$page->getRawPageData();
}
?>
<A href="http://sourceforge.net"> <IMG src="http://sourceforge.net/sflogo.php?group_id=52440&type=5" width="210" height="62" border="0" alt="SourceForge Logo"></A>
