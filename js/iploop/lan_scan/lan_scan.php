<?php
//------------------------------------------------------------------------------------------
//	LAN SCANNER created by Gareth Heyes (gareth at businessinfo co uk)
//  Blog: www.thespanner.co.uk
//	Labs site : www.businessinfo.co.uk
//  Version 2.1	
//------------------------------------------------------------------------------------------

/*  Copyright 2007  Gareth Heyes  (email : gareth[at]NOSPAM businessinfo(dot)(co)(dot)uk

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
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
?>
<html>
<head>
<title>Javascript LAN scanner</title>
<link href="lan_scan.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="lan_scan.js"></script>
</head>
<body>
<h1>Javascript LAN scanner</h1>
<script type="text/javascript">

//------------------------------------------------------------------------------------------
//	LAN SCANNER created by Gareth Heyes (gareth at businessinfo co uk)
//  Blog: www.thespanner.co.uk
//	Labs site : www.businessinfo.co.uk
//  Version 2.1	
//------------------------------------------------------------------------------------------

/*  Copyright 2007  Gareth Heyes  (email : gareth[at]NOSPAM businessinfo(dot)(co)(dot)uk

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
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


<!-- 
emailE=('gareth@' + 'businessinfo.co.uk')
document.write('<p><a href="mailto:' + emailE + '">By Gareth Heyes</a></p>')
 //-->
</script>

<p class="notice">This code is now open source. Please let me know if you find it useful.</p>
<p class="notice">Now works in Firefox and IE7.</p>
<p class="notice">Any information obtained using the scanner will not be logged in any way. All new router form submissions are anonymous</p>

<p id="startAgain"><a href="lan_scan.php">Start again</a></p>
<div id="results">
<ul>
	<li><a href="javascript:lanScan();">LAN scan</a></li>
	<li><a href="javascript:showDevices()">Show device database</a></li>
	<li><a href="javascript:showIPs();">Show IP database</a></li>
</ul>
</div>
<script type="text/javascript">
buildConnections();
</script>
<p>
<a href="http://www.businessinfo.co.uk/" title="By Businessinfo"><img src="http://www.businessinfo.co.uk/images/logo.gif" border="0"></a>
</p>
</body>
</html>
