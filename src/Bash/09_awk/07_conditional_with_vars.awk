BEGIN { lastwasmary = 0; }
(tolower($1) ~ /mary/ && !lastwasmary) { print "Mary appeared."; lastwasmary = 1; }
(tolower($1) ~ /mary/ && lastwasmary) { print "Mary appeared again"; lastwasmary = 1; }
(tolower($1) !~ /mary/ && lastwasmary) { print "No Mary."; lastwasmary = 0; }

