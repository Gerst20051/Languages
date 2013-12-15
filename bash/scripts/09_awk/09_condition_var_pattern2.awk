BEGIN { maryword = "mary"; }
(tolower($1) ~ maryword "lamb" ) { print "Mary appeared."; }
(tolower($1) !~ maryword "lamb" ) { print "No mary."; }

