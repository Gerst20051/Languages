<?php
// See related links for more status codes
 
// Use this header instruction to fix 404 headers
// produced by url rewriting...
header('HTTP/1.1 200 OK');
 
// Page was not found:
header('HTTP/1.1 404 Not Found');
 
// Access forbidden:
header('HTTP/1.1 403 Forbidden');
 
// The page moved permanently should be used for
// all redrictions, because search engines know
// what's going on and can easily update their urls.
header('HTTP/1.1 301 Moved Permanently');
 
// Server error
header('HTTP/1.1 500 Internal Server Error');
 
// Redirect to a new location:
header('Location: http://www.example.org/');
 
// Redriect with a delay:
header('Refresh: 10; url=http://www.example.org/');
print 'You will be redirected in 10 seconds';
 
// you can also use the HTML syntax:
// <meta http-equiv="refresh" content="10;http://www.example.org/ />
 
// override X-Powered-By value
header('X-Powered-By: PHP/4.4.0');
header('X-Powered-By: Brain/0.6b');
 
// content language (en = English)
header('Content-language: en');
 
// last modified (good for caching)
$time = time() - 60; // or filemtime($fn), etc
header('Last-Modified: '.gmdate('D, d M Y H:i:s', $time).' GMT');
 
// header for telling the browser that the content
// did not get changed
header('HTTP/1.1 304 Not Modified');
 
// set content length (good for caching):
header('Content-Length: 1234');
 
// Headers for an download:
header('Content-Type: application/octet-stream');
header('Content-Disposition: attachment; filename="example.zip"'); 
header('Content-Transfer-Encoding: binary');
// load the file to send:
readfile('example.zip');
 
// Disable caching of the current document:
header('Cache-Control: no-cache, no-store, max-age=0, must-revalidate');
header('Expires: Mon, 26 Jul 1997 05:00:00 GMT'); // Date in the past
header('Pragma: no-cache');
 
// set content type:
header('Content-Type: text/html; charset=iso-8859-1');
header('Content-Type: text/html; charset=utf-8');
header('Content-Type: text/plain'); // plain text file
header('Content-Type: image/jpeg'); // JPG picture
header('Content-Type: application/zip'); // ZIP file
header('Content-Type: application/pdf'); // PDF file
header('Content-Type: audio/mpeg'); // Audio MPEG (MP3,...) file
header('Content-Type: application/x-shockwave-flash'); // Flash animation
 
// show sign in box
header('HTTP/1.1 401 Unauthorized');
header('WWW-Authenticate: Basic realm="Top Secret"');
print 'Text that will be displayed if the user hits cancel or ';
?>
print 'enters wrong login data';