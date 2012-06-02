#/bin/sh

read A
if [ "x$A" = "xfoo" ] ; then
    echo "Foo"
elif [ "x$A" = "xbar" ] ; then
    echo "Bar"
else
    echo "Other"
fi
