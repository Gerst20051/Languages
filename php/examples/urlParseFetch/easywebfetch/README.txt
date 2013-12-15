EasyWebFetch - Fetch a web page from PHP code
=============================================

EasyWebFetch is a class you use to fetch a web page from your application. This class
is an alternative if your server doesn't have PHP with curl enabled, or the PHP configuration
doesn't allow opening URL with fopen(). This class fetch a web page by opening socket 
connection to remote host, so it has no dependencies and should work on any server
configuration.

Author:  Nashruddin Amin <me@nashruddin.com>
License: GPL
Website: http://www.nashruddin.com

Copyright (c) 2008 Nashruddin Amin

Features
--------
* No Dependencies
* Support for chunked transfer-encoding used in HTTP 1.1
* Support redirects
* Support Proxies
* Return headers and body or only headers. This might be useful
  for link checker applications.

Requirements
------------
* PHP 5
* A working web server

Installation
------------
Place EasyWebFetch.php in the directory containing your classes and include it
in your code.

Common Usage
------------
1. Fetch a page

	<?php
	$wf = new EasyWebFetch;
	
	if (!$wf->get("http://www.google.com")) {
		print $wf->getErrorMessage();
		exit;
	}
	$page = $wf->getContents();
	?>

2. Check if a page exists

	<?php
	$wf = new EasyWebFetch; 
	$wf->get("http://www.nashruddin.com/index.html", true); /* return headers only */
	
	if ($wf->getStatus() == 200) {
		print "page exists.";
	} else {
		print "page missing.";
	}
	?>

3. Set maximum redirects. Default is 5.

	<?php
	$wf = new EasyWebFetch;
	$wf->setMaxRedirs(3);
	$wf->get("http://www.google.com");
	?>

4. Examining the HTTP response headers

	<?php
	$wf = new EasyWebFetch;
	$wf->get("http://www.freebsd.org");
	
	$headers = $wf->getHeaders();
	
	print $headers['content_type'];
	print $headers['date'];
	/* print all */
	print_r($headers);
	?>

5. Connect through a proxy

	<?php
	$wf = new EasyWebFetch;
	$wf->setProxyHost("proxy.its.ac.id:8080");
	$wf->setProxyUser("myusername:mysecretpass");
	$wf->get("http://www.google.com");
	
	$page = $wf->getContents();
	?>


Contact
-------
Please send comments and bug reports to me@nashruddin.com.
