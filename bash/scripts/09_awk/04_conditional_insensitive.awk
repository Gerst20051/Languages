tolower($1) ~ /mary/ { print "CI Record: " $0; }
$0 !~ /Mary/ { print "Not Mary: " $0; }
$1 = "Mary" { print "Mary Record: " $0; }
