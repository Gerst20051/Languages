BEGIN {
	print "This is a test." | "/usr/bin/tail -n 1";
	print "This is only a test." | "/usr/bin/tail -n 1";
	close("/usr/bin/tail -n 1");
	print "Yikes!" | "/usr/bin/tail -n 1";

	print "This is another test" > "/tmp/testfile-awk"
	print "This is yet another test entirely" > "/tmp/testfile-awk"
}
