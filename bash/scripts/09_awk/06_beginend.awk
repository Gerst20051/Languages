BEGIN { print "Here is the line we care about."; }
/chocolate/ { print "Mmm.  Chocolate.  " $0; }
END { print "That's all that matters."; }
