<?php
 if(strstr($_SERVER["HTTP_USER_AGENT"],"MSIE")==false) {
  header("Content-type: text/javascript");
  header("Content-Disposition: inline; filename=\"download.js\"");
  header("Content-Length: ".filesize("my-file.js"));
 } else {
  header("Content-type: application/force-download");
  header("Content-Disposition: attachment; filename=\"download.js\"");
  header("Content-Length: ".filesize("my-file.js"));
 }
 header("Expires: Fri, 01 Jan 2010 05:00:00 GMT");
 if(strstr($_SERVER["HTTP_USER_AGENT"],"MSIE")==false) {
  header("Cache-Control: no-cache");
  header("Pragma: no-cache");
 }
 include("my-file.js");
?>