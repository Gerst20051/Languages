#!/usr/bin/perl -w

$content = <<BODY_CONTENT;
This is a multi-line text example.
Somehting like a menu could be placed here, and then substituted into the program later on.
A menu created in this manner would be much easier to manage because it could be changed in a single place rather than on every page.
So that's pretty much it!
BODY_CONTENT
$page = "Home\n";

print "content-type:text/html";
print <<HTML;
<html>
    <head>
        <title>
$page
        </title>
     <head>
     <body>
$content
     </body>
</html>
HTML
