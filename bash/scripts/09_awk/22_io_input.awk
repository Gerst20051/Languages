BEGIN {
	getline < "/tmp/testfile-awk";
	print "The record was " $0;

	"/bin/echo 'This is a test line'" | getline
	print "The second record was " $0;
}
