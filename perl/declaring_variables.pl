#!C:/xampp/perl/bin/perl.exe -w

$myvar = "hey";
print $myvar . "\n\n";

my $myvar = 1;
print $myvar . "\n\n";

@myvar = ("hello", "hey", "hiya");
print @myvar . "\n";
print $myvar[0] . "\n";
print $myvar[1] . "\n";
print $myvar[2] . "\n\n";

%myvar = (mon => "Monday", tue => "Tuesday", wed => "Wednesday", thur => "Thursday");
print %myvar . "\n";
print $myvar{"mon"} . "\n";
print $myvar{"tue"} . "\n";
print $myvar{"wed"} . "\n";
print $myvar{"thur"} . "\n\n";