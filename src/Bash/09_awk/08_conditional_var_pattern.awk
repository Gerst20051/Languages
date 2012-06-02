BEGIN { maryword = "mary"; keyword=maryword "lamb"; }
(tolower($1) ~ keyword) { print "Mary appeared."; }
(tolower($1) !~ keyword) { print "No mary."; }

