BEGIN {RS="i"; FS="r"}
{
    print "Record is: " $0;
    print "First field is " $1;
}
